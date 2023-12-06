// Copyright 2017-2018 Vanny Sou. All Rights Reserved.

#pragma once

#include "Gametypes.generated.h"

// constants
const uint32    DEFAULT_ONE = 1; // the default number of grid moves that a game piece can make per turn
const uint32    MAXIMUM_VALUE_1000 = 1000;
const uint32    DEFAULT_HIGH_SCORE = 75000;
const uint32    INIT_TILE_VALUE = 10;
const uint8     DEFAULT_SCORE_MULTIPLIER = 1;

static const FName SWAP_POSITIONS(TEXT("Swap Tile Positions"));

/** An enum for player ID */
UENUM(BlueprintType, meta=(ScriptName="M3Player"))
enum class EPlayer : uint8
{
    One = 0,
    Two
};

// TODO: promote to a TMap?
UENUM()
enum class EMechModule : uint16
{
    Weapon = 0,
    Defense,
    Navigation,
    Max_Type
};


UENUM(BlueprintType)
enum class EDirection : uint8
{
    Up = 0,
    Down = 1,
    Left = 2,
    Right = 3,
    MaxDir
};

USTRUCT()
struct FMatch3GameAction
{
    GENERATED_USTRUCT_BODY()
    
    FName Identifier;
    uint32 Num;

    // UPROPERTY()
    // class AMatch3GameMode* GameMode;
};

USTRUCT(BlueprintType)
struct FIntPointLine
{
    GENERATED_USTRUCT_BODY()
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, SaveGame)
    FIntPoint Start{2,0};
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, SaveGame)
    FIntPoint End{2,5};
};

USTRUCT()
struct FLabeledActorMap
{
    GENERATED_BODY()

    FLabeledActorMap() = default;
	
    explicit FLabeledActorMap(const TMap<FName, AActor*> In)
    {
        Map = In;
    }
	
    UPROPERTY()
    TMap<FName, AActor*> Map;
};

USTRUCT(BlueprintType)
struct FMatch3PlayerData
{
    GENERATED_BODY()

    FMatch3PlayerData()
    {
        Match3Score = 0;
        BonusPoints = 0;
        ScoreMultiplier = 1;
        TurnCounter = 1;
        ParticipantIndex = 1;
    }

    UPROPERTY()
    uint32 Match3Score;
	
    UPROPERTY()
    uint32 ScoreMultiplier;

    UPROPERTY()
    uint32 BonusPoints;
	
    UPROPERTY()
    uint32 TurnCounter;

    UPROPERTY()
    uint32 ParticipantIndex;
    
};