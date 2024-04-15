// Fill out your copyright notice in the Description page of Project Settings.


#include "Skills/JumpSkill.h"
#include "GameFramework/CharacterMovementComponent.h"

UJumpSkill::UJumpSkill() {
	Name = FText::FromString("Jump");
	Power = 500.0f;
	Cooldown = 1.0f;
	Tag = FName("JUMP_SKILL");
}

// TODO: Do a ground ray cast check
void UJumpSkill::Interact() {
	FVector Direction = FVector(0, 0, 1.0f) * Power;

	/**
	 * Yes yes, I could have used ACharacter->Jump() but where is the fun? :)
	 * I'm doing this only to show how to add impulse/force to the character
	 * Also if you'd like to avoid casting, you could store the character movement component in a variable on BeginPlay
	 */
	Cast<UCharacterMovementComponent>(Character->GetMovementComponent())->AddImpulse(Direction, true);
}
