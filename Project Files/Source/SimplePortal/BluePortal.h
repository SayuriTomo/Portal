// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Portal.h"
#include "BluePortal.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLEPORTAL_API ABluePortal : public APortal
{
	GENERATED_BODY()
	
public:
	ABluePortal();
	UFUNCTION()
	void CollisionHitboxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void CollisionHitboxOverlapEnd(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);
};
