// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Portal.generated.h"

UCLASS()
class SIMPLEPORTAL_API APortal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APortal();
	UPROPERTY(EditAnywhere)UStaticMeshComponent* FrameMesh;
	UPROPERTY(EditAnywhere)UStaticMeshComponent* ScreenMesh;
	UPROPERTY(EditAnywhere)USpringArmComponent* CameraArm;
	UPROPERTY(EditAnywhere)USceneCaptureComponent2D* Camera2D;

	UPROPERTY(EditAnywhere)UBoxComponent* TeleportHitBox;
	UPROPERTY(EditAnywhere)UBoxComponent* CollisionHitBox;
	
	UFUNCTION()
	void TeleportHitboxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FVector PairLocation;
	FVector TpDistance;
	FRotator PairDirection;
	
	UFUNCTION()
	void SetPair(FVector& location,FVector& distance, FRotator& direction);

	UFUNCTION()
	void CancelCollision();
	
	UFUNCTION()
	void ResetCollision();
};
