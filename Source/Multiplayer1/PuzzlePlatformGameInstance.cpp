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
#include "Interfaces/OnlineSessionInterface.h"
#include "Delegates/DelegateSignatureImpl.inl"
#include "Delegates/DelegateInstanceInterface.h"

const static FName SESSION_NAME = TEXT("My Game Session");

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
			SessionSearch = MakeShareable(new FOnlineSessionSearch());

			if (SessionSearch.IsValid()) {
				UE_LOG(LogTemp, Warning, TEXT("Looking for session"))
					SessionSearch->bIsLanQuery = true;
				
				SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
				
			}
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

	World->ServerTravel("/Game/Maps/Puzzle?listen");
}

void UPuzzlePlatformGameInstance::OnFindSessionsComplete(bool Success) {
	if (!Success) {
		UE_LOG(LogTemp, Warning, TEXT("No Success searching for session"))
	}
	else {
		SearchResults = SessionSearch->SearchResults;
		if (SearchResults.Num() == 0) {
			UE_LOG(LogTemp, Warning, TEXT("No Session Found"))
		}
		else {
			for (auto result : SearchResults) {
				UE_LOG(LogTemp, Warning, TEXT("Found session: %s"), *result.GetSessionIdStr())
			}
		}
	}
}

void UPuzzlePlatformGameInstance::OnDestroySessionComplete(FName SessionName, bool Success) {

	if (!Success) {
		CreateSession();
	}
}

void UPuzzlePlatformGameInstance::Host() {

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

void UPuzzlePlatformGameInstance::CreateSession() {
	if (SessionInterface.IsValid()) {
		FOnlineSessionSettings Settings;
		Settings.bIsLANMatch = true;
		Settings.NumPublicConnections = 2;
		Settings.bShouldAdvertise = true;
		SessionInterface->CreateSession(0, SESSION_NAME, Settings);
	}
}

void UPuzzlePlatformGameInstance::Join(const FString& Address) {
	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;
	Engine->AddOnScreenDebugMessage(-1, 2, FColor::Green, FString::Printf(TEXT("JOINING: %s"), *Address));

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

void UPuzzlePlatformGameInstance::LoadMainMenu() {
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel("/Game/Maps/MenuLevel", ETravelType::TRAVEL_Absolute);

}
void UPuzzlePlatformGameInstance::LoadMenu() {
	if (!ensure(MenuClass != nullptr))return;
	UMainMenu *Menu = CreateWidget<UMainMenu>(this, MenuClass);
	if (!ensure(Menu != nullptr))return;
	Menu->Setup();
	Menu->SetMenuInterface(this);
}

void UPuzzlePlatformGameInstance::LoadInGameMenu() {
	if (!ensure(MenuClass != nullptr))return;
	UInGameMenu* Menu = CreateWidget<UInGameMenu>(this, InGameMenuClass);
	if (!ensure(Menu != nullptr))return;
	Menu->Setup();
	Menu->SetMenuInterface(this);
}

void UPuzzlePlatformGameInstance::QuitGame() {
	auto* playerController = GetWorld()->GetFirstPlayerController();
	if (!ensure(playerController != nullptr)) return;
	playerController->ConsoleCommand("quit");
}