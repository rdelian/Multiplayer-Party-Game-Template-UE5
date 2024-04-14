// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillBase.h"

USkillBase::USkillBase() {
	PrimaryComponentTick.bCanEverTick = false;
	bAutoActivate = true;

	Available = true;
	Power = 1.0f;
	Cooldown = 0.5f;
}

void USkillBase::BeginPlay() {
	Super::BeginPlay();

	Character = Cast<AMyCharacter>(GetOwner());
}

void USkillBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USkillBase::Interact() {
	UE_LOG(LogTemp, Warning, TEXT("SkillBase::Interact()"));
}

void USkillBase::BeginCooldown() {
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateWeakLambda(this, [this]() { Available = true; });

	Available = false;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, Cooldown, false);
}