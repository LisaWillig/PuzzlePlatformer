// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformGameInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Engine.h"
#include "Blueprint/UserWidget.h"
#include "MenuSystem/MainMenu.h"
#include "PlatformTrigger.h"

UPuzzlePlatformGameInstance::UPuzzlePlatformGameInstance() {

	ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if (!ensure(MenuBPClass.Class != nullptr))return;

	MenuClass = MenuBPClass.Class;
	
}

void UPuzzlePlatformGameInstance::Init() {
	Super::Init();
	UE_LOG(LogTemp, Warning, TEXT("Found Class %s"), *MenuClass->GetName())
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


void UPuzzlePlatformGameInstance::LoadMenu() {
	if (!ensure(MenuClass != nullptr))return;
	UMainMenu *Menu = CreateWidget<UMainMenu>(this, MenuClass);
	if (!ensure(Menu != nullptr))return;
	Menu->Setup();

	Menu->SetMenuInterface(this);


}