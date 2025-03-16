#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "errors.h"

// guarantee messages are properly received from the user
int isValidCommand(int argc, char** argv, char** message, char** message_cmd) {
    // basic usage checks
    if (argc < 2) {
        printf("%s", getErrrMsg(2));
        return 1;
    }
    if (argv[1] == NULL) {
        printf("%s", getErrrMsg(2));
        return 1;
    }

    // messages can be only 255 long +1 for null terminator
    const int len = strlen(argv[1]);
    if (len > 255) {
        printf("%s", getErrrMsg(3));
        return 1;
    } else {
        *message = strdup(argv[1]);
    }

    // messages starts with *
    // NOTE: as operator precedence thinks i wanna do *(message[0]) => instead of (*message)[0], which is how we access the chars properly in this case
    if ((*message)[0] != '*') {
        printf("%s", getErrrMsg(4));
        return 1;
    }

    // CCCC can only be uppercase letters, guarantee its exactly 4 chars long
    for (int i = 1; i < 5; i++) {
        if (isupper((*message)[i]) == 0 || isdigit((*message)[i]) != 0) {
            printf("%s", getErrrMsg(5));
            return 1;
        }
    }

    // : after CCCC
    if ((*message)[5] != ':') {
        printf("%s", getErrrMsg(6));
        return 1;
    }

    // last two chars is #
    if ((*message)[strlen(*message) - 1] != '#' || (*message)[strlen(*message) - 2] != '#') {
        printf("%s", getErrrMsg(7));
        return 1;
    }

    // set last char to null terminator
    (*message)[strlen(*message)] = '\0';

    // guarantee that the message is a valid command from our list
    const char* command_list[3] = { "ASKI", "ASKA", "SETG" };
    *message_cmd = (char*)malloc((sizeof(char) * 4) + 1);
    for (int i = 1; i < 5; i++) {
        (*message_cmd)[i - 1] = (*message)[i];
    }
    const int command_list_size = sizeof(command_list) / sizeof(*command_list);
    for (int i = 0; i < command_list_size; i++) {
        int rc = strcmp(*message_cmd, command_list[i]);
        if (rc == 0) {
            break;
        }

        if (i == command_list_size - 1) {
            printf("%s", getErrrMsg(8));
            return 1;
        }
    }

    return 0;
}
