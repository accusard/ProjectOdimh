// Copyright 2022 Vanny Sou. All Rights Reserved.


#include "GameplayRunModeBase.h"
#include "ProjectOdimh/POGameInstance.h"
#include "ProjectOdimh/ClassInterface/GameplayOptionsInterface.h"
#include "ProjectOdimh/Entities/Game/Modes/GameplayOptions/Gameplay.h"
#include "ProjectOdimh/Subsystems/EventManager.h"

// Sets default values
AGameplayRunModeBase::AGameplayRunModeBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void AGameplayRunModeBase::OnTurnEnded_Implementation(AActor* Source, UBaseEvent* Event)
{
    const FName& Active = Event->TURN_ENDING_EVENT;
    
    if(Event->Is(Active))
    {
        RunCount.Reset();
    }
}

// Called when the game starts or when spawned
void AGameplayRunModeBase::BeginPlay()
{
	Super::BeginPlay();
    const UPOGameInstance* Instance = GetWorld()->GetGameInstance<UPOGameInstance>();
    
    UEventManager* EvtMgr = Instance->GetSubsystem<UEventManager>();
    EvtMgr->Delegates.OnTurnEnd.AddDynamic(this, &AGameplayRunModeBase::StepTick);
    EvtMgr->Delegates.OnActorEvent.AddDynamic(this, &AGameplayRunModeBase::OnTurnEnded);
}

const int AGameplayRunModeBase::GetOnTickFrom(AActor* Gameplay)
{
    int32 Index;
    TickingGameplays.Find(Gameplay, Index);
    if(Index == INDEX_NONE) return -1;
    
    if(AGameplay* ImplementsGameplay = Cast<AGameplay>(TickingGameplays[Index]))
    {
        return ImplementsGameplay->GetNumOfTicksBeforeRun();
    }
    
    return -1;
}

void AGameplayRunModeBase::SetGameplayToTickOn(AActor* Gameplay, const FGameStats& TickOn)
{
    if(Gameplay == nullptr) return;
    
    if(AGameplay* ImplementsGameplay = Cast<AGameplay>(Gameplay))
    {
        if(TickingGameplays.Contains(Gameplay))
            ImplementsGameplay->SetNumTicksBeforeRun(TickOn.Default);
        else
            TickingGameplays.Add(Gameplay);
    }
}

void AGameplayRunModeBase::StepTick(AActor* ActPtr, const int OnTick)
{
    if(TickingGameplays.Num() > 0 && RunCount.Value > 0)
    {
        if(ShouldTick(ActPtr, OnTick))
        {
            if(IGameplayOptionsInterface* TickGameplay = Cast<IGameplayOptionsInterface>(ActPtr))
            {
                RunCount.Value--;
                TickGameplay->Execute_Run(ActPtr);
            }
        }
    }
}

const int AGameplayRunModeBase::GetTurnNumBeforeRun(AActor* CheckGameplay, const int CurrTurn)
{
    const int TickOn = GetOnTickFrom(CheckGameplay);
    
    if(TickOn > 0)
    {
        int TurnsBeforeTick = CurrTurn;
        while(TurnsBeforeTick % TickOn != 0)
            TurnsBeforeTick++;
        
        return (TurnsBeforeTick - CurrTurn) + 1;
    }
    return TickOn;
}

const bool AGameplayRunModeBase::ShouldTick(AActor* CheckGameplay, const int OnTick)
{
    if(TickingGameplays.Contains(CheckGameplay))
    {
        if(AGameplay* ImplementsGameplay = Cast<AGameplay>(CheckGameplay))
        {
            return ImplementsGameplay->GetNumOfTicksBeforeRun() == 0 || OnTick % ImplementsGameplay->GetNumOfTicksBeforeRun() == 0;
        }
    }
    return false;
}

const TArray<AActor*> AGameplayRunModeBase::ShouldTick(const int OnTick)
{
    TArray<AActor*> CheckTickedGameplays;
    for(auto* It : TickingGameplays)
    {
        if(AGameplay* ImplementsGameplay = Cast<AGameplay>(It))
        {
            if(ImplementsGameplay->GetNumOfTicksBeforeRun() == 0 || OnTick % ImplementsGameplay->GetNumOfTicksBeforeRun() == 0)
                CheckTickedGameplays.Add(It);
        }
    }
    
    return CheckTickedGameplays;
}
