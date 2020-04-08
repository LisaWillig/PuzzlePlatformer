// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuSystem/MenuInterface.h"
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

	//UFUNCTION(Exec)
	virtual void Host() override;

	//UFUNCTION(Exec)
	virtual void Join(const FString& Address) override;

	UFUNCTION(BlueprintCallable)
	void LoadMenu();

	UFUNCTION(BlueprintCallable)
	void LoadInGameMenu();

	UFUNCTION(BlueprintCallable)
	virtual void LoadMainMenu() override;

	UFUNCTION(BlueprintCallable)
	virtual void QuitGame() override;

private:

	TSubclassOf<class UUserWidget> MenuClass; 
	TSubclassOf<class UUserWidget> InGameMenuClass;

};
