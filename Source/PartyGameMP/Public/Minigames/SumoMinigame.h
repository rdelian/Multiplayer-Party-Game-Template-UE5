// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MiniGameBase.h"
#include "SumoMinigame.generated.h"

UCLASS()
class PARTYGAMEMP_API ASumoMinigame : public AMinigameBase {
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleDefaultsOnly)
	class UBoxComponent* TriggerBox;

	ASumoMinigame();

	virtual void BeginPlay() override;
	virtual void Start() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
