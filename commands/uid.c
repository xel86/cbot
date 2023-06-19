#include "irc.h"

#include <stdio.h>

int command_main(struct irc_msg *msg, char *response)
{
    char *value;
    if ((value = irc_msg_try_get_tag(IRC_TAG_USER_ID, msg)) != NULL)
    {
        snprintf(response, MAX_IRC_PARAMS_LEN, "@%s, %s", msg->prefix.username, value);
    }
    else
    {
        return -1;
    }

    return 0;
}