/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf -t --output-file=irc_msg_command_table.h irc_msgs.gperf  */
/* Computed positions: -k'2-3' */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
#error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gperf@gnu.org>."
#endif

#line 1 "irc_msgs.gperf"

#include <string.h>
#line 4 "irc_msgs.gperf"
struct irc_msg_keyword {
    const char *name;
    enum irc_msg_command value;
};

#define TOTAL_KEYWORDS 26
#define MIN_WORD_LENGTH 3
#define MAX_WORD_LENGTH 15
#define MIN_HASH_VALUE 3
#define MAX_HASH_VALUE 49
/* maximum key range = 47, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
hash (register const char *str, register size_t len)
{
  static unsigned char asso_values[] =
    {
      50, 50, 50, 50, 50, 50, 50, 50, 50, 50,
      50, 50, 50, 50, 50, 50, 50, 50, 50, 50,
      50, 50, 50, 50, 50, 50, 50, 50, 50, 50,
      50, 50, 50, 50, 50, 50, 50, 50, 50, 50,
      50, 50, 50, 50, 50, 50, 50, 50, 15, 30,
      20, 15, 25,  5,  0,  5, 50, 50, 50, 50,
      50, 50, 50, 50, 50,  3, 50, 20, 50,  5,
      50, 50,  0, 20, 50, 50,  5, 50, 25,  0,
      50, 50,  5,  0,  0, 50, 50, 50, 50, 50,
      50, 50, 50, 50, 50, 50, 50, 50, 50, 50,
      50, 50, 50, 50, 50, 50, 50, 50, 50, 50,
      50, 50, 50, 50, 50, 50, 50, 50, 50, 50,
      50, 50, 50, 50, 50, 50, 50, 50, 50, 50,
      50, 50, 50, 50, 50, 50, 50, 50, 50, 50,
      50, 50, 50, 50, 50, 50, 50, 50, 50, 50,
      50, 50, 50, 50, 50, 50, 50, 50, 50, 50,
      50, 50, 50, 50, 50, 50, 50, 50, 50, 50,
      50, 50, 50, 50, 50, 50, 50, 50, 50, 50,
      50, 50, 50, 50, 50, 50, 50, 50, 50, 50,
      50, 50, 50, 50, 50, 50, 50, 50, 50, 50,
      50, 50, 50, 50, 50, 50, 50, 50, 50, 50,
      50, 50, 50, 50, 50, 50, 50, 50, 50, 50,
      50, 50, 50, 50, 50, 50, 50, 50, 50, 50,
      50, 50, 50, 50, 50, 50, 50, 50, 50, 50,
      50, 50, 50, 50, 50, 50, 50, 50, 50, 50,
      50, 50, 50, 50, 50, 50
    };
  return len + asso_values[(unsigned char)str[2]] + asso_values[(unsigned char)str[1]];
}

struct irc_msg_keyword *
in_word_set (register const char *str, register size_t len)
{
  static struct irc_msg_keyword wordlist[] =
    {
      {""}, {""}, {""},
#line 23 "irc_msgs.gperf"
      {"366", RPL_ENDOFNAMES },
      {""}, {""},
#line 12 "irc_msgs.gperf"
      {"NOTICE", IRC_MSG_NOTICE},
#line 14 "irc_msgs.gperf"
      {"PASS", IRC_MSG_PASS},
#line 26 "irc_msgs.gperf"
      {"376", RPL_ENDOFMOTD},
#line 32 "irc_msgs.gperf"
      {"ROOMSTATE", IRC_MSG_ROOMSTATE},
#line 30 "irc_msgs.gperf"
      {"HOSTTARGET", IRC_MSG_HOSTTARGET},
      {""},
#line 13 "irc_msgs.gperf"
      {"PART", IRC_MSG_PART},
#line 24 "irc_msgs.gperf"
      {"375", RPL_MOTDSTART},
#line 34 "irc_msgs.gperf"
      {"USERSTATE", IRC_MSG_USERSTATE},
#line 33 "irc_msgs.gperf"
      {"USERNOTICE", IRC_MSG_USERNOTICE},
      {""}, {""},
#line 28 "irc_msgs.gperf"
      {"CLEARMSG", IRC_MSG_CLEARMSG},
#line 27 "irc_msgs.gperf"
      {"CLEARCHAT", IRC_MSG_CLEARCHAT},
#line 29 "irc_msgs.gperf"
      {"GLOBALUSERSTATE", IRC_MSG_GLOBALUSERSTATE},
      {""}, {""},
#line 22 "irc_msgs.gperf"
      {"353", RPL_NAMREPLY},
#line 10 "irc_msgs.gperf"
      {"JOIN", IRC_MSG_JOIN},
      {""}, {""},
#line 35 "irc_msgs.gperf"
      {"WHISPER", IRC_MSG_WHISPER},
#line 25 "irc_msgs.gperf"
      {"372", RPL_MOTD},
#line 16 "irc_msgs.gperf"
      {"PONG", IRC_MSG_PONG},
      {""}, {""},
#line 17 "irc_msgs.gperf"
      {"PRIVMSG", IRC_MSG_PRIVMSG},
#line 20 "irc_msgs.gperf"
      {"003", RPL_CREATED},
#line 31 "irc_msgs.gperf"
      {"RECONNECT", IRC_MSG_RECONNECT},
      {""}, {""}, {""},
#line 19 "irc_msgs.gperf"
      {"002", RPL_YOURHOST},
      {""}, {""}, {""}, {""},
#line 21 "irc_msgs.gperf"
      {"004", RPL_MYINFO},
#line 11 "irc_msgs.gperf"
      {"NICK", IRC_MSG_NICK},
      {""}, {""}, {""},
#line 18 "irc_msgs.gperf"
      {"001", RPL_WELCOME},
#line 15 "irc_msgs.gperf"
      {"PING", IRC_MSG_PING}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register unsigned int key = hash (str, len);

      if (key <= MAX_HASH_VALUE)
        {
          register const char *s = wordlist[key].name;

          if (*str == *s && !strcmp (str + 1, s + 1))
            return &wordlist[key];
        }
    }
  return 0;
}
#line 36 "irc_msgs.gperf"

