#include "command_handler.h"

#include "irc.h"
#include "chat_conn.h"
#include "command_table.h"

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <dlfcn.h>
#include <limits.h>

#define COMMAND_TABLE_INIT_SIZE 16

struct cmd_table cmdtable;

static void * 
command_handler_thread(void *tmsg)
{
    /* Copy the passed in msg so that the thread can take ownership of it
     * Sidenote: Could the main thread (command_handler) change the value of vmsg
     * before we copy it? (unlikely but possible?)
     */
    struct irc_msg *msg = (struct irc_msg*)tmsg;
    char response[MAX_IRC_PARAMS_LEN];
    char cmd_name[64];
    size_t i = 1;

    /* Get command name starting after the & symbol */
    for (; msg->params[i] != '\0' && msg->params[i] != ' '; i++);
    memcpy(cmd_name, &msg->params[1], i-1);
    cmd_name[i] = '\0';

    struct command_link *cmd = cmd_table_get(&cmdtable, cmd_name);
    if (cmd != NULL)
    {
        int ret = cmd->func(msg, response);
        if (ret < 0)
        {
            fprintf(stderr, "Command %s failed during execution\n", cmd_name);
        }

        chat_conn_send_msg(msg->channel, response);
    }

    free(msg);
    return NULL;
}

int
command_handler(struct irc_msg *msg)
{
    /* We can currently just copy like this 
     * because we only have statically allocated
     * structures in an irc_msg.
     * If we ever dynamically allocate we will need
     * to do a deep copy here.
     */
    pthread_t tid;
    int result;

    struct irc_msg *copy = malloc(sizeof(struct irc_msg));
    *copy = *msg;

    result = pthread_create(&tid, NULL, command_handler_thread, (void*)copy);
    if (result != 0)
    {
        return -1;
    }

    return 0;
}

static int
link_command_functions()
{
    DIR *dir;
    struct dirent *entry;
    // TODO: Make this an cli argument
    const char *cmd_folder = "./commands";

    dir = opendir(cmd_folder);
    if (dir == NULL)
    {
        fprintf(stderr, "Unable to open command folder directory: %s\n", cmd_folder);
        return -1;
    }

    char path[PATH_MAX + 1];
    while ((entry = readdir(dir)) != NULL)
    {
        char *filename = entry->d_name;

        size_t len = strlen(filename);
        if (len > 3 && strcmp(filename + len - 3, ".so") == 0)
        {
            snprintf(path, sizeof(path), "%s/%s", cmd_folder, filename);
            void *handle = dlopen(path, RTLD_LAZY);
            if (!handle)
            {
                fprintf(stderr, "Failed to open command library file: %s\n", path);
                continue;
            }

            command_func_t command_main = (command_func_t)dlsym(handle, COMMAND_MAIN_FUNCTION);
            if (!command_main)
            {
                fprintf(stderr, "Failed to retrieve the %s command_main function pointer: %s\n", filename, dlerror());
                dlclose(handle);
                continue;
            }

            /* The command name will be the file name without extension */
            filename[len - 3] = '\0';

            struct command_link cmd = {
                .dlhandle = handle,
                .func = command_main,
            };

            cmd_table_set(&cmdtable, filename, &cmd);
            // TODO: dlclose? store handle?
        }
    }

    closedir(dir);
    return 0;
}

/* The refresh command can't be implemented as a shared library like the rest
 * of the commands because of the fact that it would unload itself on refresh.
 */
static int
refresh_command_links(struct irc_msg *msg, char *response)
{
    int ret;

    /* Delete and unlink all previous command links */
    struct cmd_table_iter it = cmd_table_iterator(&cmdtable);
    while (cmd_table_next(&it))
    {
        if (it.cmd->dlhandle == NULL)
        {
            continue;
        }
        dlclose(it.cmd->dlhandle);
    }
    cmd_table_destroy(&cmdtable);

    // reinitalize with atleast previous size?
    ret = cmd_table_init(&cmdtable, COMMAND_TABLE_INIT_SIZE);
    if (ret < 0)
    {
        fprintf(stderr, "Failed to initialize cmd_table during refresh\n");
        return -1;
    }

    ret = link_command_functions();
    if (ret < 0)
    {
        fprintf(stderr, "Failed to dynamically link command functions during refresh\n");
        return -1;
    }

    /* Insert this function into the cmd_table since it is not a shared library */
    struct command_link cmd = {
        .dlhandle = NULL,
        .func = refresh_command_links,
    };
    cmd_table_set(&cmdtable, "refresh", &cmd);

    fprintf(stdout, "Linked commands: [\n");
    it = cmd_table_iterator(&cmdtable);
    while (cmd_table_next(&it))
    {
        fprintf(stdout, "  %s\n", it.name);
    }
    fprintf(stdout, "]\n");

    snprintf(response, MAX_IRC_PARAMS_LEN,
             "@%s, Successfully refreshed commands.",
             msg->prefix.username);

    return 0;
}

int
command_handler_init()
{
    int ret;

    /* Create hash table to store command name -> command function main ptr */
    ret = cmd_table_init(&cmdtable, COMMAND_TABLE_INIT_SIZE);
    if (ret < 0)
    {
        fprintf(stderr, "Failed to initialize cmd_table\n");
        return -1;
    }

    ret = link_command_functions();
    if (ret < 0)
    {
        fprintf(stderr, "Failed to dynamically link command functions\n");
        return -1;
    }

    /* Insert refresh function into the cmd_table since it is not a shared library */
    struct command_link cmd = {
        .dlhandle = NULL,
        .func = refresh_command_links,
    };
    cmd_table_set(&cmdtable, "refresh", &cmd);

    fprintf(stdout, "Linked commands: [\n");
    struct cmd_table_iter it = cmd_table_iterator(&cmdtable);
    while (cmd_table_next(&it))
    {
        fprintf(stdout, "  %s\n", it.name);
    }
    fprintf(stdout, "]\n");

    return 0;
}