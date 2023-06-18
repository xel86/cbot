#include "command_handler.h"

#include "irc.h"
#include "chat_conn.h"

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static void * 
command_handler_thread(void *tmsg)
{
    /* Copy the passed in msg so that the thread can take ownership of it
     * Sidenote: Could the main thread (command_handler) change the value of vmsg
     * before we copy it? (unlikely but possible?)
     */
    struct irc_msg *msg = (struct irc_msg*)tmsg;
    char response[512];

    // temporary hard coded command name
    if (strcmp(&msg->params[1], "uid") == 0)
    {
        char *value;
        if ((value = irc_msg_try_get_tag(IRC_TAG_USER_ID, msg)) != NULL)
        {
            snprintf(response, sizeof(response), "@%s, %s", msg->prefix.username, value);
            chat_conn_send_msg(msg->channel, response);
        }
    }

    free(msg);
    return NULL;
}

int
command_handler(struct irc_msg *msg)
{
    /* We can currently just copy like this 
     * because we only have statically allocated
     * structures in an irc_msg.
     * If we ever dynamically allocate we will need
     * to do a deep copy here.
     */
    pthread_t tid;
    int result;

    struct irc_msg *copy = malloc(sizeof(struct irc_msg));
    *copy = *msg;

    result = pthread_create(&tid, NULL, command_handler_thread, (void*)copy);
    if (result != 0)
    {
        return -1;
    }

    return 0;
}