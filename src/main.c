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
    size_t buf_len;

    while (1)
    {
        size_t cursor = 0;

        chat_conn_get_next_buffer(buf, &buf_len);
        while (buf_len != (cursor+1))
        {
            if (parse_or_handle_irc_buffer(&msg, buf, &cursor) == IRC_MSG_PRIVMSG)
            {
                printf("(#%s) %s: %s\n", msg.channel, msg.username, msg.text);
            }
        }
    }
    return 0;
}
