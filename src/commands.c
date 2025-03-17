#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "commands.h"
#include "errors.h"

// returns ASKI response
int executeCodeAski(char** argv) {
    // ensure empty param list
    if (argv[1][6] != '#') {
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
int executeCodeAska(char** argv) {
    // ensure empty param list
    if (argv[1][6] != '#') {
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

// returns SETG response
int executeCodeSetg(char** argv) {
    // ensure that param list cant be empty
    if (argv[1][6] == '#') {
        printf("%s", getErrrMsg(11));
        return 1;
    } else {
        char hex_input[3] = { argv[1][6], argv[1][7], '\0' };

        // !SETG:00##\0
        // 012345678910
        char response[11] = "!SETG:";

        bool valid_input = false;
        char* allowed_list[21] = { "00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "0A", "0B", "0C", "0D", "0E", "0F", "10", "11", "12", "13", "14" };
        const size_t allowed_list_size = sizeof(allowed_list) / sizeof(*allowed_list);
        for (size_t i = 0; i < allowed_list_size; i++) {
            if (strcmp(hex_input, allowed_list[i]) == 0) {
                valid_input = true;
            }
        }
        if (valid_input == true) {
            response[6] = hex_input[0];
            response[7] = hex_input[1];
            response[8] = '#';
            response[9] = '#';
        } else {
            printf("%s", getErrrMsg(12));
            return 1;
        }

        response[sizeof(response) - 1] = '\0';

        printf("%s\n", response);
    }

    return 0;
}
