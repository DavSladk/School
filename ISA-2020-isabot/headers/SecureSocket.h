/**
 * @file SecureSocket.h
 * @author David Sladký
 * @brief Declaration of SecureSocket class contorling sending, recieving and parsing data.
 */
#ifndef XSLADK07_SECURESOCKET_H
#define XSLADK07_SECURESOCKET_H

#include <openssl/ssl.h>
#include <openssl/err.h>

#include <iostream>
#include <cstring>

#include "CustomException.h"

#include <thread>
#include <chrono>

/**
 * Class sending, recieving and parsing application data
 * between the bot and api server. Due to spaghettification it
 * does more that it should based on SOLID principles.
 */
class SecureSocket
{
    public:
        /**
         * A constructor.
         * Initializzes this class with token and verbose option.
         */
        SecureSocket(std::string token, bool verbose);

        /**
         * Prepares for TLS communication.
         */
        void SetUp();

        /**
         * Sends HTTP request for list of guilds.
         */
        void SendRequestForGuilds();

        /**
         * Recieve HTTP response from server.
         */
        std::string ReceiveResponse();

        /**
         * Check if response returned with OK code 200.
         */
        void CheckResponseForOK();

        /**
         * Parses out guild id from response.
         */
        void ParseOutGuildID();

        /**
         * Sends HTTP request for list of channels in a guild.
         */
        void SendRequestForGuildChannels();

        /**
         * Cleans up after TLS communication.
         */
        void clean();

        /**
         * Parses out channel ID from response.
         */
        void ParseOutChannelID();

        /**
         * Sends request for last message in a channel.
         */
        void SendRequestForLastMessage();

        /**
         * Parses out last message ID from response.
         */
        std::string ParseOutLastMessageID();

        /**
         * Sends request for messages sent to channel after last served message.
         */
        void SendRequestForLastMessages();

        /**
         * Checks if there were any new messages in a channel.
         */
        bool CheckForNoNewMessages();

        /**
         * Cycles to sends request to create new message in channel.
         */
        void EchoMessages();

        /**
         * Parses out content of last message.
         */
        std::string::size_type ParseOutLastMessageContent();

        /**
         * Parses out author's username of last message.
         */
        std::string::size_type ParseOutLastMessageUsername();

        /**
         * Cuts off HTTP headers from response.
         */
        void CutToContent();

        /**
         * Send request to create new message in channel;
         */
        void SendEchoRequest();

        std::string response = "";
        std::string lastMessageID;

    private:
        static const int BUFFER_SIZE = 1024;
        static const int ID_SIZE = 18;
        static const int BASE_JSON_MESSAGE_CONTENT_SIZE = 38;

        const SSL_METHOD* method;
        SSL_CTX* ctx;
        BIO* bio;
        SSL* ssl;
        
        std::string token;
        std::string guildID;
        std::string channelID;
        std::string content;
        std::string username;

        bool verbose;

        std::string::size_type lastPos;

        char responseBuffer[BUFFER_SIZE];

        const std::string idPrefix = "{\"id\": \"";
        const std::string channelName = "\"isa-bot\"";
        const std::string lastMessagePrefix = "\"last_message_id\": \"";
        const std::string contentPrefix = "\"content\": \"";
        const std::string usernamePrefix = "\"username\": \"";

};

#endif