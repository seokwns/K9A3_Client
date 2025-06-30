#pragma once

#include <iostream>
#include <string>
#include <sstream>

#include "../tcc/TurretControl.h"
#include "../InputHelper.h"

class CommandLine
{
public:
    static void start(TurretControl *tcc, std::map<std::string, DeviceRecord> *deviceMapper);
};