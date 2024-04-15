// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/GameModeBase.h"
#include "MyCharacter.h"
#include "GI_Base.h"

#include "GM_Gameplay.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartRoundDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndRoundDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndMatchDelegate);

UENUM(BlueprintType, meta = (BitFlags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EMatchState : uint8 {
	NOT_STARTED = 0b00000000, // 0

	ABOUT_TO_START = 0b00000001, // 1
	STARTED = 0b00000010, // 2
	ABOUT_TO_END = 0b00000100, // 4

	IN_PROGRESS = ABOUT_TO_START + STARTED // 0b00000011 -> 3
};
ENUM_CLASS_FLAGS(EMatchState);

UCLASS()
class PARTYGAMEMP_API AGM_Gameplay : public AGameModeBase {
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnStartRoundDelegate OnRoundStarted;

	UPROPERTY(BlueprintAssignable)
	FOnEndRoundDelegate OnRoundEnded;

	UPROPERTY(BlueprintAssignable)
	FOnEndRoundDelegate OnMatchEnded;

	UPROPERTY(EditDefaultsOnly)
	float RoundStartDelay;

	UPROPERTY(BlueprintReadOnly);
	TObjectPtr<UGI_Base> GameInstanceRef;

	UPROPERTY(BlueprintReadOnly);
	EMatchState MatchState;

private:
	uint8 PlayersNum;
	uint8 PlayersAlive;

	AGM_Gameplay();

	/**
	 * @brief Return true if the match didn't started yet and all players connected
	 */
	bool ShouldStart() const;

	/**
	 * @brief Set the number of players alive
	 */
	void StartRound();

	/**
	 * @brief If there is only one player alive, end the round and if there are no rounds left, end the match
	 */
	void ShouldEnd();

	/**
	 * @brief Generate a new minigame and travel to a random map from that minigame
	 */
	void EndRound();

	/**
	 * @brief Travel to Lobby
	 */
	void EndMatch();

	/**
	 * @brief Add the skills component to players pawn based on the current minigame
	 */
	void SetPlayersSkills();

	/**
	 * @brief Update alive players count and check if the round/match ShouldEnd()
	 * @param PlayerCharacter Current player pawn
	 */
	UFUNCTION()
	void OnPlayerPawnDestroyed(AActor* DestroyedActor);

private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	/**
	 * @brief Start the match when all players have connected
	 */
	virtual void PostLogin(APlayerController* NewPlayer) override;

	/**
	 * @brief Check if the player should be removed from the match and if it ShouldEnd, ignore spectators
	 */
	virtual void Logout(AController* Exiting) override;

};
