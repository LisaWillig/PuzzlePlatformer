// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Components/Button.h"

bool UMainMenu::Initialize() {
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(HostButton != nullptr)) return false;
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::Host);

	if (!ensure(JoinButton != nullptr)) return false;
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::Join);

	return true;
}

void UMainMenu::SetMenuInterface(IMenuInterface* MenuInterace) {
	this->MenuInterface = MenuInterace;
}

void UMainMenu::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) {
	RemoveFromViewport();
	auto* world = GetWorld();
	if (!ensure(world != nullptr))
		return;
	auto* playerController = world->GetFirstPlayerController();
	if (!ensure(playerController != nullptr))
		return;
	FInputModeGameOnly inputMode;
	playerController->SetInputMode(inputMode);
	playerController->bShowMouseCursor = false;
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);
}

void UMainMenu::Host() {
	if (MenuInterface == nullptr) return; 
	MenuInterface->Host();

	UE_LOG(LogTemp, Warning, TEXT("I am hosting!"))
}

void UMainMenu::Setup(){
	this->AddToViewport();
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();

	FInputModeUIOnly InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetWidgetToFocus(this->TakeWidget());

	PlayerController->SetInputMode(InputMode);
	PlayerController->bShowMouseCursor = true;


}
void UMainMenu::Join() {
	if (MenuInterface != nullptr) return;
	const FString Address = "test";
	MenuInterface->Join(Address);

	UE_LOG(LogTemp, Warning, TEXT("I am joining!"))
}