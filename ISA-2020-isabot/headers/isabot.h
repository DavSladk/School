/**
 * @file isabot.h
 * @author David Sladký
 * @brief Declaration of ISAbot class controling main flow of the bot.
 */
#ifndef XSLADK07_ISABOT_H
#define XSLADK07_ISABOT_H

#include "Options.h"
#include "CustomException.h"
#include "SecureSocket.h"

/**
 * Class controling the main flow of the program.
*/
class ISAbot
{
    public:
        /**
         * A constructor.
         * Initializes argc and argv.
         */
        ISAbot(int argc, char** argv);

        /**
         * A destructor.
         * Deletes Options object.
         */
        ~ISAbot();

        /**
         * Prints help for bot.
         */
        void printHelp();

        /**
         * Starts the main flow of the program.
         */
        void run();

    private:
        Options *opt;
        int argc;
        char** argv;

        void checkForHelp();
        void setUpOptions();
        void checkForToken();
};

#endif //XSLADK07_ISABOT_H