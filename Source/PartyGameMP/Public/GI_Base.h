// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Online/OnlineSessionNames.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "MiniGameBase.h"

#include "GI_Base.generated.h"

USTRUCT(BlueprintType)
struct FServerData {
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	int32 Id;

	UPROPERTY(BlueprintReadOnly)
	FString Name;

	UPROPERTY(BlueprintReadOnly)
	int32 CurrentPlayers;

	UPROPERTY(BlueprintReadOnly)
	int32 MaxPlayers;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FServerFoundDelegate, FServerData, ServerData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSearchingServersCompleteDelegate);

UCLASS()
class PARTYGAMEMP_API UGI_Base : public UGameInstance {
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FServerFoundDelegate OnServerFound;

	UPROPERTY(BlueprintAssignable)
	FOnSearchingServersCompleteDelegate OnSearchingServersComplete;

	UPROPERTY(BlueprintReadOnly)
	uint8 RoundsLeft;

	UPROPERTY(BlueprintReadOnly)
	uint8 MinPlayersToStartRound;

	UFUNCTION(BlueprintCallable)
	void SetRandomMinigame();

	UFUNCTION(BlueprintCallable)
	void ServerTravelToRandomMap();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FMinigameData GetCurrentMinigame() const { return CurrentMinigameRow; };


private:
	IOnlineSessionPtr SessionInterfaceRef;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	UPROPERTY()
	FName SessionNameKey;

	UPROPERTY(EditDefaultsOnly)
	UDataTable* MinigamesDataTable;

	UPROPERTY(EditDefaultsOnly)
	FMinigameData CurrentMinigameRow;

	UGI_Base();
	~UGI_Base();

	void Init() override;

	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSessionComplete(bool bWasSuccesful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnSessionFailure(const FUniqueNetId& UserId, ESessionFailure::Type FailureType);

	UFUNCTION(BlueprintCallable)
	void CreateServer();

	UFUNCTION(BlueprintCallable)
	void SearchServers();

	UFUNCTION(BlueprintCallable)
	void JoinServer(int32 atIndex);

	UFUNCTION(BlueprintCallable)
	void DestroySession();
};
