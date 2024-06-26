// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Info.h"
#include "GameFramework/Actor.h"

#include "MiniGameBase.generated.h"

class AMinigameBase;
class AGM_Gameplay;
class USkillBase;

USTRUCT(BlueprintType)
struct FMinigameData : public FTableRowBase {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta = (DisplayName = "Title"))
	FText Title;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta = (DisplayName = "Minigame"))
	TSubclassOf<AMinigameBase> MinigameClass;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta = (DisplayName = "Skills"))
	TArray<TSubclassOf<USkillBase>> SkillsClass;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta = (DisplayName = "Maps"))
	TArray<TSoftObjectPtr<UWorld>> Maps;
};

UCLASS()
class PARTYGAMEMP_API AMinigameBase : public AActor {
	GENERATED_BODY()

public:
	AMinigameBase();

protected:
	UPROPERTY()
	TObjectPtr<AGM_Gameplay> GameModeRef;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	virtual void Start();
};
