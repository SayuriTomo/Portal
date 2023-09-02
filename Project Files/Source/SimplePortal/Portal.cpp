// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal.h"
#include "SimplePortalCharacter.h"
#include "Components/SceneCaptureComponent2D.h"

// Sets default values
APortal::APortal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	FrameMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Frame"));
	FrameMesh->SetupAttachment(RootComponent);

	ScreenMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Screen"));
	ScreenMesh->SetupAttachment(FrameMesh);
	
	FrameMesh ->SetCollisionObjectType(ECC_WorldDynamic);
	FrameMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

	ScreenMesh ->SetCollisionObjectType(ECC_WorldDynamic);
	ScreenMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	
	TeleportHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TeleportHitBox Component"));
    TeleportHitBox->SetupAttachment(RootComponent);
	
	TeleportHitBox->OnComponentBeginOverlap.AddDynamic(this, &APortal::TeleportHitboxOverlapBegin);
	
	CollisionHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionHitBox Component"));
	CollisionHitBox->SetupAttachment(RootComponent);
	
	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	Camera2D = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("CameraComponent"));
	CameraArm->SetupAttachment(FrameMesh);
	Camera2D->SetupAttachment(CameraArm,USpringArmComponent::SocketName);

	CameraArm->SetRelativeLocationAndRotation
	(FVector(0.0f, 0.0f, 0.0f),FRotator(0.0f, 0.0f, 0.0f));
	CameraArm->TargetArmLength = 0.0f;
	CameraArm->bEnableCameraLag = true;
	CameraArm->CameraLagSpeed = 0.0f;
	
}

// Called when the game starts or when spawned
void APortal::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APortal::TeleportHitboxOverlapBegin(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor && OtherActor != this && Cast<ASimplePortalCharacter>(OtherActor))
	{
		FRotator EnterDirection = GetActorRotation(); 
		Cast<ASimplePortalCharacter>(OtherActor)->Teleport(PairLocation,TpDistance,EnterDirection,PairDirection);
	}
}

void APortal::SetPair(FVector& TeleportLocation, FVector& TeleportDistance, FRotator& TeleportDirection)
{
	PairLocation = TeleportLocation;
	TpDistance = TeleportDistance;
	PairDirection = TeleportDirection;
}

void APortal::CancelCollision()
{
	FrameMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECollisionResponse::ECR_Overlap);
	ScreenMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECollisionResponse::ECR_Overlap);
}

void APortal::ResetCollision()
{
	FrameMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	ScreenMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
}



