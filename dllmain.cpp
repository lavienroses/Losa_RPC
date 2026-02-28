#include "pch.h"
#include <windows.h>
#include <string>
#include <fstream>
#include <sstream>
#include <thread>
#include <shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

#include "DiscordSDK/include/discord_register.h"
#include "DiscordSDK/include/discord_rpc.h"

static const char* APPLICATION_ID = "YOUR_APPLICATION_ID";
std::string lastKnownState = "";

std::string GetLogFilePath() {
    char path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);
    PathRemoveFileSpecA(path);

    std::string folder(path);
    folder += "\\info\\pp.log";
    return folder;
}

std::string ReadLastChangeLine(const std::string& filePath) {
    std::ifstream file(filePath.c_str());
    std::string line;
    std::string lastChangeLine;

    if (!file.is_open()) return "";

    while (std::getline(file, line)) {
        if (line.find("ChangeState - Next Stage") != std::string::npos) {
            lastChangeLine = line;
        }
    }

    return lastChangeLine;
}

std::string ParseStateFromLine(const std::string& line) {
    if (line.find("Next Stage(2, 0,") != std::string::npos) return "Lobby";
    else if (line.find("Next Stage(4, 1,") != std::string::npos) return "Power Stone";
    else if (line.find("Next Stage(4, 2,") != std::string::npos) return "Prisoner";
    else if (line.find("Next Stage(4, 3,") != std::string::npos) return "Hidden Crown";
    else if (line.find("Next Stage(4, 4,") != std::string::npos) return "Square";
    else if (line.find("Next Stage(4, 5,") != std::string::npos) return "Deathmatch";
    else if (line.find("Next Stage(4, 6,") != std::string::npos) return "Timesmatch";
    else if (line.find("Next Stage(4, 7,") != std::string::npos) return "Boss Raid";
    else if (line.find("Next Stage(4, 8,") != std::string::npos) return "Skeleton Hero";
    else if (line.find("Next Stage(4, 9,") != std::string::npos) return "Soccer Game";
    else if (line.find("Next Stage(4, 10,") != std::string::npos) return "Ladder War";
    else if (line.find("Next Stage(4, 11,") != std::string::npos) return "Jiangshi Survival";
    else if (line.find("Next Stage(4, 12,") != std::string::npos) return "Test Mode";
    else if (line.find("Next Stage(4, 13,") != std::string::npos) return "HQ";
    else if (line.find("Next Stage(4, 14,") != std::string::npos) return "Dwarf";
    else if (line.find("Next Stage(4, 15,") != std::string::npos) return "Champion Mode";
    else if (line.find("Next Stage(4, 16,") != std::string::npos) return "Deep Forest";
    else if (line.find("Next Stage(4, 17,") != std::string::npos) return "Snow Town";
    else if (line.find("Next Stage(4, 18,") != std::string::npos) return "Temple of Fire";
    else if (line.find("Next Stage(4, 19,") != std::string::npos) return "Double Crown";
    else if (line.find("Next Stage(4, 21,") != std::string::npos) return "Secret Factory";
    else if (line.find("Next Stage(4, 22,") != std::string::npos) return "Team Serva AI";
    else if (line.find("Next Stage(4, 25,") != std::string::npos) return "Pantymon";
    else if (line.find("Next Stage(4, 26,") != std::string::npos) return "Classic Mode";
    else if (line.find("Next Stage(4, 27,") != std::string::npos) return "Raid Mode";
    else if (line.find("Next Stage(4, 30,") != std::string::npos) return "Flag Mode";
    else if (line.find("Next Stage(4, 31,") != std::string::npos) return "Arena Mode";
    else if (line.find("Next Stage(4, 32,") != std::string::npos) return "Battle Mode";
    else if (line.find("Next Stage(4, 33,") != std::string::npos) return "Farming Mode";

    return "Loading . . . . ";
}

void UpdateDiscordPresence(const std::string& state) {
    DiscordRichPresence discordPresence;
    memset(&discordPresence, 0, sizeof(discordPresence));

    discordPresence.state = state.c_str();
    discordPresence.details = "";

    std::string imageKey;
    std::string imageText;

    if (state == "Lobby") {
        imageKey = "lobby";
        imageText = "In the Lobby";
    }
    else if (state == "HQ") {
        imageKey = "hq";
        imageText = "Headquarters";
    }
    else if (state == "Power Stone") {
        imageKey = "power_stone";
        imageText = "Power Stone Mode";
    }
    else if (state == "Prisoner") {
        imageKey = "prisoner";
        imageText = "Prisoner Mode";
    }
    else if (state == "Hidden Crown") {
        imageKey = "hidden_crown";
        imageText = "Hidden Crown Mode";
    }
    else if (state == "Square") {
        imageKey = "square";
        imageText = "Square Mode";
    }
    else if (state == "Deathmatch") {
        imageKey = "deathmatch";
        imageText = "Deathmatch Mode";
    }
    else if (state == "Timesmatch") {
        imageKey = "timesmatch";
        imageText = "Timesmatch Mode";
    }
    else if (state == "Boss Raid") {
        imageKey = "boss_raid";
        imageText = "Boss Raid Mode";
    }
    else if (state == "Skeleton Hero") {
        imageKey = "skeleton_hero";
        imageText = "Skeleton Hero Mode";
    }
    else if (state == "Soccer Game") {
        imageKey = "soccer";
        imageText = "Soccer Game Mode";
    }
    else if (state == "Ladder War") {
        imageKey = "ladder_war";
        imageText = "Ladder War Mode";
    }
    else if (state == "Jiangshi Survival") {
        imageKey = "jiangshi_survival";
        imageText = "Jiangshi Survival Mode";
    }
    else if (state == "Test Mode") {
        imageKey = "test_mode";
        imageText = "Test Mode for PvE";
    }
    else if (state == "Dwarf") {
        imageKey = "dwarf";
        imageText = "Dwarf Mode";
    }
    else if (state == "Champion Mode") {
        imageKey = "champion";
        imageText = "Champion Mode";
    }
    else if (state == "Deep Forest") {
        imageKey = "deep_forest";
        imageText = "Monster Dungeon: Deep Forest";
    }
    else if (state == "Snow Town") {
        imageKey = "snowtown";
        imageText = "Monster Dungeon: Snow Town";
    }
    else if (state == "Temple of Fire") {
        imageKey = "temple_fire";
        imageText = "Monster Dungeon: Temple of Fire";
    }
    else if (state == "Double Crown") {
        imageKey = "double_crown";
        imageText = "Double Crown Mode";
    }
    else if (state == "Secret Factory") {
        imageKey = "secret_factory";
        imageText = "Secret Factory Mode";
    }
    else if (state == "Team Serva AI") {
        imageKey = "serva_ai";
        imageText = "Team Serva AI";
    }
    else if (state == "Pantymon") {
        imageKey = "pantymon";
        imageText = "Pantymon Mode";
    }
    else if (state == "Classic Mode") {
        imageKey = "classic";
        imageText = "Classic Mode";
    }
    else if (state == "Raid Mode") {
        imageKey = "raid_mode";
        imageText = "Raid Mode";
    }
    else if (state == "Flag Mode") {
        imageKey = "flag_mode";
        imageText = "Flag Mode";
    }
    else if (state == "Arena Mode") {
        imageKey = "arena_mode";
        imageText = "Arena Mode";
    }
    else if (state == "Battle Mode") {
        imageKey = "battle_mode";
        imageText = "Battle Mode";
    }
    else if (state == "Farming Mode") {
        imageKey = "farming_mode";
        imageText = "Farming Mode";
    }
    else {
        imageKey = "default";
        imageText = "Kalem Euy";
    }

    discordPresence.largeImageKey = imageKey.c_str();
    discordPresence.largeImageText = imageText.c_str();

    Discord_UpdatePresence(&discordPresence);
}


void InitDiscordRPC() {
    DiscordEventHandlers handlers;
    memset(&handlers, 0, sizeof(handlers));
    Discord_Initialize(APPLICATION_ID, &handlers, 1, NULL);

    std::string filePath = GetLogFilePath();

    while (true) {
        std::string lastLine = ReadLastChangeLine(filePath);
        std::string currentState = ParseStateFromLine(lastLine);

        if (currentState != lastKnownState && !currentState.empty()) {
            lastKnownState = currentState;
            UpdateDiscordPresence(currentState);
        }

        Sleep(1000);
    }
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)InitDiscordRPC, NULL, 0, NULL);
        break;
    case DLL_PROCESS_DETACH:
        Discord_Shutdown();
        break;
    }
    return TRUE;
}
