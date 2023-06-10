#include "chat_conn.h"

// https://datatracker.ietf.org/doc/html/rfc1459.html
enum irc_msg_type
{
    IRC_MSG_UNKNOWN,

    IRC_MSG_JOIN,
    IRC_MSG_NICK,
    IRC_MSG_NOTICE,
    IRC_MSG_PART,
    IRC_MSG_PASS,
    IRC_MSG_PING,
    IRC_MSG_PONG,
    IRC_MSG_PRIVMSG,

    RPL_NAMREPLY = 353,
    RPL_ENDOFNAMES = 366,
    RPL_MOTDSTART = 375,
    RPL_MOTD = 372,
    RPL_ENDOFMOTD = 376,
};


/*
 * This function takes a buffer of recevied bytes from our twitch irc chat socket.
 * It also takes an offset, which is where we start parsing the buffer from
 * This function can be called multiple times for the same buffer, since a buffer received from a
 * twitch irc socket can contain multiple irc messages.
 * If the irc message is not a PRIVMSG then this function handles it accordingly and leaves msg alone.
 * If the irc message is a PRIVMSG it will parse it into msg.
 * This design is to ensure that we only ever iterate over the any part of the message once.
 *
 * Returns the type of irc message we parsed from the buffer from the offset
 * msg is modified and filled only if irc_msg_type == IRC_MSG_PRIVMSG
 * buf is not modified
 * offset is set to the size of the single message it parsed from the buf on exit
 * this returned offset is then intended to be reused in the next call 
 * to this function with the same buf to parse the next message the buffer may contain
 * Check if the returned offset equals the total size of the buffer to see if you parsed all messages.
 *
 */
enum irc_msg_type
parse_or_handle_irc_buffer(struct chat_user_msg *msg, char *buf, size_t *cursor);
