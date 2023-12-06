// Copyright 2017-2021 Vanny Sou. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayOptionsInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGameplayOptionsInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTODIMH_API IGameplayOptionsInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
    
public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void Run();
    
    UFUNCTION(BlueprintImplementableEvent)
    const bool IsRunning() const;
    
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void Init(AActor* Source, UBaseEvent* Evt);
    
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void Receive(AActor* Source, class UBaseEvent* EventPtr);
};
