#include "chat_conn.h"
#include "irc.h"
#include "command_handler.h"

#include <stdio.h>

int
main(int argc, char **argv)
{
    int ret;

    if (argc < 2)
    {
        fprintf(stdout, "Provide a channel name to join as an argument\n");
        return 0;
    }

    ret = chat_conn_init(argv[1]);
    if (ret != CHAT_CONN_OK)
    {
        fprintf(stderr, "Failed to initalize chat connection\n");
        return 0;
    }
    ret = command_handler_init();
    if (ret != 0)
    {
        fprintf(stderr, "Failed to initalize command handler\n");
        return 0;
    }

    char buf[CHAT_CONN_BUFFER_SIZE];
    struct irc_msg msg;

    while (1)
    {
        size_t buf_len = 0;
        size_t cursor = 0;

        ret = chat_conn_get_next_buffer(buf, &buf_len);
        if (ret != CHAT_CONN_OK)
        {
            if (ret == CHAT_CONN_CONNECTION_ERROR)
            {
                fprintf(stderr, "Error getting data from twitch irc chat connection socket\n");
            }

            continue;
        }

        while (buf_len != (cursor+1))
        {
            if (parse_irc_buffer_step(buf, &cursor, &msg) < 0)
            {
                fprintf(stderr, "Error parsing buffer: %s", buf);
                break;
            }

            if (msg.command == IRC_MSG_PRIVMSG)
            {
                if (msg.params[0] == '&')
                {
                    if (command_handler(&msg) < 0)
                    {
                        fprintf(stderr, "Failed to spawn command handler thread!\n");
                    }
                }
                printf("(%s) %s: %s\n", msg.channel, msg.prefix.username, msg.params);
            }
            else if (msg.command == IRC_MSG_PING)
            {
                ret = chat_conn_send_pong();
                if (ret != CHAT_CONN_OK)
                {
                    fprintf(stderr, "Failed to send PONG message back after PING\n");
                }
            }
        }
    }
    return 0;
}
