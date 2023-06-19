#ifndef COMMAND_TABLE_H
#define COMMAND_TABLE_H

#include "irc.h"
#include "stdbool.h"

#define COMMAND_MAIN_FUNCTION "command_main"
typedef int (*command_func_t)(struct irc_msg *msg, char *response);

struct command_link {
    void *dlhandle; // handle returned from dlopen()
    command_func_t func;
};

struct cmd_entry {
    char *name;
    struct command_link cmd;
};

struct cmd_table {
    struct cmd_entry *entries;
    size_t size;
    size_t capacity;
};

int
cmd_table_init(struct cmd_table *table, size_t capacity);

struct command_link *
cmd_table_get(struct cmd_table *table, char *name);

int
cmd_table_set(struct cmd_table *table, char *name, struct command_link* cmd);

int
cmd_table_destroy(struct cmd_table *table);

struct cmd_table_iter {
    const char *name;
    struct command_link *cmd;

    struct cmd_table *_table;
    size_t _index;
};

struct cmd_table_iter
cmd_table_iterator(struct cmd_table *table);

bool
cmd_table_next(struct cmd_table_iter *it);

#endif