#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int paramCheck(int argc, char** argv, char** request) {
    // basic usage checks
    if (argc < 2) {
        printf("[ERRR] Usage: ./main <request>\n");
        return 1;
    }
    if (argv[1] == NULL) {
        printf("[ERRR] Usage: ./main <request>\n");
        return 1;
    }

    // request can be only 255 long +1 for null terminator
    const int len = strlen(argv[1]);
    if (len > 255) {
        printf("[ERRR] Request message too long. Maximum size allowed: 255\n");
        return 1;
    } else {
        *request = strdup(argv[1]);
    }

    // request starts with *
    // NOTE: as operator precedence thinks i wanna do *(request[0]) => instead of (*request)[0], which is how we access the chars properly in this case
    if ((*request)[0] != '*') {
        printf("[ERRR] Request can only start with the character '*'. Format: [*CCCC:par,par##]\n");
        return 1;
    }

    // CCCC can only be uppercase letters, guarantee its exactly 4 chars long too
    for (int i = 1; i < 5; i++) {
        if (isupper((*request)[i]) == 0 || isdigit((*request)[i]) != 0) {
            printf("[ERRR] Command code may only contain 4 uppercase letters. Format: [*CCCC:par,par##]\n");
            return 1;
        }
    }

    // : after CCCC
    if ((*request)[5] != ':') {
        printf("[ERRR] The 5th character has to be ':'. Format: [*CCCC:par,par##]\n");
        return 1;
    }

    // 255 -> #
    // 254 -> #
    // 256 -> \0
    // last two chars is #
    if ((*request)[strlen(*request) - 1] != '#' || (*request)[strlen(*request) - 2] != '#') {
        printf("[ERRR] The last two characters have to be '#'. Format: [*CCCC:par,par##]\n");
        return 1;
    }

    // set last char to null terminator
    (*request)[strlen(*request)] = '\0';

    return 0;
}

int main(int argc, char** argv) {

    // format check request
    char* request = NULL;
    int rc = paramCheck(argc, argv, &request);
    if (rc == 1) {
        return 1;
    }

    printf("Request after ParamCheck: %s\n", request);
    free(request);

    // check for hex value = >isxdigit();

    // process request

    // send response
    return 0;
}
