/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf -t --output-file=irc_tag_table.h irc_tags.gperf  */
/* Computed positions: -k'2,$' */

#ifndef IRC_MSG_TAG_TABLE_H
#define IRC_MSG_TAG_TABLE_H

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

#line 1 "irc_tags.gperf"

#include <string.h>
#line 4 "irc_tags.gperf"
struct irc_tag_keyword {
    const char *name;
    enum irc_tag_index value;
};

#define TAG_TOTAL_KEYWORDS 40
#define TAG_MIN_WORD_LENGTH 2
#define TAG_MAX_WORD_LENGTH 25
#define TAG_MIN_HASH_VALUE 3
#define TAG_MAX_HASH_VALUE 65
/* maximum key range = 63, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
hash_tag (register const char *str, register size_t len)
{
  static unsigned char asso_values[] =
    {
      66, 66, 66, 66, 66, 66, 66, 66, 66, 66,
      66, 66, 66, 66, 66, 66, 66, 66, 66, 66,
      66, 66, 66, 66, 66, 66, 66, 66, 66, 66,
      66, 66, 66, 66, 66, 66, 66, 66, 66, 66,
      66, 66, 66, 66, 66, 66, 66, 66, 66, 66,
      66, 66, 66, 66, 66, 66, 66,  0, 66, 66,
      66, 66, 66, 66, 66, 66, 66, 66, 66, 66,
      66, 66, 66, 66, 66, 66, 66, 66, 66, 66,
      66, 66, 66, 66, 66, 66, 66, 66, 66, 66,
      66, 66, 66, 66, 66, 66, 66,  0, 66, 66,
      10,  0, 66,  0,  0, 45, 66,  0,  0, 30,
      20, 25,  0, 66, 30,  0, 66, 25, 66,  0,
      66,  5, 66, 66, 66, 66, 66, 66, 66, 66,
      66, 66, 66, 66, 66, 66, 66, 66, 66, 66,
      66, 66, 66, 66, 66, 66, 66, 66, 66, 66,
      66, 66, 66, 66, 66, 66, 66, 66, 66, 66,
      66, 66, 66, 66, 66, 66, 66, 66, 66, 66,
      66, 66, 66, 66, 66, 66, 66, 66, 66, 66,
      66, 66, 66, 66, 66, 66, 66, 66, 66, 66,
      66, 66, 66, 66, 66, 66, 66, 66, 66, 66,
      66, 66, 66, 66, 66, 66, 66, 66, 66, 66,
      66, 66, 66, 66, 66, 66, 66, 66, 66, 66,
      66, 66, 66, 66, 66, 66, 66, 66, 66, 66,
      66, 66, 66, 66, 66, 66, 66, 66, 66, 66,
      66, 66, 66, 66, 66, 66, 66, 66, 66, 66,
      66, 66, 66, 66, 66, 66
    };
  return len + asso_values[(unsigned char)str[1]] + asso_values[(unsigned char)str[len - 1]];
}

struct irc_tag_keyword *
irc_tag_str_to_index(register const char *str, register size_t len)
{
  static struct irc_tag_keyword wordlist[] =
    {
      {""}, {""}, {""},
#line 41 "irc_tags.gperf"
      {"r9k", IRC_TAG_R9K},
#line 43 "irc_tags.gperf"
      {"slow", IRC_TAG_SLOW},
#line 32 "irc_tags.gperf"
      {"flags", IRC_TAG_FLAGS},
#line 11 "irc_tags.gperf"
      {"badges", IRC_TAG_BADGES},
#line 35 "irc_tags.gperf"
      {"sent-ts", IRC_TAG_SENT_TS},
      {""},
#line 28 "irc_tags.gperf"
      {"user-type", IRC_TAG_USER_TYPE},
      {""}, {""},
#line 34 "irc_tags.gperf"
      {"client-nonce", IRC_TAG_CLIENT_NONCE},
      {""}, {""},
#line 46 "irc_tags.gperf"
      {"system-msg", IRC_TAG_SYSTEM_MSG},
#line 45 "irc_tags.gperf"
      {"msg-id", IRC_TAG_MSG_ID},
#line 27 "irc_tags.gperf"
      {"user-id", IRC_TAG_USER_ID},
      {""},
#line 48 "irc_tags.gperf"
      {"thread-id", IRC_TAG_THREAD_ID},
#line 49 "irc_tags.gperf"
      {"message-id", IRC_TAG_MESSAGE_ID},
      {""},
#line 16 "irc_tags.gperf"
      {"id", IRC_TAG_ID},
#line 39 "irc_tags.gperf"
      {"target-msg-id", IRC_TAG_TARGET_MSG_ID},
#line 37 "irc_tags.gperf"
      {"target-user-id", IRC_TAG_TARGET_USER_ID},
#line 21 "irc_tags.gperf"
      {"reply-parent-display-name", IRC_TAG_REPLY_PARENT_DISPLAY_NAME},
#line 22 "irc_tags.gperf"
      {"reply-parent-msg-body", IRC_TAG_REPLY_PARENT_MSG_BODY},
      {""}, {""},
#line 18 "irc_tags.gperf"
      {"reply-parent-msg-id", IRC_TAG_REPLY_PARENT_MSG_ID},
#line 19 "irc_tags.gperf"
      {"reply-parent-user-id", IRC_TAG_REPLY_PARENT_USER_ID},
      {""},
#line 36 "irc_tags.gperf"
      {"ban-duration", IRC_TAG_BAN_DURATION},
      {""}, {""},
#line 10 "irc_tags.gperf"
      {"badge-info", IRC_TAG_BADGE_INFO},
#line 15 "irc_tags.gperf"
      {"emotes", IRC_TAG_EMOTES},
      {""},
#line 17 "irc_tags.gperf"
      {"mod", IRC_TAG_MOD},
#line 44 "irc_tags.gperf"
      {"subs-only", IRC_TAG_SUBS_ONLY},
#line 47 "irc_tags.gperf"
      {"emote-sets", IRC_TAG_EMOTE_SETS},
#line 25 "irc_tags.gperf"
      {"tmi-sent-ts", IRC_TAG_TMI_SENT_TS},
#line 23 "irc_tags.gperf"
      {"room-id", IRC_TAG_ROOM_ID},
#line 20 "irc_tags.gperf"
      {"reply-parent-user-login", IRC_TAG_REPLY_PARENT_USER_LOGIN},
#line 40 "irc_tags.gperf"
      {"followers-only", IRC_TAG_FOLLOWERS_ONLY},
#line 33 "irc_tags.gperf"
      {"emote-only", IRC_TAG_EMOTE_ONLY},
      {""},
#line 30 "irc_tags.gperf"
      {"returning-chatter", IRC_TAG_RETURNING_CHATTER},
#line 29 "irc_tags.gperf"
      {"vip", IRC_TAG_VIP},
#line 12 "irc_tags.gperf"
      {"bits", IRC_TAG_BITS},
#line 38 "irc_tags.gperf"
      {"login", IRC_TAG_LOGIN},
      {""},
#line 42 "irc_tags.gperf"
      {"rituals", IRC_TAG_RITUALS},
      {""},
#line 31 "irc_tags.gperf"
      {"first-msg", IRC_TAG_FIRST_MSG},
#line 26 "irc_tags.gperf"
      {"turbo", IRC_TAG_TURBO},
      {""},
#line 14 "irc_tags.gperf"
      {"display-name", IRC_TAG_DISPLAY_NAME},
      {""}, {""},
#line 13 "irc_tags.gperf"
      {"color", IRC_TAG_COLOR},
      {""}, {""}, {""}, {""},
#line 24 "irc_tags.gperf"
      {"subscriber", IRC_TAG_SUBSCRIBER}
    };

  if (len <= TAG_MAX_WORD_LENGTH && len >= TAG_MIN_WORD_LENGTH)
    {
      register unsigned int key = hash_tag (str, len);

      if (key <= TAG_MAX_HASH_VALUE)
        {
          register const char *s = wordlist[key].name;

          if (*str == *s && !strcmp (str + 1, s + 1))
            return &wordlist[key];
        }
    }
  return 0;
}
#line 50 "irc_tags.gperf"

#endif
