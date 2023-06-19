#include "command_table.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

/* 
 * FNV-1a hash algorithm
 * https://en.wikipedia.org/wiki/Fowler–Noll–Vo_hash_function
 */
#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

static int64_t
hash_key(const char *name)
{
    uint64_t hash = FNV_OFFSET;
    for (const char *p = name; *p; p++)
    {
        hash ^= (uint64_t)(unsigned char)(*p);
        hash *= FNV_PRIME;
    }
    return hash;
}

static int
cmd_table_set_entry(struct cmd_entry *entries, size_t capacity,
                    const char* name, struct command_link* cmd, size_t *psize)
{
    uint64_t hash = hash_key(name);
    size_t index = (size_t)(hash & (uint64_t)(capacity - 1));

    while (entries[index].name != NULL)
    {
        if (strcmp(name, entries[index].name) == 0)
        {
            entries[index].cmd = *cmd;
            return 0;
        }

        index++;
        if (index >= capacity)
        {
            index = 0;
        }
    }

    if (psize != NULL)
    {
        name = strdup(name);
        if (name == NULL)
        {
            return -1;
        }
        (*psize)++;
    }

    entries[index].name = (char*)name;
    entries[index].cmd = *cmd;
    return 0;
}

static int cmd_table_expand(struct cmd_table *table)
{
    size_t new_cap = table->capacity * 2;
    if (new_cap < table->capacity)
    {
        return -1;
    }

    struct cmd_entry *new_entries = calloc(new_cap, sizeof(struct cmd_entry));
    if (new_entries == NULL)
    {
        return -1;
    }

    for (size_t i = 0; i < table->capacity; i++) {
        struct cmd_entry entry = table->entries[i];
        if (entry.name != NULL) {
            cmd_table_set_entry(new_entries, new_cap , entry.name,
                         &entry.cmd, NULL);
        }
    }

    free(table->entries);
    table->entries = new_entries;
    table->capacity = new_cap;
    return 0;
}


int
cmd_table_init(struct cmd_table *table, size_t capacity)
{
    table->size = 0;
    table->capacity = capacity;
    table->entries = calloc(capacity, sizeof(struct cmd_entry));
    if (table->entries == NULL)
    {
        fprintf(stderr, "Failed to initalize cmd_table\n");
        return -1;
    }

    return 0;
}

struct command_link *
cmd_table_get(struct cmd_table *table, char *name)
{
    uint64_t hash = hash_key(name);
    size_t index = (size_t)(hash & (uint64_t)(table->capacity - 1));
    while (table->entries[index].name != NULL)
    {
        if (strcmp(name, table->entries[index].name) == 0)
        {
            return &table->entries[index].cmd;
        }

        index++;
        if (index >= table->capacity)
        {
            index = 0;
        }
    }

    return NULL;
}

int
cmd_table_set(struct cmd_table *table, char *name, struct command_link* cmd)
{
    if (cmd == NULL)
    {
        return -1;
    }

    if (table->size >= table->capacity / 2)
    {
        if (!cmd_table_expand(table))
        {
            return -1;
        }
    }

    return cmd_table_set_entry(table->entries, table->capacity,
                               name, cmd, &table->size);
}

int
cmd_table_destroy(struct cmd_table *table)
{
    for (size_t i = 0; i < table->capacity; i++)
    {
        free(table->entries[i].name);
    }

    free(table->entries);
    return 0;
}

struct cmd_table_iter
cmd_table_iterator(struct cmd_table *table)
{
    struct cmd_table_iter it;
    it._table = table;
    it._index = 0;
    return it;
}

bool
cmd_table_next(struct cmd_table_iter *it)
{
    struct cmd_table *table = it->_table;
    while (it->_index < table->capacity) {
        size_t i = it->_index;
        it->_index++;
        if (table->entries[i].name != NULL) {
            // Found next non-empty item, update iterator key and value.
            it->name = table->entries[i].name;
            it->cmd = &table->entries[i].cmd;
            return true;
        }
    }
    return false;
}