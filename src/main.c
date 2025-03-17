#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "commands.h"
#include "input.h"

int main(int argc, char** argv) {
    // check formatting for message
    char msg_cmd[5];
    const size_t msg_cmd_size = sizeof(msg_cmd);

    char* cmd_list[3] = { "ASKI", "ASKA", "SETG" };
    const size_t cmd_list_size = sizeof(cmd_list) / sizeof(*cmd_list);

    // while (1) {
    //  getc(stdin);

    FILE* fp;
    char* line = NULL;
    int bufferLength = 255;
    char buffer[bufferLength];

    fp = fopen("input.txt", "r");
    while (fgets(buffer, bufferLength, fp)) {
        argv[1] = buffer;
        printf("%s", buffer);

        int rc = isValidInput(argc, argv, msg_cmd, msg_cmd_size, cmd_list, cmd_list_size);
        if (rc == 1) {
            break;
        }

        // process and send response
        if (strcmp(msg_cmd, cmd_list[0]) == 0) {
            rc = executeCodeAski(argv);
            if (rc == 1) {
                break;
            }
        } else if (strcmp(msg_cmd, cmd_list[1]) == 0) {
            rc = executeCodeAska(argv);
            if (rc == 1) {
                break;
            }
        } else if (strcmp(msg_cmd, cmd_list[2]) == 0) {
            rc = executeCodeSetg(argv);
            if (rc == 1) {
                break;
            }
        }
    }

    free(line);
    fclose(fp);

    return 0;
}
