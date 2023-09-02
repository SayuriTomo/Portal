// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CloseRangeAssault.generated.h"

UCLASS()
class SIMPLEPORTAL_API ACloseRangeAssault : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACloseRangeAssault();
	UPROPERTY(EditAnywhere)UStaticMeshComponent* BaseMesh;
	UPROPERTY(EditAnywhere)AActor* Parent;
	UPROPERTY(EditAnywhere)float MaximumLifetime;
	UPROPERTY(EditAnywhere)float CurrentLifetime;

	UFUNCTION()
	void AssaultHitBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	 UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
	 const FHitResult& SweepResult);

	UFUNCTION()
	void AssaultHitEnd(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
