// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuSystem/MenuInterface.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "PuzzlePlatformGameInstance.generated.h"

/**
 *
 */
UCLASS()
class MULTIPLAYER1_API UPuzzlePlatformGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

public:
	UPuzzlePlatformGameInstance();
	virtual void Init() override;
	virtual void Host() override;
	virtual void Join(const FString& Address) override;

	class UMainMenu* Menu;

	void CreateSession();

	UFUNCTION(BlueprintCallable)
	void LoadMenu();

	UFUNCTION(BlueprintCallable)
	void LoadInGameMenu();

	UFUNCTION(BlueprintCallable)
	virtual void LoadMainMenu() override;

	UFUNCTION(BlueprintCallable)
	virtual void QuitGame() override;

	UFUNCTION(BlueprintCallable)
	virtual void UpdateServerList() override;

private:

	

	TSubclassOf<class UUserWidget> MenuClass; 
	TSubclassOf<class UUserWidget> InGameMenuClass;
	TArray<FOnlineSessionSearchResult> SearchResults;

	class TSharedPtr<FOnlineSessionSearch> SessionSearch; 
	IOnlineSessionPtr SessionInterface;
	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void OnFindSessionsComplete(bool Success);

};
