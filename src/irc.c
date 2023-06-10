#include "irc.h"
#include "chat_conn.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

/* 
 * Returns true if character is in buf, or false if we reached the end of the buf
 * buf must be null terminated
 * cursor is incremented to the index of the first target or the null term char
 */
static bool
advance_cursor_till_target(char *buf, size_t *cursor, char target)
{
    assert(cursor != NULL);

    while (buf[*cursor])
    {
        if (buf[*cursor] == target)
            return true;
        else
         (*cursor)++;
    }
    return false;
}

static bool
extract_token_till_target(char *buf, size_t *cursor, char target, char *token, size_t max_len)
{
    size_t token_start = *cursor;
    size_t token_len;

    if(!advance_cursor_till_target(buf, cursor, target))
    {
        return false;
    }

    token_len = (*cursor) - token_start;

    /* This should ideally never trigger, but if for whatever reason
     * a token is longer than twitch specified it can be than just 
     * advance till the next message / end of buffer. 
     * Check 1 more than the token length since we will be adding
     * a null termination character to the token. */
    if (token_len+1 > max_len)
    {
        if(advance_cursor_till_target(buf, cursor, '\n'))
        {
            cursor++;
        }
        return false;
    }

    strncpy(token, &buf[token_start], token_len);
    token[token_len] = '\0';

    return true;
}

/*
 * Sample Non-PRIVMSG IRC message:
 * :tmi.twitch.tv 002 oectbot :Your host is tmi.twitch.tv\r\n
 * 
 * Sample PRIVMSG IRC message:
 * :displayname!username@username.tmi.twitch.tv PRIVMSG #channel :message text\r\n
 *
 */
enum irc_msg_type
parse_or_handle_irc_buffer(struct chat_user_msg *msg, char *buf, size_t *cursor)
{
    // Skip over tags for now
    if(!advance_cursor_till_target(buf, cursor, ':'))
    {
        return IRC_MSG_UNKNOWN;
    }

    /* Attempt to find a ! marker or a space.
     * Allows us to determine whether this is a PRIVMSG from a user
     * or a Non-PRIVMSG sent from twitch.
     */
    while(buf[*cursor])
    {
        if (buf[*cursor] != '!' && buf[*cursor] != ' ')
        {
            (*cursor)++;
        }
        else
        {
            break;
        }
    }

    if (!buf[*cursor])
    {
        fprintf(stderr, "Attempted to parse incomplete IRC message in buffer: %s\n", buf);
        return IRC_MSG_UNKNOWN;
    }

    if (buf[*cursor] == ' ')
    {
        // Not a PRIVMSG, handle and return without setting msg
        if(advance_cursor_till_target(buf, cursor, '\n'))
        {
            cursor++;
        }
        return IRC_MSG_UNKNOWN;
    }

    /* IRC Message is a PRIVMSG */

    /* Advance past username marker '!' */
    (*cursor)++;

    if (!extract_token_till_target(buf, cursor, '@', msg->username, MAX_USERNAME_LEN))
    {
        fprintf(stderr, "Error parsing username from irc PRIVMSG in buffer: %s\n", buf);
        return IRC_MSG_UNKNOWN;
    }

    if(!advance_cursor_till_target(buf, cursor, ' '))
    {
        return IRC_MSG_UNKNOWN;
    }
    (*cursor)++;

    char type[32];
    if (!extract_token_till_target(buf, cursor, ' ', type, 32))
    {
        fprintf(stderr, "Error parsing command type from irc PRIVMSG in buffer: %s\n", buf);
        return IRC_MSG_UNKNOWN;
    }

    if (strcmp(type, "PRIVMSG") != 0)
    {
        // Non-PRIVMSG with a username !...@ in prefix
        // Most likely JOIN msg(s)
        if(advance_cursor_till_target(buf, cursor, '\n'))
        {
            cursor++;
        }
        return IRC_MSG_UNKNOWN;
    }

    if(!advance_cursor_till_target(buf, cursor, '#'))
    {
        return IRC_MSG_UNKNOWN;
    }
    (*cursor)++;

    if (!extract_token_till_target(buf, cursor, ' ', msg->channel, MAX_USERNAME_LEN))
    {
        fprintf(stderr, "Error parsing channel name from irc PRIVMSG in buffer: %s\n", buf);
        return IRC_MSG_UNKNOWN;
    }

    if(!advance_cursor_till_target(buf, cursor, ':'))
    {
        return IRC_MSG_UNKNOWN;
    }
    (*cursor)++;

    if (!extract_token_till_target(buf, cursor, '\r', msg->text, MAX_MESSAGE_TEXT_LEN))
    {
        fprintf(stderr, "Error parsing message text from irc PRIVMSG in buffer: %s\n", buf);
        return IRC_MSG_UNKNOWN;
    }

    /* Skip \r\n at end of message and leave the cursor at the start of next message
     * or a null termination character. */
    (*cursor)++;
    if (buf[*cursor] != '\n')
    {
        fprintf(stderr, "Error parsing message text from irc PRIVMSG in buffer: %s\n", buf);
        return IRC_MSG_UNKNOWN;
    }
    (*cursor)++;

    return IRC_MSG_PRIVMSG;
}

