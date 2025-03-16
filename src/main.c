#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "commands.h"
#include "input.h"

int main(int argc, char** argv) {
    // check formatting for message
    char* message = NULL;
    char* message_cmd = NULL;

    char* command_list[3] = { "ASKI", "ASKA", "SETG" };

    int rc = isValidInput(argc, argv, &message, &message_cmd, command_list);
    if (rc == 1) {
        free(message);
        free(message_cmd);
        return 1;
    }

    // process and send response
    if (strcmp(message_cmd, command_list[0]) == 0) {
        rc = executeCodeAski(&message);
        if (rc == 1) {
            free(message);
            free(message_cmd);
            return 1;
        }
    } else if (strcmp(message_cmd, command_list[1]) == 0) {
        rc = executeCodeAska(&message);
        if (rc == 1) {
            free(message);
            free(message_cmd);
            return 1;
        }
    } else if (strcmp(message_cmd, command_list[2]) == 0) {
        rc = executeCodeSetg(&message);
        if (rc == 1) {
            free(message);
            free(message_cmd);
            return 1;
        }
    }

    free(message);
    free(message_cmd);
    return 0;
}
