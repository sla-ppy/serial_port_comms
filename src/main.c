#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "commands.h"
#include "input.h"

int main(int argc, char** ReceiveBuffer) {
    // check formatting for message
    char msg_cmd[5];
    const size_t msg_cmd_size = sizeof(msg_cmd);

    char* cmd_list[3] = { "ASKI", "ASKA", "SETG" };
    const size_t cmd_list_size = sizeof(cmd_list) / sizeof(*cmd_list);

    size_t ReceiveBufferLen;

    while (1) {
        int rc = isValidInput(argc, ReceiveBuffer, msg_cmd, msg_cmd_size, cmd_list, cmd_list_size);
        if (rc == 1) {
            break;
        }

        // process and send response
        if (strcmp(msg_cmd, cmd_list[0]) == 0) {
            rc = executeCodeAski(ReceiveBuffer);
            ReceiveBufferLen = 0;
            if (rc == 1) {
                break;
            }
        } else if (strcmp(msg_cmd, cmd_list[1]) == 0) {
            rc = executeCodeAska(ReceiveBuffer);
            ReceiveBufferLen = 0;
            if (rc == 1) {
                break;
            }
        } else if (strcmp(msg_cmd, cmd_list[2]) == 0) {
            rc = executeCodeSetg(ReceiveBuffer);
            ReceiveBufferLen = 0;
            if (rc == 1) {
                break;
            }
        }
    }

    return 0;
}
