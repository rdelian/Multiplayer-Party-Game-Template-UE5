// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/GM_Lobby.h"
#include "Lobby/GS_Lobby.h"
#include "GameFramework/GameState.h"

void AGM_Lobby::BeginPlay() {
	Super::BeginPlay();

	GameStateClass = AGS_Lobby::StaticClass();

	// Store the GameInstance so we avoid casting multiple times, even if it's 
	//  not used multiple times in this class it could be useful in blueprints
	GameInstanceRef = Cast<UGI_Base>(GetGameInstance());
}

void AGM_Lobby::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) {
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
}

void AGM_Lobby::Logout(AController* Exiting) {
	Super::Logout(Exiting);
}

void AGM_Lobby::Start() {
	GameInstanceRef->RoundsLeft = 3; // Could be set from the UI
	GameInstanceRef->MinPlayersToStartRound = GameState->PlayerArray.Num();
	GameInstanceRef->SetRandomMinigame();
	GameInstanceRef->ServerTravelToRandomMap();
}
