// Copyright 2022 Vanny Sou. All Rights Reserved.

#include "POSaveGame.h"



void UPOSaveGame::AddTile(int Type)
{
    SavedGrid.TileTypes.Add(Type);
}

void UPOSaveGame::AddGameStatsInfo(const FString& SaveStr, const FGameStats& Stats)
{
    const FString& Default = FString("Default") + SaveStr;
    const FString& Val = FString("Value") + SaveStr;
    CustomInt.Add(Default, Stats.Default);
    CustomInt.Add(Val, Stats.Value);
}

void UPOSaveGame::GetGameStatsInfo(const FString& LoadStr, FGameStats& Out)
{
    const FString& Default = FString("Default") + LoadStr;
    const FString& Val = FString("Value") + LoadStr;
    
    if(const int SetDef = *CustomInt.Find(Default))
    {
        if(const int SetVal = *CustomInt.Find(Val))
        {
            Out = FGameStats(SetVal, SetDef);
            return;
        }
    }
    Out = FGameStats(1);
}


const int32 FBoardSaveData::GetNumberOfTiles() const
{
    return TileTypes.Num();
}

UPOSaveGame::UPOSaveGame()
{
    // Player = FMatch3PlayerData(InitScore);
    
    SaveSlotName = DEFAULT_SAVE_SLOT;
    UserIndex = 0;
    
    DEFAULT = DEFAULT_SAVE_SLOT;
    RESET = RESET_GAME_SLOT;
    CONTINUE = CONTINUE_GAME_SLOT;
    PREVIOUS = LAST_SUCCESSFUL_SLOT;
}




