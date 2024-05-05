// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillBase.h"

USkillBase::USkillBase() {
	PrimaryComponentTick.bCanEverTick = false;
	bAutoActivate = true;

	Name = FText::FromString("UNK_NAME");
	bAvailable = true;
	Cooldown = 1.0f;
	Power = 1.0f;
	Tag = NAME_None;
}

void USkillBase::BeginPlay() {
	Super::BeginPlay();

	Character = Cast<AMyCharacter>(GetOwner());

	if (Tag.IsNone()) {
		UE_LOG(LogTemp, Error, TEXT("[%hs] | Tag field is missing for %s"), __FUNCTION__, *GetName());
		return;
	}

	ComponentTags.Add(Tag);
}

void USkillBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USkillBase::TryToInteract() {
	if (!bAvailable)
		return;

	Interact();
	BeginCooldown();
}

void USkillBase::Interact() {
	UE_LOG(LogTemp, Error, TEXT("SkillBase::Interact() base class"));
}

void USkillBase::BeginCooldown() {
	/** 
	 * Functionality could also be implemented without a timer, we could have stored a timestamp
	 *  and verify in TryToInteract() if the Cooldown time has passed (perhaps in another function for reusability)
	 */

	FTimerHandle TimerHandle;
	const FTimerDelegate TimerDelegate = FTimerDelegate::CreateWeakLambda(this, [this]() { bAvailable = true; });

	bAvailable = false;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, Cooldown, false);
}
