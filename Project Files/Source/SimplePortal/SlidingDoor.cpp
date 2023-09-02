// Fill out your copyright notice in the Description page of Project Settings.


#include "SlidingDoor.h"

// Sets default values
ASlidingDoor::ASlidingDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;
	PadActivated = false;
}

// Called when the game starts or when spawned
void ASlidingDoor::BeginPlay()
{
	Super::BeginPlay();
	InitialLocation = GetActorLocation();
	TargetLocation = InitialLocation + FVector(1000.0f,0,0);
}

// Called every frame
void ASlidingDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(PadActivated&&GetActorLocation()!=TargetLocation)
	{
		SetActorLocation(GetActorLocation() + FVector(1.0f,0,0));
	}
	else if(!PadActivated&&GetActorLocation()!=InitialLocation)
	{
		SetActorLocation(GetActorLocation() - FVector(1.0f,0,0));
	}
}

