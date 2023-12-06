// Copyright 2022 Vanny Sou. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FGameStats.generated.h"




/**
 * A data structs that tracks the minimum and maximum value of a GameStat object
 */
USTRUCT(BlueprintType)
struct FGameStats
{
    GENERATED_USTRUCT_BODY()

    
    UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="", SaveGame)
    int32 Value;

    UPROPERTY(BlueprintReadOnly, SaveGame)
    int32 Default;

    // constructors
    FGameStats();
    FGameStats(const uint32 SetMax);
    FGameStats(const uint32 SetRemaining, const uint32 SetMax);
    
    void Reset();
};
