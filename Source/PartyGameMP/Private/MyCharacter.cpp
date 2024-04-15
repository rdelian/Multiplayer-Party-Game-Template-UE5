// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerState.h"
#include "EnhancedInputSubsystems.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "Engine/LocalPlayer.h"
#include "SkillBase.h"

AMyCharacter::AMyCharacter() {
	PrimaryActorTick.bCanEverTick = false;

	// Create spring arm
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 350.0f;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;

	// Create camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	// Trigger Box
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetupAttachment(RootComponent);
	TriggerBox->SetBoxExtent(FVector(34.0f, 34.0f, 34.f));
}

void AMyCharacter::BeginPlay() {
	Super::BeginPlay();

	APlayerController* Pc = Cast<APlayerController>(Controller);
	if (!Pc) return;

	UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(Pc->GetLocalPlayer());
	if (!EnhancedInputSubsystem) return;

	EnhancedInputSubsystem->AddMappingContext(DefaultMappingContext, 0);
}

void AMyCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (!EnhancedInputComponent) return;

	EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &AMyCharacter::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyCharacter::Look);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AMyCharacter::JumpAbility);
}

void AMyCharacter::Move(const FInputActionValue& Value) {
	const FVector2D InputAxisVector = Value.Get<FVector2D>();
	const FVector ForwardVector = Camera->GetForwardVector();
	const FVector RightVector = Camera->GetRightVector();
	const FVector MoveDirection = ForwardVector * InputAxisVector.Y + RightVector * InputAxisVector.X;

	AddMovementInput(MoveDirection, 1.0f, false);
}

void AMyCharacter::Look(const FInputActionValue& Value) {
	const FVector2D InputAxisVector = Value.Get<FVector2D>();

	SpringArm->AddRelativeRotation(FRotator(-InputAxisVector.Y, InputAxisVector.X, 0.0f), false);
}

void AMyCharacter::JumpAbility(const FInputActionValue& Value) {
	/* We could have stored the ability component, but the operation is very fast - testing for microseconds still shows 0.000000 */
	if (USkillBase* JumpSkill = FindComponentByTag<USkillBase>("JUMP_SKILL")) {
		JumpSkill->TryToInteract();
	}
}