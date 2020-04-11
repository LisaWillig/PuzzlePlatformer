// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformGameInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Engine.h"
#include "Blueprint/UserWidget.h"
#include "MenuSystem/MainMenu.h"
#include "MenuSystem/InGameMenu.h"
#include "PlatformTrigger.h"
#include "MenuSystem/MenuWidget.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

#include "Delegates/DelegateSignatureImpl.inl"
#include "Delegates/DelegateInstanceInterface.h"

const static FName SESSION_NAME = NAME_GameSession;
const static FName SERVERNAME_SETTING_KEY = TEXT("ServerName");

UPuzzlePlatformGameInstance::UPuzzlePlatformGameInstance() {

	ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if (!ensure(MenuBPClass.Class != nullptr))return;
	MenuClass = MenuBPClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBPClass(TEXT("/Game/MenuSystem/WBP_InGameMenu"));
	if (!ensure(InGameMenuBPClass.Class != nullptr))return;
	InGameMenuClass = InGameMenuBPClass.Class;
}

void UPuzzlePlatformGameInstance::Init() {
	Super::Init();

	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem != nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Subsystem: %s"), *Subsystem->GetSubsystemName().ToString())
			SessionInterface = Subsystem->GetSessionInterface();

		if (SessionInterface.IsValid()) {
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformGameInstance::OnCreateSessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPuzzlePlatformGameInstance::OnFindSessionsComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformGameInstance::OnJoinSessionComplete);
		} 
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Found no subsystem"));
	}
}

void UPuzzlePlatformGameInstance::OnCreateSessionComplete(FName SessionName, bool Success) {
	
	if (!Success) {
		UE_LOG(LogTemp, Warning, TEXT("No Session was created!"))
		return;
	}
	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(-1, 2, FColor::Green, TEXT("Hosting"));

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	World->ServerTravel("/Game/Maps/Lobby?listen");
}

void UPuzzlePlatformGameInstance::OnFindSessionsComplete(bool Success) {
	if (Success &&SessionSearch.IsValid() && Menu != nullptr) {
		TArray<FServerData> ServerNames;
		for (const FOnlineSessionSearchResult result : SessionSearch->SearchResults) {
			
			FServerData Data;
			Data.NameId = result.GetSessionIdStr();
			Data.MaxPlayers = result.Session.SessionSettings.NumPublicConnections;
			Data.CurrentPlayers = result.Session.SessionSettings.NumPublicConnections - result.Session.NumOpenPublicConnections;
			Data.HostUsername = result.Session.OwningUserName;
			FString ServerUserName;
			if (result.Session.SessionSettings.Get(SERVERNAME_SETTING_KEY, ServerUserName)) {
				Data.Name = ServerUserName;
				UE_LOG(LogTemp, Warning, TEXT("Session: %s"), *ServerUserName)
			}
			else {
				Data.Name = Data.NameId;
				UE_LOG(LogTemp, Warning, TEXT("No Settings found"))
			}
			ServerNames.Add(Data);
		}
		Menu->SetServerList(ServerNames);
	}
}

void UPuzzlePlatformGameInstance::OnDestroySessionComplete(FName SessionName, bool Success) {
	if (!Success) {
		CreateSession();
	}
}

void UPuzzlePlatformGameInstance::Host(FText UserServerName) {
	ServerName = UserServerName;
	if (SessionInterface.IsValid()) {
		auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
		if (ExistingSession != nullptr) {
			SessionInterface->DestroySession(SESSION_NAME);
		}
		else {
			CreateSession();
		}
	}
}

void UPuzzlePlatformGameInstance::UpdateServerList() {
	SessionSearch = MakeShareable(new FOnlineSessionSearch());

	if (SessionSearch.IsValid()) {
		UE_LOG(LogTemp, Warning, TEXT("Looking for session"))

			if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL") {
				SessionSearch->bIsLanQuery = true;
		}
			else {
				SessionSearch->bIsLanQuery = false;
			}
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

void UPuzzlePlatformGameInstance::CreateSession() {
	if (SessionInterface.IsValid()) {

		FOnlineSessionSettings Settings;

		if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL") {
			Settings.bIsLANMatch = true;
		}
		else {
			Settings.bIsLANMatch = false;
		}
		Settings.NumPublicConnections = 5;
		Settings.bShouldAdvertise = true;
		Settings.bUsesPresence = true;
		if (ServerName.ToString() != "") {
			Settings.Set(SERVERNAME_SETTING_KEY, ServerName.ToString(), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		}
		
		SessionInterface->CreateSession(0, SESSION_NAME, Settings);
	}
}

void UPuzzlePlatformGameInstance::Join(uint32 Index) {
	
	if (!SessionInterface.IsValid()) return; 
	if (!SessionSearch.IsValid()) return;
	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);
}

void UPuzzlePlatformGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result) {
	
	if (!SessionInterface.IsValid()) return;

	FString Info;
	if (!SessionInterface->GetResolvedConnectString(SessionName, Info)) {
		UE_LOG(LogTemp, Warning, TEXT("Could Not Get Connect String."))
			return;
	};

	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;
	PlayerController->ClientTravel(Info, ETravelType::TRAVEL_Absolute);
}

void UPuzzlePlatformGameInstance::StartSession() {
	if (!SessionInterface.IsValid()) return;
	SessionInterface->StartSession(SESSION_NAME);
}

void UPuzzlePlatformGameInstance::LoadMainMenu() {
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel("/Game/Maps/MenuLevel", ETravelType::TRAVEL_Absolute);

}
void UPuzzlePlatformGameInstance::LoadMenu() {
	if (!ensure(MenuClass != nullptr))return;
	Menu = CreateWidget<UMainMenu>(this, MenuClass);
	if (!ensure(Menu != nullptr))return;
	Menu->Setup();
	Menu->SetMenuInterface(this);
}

void UPuzzlePlatformGameInstance::LoadInGameMenu() {
	if (!ensure(InGameMenuClass != nullptr))return;
	UInGameMenu* InGameMenu = CreateWidget<UInGameMenu>(this, InGameMenuClass);
	if (!ensure(InGameMenu != nullptr))return;
	InGameMenu->Setup();
	InGameMenu->SetMenuInterface(this);
}

void UPuzzlePlatformGameInstance::QuitGame() {
	auto* playerController = GetWorld()->GetFirstPlayerController();
	if (!ensure(playerController != nullptr)) return;
	playerController->ConsoleCommand("quit");
}