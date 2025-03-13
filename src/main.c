#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

// returns the correct, length-guaranteed errr message
char* getErrrMsg(int errr_code) {
    char* errr_list[7] = {
        "[ERRR] Usage: ./main <message>\n",
        "[ERRR] Message too long. Maximum size allowed: 255\n",
        "[ERRR] Message must start with character '*'. Format: [*CCCC:par,par##]\n",
        "[ERRR] Command code may only contain 4 uppercase letters. Format: [*CCCC:par,par##]\n",
        "[ERRR] The 5th character has to be ':'. Format: [*CCCC:par,par##]\n",
        "[ERRR] The last two characters have to be '#'. Format: [*CCCC:par,par##]\n",
        "[ERRR] Invalid command. Command List: [ASKI, ASKA, SETG]\n"
    };

    // loop through error messages, guarantee all are below the max length of 127
    int errr_list_size = sizeof(errr_list) / sizeof(*errr_list);
    for (int i = 0; i < errr_list_size; i++) {
        if (strlen(errr_list[i]) > 126) { // 126, +1 for null terminator
            printf("[ERRR] Errr list contains string literal, where length is incorrect!\n");
            exit(1);
        }
    }

    return errr_list[errr_code];
}

// guarantee sure messages are properly received by the user
int paramCheck(int argc, char** argv, char** message) {
    // basic usage checks
    if (argc < 2) {
        printf("%s", getErrrMsg(0));
        return 1;
    }
    if (argv[1] == NULL) {
        printf("%s", getErrrMsg(0));
        return 1;
    }

    // messages can be only 255 long +1 for null terminator
    const int len = strlen(argv[1]);
    if (len > 255) {
        printf("%s", getErrrMsg(1));
        return 1;
    } else {
        *message = strdup(argv[1]);
    }

    // messages starts with *
    // NOTE: as operator precedence thinks i wanna do *(message[0]) => instead of (*message)[0], which is how we access the chars properly in this case
    if ((*message)[0] != '*') {
        printf("%s", getErrrMsg(2));
        return 1;
    }

    // CCCC can only be uppercase letters, guarantee its exactly 4 chars long too
    for (int i = 1; i < 5; i++) {
        if (isupper((*message)[i]) == 0 || isdigit((*message)[i]) != 0) {
            printf("%s", getErrrMsg(3));
            return 1;
        }
    }

    // : after CCCC
    if ((*message)[5] != ':') {
        printf("%s", getErrrMsg(4));
        return 1;
    }

    // 255 -> #
    // 254 -> #
    // 256 -> \0
    // last two chars is #
    if ((*message)[strlen(*message) - 1] != '#' || (*message)[strlen(*message) - 2] != '#') {
        printf("%s", getErrrMsg(5));
        return 1;
    }

    // set last char to null terminator
    (*message)[strlen(*message)] = '\0';

    return 0;
}

int main(int argc, char** argv) {

    // format check message
    char* message = NULL;
    int rc = paramCheck(argc, argv, &message);
    if (rc == 1) {
        return 1;
    }

    // check if parameter is valid command
    char* command_list[3] = { "ASKI", "ASKA", "SETG" };
    char* message_cmd = malloc((sizeof(char) * 4) + 1);
    for (int i = 1; i < 5; i++) {
        message_cmd[i - 1] = message[i];
    }
    int command_list_size = sizeof(command_list) / sizeof(*command_list);
    for (int i = 0; i < command_list_size; i++) {
        rc = strcmp(message_cmd, command_list[i]);
        if (rc == 0) {
            break;
        }

        if (i == command_list_size - 1) {
            printf("%s", getErrrMsg(6));
            return 1;
        }
    }

    // check for hex value = >isxdigit();

    // process message

    // send response
    free(message);
    free(message_cmd);
    return 0;
}
