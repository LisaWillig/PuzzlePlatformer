// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYER1_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()
	

public:

	void SetMenuInterface(IMenuInterface* MenuInerface);
	void Setup();
	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;
protected:

	virtual bool Initialize() override;

private:

	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton;

	UFUNCTION()
	void Host();
	UFUNCTION()
	void Join();

	IMenuInterface* MenuInterface;


};
