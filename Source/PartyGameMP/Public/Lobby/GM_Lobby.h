// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/GameModeBase.h"
#include "GI_Base.h"

#include "GM_Lobby.generated.h"

/**
 *
 */
UCLASS()
class PARTYGAMEMP_API AGM_Lobby : public AGameModeBase {
	GENERATED_BODY()

private:
	TObjectPtr<UGI_Base> GameInstanceRef;

	UFUNCTION(BlueprintCallable)
	void Start();

	void BeginPlay() override;

	void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	void Logout(AController* Exiting) override;
};
