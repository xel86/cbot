#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include "irc.h"

/* command_handler will always do a deep copy of msg
 * and take ownership of whatever data it has in it 
 * command handler is a non-blocking function and will
 * spawn a thread to handle whatever operations the 
 * command may have us doing.
 * caller will move on immediately after calling this function
 */
int
command_handler(struct irc_msg *msg);


#endif