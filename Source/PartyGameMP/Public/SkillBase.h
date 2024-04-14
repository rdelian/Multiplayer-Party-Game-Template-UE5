// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "MyCharacter.h"
#include "Components/ActorComponent.h"
#include "SkillBase.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), Blueprintable)
class PARTYGAMEMP_API USkillBase : public UActorComponent {
	GENERATED_BODY()

protected:
	USkillBase();

	TObjectPtr<AMyCharacter> Character;

	UPROPERTY(BlueprintReadOnly)
	FString Name;

	UPROPERTY(BlueprintReadOnly)
	float Cooldown;

	UPROPERTY(BlueprintReadOnly)
	bool Available;

	UPROPERTY(BlueprintReadOnly)
	float Power;

	/*
	* Bind on different InputActions, should be overriden in child classes
	*/
	UFUNCTION(BlueprintCallable)
	virtual void Interact();

	/*
	* Set the Available variable to true after the cooldown delay
	*/
	UFUNCTION(BlueprintCallable)
	void BeginCooldown();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
