#include "Discord.h"
#include "disrpc/Includes/discord_rpc.h"
#include <time.h>
#include <string>
#include <lmcons.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <random>

void Discord::Initialize()
{
    DiscordEventHandlers Handle;
    memset(&Handle, 0, sizeof(Handle));
    Discord_Initialize("1249731946724982824", &Handle, 1, NULL);
}

void Discord::Update()
{
    const std::string characters = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, characters.size() - 1);

    std::string randomString;
    for (int i = 0; i < 6; ++i) {
        randomString += characters[dis(gen)];
    }

    char name[UNLEN + 1];
    DWORD username_len = UNLEN + 1;
    GetUserNameA(name, &username_len);
    std::string winname = name;

    std::string secret = "UserNAME : ";
    std::string rand = "SOSAL DA : ";

    std::string state = secret + winname;
    std::string randomuid = rand + randomString;

    DiscordRichPresence discord;
    memset(&discord, 0, sizeof(discord));
    discord.details = state.c_str();
    discord.state = randomuid.c_str();
    discord.largeImageText = "";
    discord.startTimestamp = time(0);
    discord.largeImageKey = "image_1_";
    discord.smallImageKey = "2";
    Discord_UpdatePresence(&discord);
}

void Discord::Stop()
{
    Discord_Shutdown();
}