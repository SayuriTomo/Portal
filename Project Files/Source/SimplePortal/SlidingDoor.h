// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SlidingDoor.generated.h"

UCLASS()
class SIMPLEPORTAL_API ASlidingDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASlidingDoor();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interact")
	UStaticMeshComponent* BaseMesh;
	FVector InitialLocation;
	FVector TargetLocation;
	bool PadActivated;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
