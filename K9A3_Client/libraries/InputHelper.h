#pragma once
#include <iostream>
#include <limits>

static void HandleInputError()
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Invalid input. Please try again.\n";
}

static bool getInput(const std::string &prompt, uint16_t &value)
{
    std::cout << prompt;
    std::cin >> std::hex >> value;
    if (std::cin.fail())
    {
        HandleInputError();
        return false;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return true;
}