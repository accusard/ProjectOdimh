// Copyright 2022 Vanny Sou. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectOdimh/ClassInterface/DataSaveInterface.h"
#include "ProjectOdimh/ClassInterface/TurnEventInterface.h"
#include "Grid.generated.h"

class ATile;
class USoundCue;

USTRUCT()
struct FTileCount
{
    GENERATED_USTRUCT_BODY()
    
    UPROPERTY()
    int Type = -1;
    
    UPROPERTY()
    int TotalNum = 0;
};

USTRUCT(BlueprintType)
struct FGridSpawningParameters
{
    GENERATED_USTRUCT_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    bool bRandomTileType;
    
    UPROPERTY(BlueprintReadOnly)
    bool bLoadSprites;
    
    UPROPERTY(BlueprintReadOnly)
    FString SaveSlotName;
    
    FGridSpawningParameters();
    FGridSpawningParameters(const bool bRandomizeTiles, const bool bLoadDefaultSprites, const FString& SlotName);
};
/**
 * A single grid handles events that  pertain to and contain grid coordinations of each Tile objects
 */
UCLASS()
class PROJECTODIMH_API AGrid : public AActor, public IDataSaveInterface, public ITurnEventInterface
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadOnly)
    float DELAY_BETWEEN_TILE_BURSTS = 0.08f;
    
public:
    friend class UGridEvent;
    
    // Sets default values for this actor's properties
    AGrid();
    
    //~DataSave interface
    virtual void Save(USaveGame* Data) override;
    virtual const bool Load(USaveGame* Data) override;
    //~DataSave interface
    
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    const FVector2D& GetTileCoords(const FVector& Location);
    
    const FVector2D& GetTileCoords(ATile* Tile);
    
    const FVector2D GetTileCoords(const uint32 TileIndex);
    
    UFUNCTION(BlueprintImplementableEvent)
    ATile* GetTile(const FIntPoint& Coord) const;
    
    /** Collect data on the number of type occurences that is currently on the grid */
    const TArray<FTileCount> TallyAllTileTypes();
    
    /** Find the number of occurences based on a tile type */
    UFUNCTION(BlueprintCallable)
    const int GetNumOfOccurence(const int Type);
    
    /** Check the grid if there are available matching tiles */
    UFUNCTION(BlueprintCallable)
    const bool MatchingTilesAvailable(int NumOfTileTypes);
    
    const bool HasTilePositionChanged(ATile* Tile);
    
    void NotifyGridStateChanged();
    
    UFUNCTION()
    void TilesChangedPosition(AActor* Tile);
    
    UFUNCTION()
    void SetOldLocation(AActor* Tile);
    
    /** Determines the distance between a Tile object and another location */
    const float GetDistanceBetween(ATile* Tile, FVector2D OtherPosition);
    
    /** Get the distance between two Tile objects. */
    const float GetDistanceBetween(ATile* TileA, ATile* TileB);
    
    UFUNCTION(BlueprintCallable)
    void OnInitialTilesSpawned(TArray<ATile*> NonMatchTiles);
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    USoundCue* StateChangeCue;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    USoundCue* TileMatchCue;
    
    void NewGrid();
    
    void OnEventEnd();
    
    const int32 GetTotalMatchedThisTurn() { return TotalNumTilesMatchedThisTurn; }
    
    void ResetAccumulatedMatchedTiles();
    
    /** Blueprint Events */
public:
    /** Get the value of GridSize that was assigned in blueprint */
    UFUNCTION(BlueprintImplementableEvent)
    const float GetGridSize() const;
    
    UFUNCTION(BlueprintImplementableEvent)
    const bool IsTilesBursting() const;
    
    UFUNCTION(BlueprintImplementableEvent)
    const bool TilesFinishedFilling() const;
    
    UFUNCTION(BlueprintImplementableEvent)
    const bool IsPickState() const;
    
    UFUNCTION(BlueprintImplementableEvent)
    TArray<ATile*> GetTiles();
    
    UFUNCTION(BlueprintImplementableEvent)
    void InitTiles(const FGridSpawningParameters& Param);
    
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void NotifyBoardStateChanged(uint8 OldState, UGridEvent* GridEvent, const TArray<ATile*>& ReservingTiles);
    
    UFUNCTION(BlueprintImplementableEvent)
    void RegisterBoardState(const FName& NewState) const;
    
protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    
    /** Call the blueprint library function GetGridFromLocation to get the grid coordinates by using a Vector location
     * and set it to this object's member data - GridLocation */
    UFUNCTION(BlueprintImplementableEvent)
    void OnRetrieveGridLocation(const FVector& Location);
    
    /** Spawn a tile on the grid based on a transform and tile type. */
    UFUNCTION(BlueprintCallable)
    ATile* SpawnTile(TSubclassOf<ATile> Class, const FTransform& Transform, const int Type, const bool bRegisterPos);
    
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    ATile* SpawnTileToGrid(TSubclassOf<ATile> Class, const FVector2D& GridCoord, const int Type, const bool bRegisterPos);
    
    /** Spawn an actor directly to grid. Only possible if that grid's space is empty. Can notify GameMode of a grid state change after spawning. */
    UFUNCTION(BlueprintNativeEvent)
    void SpawnTileToEmptyGrid(ATile* Tile, const bool bNotifyStateChange);
    
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, DisplayName="OnTurnEnded")
    void OnTurnEnded(AActor* EvtCaller, class UBaseEvent* Event);
    
    UFUNCTION(BlueprintCallable)
    void HandleTilesSwapped(AController* GridController, ATile* DynamicTile, ATile* StaticTile);
    
    UFUNCTION(BlueprintCallable)
    TArray<ATile*> RandomizeNewTiles(TArray<class ATile*> OldTiles, TSubclassOf<ATile> Class);
    
    UFUNCTION(BlueprintImplementableEvent)
    const int GetRandomMatchType() const;
    
    UFUNCTION(BlueprintImplementableEvent)
    void RegisterPosition(ATile* Tile, const FVector2D& Coord);
    
    /** The grid coordinates that was last retrieved from blueprint */
    UPROPERTY(BlueprintReadWrite)
    FVector2D GridLocation;
    
    /** Tracks a list of tile types to spawn next */
    UPROPERTY(BlueprintReadWrite)
    TArray<int> NextTileToSpawn;
    
    /** The required number of consecutive row or column for the game to consider it a successful tile match */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tiles")
    int TilesNeededForMatch;
    
    UPROPERTY(BlueprintReadWrite)
    int32 TotalNumTilesMatchedThisTurn;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 SizeX;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 SizeY;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tiles")
    uint8 bSaveTerminalCoords : 1;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tiles")
    uint8 bDefaultTerminalCoordOnBeginPlay :1;
    
    UPROPERTY(BlueprintReadWrite)
    uint8 bGridStateChanged : 1;
    
    UPROPERTY(BlueprintReadWrite)
    uint8 bTilesHaveSwapped : 1;
    
private:
    /** A state in which there are no matching tiles (2 or less occurences) available */
    uint8 bNoMatchingTiles : 1;
};

