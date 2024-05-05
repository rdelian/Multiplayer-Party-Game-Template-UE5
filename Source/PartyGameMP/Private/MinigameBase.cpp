// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniGameBase.h"
#include "Gameplay/GM_Gameplay.h"

AMinigameBase::AMinigameBase() {
	PrimaryActorTick.bCanEverTick = false;
}

void AMinigameBase::BeginPlay() {
	Super::BeginPlay();

	bNetLoadOnClient = false;
	bReplicates = false;

	UE_LOG(LogTemp, Warning, TEXT("AMinigameBase::BeginPlay() | Auth=%d"), (HasAuthority() ? 1 : 0));

	GameModeRef = Cast<AGM_Gameplay>(GetWorld()->GetAuthGameMode());

	if (!GameModeRef) {
		UE_LOG(LogTemp, Error, TEXT("AMinigameBase::BeginPlay() -> Current gamemode is different than AGM_Gameplay"));
		return;
	}

	GameModeRef->OnRoundStarted.AddDynamic(this, &AMinigameBase::Start);
}

void AMinigameBase::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AMinigameBase::Start() {
	UE_LOG(LogTemp, Warning, TEXT("AMinigameBase::Start()"));
}
