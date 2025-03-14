#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

// returns the correct, length-guaranteed errr message
const char* getErrrMsg(const int errr_code) {
    const char* errr_list[11] = {
        "[ERRR] Errr list contains string literal where length is incorrect!\n",
        "[ERRR] Usage: ./main <message>\n",
        "[ERRR] Message too long. Maximum size allowed: 255\n",
        "[ERRR] Message must start with character '*'. Format: [*CCCC:param##]\n",
        "[ERRR] Command code may only contain 4 uppercase letters. Format: [*CCCC:param##]\n",
        "[ERRR] The 5th character has to be ':'. Format: [*CCCC:param##]\n",
        "[ERRR] The last two characters have to be '#'. Format: [*CCCC:param##]\n",
        "[ERRR] Invalid command. Command List: [ASKI | ASKA | SETG]\n",
        "[ERRR] ASKI must have an empty param list! Format: [*ASKI:##]\n",
        "[ERRR] ASKA must have an empty param list! Format: [*ASKA:##]\n",
        "[ERRR] SETG must contain gain value in HEX format! Format: [*SETG:07##]\n"
    };

    // loop through error messages, guarantee all are below the max length of 127
    const int errr_list_size = sizeof(errr_list) / sizeof(*errr_list);
    for (int i = 0; i < errr_list_size; i++) {
        if (strlen(errr_list[i]) > 126) { // 126, +1 for null terminator
            printf("%s", errr_list[0]);
            exit(1);
        }
    }

    // TODO: implement "Error description cannot contain the ',' (comma) separator character"
    //  "[ERRR] Errr list contains string with a comma character!\n",

    return errr_list[errr_code];
}

// guarantee sure messages are properly received by the user
int paramCheck(int argc, char** argv, char** message) {
    // basic usage checks
    if (argc < 2) {
        printf("%s", getErrrMsg(1));
        return 1;
    }
    if (argv[1] == NULL) {
        printf("%s", getErrrMsg(1));
        return 1;
    }

    // messages can be only 255 long +1 for null terminator
    const int len = strlen(argv[1]);
    if (len > 255) {
        printf("%s", getErrrMsg(2));
        return 1;
    } else {
        *message = strdup(argv[1]);
    }

    // messages starts with *
    // NOTE: as operator precedence thinks i wanna do *(message[0]) => instead of (*message)[0], which is how we access the chars properly in this case
    if ((*message)[0] != '*') {
        printf("%s", getErrrMsg(3));
        return 1;
    }

    // CCCC can only be uppercase letters, guarantee its exactly 4 chars long too
    for (int i = 1; i < 5; i++) {
        if (isupper((*message)[i]) == 0 || isdigit((*message)[i]) != 0) {
            printf("%s", getErrrMsg(4));
            return 1;
        }
    }

    // : after CCCC
    if ((*message)[5] != ':') {
        printf("%s", getErrrMsg(5));
        return 1;
    }

    // 255 -> #
    // 254 -> #
    // 256 -> \0
    // last two chars is #
    if ((*message)[strlen(*message) - 1] != '#' || (*message)[strlen(*message) - 2] != '#') {
        printf("%s", getErrrMsg(6));
        return 1;
    }

    // set last char to null terminator
    (*message)[strlen(*message)] = '\0';

    return 0;
}

int main(int argc, char** argv) {

    // check formatting for message
    char* message = NULL;
    int rc = paramCheck(argc, argv, &message);
    if (rc == 1) {
        return 1;
    }

    // check if parameter is valid command
    const char* command_list[3] = { "ASKI", "ASKA", "SETG" };
    char* message_cmd = (char*)malloc((sizeof(char) * 4) + 1);
    for (int i = 1; i < 5; i++) {
        message_cmd[i - 1] = message[i];
    }
    const int command_list_size = sizeof(command_list) / sizeof(*command_list);
    for (int i = 0; i < command_list_size; i++) {
        rc = strcmp(message_cmd, command_list[i]);
        if (rc == 0) {
            break;
        }

        if (i == command_list_size - 1) {
            printf("%s", getErrrMsg(7));
            return 1;
        }
    }

    // process response

    // send response
    if (strcmp(message_cmd, "ASKI") == 0) {
        if (message[6] != '#') {
            printf("%s", getErrrMsg(8));
            return 1;
        } else {
            const char* device_type = "BRTK12";
            const char* serial_number = "34";
            const char* manufacturing_date = "2022-11-23";
            const char* firmware_version = "v2.4";

            printf("!ASKI:%s,%s,%s,%s##\n", device_type, serial_number, manufacturing_date, firmware_version);
        }

    } else if (strcmp(message_cmd, "ASKA") == 0) {
        // may have entirely missed the point on encoding specs?
        // but why would i need to encode anything if ASKA doesn't take anything as input?
        // i got really confused here, went with my gut
        // i believe the real answer is that i have to take store ASCII strings then convert them by the encoding
        // and then return the results?

        if (message[6] != '#') {
            printf("%s", getErrrMsg(9));
            return 1;
        } else {
            const float temperature = 32.4; // float
            const float supply_voltage = 11.7; // float
            const unsigned int alarms = 0x23; // HEX value
            const unsigned int gain = 0x5; // HEX value

            printf("!ASKA:%.1f,%.1f,%.2X,%.2X##\n", temperature, supply_voltage, alarms, gain);
        }

    } else if (strcmp(message_cmd, "SETG") == 0) {
        char* hex_input = (char*)malloc((sizeof(char) * 2) + 1);
        hex_input[0] = message[6];
        hex_input[1] = message[7];
        hex_input[strlen(hex_input)] = '\0';

        if (message[6] == '#') {
            printf("%s", getErrrMsg(10));
            return 1;
        }

        const char* resp_beg = "!SETG:";
        const char* resp_end = "##\n";
        char* full_resp = (char*)malloc(strlen(resp_beg) + (sizeof(char) * 2) + strlen(resp_end) + 1);
        strcpy(full_resp, resp_beg);

        const char* two_chars_list[5] = { "10", "11", "12", "13", "14" };
        const int two_chars_list_size = sizeof(two_chars_list) / sizeof(*two_chars_list);
        for (int i = 0; i < two_chars_list_size; i++) {
            if (strcmp(hex_input, two_chars_list[i]) == 0) {
                full_resp[6] = message[6];
                full_resp[7] = message[7];
            }
        }

        // TODO: guarantee correct input range
        char* allowed_list[16] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F" };
        for (int i = 0; i < 2; i++) {
            for (int y = 0; y < 15; y++) {
                if (hex_input[i] != (*allowed_list)[y]) {
                    continue;
                } else {
                    printf("Matching char found: %c\n", (*allowed_list)[y]);
                    break;
                }
            }
        }
        strcat(full_resp, resp_end);
        full_resp[strlen(full_resp)] = '\0';

        printf("%s", full_resp);
        free(full_resp);

        free(hex_input);
    }

    // todo: guarantee an empty list on ASKI and ASKA

    // TODO: how to guarantee that we only use ASCII inside our program?
    // not sure how to implement it
    // gave it thought, and the suggestion was that if i was working with files,
    // i could look at the file header and make sure its ASCII encoded, but thats not an option here
    // LC_ALL if on POSIX is a thing

    free(message);
    free(message_cmd);
    return 0;
}
