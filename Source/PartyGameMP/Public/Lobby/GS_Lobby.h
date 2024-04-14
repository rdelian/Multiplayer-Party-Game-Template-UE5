// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GS_Lobby.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayersCountUpdateSignature);

UCLASS()
class PARTYGAMEMP_API AGS_Lobby : public AGameStateBase
{
	GENERATED_BODY()

	AGS_Lobby();

	int PlayersCount;

	UPROPERTY(BlueprintAssignable)
	FPlayersCountUpdateSignature OnPlayersCountUpdate;

	void BeginPlay() override;
	void Tick(float DeltaTime) override;
	
};
