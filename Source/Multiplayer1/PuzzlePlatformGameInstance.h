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
	void Host() override;

	//UFUNCTION(Exec)
	void Join(const FString& Address) override;

	UFUNCTION(BlueprintCallable)
	void LoadMenu();

private:

	TSubclassOf<class UUserWidget> MenuClass; 

};
