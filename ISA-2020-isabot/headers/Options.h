/**
 * @file Options.h
 * @author David Sladký
 * @brief Declaration of Options class, class for parsing command line parametrs.
 */
#ifndef XSLADK07_OPTIONS_H
#define XSLADK07_OPTIONS_H

#include <string>
#include <iostream>

#include <cstring>

#include "CustomException.h"

/**
 * Class parsing command line paramets and providing methods to access their values.
 */
class Options
{
    public:
        /**
         * A constructor.
         * Initializes argc and argv for later parsing.
         */
        Options(int argc, char** argv);

        /**
         * @return Flag if -h/--help is set.
         */
        bool helpIsSet();

        /**
         * @return Flag if -v/--verbose is set.
         */
        bool verboseIsSet();

        /**
         * @return Flag if -t is set.
         */
        bool tokenIsSet();

        /**
         * @return Bot token.
         */
        std::string getToken();

        /**
         * Debug method printing out values of flags.
         */
        void printOptionsValue();

        /**
         * Parses command line options and saves them into corresponding class members.
         */
        void parseOptions();

    private:
        bool helpFlag = false; /**< Storing help flag. */
        bool verboseFlag = false; /**< Storing verbose flag. */
        bool tokenFlag = false; /**< Storing token flag. */
        std::string token = ""; /**< Storing token passed by -t. */

        int argc;
        char** argv;

        const int MATCH = 0;
};

#endif //XSLADK07_OPTIONS_H