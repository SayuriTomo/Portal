// Fill out your copyright notice in the Description page of Project Settings.


#include "SpecficWall.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Microsoft/AllowMicrosoftPlatformTypes.h"

// Sets default values
ASpecficWall::ASpecficWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;
	BaseMesh->SetCollisionObjectType(ECC_WorldDynamic);
	BaseMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

	HasBluePortal= false;
	HasOrangePortal = false;
}

// Called when the game starts or when spawned
void ASpecficWall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpecficWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ASpecficWall::CancelCollision()
{
	BaseMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECollisionResponse::ECR_Overlap);
}

void ASpecficWall::ResetCollision()
{
	BaseMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
}



