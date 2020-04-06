// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"
#include "Math/Vector.h"

AMovingPlatform::AMovingPlatform(){
	PrimaryActorTick.bCanEverTick = true;
	SetMobility(EComponentMobility::Movable);

}

void AMovingPlatform::BeginPlay() {
	Super::BeginPlay();

	globalTarget = GetTransform().TransformPosition(TargetLocation);
	globalStart = GetActorLocation();

	if (HasAuthority()) {
		SetReplicates(true);
		SetReplicatingMovement(true);
	}
	
}
void AMovingPlatform::addActiveTrigger() {
	activeTriggers++;
}

void AMovingPlatform::removeActiveTrigger() {
	if (activeTriggers > 0) {
		activeTriggers--;
	}
}

void AMovingPlatform::Tick(float DeltaTime) {
	
	Super::Tick(DeltaTime);

	if (activeTriggers > 0) {
		if (HasAuthority()) {

			FVector myPos = GetActorLocation();
			float actor = FVector::DistSquared(globalStart, myPos);
			float goal = FVector::DistSquared(globalStart, globalTarget);

			if (actor > goal) {
				FVector newVector = globalStart;
				globalStart = globalTarget;
				globalTarget = newVector;
			}

			FVector direction = (globalTarget - globalStart).GetSafeNormal();
			myPos += direction * velocity * DeltaTime;

			this->SetActorLocation(myPos);
		}
	}


	
}

