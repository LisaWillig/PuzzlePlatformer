// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PuzzlePlatformGameInstance.generated.h"

/**
 *
 */
UCLASS()
class MULTIPLAYER1_API UPuzzlePlatformGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPuzzlePlatformGameInstance();
	virtual void Init() override;

	UFUNCTION(Exec)
		void Host();

	UFUNCTION(Exec)
		void Join(const FString& Address);

};
