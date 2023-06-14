#include "chat_conn.h"

#define MAX_IRC_USERNAME_LEN 32
#define MAX_IRC_PARAMS_LEN 1024
#define MAX_IRC_COMMAND_STR_LEN 32

struct irc_msg_tags
{
};

struct irc_msg_prefix
{
    char nickname[MAX_IRC_USERNAME_LEN];
    char username[MAX_IRC_USERNAME_LEN];
    char host[64];
};

// https://datatracker.ietf.org/doc/html/rfc1459.html
// https://dev.twitch.tv/docs/irc/commands/
enum irc_msg_command
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

    RPL_WELCOME = 001,
    RPL_YOURHOST = 002,
    RPL_CREATED = 003,
    RPL_MYINFO = 004,
    RPL_NAMREPLY = 353,
    RPL_ENDOFNAMES = 366,
    RPL_MOTDSTART = 375,
    RPL_MOTD = 372,
    RPL_ENDOFMOTD = 376,

    /* Twitch Specific Commands */
    IRC_MSG_CLEARCHAT,
    IRC_MSG_CLEARMSG,
    IRC_MSG_GLOBALUSERSTATE,
    IRC_MSG_HOSTTARGET,
    IRC_MSG_RECONNECT,
    IRC_MSG_ROOMSTATE,
    IRC_MSG_USERNOTICE,
    IRC_MSG_USERSTATE,
    IRC_MSG_WHISPER,
};

struct irc_msg
{
    struct irc_msg_tags tags;
    struct irc_msg_prefix prefix;
    enum irc_msg_command command;
    char channel[MAX_IRC_USERNAME_LEN];
    char params[MAX_IRC_PARAMS_LEN];
};

/*
 * This function parses a single irc message from a char* buf.
 * cursor is the index at which to start parsing a single irc message from buf
 * buf is parsed until a \n character, indicating the end of the irc message
 * cursor will be updated to be the index after the \n
 * this index be the start of a new irc message in buf, or a null term character
 *
 * Caller Expectations:
 *     buf is null terminated
 *     buf only contains complete irc messages, each ending with \r\n
 *     cursor is an index into buf to the start of an irc message
 */
int
parse_irc_buffer_step(char *buf, size_t *cursor, struct irc_msg *msg);
