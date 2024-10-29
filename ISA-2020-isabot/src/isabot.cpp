/**
 * @file isabot.cpp
 * @author David Sladký
 * @brief Implementation of isabot.h, main execution flow control class.
 */

#include "isabot.h"

ISAbot::ISAbot(int argc, char** argv)
{
    this->argc = argc;
    this->argv = argv;
}

ISAbot::~ISAbot()
{
    delete this->opt;
}

void ISAbot::printHelp()
{
    std::cout << "Usage: isa-bot [-h|--help] [-v|--verbose] -t <bot_access_token>" << std::endl;
    std::cout << "-h|--help : Prints out this help end exits." << std::endl;
    std::cout << "-v|--verbose : Prints out log of echoed messages." << std::endl;
    std::cout << "-t <bot_access_token> : Access token for the bot to communicate with discord api server." << std::endl;
    std::cout << "Help is printed if no option is used." << std::endl;
}

void ISAbot::setUpOptions()
{
    this->opt = new Options(argc, argv);
    opt->parseOptions();
}

void ISAbot::checkForHelp()
{
    if(
        opt->helpIsSet() ||
        ( !opt->tokenIsSet() && !opt->verboseIsSet() )
    )
    {
        printHelp();
        exit(0);
    }
}

void ISAbot::checkForToken()
{
    if ( !opt->tokenIsSet() )
    {
        throw CustomException("Option -t must be specified.", OPTION_MISSING);
    }
    
    if ( opt->getToken().empty() )
    {
        throw CustomException("Token can not be empty.", MISSING_TOKEN);
    }
}

void ISAbot::run()
{
    setUpOptions();
    checkForHelp();
    checkForToken();

    SecureSocket soc(opt->getToken(), opt->verboseIsSet());


    soc.SetUp();
    soc.SendRequestForGuilds();
    soc.response = soc.ReceiveResponse();
    soc.clean();
    soc.CheckResponseForOK();
    soc.CutToContent();
    soc.ParseOutGuildID();

    soc.SetUp();
    soc.SendRequestForGuildChannels();
    soc.response = soc.ReceiveResponse();
    soc.clean();
    soc.CheckResponseForOK();
    soc.CutToContent();
    soc.ParseOutChannelID();

    soc.SetUp();
    soc.SendRequestForLastMessage();
    soc.response = soc.ReceiveResponse();
    soc.clean();
    soc.CheckResponseForOK();
    soc.CutToContent();
    soc.lastMessageID = soc.ParseOutLastMessageID();

    while (true)
    {
        soc.SetUp();
        soc.SendRequestForLastMessages();
        soc.response = soc.ReceiveResponse();
        soc.clean();
        soc.CheckResponseForOK();
        soc.CutToContent();

        if ( soc.CheckForNoNewMessages() )
        {
            ;
        }
        else
        {
            soc.EchoMessages();
        }
        std::this_thread::sleep_for (std::chrono::seconds(3));
    }

}