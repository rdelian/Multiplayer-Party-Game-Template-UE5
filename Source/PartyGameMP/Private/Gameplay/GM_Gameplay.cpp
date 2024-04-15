// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/GM_Gameplay.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/PlayerController.h"
#include "Minigames/SumoMinigame.h"
#include "MinigameBase.h"
#include "Gameplay/GS_Gameplay.h"
#include "SkillBase.h"

AGM_Gameplay::AGM_Gameplay() {
	PrimaryActorTick.bCanEverTick = true;
	//PrimaryActorTick.TickInterval = 1.0f;

	bUseSeamlessTravel = true;

	MatchState = EMatchState::NOT_STARTED;
	PlayersNum = 0;
	PlayersAlive = 0;
	RoundStartDelay = 5.0f;
}

void AGM_Gameplay::OnPlayerPawnDestroyed(AActor* DestroyedActor) {
	/**
	* A "cast check" is not needed at this time because only "playing" Player Pawn binds this
	* @see AGM_Gameplay::PostLogin
	*/
	if (MatchState != EMatchState::STARTED) return;
	if (!DestroyedActor) return;

	PlayersAlive--;
	UE_LOG(LogTemp, Warning, TEXT("[%hs] PlayersAlive--: %d"), __FUNCTION__, PlayersAlive);

	ShouldEnd();
}

void AGM_Gameplay::BeginPlay() {
	Super::BeginPlay();

	GameInstanceRef = Cast<UGI_Base>(GetGameInstance());

	UClass* CurrentMinigameClass = GameInstanceRef->GetCurrentMinigame().MinigameClass;

	if (!CurrentMinigameClass) {
		UE_LOG(LogTemp, Error, TEXT("[%hs] GetCurrentMinigame() returned a nullptr."), __FUNCTION__);
		return;
	}

	AMinigameBase* Minigame = GetWorld()->SpawnActor<AMinigameBase>(CurrentMinigameClass, FVector::ZeroVector, FRotator::ZeroRotator);

	if (!Minigame) {
		UE_LOG(LogTemp, Error, TEXT("[%hs] Minigame actor failed to spawn."), __FUNCTION__);
		return;
	}
}

void AGM_Gameplay::Tick(float DeltaSeconds) {
	auto FormattedText = FString::Printf(TEXT("P_Num: %d, P_Alive: %d, MinP: %d"),
		PlayersNum, PlayersAlive, GameInstanceRef->MinPlayersToStartRound
	);

	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Green, FormattedText, true);
}

bool AGM_Gameplay::ShouldStart() const {
	/* Called from PostLogin which runs before BeginPlay() */
	if (!GameInstanceRef) return false;

	UE_LOG(LogTemp, Warning, TEXT("[%hs] PlayersNum: %d. MinPlayersToStartRound: %d"), __FUNCTION__, MatchState, GameInstanceRef->MinPlayersToStartRound);

	return MatchState == EMatchState::NOT_STARTED && PlayersNum == GameInstanceRef->MinPlayersToStartRound;
}

void AGM_Gameplay::StartRound() {
	PlayersAlive = PlayersNum;
	MatchState = EMatchState::STARTED;

	SetPlayersSkills();

	UE_LOG(LogTemp, Warning, TEXT("[%hs] Call OnRoundStarted. %d"), __FUNCTION__, GameInstanceRef->MinPlayersToStartRound);
	OnRoundStarted.Broadcast();
}

void AGM_Gameplay::ShouldEnd() {
	UE_LOG(LogTemp, Warning, TEXT("[%hs] PlayersAlive: %d. MinPlayersToStartRound: %d"), __FUNCTION__, PlayersAlive, GameInstanceRef->MinPlayersToStartRound);
	/**
	 * For now there is no use case only for the check, like ShouldStart(),
	 * so this function does a bit more than it should do ... maybe
	 */
	if (PlayersAlive > 1) return;

	MatchState = EMatchState::ABOUT_TO_END;

	GameInstanceRef->RoundsLeft--;

	if (GameInstanceRef->RoundsLeft == 0) {
		EndMatch();
	}
	else {
		EndRound();
	}
}

void AGM_Gameplay::EndRound() {
	GameInstanceRef->SetRandomMinigame();
	GameInstanceRef->ServerTravelToRandomMap(); /* could be delayed for UI purposes */

	OnRoundEnded.Broadcast();

	UE_LOG(LogTemp, Warning, TEXT("[%hs] Rounds left: %d"), __FUNCTION__, GameInstanceRef->RoundsLeft);
}

void AGM_Gameplay::EndMatch() {
	GetWorld()->ServerTravel("/Game/Maps/Lobby?listen"); /* could be delayed for UI purposes*/

	OnMatchEnded.Broadcast();

	UE_LOG(LogTemp, Warning, TEXT("[%hs] Match ended, traveling to Lobby"), __FUNCTION__);
}

void AGM_Gameplay::PostLogin(APlayerController* NewPlayer) {
	UE_LOG(LogTemp, Warning, TEXT("[%hs] PostLogin"), __FUNCTION__);

	/* Handle player connecting so they could at least play next round */
	if (EnumHasAnyFlags(MatchState, EMatchState::IN_PROGRESS)) {
		GameInstanceRef->MinPlayersToStartRound++;
		UE_LOG(LogTemp, Error, TEXT("[%hs] MinPlayersToStartRound=%d"), __FUNCTION__, GameInstanceRef->MinPlayersToStartRound);
	}

	/* Don't spawn a pawn for the player if the match has already started */
	if (MatchState == EMatchState::STARTED) return;

	Super::PostLogin(NewPlayer);

	NewPlayer->GetPawn()->OnDestroyed.AddDynamic(this, &AGM_Gameplay::OnPlayerPawnDestroyed);

	PlayersNum++;

	if (ShouldStart()) {
		FTimerHandle TimerHandle;
		FTimerDelegate TimerDelegate;

		TimerDelegate.BindUObject(this, &AGM_Gameplay::StartRound);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, RoundStartDelay, false);

		MatchState = EMatchState::ABOUT_TO_START;

		UE_LOG(LogTemp, Warning, TEXT("[%hs] Round is about to start"), __FUNCTION__);
	}
}

void AGM_Gameplay::Logout(AController* Exiting) {
	UE_LOG(LogTemp, Warning, TEXT("[%hs] Logout. MatchState=%d"), __FUNCTION__, MatchState);

	/**
	 * "if (MatchState & EMatchState::IN_PROGRESS)" bitwise implicit conversion to bool is not possible in strongly typed enums
	 * therefore we either use "!= EMatchState::NOT_STARTED" or EnumHasAnyFlags
	 * @see https://dev.epicgames.com/documentation/en-us/unreal-engine/epic-cplusplus-coding-standard-for-unreal-engine#strongly-typedenums
	 */
	if (EnumHasAnyFlags(MatchState, EMatchState::IN_PROGRESS)) {
		GameInstanceRef->MinPlayersToStartRound--;
		UE_LOG(LogTemp, Error, TEXT("[%hs] MinPlayersToStartRound=%d"), __FUNCTION__, GameInstanceRef->MinPlayersToStartRound);
	}

	/** 
	 * We do this only on ::ABOUT_TO_START because if the game already ::STARTED - PlayersNum won't increase @see PostLogin()
	 * therefore we don't need to decreased if the player joined after ::STARTED
	 */
	if (MatchState == EMatchState::ABOUT_TO_START) {
		PlayersNum--;
	}

	Super::Logout(Exiting);
}

void AGM_Gameplay::SetPlayersSkills() {
	auto GsRef = Cast<AGameStateBase>(GameState);

	for (const auto& Ps : GsRef->PlayerArray) {
		if (TObjectPtr<APawn> PlayerPawn = Ps->GetPawn()) {
			for (const auto& SkillComponent : GameInstanceRef->GetCurrentMinigame().SkillsClass) {
				PlayerPawn->AddComponentByClass(SkillComponent, false, FTransform::Identity, false);
			}
		}
	}
}
