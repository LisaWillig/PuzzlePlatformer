// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "TimerManager.h"
#include "PuzzlePlatformGameInstance.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	NbOfPlayers++;
	if (NbOfPlayers > 2) {
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ALobbyGameMode::StartSession, 5.0f, false);
	}
}

void ALobbyGameMode::StartSession() {
	UWorld* World = GetWorld();
	auto GameInstance = Cast<UPuzzlePlatformGameInstance>(GetGameInstance());
	if (GameInstance != nullptr) {
		GameInstance->StartSession();
	}
	if (!ensure(World != nullptr))return;
	bUseSeamlessTravel = true;
	World->ServerTravel("/Game/Maps/Puzzle?listen");
}
void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	NbOfPlayers--;
}
