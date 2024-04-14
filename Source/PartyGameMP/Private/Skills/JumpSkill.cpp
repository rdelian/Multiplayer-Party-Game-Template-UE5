// Fill out your copyright notice in the Description page of Project Settings.


#include "Skills/JumpSkill.h"
#include "GameFramework/CharacterMovementComponent.h"

UJumpSkill::UJumpSkill() {
	PrimaryComponentTick.bCanEverTick = true;

	Power = 500.0f;
	Cooldown = 1.0f;
}

// Yes yes, I could have used ACharacter->Jump() but where's the fun? :)
void UJumpSkill::Interact() {
	FVector Direction = FVector(0, 0, 1.0f) * Power;

	Cast<UCharacterMovementComponent>(Character->GetMovementComponent())->AddImpulse(Direction, true);

	Super::BeginCooldown();
}
