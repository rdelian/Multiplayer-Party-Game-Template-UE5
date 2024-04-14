// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillBase.h"
#include "JumpSkill.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PARTYGAMEMP_API UJumpSkill : public USkillBase {
	GENERATED_BODY()

protected:

	UJumpSkill();

	virtual void Interact() override;
};
