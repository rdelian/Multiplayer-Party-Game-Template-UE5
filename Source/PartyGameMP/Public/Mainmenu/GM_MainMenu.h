// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GM_MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class PARTYGAMEMP_API AGM_MainMenu : public AGameModeBase {
	GENERATED_BODY()
	
	virtual void BeginPlay() override;
};
