// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "CompanionCube.generated.h"


UCLASS()
class SIMPLEPORTAL_API ACompanionCube : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACompanionCube();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interact")
	UStaticMeshComponent* BaseMesh;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool BeCatch;
	
	UFUNCTION()
	void BePutDown();
};
