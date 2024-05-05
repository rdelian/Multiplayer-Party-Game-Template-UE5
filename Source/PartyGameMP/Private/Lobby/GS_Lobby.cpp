// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/GS_Lobby.h"

AGS_Lobby::AGS_Lobby() {
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.5f;

	PlayersCount = 0;
}

void AGS_Lobby::BeginPlay() {
	Super::BeginPlay();

	GetWorld()->GetFirstLocalPlayerFromController()->PlayerController->SetShowMouseCursor(true);
	//GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
}

void AGS_Lobby::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	const int CurrentPlayersNum = PlayerArray.Num();

	if (PlayersCount == CurrentPlayersNum) return;

	PlayersCount = CurrentPlayersNum;
	OnPlayersCountUpdate.Broadcast();
}
