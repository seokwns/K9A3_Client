#include <iostream>

#include "./libraries/constants/constants.h"
#include "./libraries/constants/DeviceConstants.h"
#include "./libraries/tcc/TurretControl.h"
#include "./libraries/InputHelper.h"
#include "./devices/attla/AttlaDriver.h"

int main()
{
    // std::map<std::string, DeviceRecord> deviceMapper;

    // deviceMapper["tcc"] = { 0x10, "192.168.100.100", 9000 };
    // deviceMapper["attla"] = { 0x22, "192.168.100.100", 9003 };

    // TurretControl::getInstance().addDevice(&AttlaDriver::getInstance());
    TurretControl::getInstance().addDevices(
        &AttlaDriver::getInstance());

    // CommandLine::start(&tcc, &deviceMapper);

    while (true)
    {
        std::string command;
        std::cout << "";
        std::getline(std::cin, command);
        /*
        std::istringstream stream(command);
        std::string deviceName;
        std::string funcName;

        stream >> deviceName;
        stream >> funcName;
        */
        AttlaDriver &attla = AttlaDriver::getInstance();

        if (command == "attla encoder reset result")
        {
            unsigned short target, result;
            getInput("- target: ", target);
            getInput("- result: ", result);
            attla.attla_tcc_encoder_reset_result_ref(target, result);
        }
        else if (command == "attla drive cmd cancel")
        {
            unsigned short result;
            getInput("- result: ", result);
            attla.attla_tcc_drive_cmd_cancel_ref(result);
        }
        else if (command == "attla validation result")
        {
            unsigned short action, result;
            getInput("- action: ", action);
            getInput("- result: ", result);
            attla.attla_tcc_validation_result_upd(action, result);
        }
    }

    return 0;
}