// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Portal.h"
#include "OrangePortal.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLEPORTAL_API AOrangePortal : public APortal
{
	GENERATED_BODY()
	
public:
	AOrangePortal();
	UFUNCTION()
	void CollisionHitboxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void CollisionHitboxOverlapEnd(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);
};
