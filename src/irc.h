#include "chat_conn.h"

#define MAX_IRC_USERNAME_LEN 32
#define MAX_IRC_PARAMS_LEN 1024
#define MAX_IRC_COMMAND_STR_LEN 32

/*
 * https://dev.twitch.tv/docs/irc/tags/
 * https://git.kotmisia.pl/Mm2PL/docs/src/branch/master/irc.md#clearchat
 */
enum irc_tag_index
{
    /* PRIVMSG */
    IRC_TAG_BADGE_INFO = 0,
    IRC_TAG_BADGES = 1,
    IRC_TAG_BITS = 2,
    IRC_TAG_COLOR = 3,
    IRC_TAG_DISPLAY_NAME = 4,
    IRC_TAG_EMOTES = 5,
    IRC_TAG_ID = 6,
    IRC_TAG_MOD = 7,
    IRC_TAG_REPLY_PARENT_MSG_ID = 8,
    IRC_TAG_REPLY_PARENT_USER_ID = 9,
    IRC_TAG_REPLY_PARENT_USER_LOGIN = 10,
    IRC_TAG_REPLY_PARENT_DISPLAY_NAME = 11,
    IRC_TAG_REPLY_PARENT_MSG_BODY = 12,
    IRC_TAG_ROOM_ID = 13,
    IRC_TAG_SUBSCRIBER = 14,
    IRC_TAG_TMI_SENT_TS = 15,
    IRC_TAG_TURBO = 16,
    IRC_TAG_USER_ID = 17,
    IRC_TAG_USER_TYPE = 18,
    IRC_TAG_VIP = 19,
    IRC_TAG_RETURNING_CHATTER = 20,
    IRC_TAG_FIRST_MSG = 21,
    IRC_TAG_FLAGS = 22,
    IRC_TAG_EMOTE_ONLY = 23,
    IRC_TAG_CLIENT_NONCE = 24,
    IRC_TAG_SENT_TS = 25,

    /* CLEARCHAT */
    IRC_TAG_BAN_DURATION = 26,
    IRC_TAG_TARGET_USER_ID = 27,

    /* CLEARMSG */
    IRC_TAG_LOGIN = 28,
    IRC_TAG_TARGET_MSG_ID = 29,

    /* ROOMSTATE */
    IRC_TAG_FOLLOWERS_ONLY = 30,
    IRC_TAG_R9K = 31,
    IRC_TAG_RITUALS = 32,
    IRC_TAG_SLOW = 33,
    IRC_TAG_SUBS_ONLY = 34,

    /* USERNOTICE */
    IRC_TAG_MSG_ID = 35,
    IRC_TAG_SYSTEM_MSG = 36,

    /* USERSTATE */
    IRC_TAG_EMOTE_SETS = 37,
    IRC_TAG_THREAD_ID = 38,
    IRC_TAG_MESSAGE_ID = 39,
};

/* Update this value if we add any new tags to account for
 * If we don't we are in big trouble
 */
#define IRC_TAG_COUNT 40

/* Used to store the value of each tag if present in a message.
 * tag names such as "user-id" will be translated into an irc_tag_index enum,
 * said enum will then be used as an index to the value for the tag 
 * in a irc_msg_tags array.
 */
typedef char irc_msg_tags[64][1024];

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
    irc_msg_tags tags;
    struct irc_msg_prefix prefix;
    enum irc_msg_command command;
    char channel[MAX_IRC_USERNAME_LEN];
    char params[MAX_IRC_PARAMS_LEN];
};

/* Checks if the given tag was filled for the given msg.
 * If it is, return a pointer to the value (msg.tags[tag])
 * If it was not filled (not included in msg), return NULL
 */
char *
irc_msg_try_get_tag(enum irc_tag_index tag, struct irc_msg *msg);

/*
 * Pretty prints all the tags in a message 
 * (used for debug purposes)
 */
void
irc_msg_print_tags(struct irc_msg *msg);

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
