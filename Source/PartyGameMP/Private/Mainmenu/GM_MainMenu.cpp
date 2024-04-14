// Fill out your copyright notice in the Description page of Project Settings.


#include "Mainmenu/GM_MainMenu.h"

AGM_MainMenu::AGM_MainMenu()
{
	
}

void AGM_MainMenu::BeginPlay() {
	Super::BeginPlay();

	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
}
