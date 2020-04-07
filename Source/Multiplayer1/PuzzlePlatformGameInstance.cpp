// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformGameInstance.h"
#include "Engine/Engine.h"

UPuzzlePlatformGameInstance::UPuzzlePlatformGameInstance() {
}

void UPuzzlePlatformGameInstance::Init() {
	Super::Init();
	
}

void UPuzzlePlatformGameInstance::Host() {
	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(-1, 2, FColor::Green, TEXT("Hosting"));

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	World->ServerTravel("/Game/Maps/Puzzle?listen");
}


void UPuzzlePlatformGameInstance::Join(const FString& Address) {
	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;
	Engine->AddOnScreenDebugMessage(-1, 2, FColor::Green, FString::Printf(TEXT("JOINING: %s"), *Address));

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

/*void ClientTravel
(
	const FString& URL,
	enum ETravelType TravelType,
	bool bSeamless,
	FGuid MapPackageGuid
)*/