#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "../src/irc.h"
#include "../src/chat_conn.h"

#define MAX_LINES 1000
#define MAX_LENGTH 8196

int main() {
    FILE *file = fopen("ircbench.txt", "r");
    if (file == NULL) {
        printf("Failed to open the file.\n");
        return 1;
    }

    char lines[MAX_LINES][MAX_LENGTH];
    int line_count = 0;

    while (line_count < MAX_LINES && fgets(lines[line_count], sizeof(lines[line_count]), file) != NULL) {
        int length = strlen(lines[line_count]);

        if (length >= 2) {
            lines[line_count][length-1] = '\r';
            lines[line_count][length] = '\n';
            lines[line_count][length+1] = '\0';
        }

        line_count++;
    }

    fclose(file);

    struct irc_msg msg;
    size_t cursor = 0;

    clock_t start_time = clock();
    // Print the lines
    for (int i = 0; i < line_count; i++) {
        cursor = 0;
        if (parse_irc_buffer_step(lines[i], &cursor, &msg) < 0)
        {
            printf("%s", lines[i]);
        }
    }
    clock_t end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("last msg: (%s) %s (%s): %s\n", msg.channel, msg.prefix.username, msg.tags[IRC_TAG_USER_ID], msg.params);
    printf("(%d lines) Elapsed time: %f seconds\n", line_count, elapsed_time);

    return 0;
}