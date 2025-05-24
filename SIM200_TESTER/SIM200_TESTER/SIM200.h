#pragma once
#include "common.h"

#define IMD_RESPONSE 0x0A100100
#define HOST_REQUEST 0x0A100100

// Data Array Size
#define PART_NAME_SIZE 4
#define FW_INFO_SIZE 3
#define SERIAL_SIZE 4

// Manufacturer Data
#define FUNC_GET_PART_NAME_0     0x01
#define FUNC_GET_PART_NAME_1     0x02
#define FUNC_GET_PART_NAME_2     0x03
#define FUNC_GET_PART_NAME_3     0x04
#define FUNC_GET_FW_VERSION      0x05
#define FUNC_GET_FW_INFO_0       0x06
#define FUNC_GET_FW_INFO_1       0x07
#define FUNC_GET_FW_INFO_2       0x08
#define FUNC_GET_SERIAL_0        0x09
#define FUNC_GET_SERIAL_1        0x0A
#define FUNC_GET_SERIAL_2        0x0B
#define FUNC_GET_SERIAL_3        0x0C

// Measurements
#define FUNC_GET_VNEG            0x60
#define FUNC_GET_VPOS            0x61
#define FUNC_GET_VEXC            0x62
#define FUNC_GET_VBATT           0x63
#define FUNC_GET_VPOWER          0x64
#define FUNC_GET_TEMP            0x65
#define FUNC_GET_VCAL_NEG        0x66
#define FUNC_GET_VCAL_POS        0x67
#define FUNC_GET_UPTIME          0x6F

// Electrical isolation
#define FUNC_GET_ISO_STATE       0xE0
#define FUNC_GET_ISO_RESISTANCE  0xE1
#define FUNC_GET_Y_CAP           0xE2
#define FUNC_GET_HV_MEASURE      0xE3
#define FUNC_GET_BATTERY_V       0xE4
#define FUNC_GET_ERROR_FLAGS     0xE5
#define FUNC_GET_DYN_ISO         0xE6
#define FUNC_GET_DYN_CAP         0xE7

// Configuration
#define FUNC_GET_MAX_VOLTAGE     0xF0
#define FUNC_GET_ISO_THRESHOLD   0xF1

// Commands
#define FUNC_RESET               0xC1
#define FUNC_DISABLE             0xC2
#define FUNC_SETUP_MODE          0xC3


array <string, PART_NAME_SIZE> part_name;
array <string, FW_INFO_SIZE> version_info;

string SIM200_part_name;
string SIM200_fw_version;

uint32_t SIM200_serial_number[4];
int32_t SIM200_vnes_hi_res;

template <size_t N>
void append_part_data(array<string, N>& target_array, int part_index, const TPCANMsg& msg) {
    if (part_index < 0 || part_index >= static_cast<int>(target_array.size())) {
        cerr << "Àß¸øµÈ part index: " << part_index << endl;
        return;
    }
    
    string& dest = target_array[part_index];
    if (!dest.empty()) dest.clear();
    for (int i = 1; i < 8; ++i) {
        if (msg.DATA[i] != 0x00)
            dest += static_cast<char>(msg.DATA[i]);
    }
}

typedef enum Endian{
    big_endian,
    little_endian
};

template <typename T>
T parseBytesToInt(const uint8_t* data, size_t length, Endian endian = Endian::Big) {
    static_assert(std::is_integral<T>::value, "T must be an integral type");

    if (length < 1 || length > 4)
        throw invalid_argument("Length must be between 1 and 4 bytes");

    uint32_t value = 0;

    if (endian == Endian::big_endian) {
        for (size_t i = 0; i < length; ++i) {
            value = (value << 8) | data[i];
        }
    }
    else { // Endian::little_endian
        for (size_t i = 0; i < length; ++i) {
            value |= (data[i] << (8 * i));
        }
    }

    return static_cast<T>(value);
}


