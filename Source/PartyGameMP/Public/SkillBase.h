// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "MyCharacter.h"
#include "Components/ActorComponent.h"
#include "SkillBase.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), Blueprintable)
class PARTYGAMEMP_API USkillBase : public UActorComponent {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void TryToInteract();

protected:
	USkillBase();

	TObjectPtr<AMyCharacter> Character;

	UPROPERTY(EditDefaultsOnly)
	FText Name;

	UPROPERTY(EditDefaultsOnly)
	float Cooldown;

	UPROPERTY(EditDefaultsOnly)
	bool bAvailable;

	UPROPERTY(EditDefaultsOnly)
	float Power;

	UPROPERTY(EditDefaultsOnly)
	FName Tag;

	/*
	 * Sets bAvailable to false and then to true after the Cooldown delay
	 */
	UFUNCTION(BlueprintCallable)
	void BeginCooldown();

	/*
	 * Actual skill functionality, should be overriden in child classes
	 */
	UFUNCTION(BlueprintCallable)
	virtual void Interact();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
