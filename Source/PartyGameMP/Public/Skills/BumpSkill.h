// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillBase.h"
#include "BumpSkill.generated.h"

/**
 *
 */
UCLASS()
class PARTYGAMEMP_API UBumpSkill : public USkillBase {
	GENERATED_BODY()

protected:

	UBumpSkill();

	virtual void BeginPlay() override;
	virtual void Interact() override;

	UFUNCTION()
	void OnOverlapBegin(AActor* LocalActor, AActor* OtherActor);
};
