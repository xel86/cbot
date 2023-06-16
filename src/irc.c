#include "irc.h"
#include "irc_msg_command_table.h"
#include "irc_tag_table.h"
#include "chat_conn.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

static size_t 
parse_irc_tags(char *buf, size_t cursor, struct irc_msg *msg)
{
    char tag_str[MAX_IRC_COMMAND_STR_LEN];
    enum irc_tag_index tag;
    size_t token_len;
    size_t token_start;

    /* This is important so that we can guarantee when checking
     * if a tag exists on a message that a non-present tag's
     * value will be a null term char.
     */
    //memset(msg->tags, '\0', sizeof(msg->tags));

    /* Skip '@' symbol at start of tags */
    cursor++;

    while (buf[cursor] != ' ')
    {
        token_start = cursor;
        for (; buf[cursor] != '='; cursor++);
        token_len = cursor - token_start; 
        memcpy(tag_str, &buf[token_start], token_len);
        tag_str[token_len] = '\0';
        cursor++;

        /* Get hash */
        const struct irc_tag_keyword *result = irc_tag_str_to_index(tag_str, token_len);
        if (result == NULL)
        {
            for (; buf[cursor] != ';' && buf[cursor] != ' '; cursor++);

            if(buf[cursor] == ' ')
            {
                break;
            }

            cursor++;
            continue;
        }

        tag = result->value;

        token_start = cursor;
        for (; buf[cursor] != ';' && buf[cursor] != ' '; cursor++);
        token_len = cursor - token_start; 
        memcpy(msg->tags[tag], &buf[token_start], token_len);
        msg->tags[tag][token_len] = '\0';

        if(buf[cursor] == ' ')
        {
            break;
        }

        cursor++;
    }

    return cursor;
}

static size_t 
parse_irc_prefix(char *buf, size_t cursor, struct irc_msg *msg)
{
    size_t start = cursor;
    size_t nick_end = 0;
    size_t user_end = 0;
    size_t token_start;
    size_t token_len;

    for (; buf[cursor] != ' '; cursor++)
    {
        if (buf[cursor] == '!')
        {
            nick_end = cursor;
        }
        else if (buf[cursor] == '@')
        {
            user_end = cursor;
        }
    }

    if (nick_end > 0)
    {
        /* Prefix with nick + user */

        /* Nickname */
        token_start = start + 1;
        token_len = nick_end - token_start;
        memcpy(msg->prefix.nickname, &buf[token_start], token_len);
        msg->prefix.nickname[token_len] = '\0';

        /* Username */
        token_start = nick_end + 1;
        token_len = user_end - token_start;
        memcpy(msg->prefix.username, &buf[token_start], token_len);
        msg->prefix.username[token_len] = '\0';

        /* Host */
        token_start = user_end + 1;
        token_len = cursor - token_start;
        memcpy(msg->prefix.host, &buf[token_start], token_len);
        msg->prefix.host[token_len] = '\0';
    }
    else
    {
        /* Prefix with just host */

        /* Host */
        token_start = start + 1;
        token_len = cursor - token_start;
        memcpy(msg->prefix.host, &buf[token_start], token_len);
        msg->prefix.host[token_len] = '\0';
    }

    return cursor;
}

static size_t 
parse_irc_command(char *buf, size_t cursor, struct irc_msg *msg)
{
    char cmd_str[MAX_IRC_COMMAND_STR_LEN];
    size_t token_len;
    size_t token_start;

    token_start = cursor;
    for (; buf[cursor] != ' '; cursor++);
    token_len = cursor - token_start; 
    memcpy(cmd_str, &buf[token_start], token_len);
    cmd_str[token_len] = '\0';

    /* Use a perfect hash table generated from gperf to translate type str to enum.
     * Example: "PRIVMSG" -> IRC_MSG_PRIVMSG
     */
    const struct irc_msg_keyword *result = irc_msg_cmd_str_to_enum(cmd_str, token_len);
    if (result == NULL)
    {
        return 0;
    }

    msg->command = result->value;

    return cursor;
}

static size_t 
parse_irc_params(char *buf, size_t cursor, struct irc_msg *msg)
{
    size_t token_len;
    size_t token_start;

    if (msg->command == IRC_MSG_PRIVMSG)
    {
        /* IRC Receiver (Channel Name) */
        /* This includes the mask! ('#' or '$') */
        token_start = cursor;
        for (; buf[cursor] != ' '; cursor++);
        token_len = cursor - token_start; 
        memcpy(msg->channel, &buf[token_start], token_len);
        msg->channel[token_len] = '\0';

        /* Go past space ' '*/
        cursor++;

        /* Go past ':' at start of message text*/
        cursor++;

        /* IRC PRIVMSG Parameter (Text) */
        token_start = cursor;
        for (; buf[cursor] != '\r'; cursor++);
        token_len = cursor - token_start; 
        memcpy(msg->params, &buf[token_start], token_len);
        msg->params[token_len] = '\0';
    }
    else
    {
        /* For Non-PRIVMSGs, just grab the entire param string */
        token_start = cursor;
        for (; buf[cursor] != '\r'; cursor++);
        token_len = cursor - token_start; 
        memcpy(msg->params, &buf[token_start], token_len);
        msg->params[token_len] = '\0';
    }

    return cursor;
}

/*
 * Sample Non-PRIVMSG IRC message:
 * :tmi.twitch.tv 002 oectbot :Your host is tmi.twitch.tv\r\n
 * 
 * Sample PRIVMSG IRC message:
 * :displayname!username@username.tmi.twitch.tv PRIVMSG #channel :message text\r\n
 *
 */
int
parse_irc_buffer_step(char *buf, size_t *cursor, struct irc_msg *msg)
{
    size_t i;

    for (i = *cursor; buf[i] != '\n'; i++)
    {
        /* IRC Tags */
        if (buf[i] == '@')
        {
            if ((i = parse_irc_tags(buf, i, msg)) == 0)
            {
                fprintf(stderr, "Failed to parse irc tags\n");
                msg->command = IRC_MSG_UNKNOWN;
                return -1;
            }
        }
        /* IRC Prefix */
        else if (buf[i] == ':')
        {
            if ((i = parse_irc_prefix(buf, i, msg)) == 0)
            {
                fprintf(stderr, "Failed to parse irc prefix\n");
                msg->command = IRC_MSG_UNKNOWN;
                return -1;
            }
        }
        /* IRC Command + Params */
        else
        {
            if ((i = parse_irc_command(buf, i, msg)) == 0)
            {
                fprintf(stderr, "Failed to parse irc command\n");
                msg->command = IRC_MSG_UNKNOWN;
                return -1;
            }
            i++;
            if ((i = parse_irc_params(buf, i, msg)) == 0)
            {
                fprintf(stderr, "Failed to parse irc params\n");
                return -1;
            }
        }
    }

    /* Return cursor at the start of the next irc message,
     * or a null term char indicating the end of the buffer */
    *cursor = i+1;

    return 0;
}

char *
irc_msg_try_get_tag(enum irc_tag_index tag, struct irc_msg *msg)
{
    if (msg->tags[tag][0] == '\0')
    {
        return NULL;
    }
    else
    {
        return msg->tags[tag];
    }
}

void
irc_msg_print_tags(struct irc_msg *msg)
{
    char *tag;

    printf("msg tags: [\n");
    for (int i = 0; i < IRC_TAG_COUNT; i++)
    {
        if ((tag = irc_msg_try_get_tag(i, msg)) != NULL)
        {
            printf("  %s\n", tag);
        }
    }
    printf("]\n");
}