#include "chat_conn.h"
#include "irc.h"

#include <stdio.h>

int
main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stdout, "Provide a channel name to join as an argument\n");
        return 0;
    }

    chat_conn_init(argv[1]);

    char buf[CHAT_CONN_BUFFER_SIZE];
    struct chat_user_msg msg;
    enum chat_conn_retval ret;
    enum irc_msg_type type;

    while (1)
    {
        size_t buf_len = 0;
        size_t cursor = 0;

        ret = chat_conn_get_next_buffer(buf, &buf_len);
        if (ret != CHAT_CONN_OK)
        {
            fprintf(stderr, "Error getting data from twitch irc chat connection socket\n");
            continue;
        }

        while (buf_len != (cursor+1))
        {
            type = parse_or_handle_irc_buffer(&msg, buf, &cursor);
            if (type == IRC_MSG_PRIVMSG)
            {
                printf("(#%s) %s: %s\n", msg.channel, msg.username, msg.text);
            }
            else if (type == IRC_MSG_PING)
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
