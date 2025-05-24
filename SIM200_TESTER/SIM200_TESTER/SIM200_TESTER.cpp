#include "common.h"
#include "PCANBasic.h"
#include "SIM200.h"

using namespace std;

bool keepRunning = true;

// 종료 이벤트 핸들러 함수
BOOL WINAPI ConsoleHandler(DWORD signal) {
    switch (signal) {
    case CTRL_C_EVENT:
        cout << "\n[Ctrl+C] 종료 요청됨\n";
        break;
    case CTRL_CLOSE_EVENT:
        cout << "\n[콘솔창 X] 종료 요청됨\n";
        break;
    case CTRL_BREAK_EVENT:
        cout << "\n[Ctrl+Break] 종료 요청됨\n";
        break;
    case CTRL_LOGOFF_EVENT:
        cout << "\n[로그아웃] 종료 요청됨\n";
        break;
    case CTRL_SHUTDOWN_EVENT:
        cout << "\n[시스템 종료] 종료 요청됨\n";
        break;
    default:
        cout << "\n[기타 종료] 요청됨\n";
        break;
    }

    keepRunning = false;
    return TRUE;  // 처리 완료
}

int main() {
    // 종료 이벤트 핸들러 등록
    SetConsoleCtrlHandler(ConsoleHandler, TRUE);

    // CAN 초기화
    TPCANStatus status = CAN_Initialize(PCAN_USBBUS1, PCAN_BAUD_500K);
    if (status != PCAN_ERROR_OK) {
        cout << "CAN 초기화 실패" << endl;
        return -1;
    }

    cout << "CAN 초기화 성공!" << endl;
    cout << "Ctrl+C 또는 콘솔 X버튼을 눌러 종료하세요\n";

    // 수신 루프
    while (keepRunning) {
        TPCANMsg msg;
        status = CAN_Read(PCAN_USBBUS1, &msg, nullptr);

        if (status == PCAN_ERROR_QRCVEMPTY)
            continue;

        if (msg.ID != IMD_RESPONSE)
            continue;

        cout << "ID: 0x" << hex << msg.ID << " Data:";
        for (int i = 0; i < msg.LEN; ++i)
            cout << " " << hex << (int)msg.DATA[i];
        cout << endl;

        switch (msg.DATA[0]) {
            // Manufacturer Data
        case FUNC_GET_PART_NAME_0:
            cout << "Get part name (part 0)" << endl;
            append_part_data(part_name, 0, msg);
            break;

        case FUNC_GET_PART_NAME_1:
            cout << "Get part name (part 1)" << endl;
            append_part_data(part_name, 1, msg);
            break;

        case FUNC_GET_PART_NAME_2:
            cout << "Get part name (part 2)" << endl;
            append_part_data(part_name, 2, msg);
            break;

        case FUNC_GET_PART_NAME_3:
            cout << "Get part name (part 3)" << endl;
            append_part_data(part_name, 3, msg);
            
            SIM200_part_name = part_name[0] + part_name[1] + part_name[2] + part_name[3];
            cout << SIM200_part_name << endl;
            break;

        case FUNC_GET_FW_VERSION:
            cout << "Get FW version" << endl;
            SIM200_fw_version = to_string(msg.DATA[1]) + "." + to_string(msg.DATA[2]) + "." + to_string(msg.DATA[3]) + "." + to_string(msg.DATA[4]);
            cout << "SIM200_fw_version : " << SIM200_fw_version << endl;
            break;

        case FUNC_GET_FW_INFO_0:
            cout << "Get FW version info (part 0)" << endl;
            append_part_data(version_info, 0, msg);
            cout << version_info[0] << endl;
            break;

        case FUNC_GET_FW_INFO_1:
            cout << "Get FW version info (part 1)" << endl;
            append_part_data(version_info, 1, msg);
            cout << version_info[1] << endl;
            break;

        case FUNC_GET_FW_INFO_2:
            cout << "Get FW version info (part 2)" << endl;
            append_part_data(version_info, 2, msg);
            cout << version_info[2] << endl;
            break;

        case FUNC_GET_SERIAL_0:
            cout << "Get serial number (part 0)" << endl;
            SIM200_serial_number[0] = parseBytesToInt<uint32_t>(&msg.DATA[1], 4, big_endian);
            cout << to_string(SIM200_serial_number[0]) << endl;
            break;

        case FUNC_GET_SERIAL_1:
            cout << "Get serial number (part 1)" << endl;
            SIM200_serial_number[1] = parseBytesToInt<uint32_t>(&msg.DATA[1], 4, big_endian);
            cout << to_string(SIM200_serial_number[1]) << endl;
            break;

        case FUNC_GET_SERIAL_2:
            cout << "Get serial number (part 2)" << endl;
            SIM200_serial_number[2] = parseBytesToInt<uint32_t>(&msg.DATA[1], 4, big_endian);
            cout << to_string(SIM200_serial_number[2]) << endl;
            break;

        case FUNC_GET_SERIAL_3:
            cout << "Get serial number (part 3)" << endl;
            SIM200_serial_number[3] = parseBytesToInt<uint32_t>(&msg.DATA[1], 4, big_endian);
            cout << to_string(SIM200_serial_number[3]) << endl;
            break;

            // Measurements
        case FUNC_GET_VNEG:
            cout << "Get high-resolution Vneg" << endl;
            break;

        case FUNC_GET_VPOS:
            cout << "Get high-resolution Vpos" << endl;
            break;

        case FUNC_GET_VEXC:
            cout << "Get high-resolution Vexc" << endl;
            break;

        case FUNC_GET_VBATT:
            cout << "Get high-resolution Vbatt" << endl;
            break;

        case FUNC_GET_VPOWER:
            cout << "Get high-resolution Vpower" << endl;
            break;

        case FUNC_GET_TEMP:
            cout << "Get high-resolution temperature" << endl;
            break;

        case FUNC_GET_VCAL_NEG:
            cout << "Get high-resolution Vcal_neg" << endl;
            break;

        case FUNC_GET_VCAL_POS:
            cout << "Get high-resolution Vcal_pos" << endl;
            break;

        case FUNC_GET_UPTIME:
            cout << "Get uptime counter" << endl;
            break;

            // Electrical isolation
        case FUNC_GET_ISO_STATE:
            cout << "Get isolation state" << endl;
            break;

        case FUNC_GET_ISO_RESISTANCE:
            cout << "Get isolation resistances" << endl;
            break;

        case FUNC_GET_Y_CAP:
            cout << "Get system Y-capacitances" << endl;
            break;

        case FUNC_GET_HV_MEASURE:
            cout << "Get HV rail measurements" << endl;
            break;

        case FUNC_GET_BATTERY_V:
            cout << "Get battery voltage" << endl;
            break;

        case FUNC_GET_ERROR_FLAGS:
            cout << "Get error flags" << endl;
            break;

        case FUNC_GET_DYN_ISO:
            cout << "Get dynamic electrical isolation" << endl;
            break;

        case FUNC_GET_DYN_CAP:
            cout << "Get dynamic capacitive energy" << endl;
            break;

            // Configuration
        case FUNC_GET_MAX_VOLTAGE:
            cout << "Get max voltage setting" << endl;
            break;

        case FUNC_GET_ISO_THRESHOLD:
            cout << "Get isolation threshold settings" << endl;
            break;

            // Commands
        case FUNC_RESET:
            if (msg.DATA[1] == 0x01 && msg.DATA[2] == 0x23)
                cout << "Reset command" << endl;
            else
                cout << "Unknown C1 command format" << endl;
            break;

        case FUNC_DISABLE:
            if (msg.DATA[1] == 0xEC && msg.DATA[2] == 0x00)
                cout << "Disable command" << endl;
            else
                cout << "Unknown C2 command format" << endl;
            break;

        case FUNC_SETUP_MODE:
            if (msg.DATA[1] == 0x28 && msg.DATA[2] == 0xD5)
                cout << "Setup mode command" << endl;
            else
                cout << "Unknown C3 command format" << endl;
            break;

            // Default
        default:
            cout << "Unknown command: 0x" << hex << (int)msg.DATA[0] << endl;
            break;
        }


    }

    // 종료 처리
    cout << "CAN 장치 해제 중...\n";
    CAN_Uninitialize(PCAN_USBBUS1);
    cout << "정상 종료됨.\n";

    return 0;
}
