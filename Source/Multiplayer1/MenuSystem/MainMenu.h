// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "MenuInterface.h"
#include "MainMenu.generated.h"

USTRUCT()
struct FServerData {
	GENERATED_BODY()

	FString Name; 
	uint16 CurrentPlayers;
	uint16 MaxPlayers;
	FString HostUsername;
};

/**
 * 
 */
UCLASS()
class MULTIPLAYER1_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()
	
public:
	UMainMenu();

	void SetServerList(TArray<FServerData> ServerNames);
	void SelectIndex(uint32 Index);

protected:

	virtual bool Initialize() override;
	
private:

	TSubclassOf<class UUserWidget> ServerScrollClass;

	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinMenuButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* CancelButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* QuitGameButton;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* MenuSwitch;

	UPROPERTY(meta = (BindWidget))
	class UWidget* JoinMenu;

	UPROPERTY(meta = (BindWidget))
	class UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
	class UPanelWidget* ServerList;



	UFUNCTION()
	void Quit();
	UFUNCTION()
	void Host();
	UFUNCTION()
	void Join();
	UFUNCTION()
	void OpenJoinMenu();
	UFUNCTION()
	void OpenMainMenu();

	TOptional<uint32> SelectedIndex;


};
