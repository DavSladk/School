/**
 * @file Options.cpp
 * @author David Sladký
 * @brief Implementation of Options.h, class for parsing command line parametrs.
 */
#include "Options.h"

Options::Options(int argc, char** argv)
{
    this->argc = argc;
    this->argv = argv;
}

bool Options::helpIsSet()
{
    return this->helpFlag;
}

bool Options::verboseIsSet()
{
    return this->verboseFlag;
}

bool Options::tokenIsSet()
{
    return this->tokenFlag;
}

std::string Options::getToken()
{
    return this->token;
}

void Options::parseOptions()
{
    if ( argc == 1 )
    {
        helpFlag = true;
        return;
    }

    for (int i = 1; i < this->argc; i++)
    {
        if ( strcmp(argv[i], "-h") == MATCH || strcmp(argv[i], "--help") == MATCH)
        {
            helpFlag = true;
        }
        else if ( strcmp(argv[i], "-v") == MATCH || strcmp(argv[i], "--verbose") == MATCH)
        {
            verboseFlag = true;
        }
        else if ( strcmp(argv[i], "-t") == MATCH)
        {
            if ( argc == i + 1 )
            {
                throw CustomException("Missing token after -t option.", MISSING_TOKEN);
            }
            tokenFlag = true;
            token = argv[i + 1];
            i++;
        }
        else
        {
            std::string msg = "Unknow parametr " + std::string(argv[i]) + " on position " + std::to_string(i) + ".";
            throw CustomException(msg, OPTION_UNKNOW);
        }
        
    }

}

void Options::printOptionsValue()
{
    std::cout << std::boolalpha;
    std::cout << "-h/--help : " << helpFlag << std::endl;
    std::cout << "-v/--verbose : " << verboseFlag << std::endl;
    std::cout << "-t : " << tokenFlag << " - \"" << token << "\"" << std::endl;
    std::cout << std::noboolalpha;
}