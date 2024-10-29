/**
 * @file main.cpp
 * @author David Sladký
 * @brief Main.
 */
#include <iostream>
#include "isabot.h"

int main(int argc, char **argv)
{
    try
    {
        ISAbot bot(argc, argv);
        bot.run();
    }
    catch (CustomException e)
    {
        std::cerr << e.Message() << std::endl;
    }
    return 0;
}