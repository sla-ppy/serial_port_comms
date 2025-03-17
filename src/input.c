#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "errors.h"

// guarantee messages are properly received from the user
int isValidInput(int argc, char** ReceiveBuffer, char* msg_cmd, const size_t msg_cmd_size, char** cmd_list, const size_t cmd_list_size) {
    // basic usage checks
    if (argc < 2) {
        printf("%s", getErrrMsg(2));
        return 1;
    }
    if (ReceiveBuffer[1] == NULL) {
        printf("%s", getErrrMsg(2));
        return 1;
    }

    // messages can be only 255 long +1 for null terminator
    const int len = strlen(ReceiveBuffer[1]);
    if (len > 255) {
        printf("%s", getErrrMsg(3));
        return 1;
    }

    // messages starts with *
    // NOTE: as operator precedence thinks i wanna do *(message[0]) => instead of (*message)[0], which is how we access the chars properly in this case
    if (ReceiveBuffer[1][0] != '*') {
        printf("%s", getErrrMsg(4));
        return 1;
    }

    // CCCC can only be uppercase letters, guarantee its exactly 4 chars long
    for (int i = 1; i < 5; i++) {
        if (isupper(ReceiveBuffer[1][i]) == 0 || isdigit(ReceiveBuffer[1][i]) != 0) {
            printf("%s", getErrrMsg(5));
            return 1;
        }
    }

    // : after CCCC
    if (ReceiveBuffer[1][5] != ':') {
        printf("%s", getErrrMsg(6));
        return 1;
    }

    // NOTE: fgets() and getline() both required me to use these indexes and they both end each line by including \n and \0
    // depending on how data is read into ReceiveBuffer, my solution might fail here!
    // if so, set [arg_size -1] to [arg_size -2] and [arg_size -2] to [arg_size -3]

    // last two chars is #
    size_t arg_size = strlen(ReceiveBuffer[1]);
    if (ReceiveBuffer[1][arg_size - 1] != '#' || ReceiveBuffer[1][arg_size - 2] != '#') {
        printf("%s", getErrrMsg(7));
        return 1;
    }

    // reassign message to cmd for an easier time operating on it
    for (int i = 1; i < 5; i++) {
        msg_cmd[i - 1] = ReceiveBuffer[1][i];
    }

    // set last char to null terminator
    msg_cmd[msg_cmd_size - 1] = '\0';

    for (size_t i = 0; i < cmd_list_size; i++) {
        int rc = strcmp(msg_cmd, cmd_list[i]);
        if (rc == 0) {
            break;
        }

        if (i == cmd_list_size - 1) {
            printf("%s", getErrrMsg(8));
            return 1;
        }
    }

    return 0;
}
