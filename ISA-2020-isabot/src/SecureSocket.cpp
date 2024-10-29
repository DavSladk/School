/**
 * @file SecureSocket.cpp
 * @author David Sladký
 * @brief Implementation of SecureSocket.h, class for sending, recieving and manipulaiton of data.
 */
#include "SecureSocket.h"

SecureSocket::SecureSocket(std::string token, bool verbose)
{
    this->token = token;
    this->verbose = verbose;
}

void SecureSocket::SetUp()
{
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    
    method = TLSv1_2_client_method();
    if ( method == NULL )
    {
        throw CustomException("Obtaining TLS method failed.", TEMP);
    }

    ctx = SSL_CTX_new(method);
    if ( ctx == NULL )
    {
        throw CustomException("Obtaining CTX failed.", TEMP);
    }

    bio = BIO_new_ssl_connect(ctx);
    if ( bio == NULL)
    {
        throw CustomException("Obtaining BIO failed.", TEMP);
    }

    BIO_get_ssl(bio, &ssl);

    SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);

    BIO_set_conn_hostname(bio, "discord.com:https");

    if ( BIO_do_connect(bio) < 0)
    {
        throw CustomException("Connecting to server failed.", TEMP);
    }

}

void SecureSocket::SendRequestForGuilds()
{
    std::string guildRequest = "GET /api/users/@me/guilds HTTP/1.1\r\nHost: discord.com\r\nConnection: close\r\nAuthorization: Bot " + token + "\r\n\r\n";
    if( BIO_puts(bio, guildRequest.c_str()) < 1 )
    {
        throw CustomException("Sending request for list of guilds failed.", TEMP);
    }
}

std::string SecureSocket::ReceiveResponse()
{
    std::string tmp = "";
    while (true)
    {
        memset(responseBuffer, '\0', BUFFER_SIZE);
        if ( BIO_read(bio, responseBuffer, BUFFER_SIZE) <= 0 )
        {
            break;
        }
        tmp.append(responseBuffer);
    }
    return tmp;
}

void SecureSocket::CheckResponseForOK()
{
    if ( response.compare(9,3,"200") != 0 )
    {
        throw CustomException("Response did not returned with code 200.", TEMP);
    }
}

void SecureSocket::ParseOutGuildID()
{
    std::string::size_type pos;
    pos = response.find(idPrefix, 0);
    if ( pos == std::string::npos )
    {
        throw CustomException("Response does not have guild id.", TEMP);
    }
    guildID = response.substr(pos + idPrefix.size(), ID_SIZE);
}

void SecureSocket::SendRequestForGuildChannels()
{
    std::string channelsRequest = "GET /api/guilds/" + guildID + "/channels HTTP/1.1\r\nHost: discord.com\r\nConnection: close\r\nAuthorization: Bot " + token + "\r\n\r\n";
    if( BIO_puts(bio, channelsRequest.c_str()) < 1 )
    {
        throw CustomException("Sending request for list of guild channels.", TEMP);
    }
}

void SecureSocket::ParseOutChannelID()
{
    std::string::size_type pos;
    std::string tmp;

    pos = response.find(channelName, 0);
    if ( pos == std::string::npos )
    {
        throw CustomException("Response does not have channel id.", TEMP);
    }
    tmp = response.substr(0, pos);
    pos = tmp.rfind(idPrefix, tmp.size());
    channelID = tmp.substr(pos + idPrefix.size(), ID_SIZE);
}

void SecureSocket::SendRequestForLastMessage()
{
    std::string channelRequest = "GET /api/channels/" + channelID + "/messages?limit=1 HTTP/1.1\r\nHost: discord.com\r\nConnection: close\r\nAuthorization: Bot " + token + "\r\n\r\n";
    if( BIO_puts(bio, channelRequest.c_str()) < 1 )
    {
        throw CustomException("Sending request for last message in channel failed.", TEMP);
    }
}

std::string SecureSocket::ParseOutLastMessageID()
{
    std::string::size_type pos;
    std::string tmp;

    pos = response.find(idPrefix, 0);
    lastPos = pos;
    if (pos == std::string::npos)
    {
        return lastMessageID;
    }
    tmp = response.substr(pos + idPrefix.size(), ID_SIZE);

    return tmp;
}

void SecureSocket::SendRequestForLastMessages()
{
    std::string messagesRequest = "GET /api/channels/" + channelID + "/messages?after=" + lastMessageID + "&limit=100 HTTP/1.1\r\nHost: discord.com\r\nConnection: close\r\nAuthorization: Bot " + token + "\r\n\r\n";
    if( BIO_puts(bio, messagesRequest.c_str()) < 1 )
    {
        throw CustomException("Sending request for last messages in channel failed.", TEMP);
    }
}

bool SecureSocket::CheckForNoNewMessages()
{
    std::string::size_type tmp;
    tmp = response.find("\r\n\r\n[]", 0);
    if(tmp != std::string::npos)
    {
        return true;
    }
    return false;
}

std::string::size_type SecureSocket::ParseOutLastMessageContent()
{
    std::string::size_type tmp1, tmp2;

    tmp1 = response.find(contentPrefix);
    if ( tmp1 == std::string::npos )
    {
        throw CustomException("Response does not have message content.", TEMP);
    }
    tmp2 = response.find("\"", tmp1 + contentPrefix.size());
    content = response.substr(tmp1 + contentPrefix.size(), tmp2 - tmp1 - contentPrefix.size());

    return tmp2;
}

std::string::size_type SecureSocket::ParseOutLastMessageUsername()
{
    std::string::size_type tmp1, tmp2;

    tmp1 = response.find(usernamePrefix);
    if ( tmp1 == std::string::npos )
    {
        throw CustomException("Response does not have author of last message.", TEMP);
    }
    tmp2 = response.find("\"", tmp1 + usernamePrefix.size());
    username = response.substr(tmp1 + usernamePrefix.size(), tmp2 - tmp1 - usernamePrefix.size());

    return tmp2;
}

void SecureSocket::EchoMessages()
{
    std::string::size_type pos;
    lastMessageID = ParseOutLastMessageID();
    response = response.substr(lastPos);
    while(true)
    {
        pos = ParseOutLastMessageContent();
        response = response.substr(pos);
        pos = ParseOutLastMessageUsername();
        response = response.substr(pos);

        if (username.find("bot") != std::string::npos)
        {
            ParseOutLastMessageID();
            if ( lastPos == std::string::npos)
            {
                break;
            }
            response = response.substr(lastPos);
            continue;
        }

        SetUp();
        SendEchoRequest();
        ReceiveResponse();        
        clean();

        if ( verbose )
        {
            std::cout << "isa-bot - " << username << ": " << content  << std::endl;
        }
        ParseOutLastMessageID();
        if ( lastPos == std::string::npos)
        {
            break;
        }
        response = response.substr(lastPos);
        std::this_thread::sleep_for (std::chrono::seconds(1));
    }


}

void SecureSocket::SendEchoRequest()
{
    int contentLength = BASE_JSON_MESSAGE_CONTENT_SIZE + content.size() + username.size();

    std::string echoRequest = "POST /api/channels/" + channelID + "/messages HTTP/1.1\r\nHost: discord.com\r\nConnection: close\r\nContent-Type: application/json\r\nContent-Length: " + std::to_string(contentLength) + "\r\nAuthorization: Bot " + token + "\r\n\r\n{\"content\": \"echo: " + username + " - " + content + "\", \"tts\": false}";

    if( BIO_puts(bio, echoRequest.c_str()) < 1 )
    {
        throw CustomException("Sending request for creating message failed.", TEMP);
    }
};

void SecureSocket::CutToContent()
{
    std::string::size_type tmp;
    tmp = response.find("\r\n\r\n");
    response = response.substr(tmp);
}

void SecureSocket::clean()
{
    SSL_CTX_free(ctx);
    BIO_free_all(bio);
}