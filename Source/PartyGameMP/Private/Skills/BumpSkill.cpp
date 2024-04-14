// Fill out your copyright notice in the Description page of Project Settings.


#include "Skills/BumpSkill.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

UBumpSkill::UBumpSkill() {
	Power = 1.5f;
	Cooldown = 0.5f;
}

void UBumpSkill::BeginPlay() {
	Super::BeginPlay();

	if (!Character->HasAuthority()) return;

	Character->OnActorBeginOverlap.AddDynamic(this, &UBumpSkill::OnOverlapBegin);
}

void UBumpSkill::OnOverlapBegin(AActor* LocalActor, AActor* OtherActor) {
	if (!Available) return;
	if (!Cast<ACharacter>(OtherActor)) return;

	FVector LocalVelocity = LocalActor->GetVelocity();
	FVector OtherVelocity = OtherActor->GetVelocity();

	if (LocalVelocity.Length() < OtherVelocity.Length()) return;

	LocalActor->GetComponentByClass<UCharacterMovementComponent>()->AddImpulse(OtherVelocity * Power, true);
	OtherActor->GetComponentByClass<UCharacterMovementComponent>()->AddImpulse(LocalVelocity * Power, true);

	Super::BeginCooldown();
}