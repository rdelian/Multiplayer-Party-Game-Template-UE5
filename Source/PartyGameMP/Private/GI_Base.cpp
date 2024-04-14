// Fill out your copyright notice in the Description page of Project Settings.


#include "GI_Base.h"
#include "Minigames/SumoMinigame.h"

UGI_Base::UGI_Base() {

}

UGI_Base::~UGI_Base() {
	if (SessionInterfaceRef.IsValid()) {
		DestroySession();
	}
}

void UGI_Base::Init() {
	Super::Init();
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (!Subsystem) return;

	SessionInterfaceRef = Subsystem->GetSessionInterface();
	if (!SessionInterfaceRef.IsValid()) return;

	SessionNameKey = FName("TestSession");

	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->bIsLanQuery = (Subsystem->GetSubsystemName() == "NULL");
	SessionSearch->MaxSearchResults = 10000;
	SessionSearch->QuerySettings.Set("SEARCH_PRESENCE", true, EOnlineComparisonOp::Equals);

	SessionInterfaceRef->OnCreateSessionCompleteDelegates.AddUObject(this, &UGI_Base::OnCreateSessionComplete);
	SessionInterfaceRef->OnFindSessionsCompleteDelegates.AddUObject(this, &UGI_Base::OnFindSessionComplete);
	SessionInterfaceRef->OnJoinSessionCompleteDelegates.AddUObject(this, &UGI_Base::OnJoinSessionComplete);
	SessionInterfaceRef->OnSessionFailureDelegates.AddUObject(this, &UGI_Base::OnSessionFailure);

	if (!MinigamesDataTable) {
		UE_LOG(LogTemp, Error, TEXT("[%hs]-> MinigamesDataTable must be set in the Class Defaults of the Blueprint"), __FUNCTION__);
	}

	UE_LOG(LogTemp, Warning, TEXT("[S] Init()"));
}

// Network - Sessions

void UGI_Base::CreateServer() {
	TSharedPtr<FOnlineSessionSettings> SessionSettings = MakeShareable(new FOnlineSessionSettings());
	SessionSettings->bIsDedicated = false;
	SessionSettings->bIsLANMatch = (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL");
	SessionSettings->bAllowJoinInProgress = true;
	SessionSettings->bShouldAdvertise = true;
	SessionSettings->bUsesPresence = true;
	SessionSettings->NumPublicConnections = 5;

	SessionInterfaceRef->CreateSession(*GetWorld()->GetFirstLocalPlayerFromController()->GetPreferredUniqueNetId(), SessionNameKey, *SessionSettings);

	UE_LOG(LogTemp, Warning, TEXT("[I] CreateServer()"));
}

void UGI_Base::SearchServers() {
	SessionInterfaceRef->FindSessions(0, SessionSearch.ToSharedRef());

	UE_LOG(LogTemp, Warning, TEXT("[I] SearchServers()"));
}

void UGI_Base::JoinServer(int32 AtIndex) {
	SessionInterfaceRef->JoinSession(*GetWorld()->GetFirstLocalPlayerFromController()->GetPreferredUniqueNetId(), SessionNameKey, SessionSearch->SearchResults[AtIndex]);

	UE_LOG(LogTemp, Warning, TEXT("[I] JoinServer()"));
}

void UGI_Base::DestroySession() {
	SessionInterfaceRef->DestroySession(SessionNameKey);

	UE_LOG(LogTemp, Warning, TEXT("[S] DestroySession()"));
}

void UGI_Base::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful) {
	if (!bWasSuccessful) {
		UE_LOG(LogTemp, Warning, TEXT("[E] Failed to create session"));
		return;
	}

	GetWorld()->ServerTravel("/Game/Maps/Lobby?listen");

	UE_LOG(LogTemp, Warning, TEXT("OnCreateSessionComplete -> Server travel to lobby"));
}

void UGI_Base::OnFindSessionComplete(bool bWasSuccesful) {
	if (!bWasSuccesful) {
		UE_LOG(LogTemp, Warning, TEXT("[E] Failed to find session"));
		OnSearchingServersComplete.Broadcast();
		return;
	}

	if (!SessionSearch->SearchResults.Num()) {
		UE_LOG(LogTemp, Warning, TEXT("[E] No sessions found"));
		OnSearchingServersComplete.Broadcast();
		return;
	}

	TArray<FOnlineSessionSearchResult> SearchResults = SessionSearch->SearchResults;

	for (int i = 0; i < SearchResults.Num(); i++) {
		FOnlineSessionSearchResult Result = SearchResults[i];

		FServerData Data;
		Data.Name = Result.Session.OwningUserName;
		Data.MaxPlayers = Result.Session.SessionSettings.NumPublicConnections;
		Data.CurrentPlayers = Data.MaxPlayers - Result.Session.NumOpenPublicConnections;
		Data.Id = i;

		OnServerFound.Broadcast(Data);
	}

	OnSearchingServersComplete.Broadcast();

	UE_LOG(LogTemp, Warning, TEXT("[S] OnFindSessionComplete() | SeshNum=%d"), SearchResults.Num());
}

void UGI_Base::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result) {
	APlayerController* PController = GetWorld()->GetFirstPlayerController();

	if (!PController) {
		UE_LOG(LogTemp, Warning, TEXT("[I] OnJoinSessionComplete() PC isn't valid"));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "PC isn't valid");
		return;
	}

	FString JoinAddress = "";
	SessionInterfaceRef->GetResolvedConnectString(SessionName, JoinAddress);

	PController->ClientTravel(JoinAddress, ETravelType::TRAVEL_Absolute);

	UE_LOG(LogTemp, Warning, TEXT("[I] OnJoinSessionComplete()"));
}

void UGI_Base::OnSessionFailure(const FUniqueNetId& UserId, ESessionFailure::Type FailureType) {
	UE_LOG(LogTemp, Warning, TEXT("[E] Session failure"));
	DestroySession();
}

void UGI_Base::SetRandomMinigame() {
	TArray<FName> Rows = MinigamesDataTable->GetRowNames();
	uint16 RandomIndex = FMath::RandRange(0, Rows.Num() - 1);

	CurrentMinigameRow = *MinigamesDataTable->FindRow<FMinigameData>(Rows[RandomIndex], nullptr);

	UE_LOG(LogTemp, Warning, TEXT("[%hs] Set \"%s\" as the random minigame"), __FUNCTION__, *CurrentMinigameRow.Title.ToString());
}

void UGI_Base::ServerTravelToRandomMap() {
	if (!CurrentMinigameRow.Maps.Num()) {
		UE_LOG(LogTemp, Warning, TEXT("[%hs]-> No maps found"), __FUNCTION__);
		return;
	}

	uint8 RandomMapIndex = FMath::RandRange(0, CurrentMinigameRow.Maps.Num() - 1);

	GetWorld()->ServerTravel(CurrentMinigameRow.Maps[RandomMapIndex].GetLongPackageName() + "?listen");
}
