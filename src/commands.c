#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "commands.h"
#include "errors.h"

// returns ASKI response
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

// returns ASKA response
int executeCodeAska(char** message) {
    if ((*message)[6] != '#') {
        printf("%s", getErrrMsg(10));
        return 1;
    } else {
        const char* temperature = "0.123456"; // float
        const char* supply_voltage = "11.7"; // float
        const unsigned int alarms = 0x23; // HEX value
        const unsigned int gain = 0x5; // HEX value

        printf("!ASKA:%s,%s,%.2X,%.2X##\n", temperature, supply_voltage, alarms, gain);
    }

    return 0;
}

typedef enum { true,
    false } bool;

// returns SETG response
int executeCodeSetg(char** message) {
    if ((*message)[6] == '#') {
        printf("%s", getErrrMsg(11));
        return 1;
    } else {
        char* hex_input = (char*)malloc(3);
        if (hex_input == NULL) {
            free(hex_input);
            return 1;
        }
        hex_input[0] = (*message)[6];
        hex_input[1] = (*message)[7];
        hex_input[strlen(hex_input)] = '\0';

        const char* resp_beg = "!SETG:";
        const char* resp_end = "##\n";
        char* full_resp = (char*)malloc(strlen(resp_beg) + (sizeof(char) * 2) + strlen(resp_end) + 1);
        if (full_resp == NULL) {
            free(hex_input);
            free(full_resp);
            return 1;
        }

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
    }

    return 0;
}
