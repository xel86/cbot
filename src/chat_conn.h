#ifndef CHAT_CONN_H
#define CHAT_CONN_H

#include <sys/types.h>
#include <time.h>

#define MAX_USERNAME_LEN 26
#define MAX_MESSAGE_TEXT_LEN 512

#define CHAT_CONN_BUFFER_SIZE 4096


struct chat_user_msg
{
    char channel[MAX_USERNAME_LEN];
    char username[MAX_USERNAME_LEN];
    char text[MAX_MESSAGE_TEXT_LEN];
    time_t ts;
};

enum chat_conn_retval {
    CHAT_CONN_OK = 0,
    CHAT_CONN_AUTH_ERROR = 1,
    CHAT_CONN_CONNECTION_ERROR = 2,
};

/*
 * Establishes a socket connection with the twitch irc server
 */
int
chat_conn_init(char *channel);

/*
 * Retrieves the next buffer from the twitch irc socket
 * A recv call on a socket connected to a twitch irc connection may return more than one
 * irc message at a time, separated by a \r\n at the end of each message.
 * This results in this function having to be get_next_buffer instead of get_next_message
 * without having some weird left over buffer variable that chat_conn.c has to keep track off.
 * Caller is responsible for parsing message(s) out of the buffer.
 * char *buf should be preallocated with a size of CHAT_CONN_BUFFER_SIZE.
 * size_t *bytes_out is the number of bytes in the returned buf
 */
int
chat_conn_get_next_buffer(char *buf, size_t *bytes_out);

/*
 * Closes socket connection with twitch irc server and
 * performs all necessary cleanup
 */
int
chat_conn_fini();

#endif
