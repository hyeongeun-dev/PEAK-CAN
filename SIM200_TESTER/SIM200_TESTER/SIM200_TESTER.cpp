#include "common.h"
#include "PCANBasic.h"
#include "SIM200.h"

using namespace std;

bool keepRunning = true;

// 종료 이벤트 핸들러 함수
BOOL WINAPI ConsoleHandler(DWORD signal) {
    switch (signal) {
    case CTRL_C_EVENT:
        std::cout << "\n[Ctrl+C] 종료 요청됨\n";
        break;
    case CTRL_CLOSE_EVENT:
        std::cout << "\n[콘솔창 X] 종료 요청됨\n";
        break;
    case CTRL_BREAK_EVENT:
        std::cout << "\n[Ctrl+Break] 종료 요청됨\n";
        break;
    case CTRL_LOGOFF_EVENT:
        std::cout << "\n[로그아웃] 종료 요청됨\n";
        break;
    case CTRL_SHUTDOWN_EVENT:
        std::cout << "\n[시스템 종료] 종료 요청됨\n";
        break;
    default:
        std::cout << "\n[기타 종료] 요청됨\n";
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
        std::cout << "CAN 초기화 실패" << std::endl;
        return -1;
    }

    std::cout << "CAN 초기화 성공!" << std::endl;
    std::cout << "Ctrl+C 또는 콘솔 X버튼을 눌러 종료하세요\n";

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
            std::cout << "Get FW version" << std::endl;
            break;

        case FUNC_GET_FW_INFO_0:
            std::cout << "Get FW version info (part 0)" << std::endl;
            break;

        case FUNC_GET_FW_INFO_1:
            std::cout << "Get FW version info (part 1)" << std::endl;
            break;

        case FUNC_GET_FW_INFO_2:
            std::cout << "Get FW version info (part 2)" << std::endl;
            break;

        case FUNC_GET_SERIAL_0:
            std::cout << "Get serial number (part 0)" << std::endl;
            break;

        case FUNC_GET_SERIAL_1:
            std::cout << "Get serial number (part 1)" << std::endl;
            break;

        case FUNC_GET_SERIAL_2:
            std::cout << "Get serial number (part 2)" << std::endl;
            break;

        case FUNC_GET_SERIAL_3:
            std::cout << "Get serial number (part 3)" << std::endl;
            break;

            // Measurements
        case FUNC_GET_VNEG:
            std::cout << "Get high-resolution Vneg" << std::endl;
            break;

        case FUNC_GET_VPOS:
            std::cout << "Get high-resolution Vpos" << std::endl;
            break;

        case FUNC_GET_VEXC:
            std::cout << "Get high-resolution Vexc" << std::endl;
            break;

        case FUNC_GET_VBATT:
            std::cout << "Get high-resolution Vbatt" << std::endl;
            break;

        case FUNC_GET_VPOWER:
            std::cout << "Get high-resolution Vpower" << std::endl;
            break;

        case FUNC_GET_TEMP:
            std::cout << "Get high-resolution temperature" << std::endl;
            break;

        case FUNC_GET_VCAL_NEG:
            std::cout << "Get high-resolution Vcal_neg" << std::endl;
            break;

        case FUNC_GET_VCAL_POS:
            std::cout << "Get high-resolution Vcal_pos" << std::endl;
            break;

        case FUNC_GET_UPTIME:
            std::cout << "Get uptime counter" << std::endl;
            break;

            // Electrical isolation
        case FUNC_GET_ISO_STATE:
            std::cout << "Get isolation state" << std::endl;
            break;

        case FUNC_GET_ISO_RESISTANCE:
            std::cout << "Get isolation resistances" << std::endl;
            break;

        case FUNC_GET_Y_CAP:
            std::cout << "Get system Y-capacitances" << std::endl;
            break;

        case FUNC_GET_HV_MEASURE:
            std::cout << "Get HV rail measurements" << std::endl;
            break;

        case FUNC_GET_BATTERY_V:
            std::cout << "Get battery voltage" << std::endl;
            break;

        case FUNC_GET_ERROR_FLAGS:
            std::cout << "Get error flags" << std::endl;
            break;

        case FUNC_GET_DYN_ISO:
            std::cout << "Get dynamic electrical isolation" << std::endl;
            break;

        case FUNC_GET_DYN_CAP:
            std::cout << "Get dynamic capacitive energy" << std::endl;
            break;

            // Configuration
        case FUNC_GET_MAX_VOLTAGE:
            std::cout << "Get max voltage setting" << std::endl;
            break;

        case FUNC_GET_ISO_THRESHOLD:
            std::cout << "Get isolation threshold settings" << std::endl;
            break;

            // Commands
        case FUNC_RESET:
            if (msg.DATA[1] == 0x01 && msg.DATA[2] == 0x23)
                std::cout << "Reset command" << std::endl;
            else
                std::cout << "Unknown C1 command format" << std::endl;
            break;

        case FUNC_DISABLE:
            if (msg.DATA[1] == 0xEC && msg.DATA[2] == 0x00)
                std::cout << "Disable command" << std::endl;
            else
                std::cout << "Unknown C2 command format" << std::endl;
            break;

        case FUNC_SETUP_MODE:
            if (msg.DATA[1] == 0x28 && msg.DATA[2] == 0xD5)
                std::cout << "Setup mode command" << std::endl;
            else
                std::cout << "Unknown C3 command format" << std::endl;
            break;

            // Default
        default:
            std::cout << "Unknown command: 0x" << std::hex << (int)msg.DATA[0] << std::endl;
            break;
        }


    }

    // 종료 처리
    std::cout << "CAN 장치 해제 중...\n";
    CAN_Uninitialize(PCAN_USBBUS1);
    std::cout << "정상 종료됨.\n";

    return 0;
}
