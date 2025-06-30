#include "./CommandLine.h"

void CommandLine::start(TurretControl *tcc, std::map<std::string, DeviceRecord> *deviceMapper)
{
    std::map<std::string, std::map<std::string, std::function<void()>>> funcMap;

    // funcMap["attla"]["ibit-run"] = &
    /*
    while (true)
    {
        std::string command;
        std::cout << " > ";
        std::getline(std::cin, command);

        std::istringstream stream(command);
        std::string deviceName;
        std::string funcName;

        stream >> deviceName;
        stream >> funcName;





        if (command == "attla ibit run")
        {
            attla.IBIT_RUN_CMD(0x00);
        }
        else if (command == "attla ibit result")
        {
            attla.IBIT_RESULT_REQ();
        }
        else if (command == "attla validation cmd")
        {
            unsigned short action, speed;
            getInput("- action: ", action);
            getInput("- speed: ", speed);
            attla.tcc_attla_validation_cmd(action, speed);
        }
        else if (command == "attla state")
        {
            std::cout << std::endl;
            attla.print();
            std::cout << std::endl;
        }

    }
    */
}