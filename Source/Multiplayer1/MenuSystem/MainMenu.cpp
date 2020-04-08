// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/WidgetSwitcher.h"

bool UMainMenu::Initialize() {
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(HostButton != nullptr)) return false;
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::Host);

	if (!ensure(JoinMenuButton != nullptr)) return false;
	JoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);

	if (!ensure(CancelButton != nullptr)) return false;
	CancelButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);

	if (!ensure(JoinButton != nullptr)) return false;
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::Join);

	if (!ensure(QuitGameButton != nullptr)) return false;
	QuitGameButton->OnClicked.AddDynamic(this, &UMainMenu::Quit);

	return true;
}

void UMainMenu::Quit() {
	if (MenuInterface == nullptr) return;
	MenuInterface->QuitGame();
}

void UMainMenu::Host() {
	if (MenuInterface == nullptr) return; 
	MenuInterface->Host();
}

void UMainMenu::Join() {
	if (MenuInterface == nullptr) return;
	if (IPAddressField == nullptr) return;
	const FText Address = IPAddressField->GetText();
	MenuInterface->Join(Address.ToString());
}

void UMainMenu::OpenJoinMenu() {
	if (!ensure(MenuSwitch != nullptr)) return;
	if (!ensure(JoinMenu != nullptr)) return;
	MenuSwitch->SetActiveWidget(JoinMenu);
}

void UMainMenu::OpenMainMenu() {
	if (!ensure(MenuSwitch != nullptr)) return;
	if (!ensure(MainMenu != nullptr)) return;
	MenuSwitch->SetActiveWidget(MainMenu);
}