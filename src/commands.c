#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "commands.h"
#include "errors.h"

int executeCodeAski(char** message) {
    if ((*message)[6] != '#') {
        printf("%s", getErrrMsg(9));
        return 1;
    } else {
        const char* device_type = "BRTK12";
        const char* serial_number = "34";
        const char* manufacturing_date = "2022-11-23";
        const char* firmware_version = "v2.4";

        printf("!ASKI:%s,%s,%s,%s##\n", device_type, serial_number, manufacturing_date, firmware_version);
    }

    return 0;
}

int executeCodeAska(char** message) {
    // may have entirely missed the point on encoding specs?
    // but why would i need to encode anything if ASKA doesn't take anything as input?
    // i got really confused here, went with my gut
    // i believe the real answer is that i have to take store ASCII strings then convert them by the encoding
    // and then return the results?

    if ((*message)[6] != '#') {
        printf("%s", getErrrMsg(10));
        return 1;
    } else {
        const float temperature = 32.4; // float
        const float supply_voltage = 11.7; // float
        const unsigned int alarms = 0x23; // HEX value
        const unsigned int gain = 0x5; // HEX value

        printf("!ASKA:%.1f,%.1f,%.2X,%.2X##\n", temperature, supply_voltage, alarms, gain);
    }

    return 0;
}

typedef enum { true,
    false } bool;

int executeCodeSetg(char** message) {
    char* hex_input = (char*)malloc((sizeof(char) * 2) + 1);
    hex_input[0] = (*message)[6];
    hex_input[1] = (*message)[7];
    hex_input[strlen(hex_input)] = '\0';

    if ((*message)[6] == '#') {
        printf("%s", getErrrMsg(11));
        return 1;
    }

    const char* resp_beg = "!SETG:";
    const char* resp_end = "##\n";
    char* full_resp = (char*)malloc(strlen(resp_beg) + (sizeof(char) * 2) + strlen(resp_end) + 1);
    strcpy(full_resp, resp_beg);

    bool valid_input = false;
    char* allowed_list[21] = { "00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "0A", "0B", "0C", "0D", "0E", "0F", "10", "11", "12", "13", "14" };
    const int allowed_list_size = sizeof(allowed_list) / sizeof(*allowed_list);
    for (int i = 0; i < allowed_list_size; i++) {
        if (strcmp(hex_input, allowed_list[i]) == 0) {
            valid_input = true;
        }
    }
    if (valid_input == true) {
        full_resp[6] = hex_input[0];
        full_resp[7] = hex_input[1];
    } else {
        printf("%s", getErrrMsg(12));
        return 1;
    }
    free(hex_input);

    strcat(full_resp, resp_end);
    full_resp[strlen(full_resp)] = '\0';

    printf("%s", full_resp);
    free(full_resp);

    return 0;
}
