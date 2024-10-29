# isa-bot

## Usage

isa-bot [-h|--help] [-v|--verbose] -t <bot_access_token>

* -h|--help : Prints out this help end exits.
* -v|--verbose : Prints out log of echoed messages.
* -t <bot_access_token> : Access token for the bot to communicate with discord api server.

Help is printed if no option is used. Bot terminates immediately after that.

## Description
Very simple echo Discord bot. Connects to the first Discord server he is in returned by Discord API. Then echoes messages from users (if they username does not contain "bot" substring) in text channel named isa-bot (if present) in format:

```
echo: <username> - <message>
```

## Required to build
Below listed versions are the lowest one the bot was tested on. You can use the newer versions too.

* openssl - 1.0.2k
* g++ - 7.5.0
* cmake - 3.16.3
* make - 3.82

## Build
If build for the first time, then execute `make` in root directory. After that you can use `make build`. When you use newer versions of openssl, you will see warnings about depricated functions. Ignore those warnings, the bot should work fine.

## Examples of usage
```
>$ isabot -h
Usage: isa-bot [-h|--help] [-v|--verbose] -t <bot_access_token>
-h|--help : Prints out this help end exits.
-v|--verbose : Prints out log of echoed messages.
-t <bot_access_token> : Access token for the bot to communicate with discord api     server.
Help is printed if no option is used.
```

```
>$ ./isabot -t <token>
```

```
>$ ./isabot -v -t <token>
isa-bot - Pepa: 1
isa-bot - Pepa: 2
isa-bot - Pepa: 3
```
Outputs with `-v` used are just a examples.


## List of files
```
.
├── CMakeLists.txt
├── Makefile
├── README.md
├── headers
│   ├── CustomException.h
│   ├── Options.h
│   ├── SecureSocket.h
│   └── isabot.h
├── manual.pdf
└── src
    ├── CustomException.cpp
    ├── Options.cpp
    ├── SecureSocket.cpp
    ├── isabot.cpp
    └── main.cpp

2 directories, 13 files
```