// Fill out your copyright notice in the Description page of Project Settings.


#include "CompanionCube.h"
#include "Camera/CameraComponent.h"
#include "SimplePortalCharacter.h"

// Sets default values
ACompanionCube::ACompanionCube()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;
	BaseMesh->SetCollisionObjectType(ECC_WorldDynamic);
	BaseMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	
	BeCatch = false;
}

// Called when the game starts or when spawned
void ACompanionCube::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACompanionCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(BeCatch == true)
	{
		const FRotator CubeRotation = Cast<ASimplePortalCharacter>(GetOwner())->GetFirstPersonCameraComponent()->GetComponentRotation();
		const FVector CubeLocation = GetOwner()->GetActorLocation() + CubeRotation.RotateVector(FVector(200.0f, 0.0f, 10.0f));
		SetActorLocation(CubeLocation);
		BaseMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECollisionResponse::ECR_Overlap);
	}

}

void ACompanionCube::BePutDown()
{
	BaseMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	SetActorLocation(FVector(GetActorLocation().X,GetActorLocation().Y,0));
}




