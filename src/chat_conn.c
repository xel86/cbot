#include "chat_conn.h"

#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define SERVER "irc.chat.twitch.tv"
#define PORT 6667

static int m_initialized;
static int m_socket;
static char leftover_buffer[CHAT_CONN_BUFFER_SIZE];
static size_t leftover_size = 0;

static int
authenticate_irc_bot_user()
{
    char cmd[128];
    char *username = "oectbot";
    char *pass;

    pass = getenv("CBOT_USER_OAUTH");
    if (pass == NULL)
    {
        fprintf(stderr, "Failed to get env var $CBOT_USER_OAUTH to authenticate\n");
        return CHAT_CONN_CONNECTION_ERROR;
    }

    snprintf(cmd, sizeof(cmd), "PASS oauth:%s\r\n", pass);
    if (send(m_socket, cmd, strlen(cmd), 0) < 0) {
        fprintf(stderr, "Failed to send PASS command\n");
        return CHAT_CONN_CONNECTION_ERROR;
    }

    snprintf(cmd, sizeof(cmd), "NICK %s\r\n", username);
    if (send(m_socket, cmd, strlen(cmd), 0) < 0) {
        fprintf(stderr, "Failed to send NICK command\n");
        return CHAT_CONN_CONNECTION_ERROR;
    }

    return CHAT_CONN_OK;
}

int
chat_conn_init(char *channel)
{
    char cmd[128];
    struct sockaddr_in server;

    // Resolve server IP address
    struct addrinfo hints, *servinfo, *p;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    int status = getaddrinfo(SERVER, NULL, &hints, &servinfo);
    if (status != 0) {
      fprintf(stderr, "Failed to resolve server IP address: %s\n",
              gai_strerror(status));
      return CHAT_CONN_CONNECTION_ERROR;
    }

    // Find the first suitable IP address
    for (p = servinfo; p != NULL; p = p->ai_next) {
      if (p->ai_family == AF_INET) {
        server.sin_addr = ((struct sockaddr_in *)p->ai_addr)->sin_addr;
        break;
      }
    }

    freeaddrinfo(servinfo);

    // Create socket
    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socket == -1) {
      fprintf(stderr, "Failed to create socket for twitch chat connection\n");
      return CHAT_CONN_CONNECTION_ERROR;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    // Connect to server
    if (connect(m_socket, (struct sockaddr *)&server, sizeof(server)) < 0) {
      fprintf(stderr, "Failed connecting to twitch irc chat server.\n");
      return CHAT_CONN_CONNECTION_ERROR;
    }

    if (authenticate_irc_bot_user() != CHAT_CONN_OK)
    {
        return CHAT_CONN_CONNECTION_ERROR;
    }

    snprintf(cmd, sizeof(cmd), "JOIN #%s\r\n", channel);
    if (send(m_socket, cmd, strlen(cmd), 0) < 0) {
        fprintf(stderr, "Failed to send JOIN command\n");
        return CHAT_CONN_CONNECTION_ERROR;
    }

    // TODO: Probably want to make capability requests optional through cli arg/config
    snprintf(cmd, sizeof(cmd), "CAP REQ :twitch.tv/commands twitch.tv/tags\r\n");
    if (send(m_socket, cmd, strlen(cmd), 0) < 0) {
        fprintf(stderr, "Failed to send JOIN command\n");
        return CHAT_CONN_CONNECTION_ERROR;
    }

    m_initialized = 1;
    return CHAT_CONN_OK;
}

int
chat_conn_get_next_buffer(char *buf, size_t *data_len_out)
{
    size_t data_len;
    int i;

    /* If there is a leftover buffer because a previous recv call had a cutoff message
     * from twitch, copy that to the front of our working data buffer to continue off it */
    if (leftover_size != 0)
    {
        memcpy(buf, leftover_buffer, leftover_size);
    }

    size_t received_bytes = recv(m_socket, &buf[leftover_size], (CHAT_CONN_BUFFER_SIZE - leftover_size) - 1, 0);
    if (received_bytes < 0) {
        fprintf(stderr, "Failed to receive irc message from twitch irc server");
        return CHAT_CONN_CONNECTION_ERROR;
    }

    /* Get our total data_len in our buffer considering potential leftovers */
    data_len = received_bytes + leftover_size;

    /* Reset leftover buffer size since we have fully 
     * incorporated it into our working buffer */
    leftover_size = 0;

    /* Begin to check the end of our working buffer to see if we have
     * received any incomplete/cutoff data from our recv call.
     * All complete twitch irc messages end with a \r\n
     * If the first character is a newline, we only have complete messages.
     * If not, search from the back inwards to find where the last complete
     * message begins and copy the cutoff incomplete message into our
     * leftover buffer to read from next function call. 
     * Mark our new data len to only contain the complete messages by
     * appending a null term char at the end of our last complete message. */
    i = data_len-1;
    if (buf[i] != '\n')
    {
        for (; i >= 0; i--)
        {
            if (buf[i] == '\n')
            {
                size_t start_of_cutoff = i+1;
                leftover_size = data_len-(start_of_cutoff);
                memcpy(leftover_buffer, &buf[start_of_cutoff], leftover_size);
                buf[start_of_cutoff] = '\0';
                data_len = start_of_cutoff+1;
                break;
            }
        }

        if (i == -1)
        {
            /* No complete messages found in received bytes from twitch
             * put the whole thing in leftover buffer
             */
            leftover_size = data_len;
            memcpy(leftover_buffer, buf, leftover_size);
            return CHAT_CONN_NO_FULL_MSG;
        }
    }
    else
    {
        buf[data_len] = '\0';
        data_len++;
    }

    /* Add one byte because of the null termination character getting added above */
    *data_len_out = data_len;

    return CHAT_CONN_OK;
}

int
chat_conn_send_msg(char *channel, char *text)
{
    // TODO: every other message append a invis char to bypass
    //       same message twitch check. will this require mutex?
    char msg[1024];
    snprintf(msg, sizeof(msg), "PRIVMSG %s :%s\r\n", channel, text);
    if (send(m_socket, msg, strlen(msg), 0) < 0) {
        return CHAT_CONN_CONNECTION_ERROR;
    }

    return CHAT_CONN_OK;
}

int
chat_conn_send_pong()
{
    char *pong = "PONG :tmi.twitch.tv\r\n";
    if (send(m_socket, pong, strlen(pong), 0) < 0) {
        return CHAT_CONN_CONNECTION_ERROR;
    }

    return CHAT_CONN_OK;
}

int
chat_conn_fini()
{
    close(m_socket);
    return CHAT_CONN_OK;
}
