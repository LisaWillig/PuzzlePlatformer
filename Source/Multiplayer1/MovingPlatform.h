// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MovingPlatform.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYER1_API AMovingPlatform : public AStaticMeshActor
{
	GENERATED_BODY()

public:

	AMovingPlatform();
	/** Pawn mesh: 1st person view (arms; seen only by self) */

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Moving")
	float velocity = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Moving", Meta = (MakeEditWidget = true))
	FVector TargetLocation;

	FVector globalTarget;
	FVector globalStart;

	void addActiveTrigger();
	void removeActiveTrigger();

	UPROPERTY(EditAnywhere)
	int activeTriggers = 1;

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
	UMaterialInstance* TriggerActive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
	UMaterialInstance* TriggerSleep;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
	UMaterialInstance* TriggerNeutral;

	
};
