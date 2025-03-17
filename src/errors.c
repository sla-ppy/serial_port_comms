#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "errors.h"

// returns length-guaranteed errr message, ensures lack of comma character in errr description
const char* getErrrMsg(const int errr_code) {
    const char* errr_list[13] = {
        "[ERRR] Errr list contains string literal where length is incorrect!\n\0", // 0
        "[ERRR] Errr list contains string with a comma character!\n\0", // 1
        "[ERRR] Usage: ./main <message>. Messages: [ASKI|ASKA|SETG]\n\0", // 2
        "[ERRR] Message too long. Maximum size allowed: 255\n\0", // 3
        "[ERRR] Message must start with character '*'. Format: [*CCCC:param##]\n\0", // 4
        "[ERRR] Command code may only contain 4 uppercase letters. Format: [*CCCC:param##]\n\0", // 5
        "[ERRR] The 5th character has to be ':'. Format: [*CCCC:param##]\n\0", // 6
        "[ERRR] The last two characters have to be '#'. Format: [*CCCC:param##]\n\0", // 7
        "[ERRR] Invalid command. Command List: [ASKI | ASKA | SETG]\n\0", // 8
        "[ERRR] ASKI must have an empty param list! Format: [*ASKI:##]\n\0", // 9
        "[ERRR] ASKA must have an empty param list! Format: [*ASKA:##]\n\0", // 10
        "[ERRR] SETG must contain gain value in HEX format! Format: [*SETG:07##]\n\0", // 11
        "[ERRR] SETG must get a HEX value between 0h and 20h with the correct format. Format: [*SETG:07##]\n\0" // 12
    };

    const size_t errr_list_size = sizeof(errr_list) / sizeof(*errr_list);
    for (size_t i = 0; i < errr_list_size; i++) {

        // make sure length is below 128
        int errr_msg_size = strlen(errr_list[i]);
        if (errr_msg_size > 126) { // 126, +1 for null terminator
            printf("%s", errr_list[0]);
            exit(1);
        }

        // check for comma
        for (int y = 0; y < errr_msg_size; y++) {
            if (errr_list[i][y] == ',') {
                printf("%s", errr_list[1]);
                exit(1);
            }
        }
    }

    return errr_list[errr_code];
}
