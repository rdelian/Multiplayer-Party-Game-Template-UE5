// Fill out your copyright notice in the Description page of Project Settings.


#include "Minigames/SumoMinigame.h"
#include "Components/BoxComponent.h"
#include "MyCharacter.h"
#include "Gameplay/GM_Gameplay.h"

ASumoMinigame::ASumoMinigame() {
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetupAttachment(RootComponent);
	TriggerBox->SetBoxExtent(FVector(5000.0f, 5000.0f, 20.f));
	TriggerBox->SetGenerateOverlapEvents(true);
	TriggerBox->bTraceComplexOnMove = true;
	TriggerBox->bHiddenInGame = false;

	RootComponent = TriggerBox;
}

void ASumoMinigame::BeginPlay() {
	Super::BeginPlay();

	// Set the pos under the spawn points
	const FVector SpawnPointLocation = GameModeRef->FindPlayerStart(nullptr)->GetActorLocation();
	SetActorLocation(SpawnPointLocation - FVector(0.0f, 0.0f, 500.0f));

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ASumoMinigame::OnOverlapBegin);

	UE_LOG(LogTemp, Warning, TEXT("[%hs] TriggerBox Location set to: %s"), __FUNCTION__,
	       *GetActorLocation().ToString());
}

void ASumoMinigame::Start() {
	UE_LOG(LogTemp, Warning, TEXT("[%hs]"), __FUNCTION__);
}

void ASumoMinigame::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                   const FHitResult& SweepResult){
	if (AMyCharacter* PlayerPawn = Cast<AMyCharacter>(OtherActor)) {
		PlayerPawn->Destroy();
	}
}
