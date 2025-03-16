#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "commands.h"
#include "input.h"

int main(int argc, char** argv) {
    // check formatting for message
    char* message = NULL;
    char* message_cmd = NULL;
    int rc = isValidCommand(argc, argv, &message, &message_cmd);
    if (rc == 1) {
        return 1;
    }

    // process and send response
    if (strcmp(message_cmd, "ASKI") == 0) {
        rc = executeCodeAski(&message);
        if (rc == 1) {
            return 1;
        }
    } else if (strcmp(message_cmd, "ASKA") == 0) {
        rc = executeCodeAska(&message);
        if (rc == 1) {
            return 1;
        }
    } else if (strcmp(message_cmd, "SETG") == 0) {
        rc = executeCodeSetg(&message);
        if (rc == 1) {
            return 1;
        }
    }

    free(message);
    free(message_cmd);
    return 0;
}
