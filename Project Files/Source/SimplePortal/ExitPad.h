// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "ExitPad.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPressurePadActivated);

UCLASS()
class SIMPLEPORTAL_API AExitPad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExitPad();
	//UPROPERTY(BlueprintAssignable)FOnPressurePadActivated ExitPadHitBoxDelegate;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interact")
	UStaticMeshComponent* ExitPadMesh;
	UPROPERTY(EditAnywhere)UBoxComponent* ExitPadHitBox;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void OnHitboxOverlapBegin(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,
     UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnHitboxOverlapEnd(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);

};
