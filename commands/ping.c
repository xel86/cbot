#include "irc.h"

#include <stdio.h>

int command_main(struct irc_msg *msg, char *response)
{
    snprintf(response, MAX_IRC_PARAMS_LEN, "@%s, pong!", msg->prefix.username);
    return 0;
}