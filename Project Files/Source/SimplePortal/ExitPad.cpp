// Fill out your copyright notice in the Description page of Project Settings.


#include "ExitPad.h"

#include "SlidingDoor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AExitPad::AExitPad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	ExitPadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	ExitPadMesh->SetupAttachment(RootComponent);
	ExitPadHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Hitbox Component"));
	ExitPadHitBox->SetupAttachment(RootComponent);
	ExitPadHitBox->OnComponentBeginOverlap.AddDynamic(this, &AExitPad::OnHitboxOverlapBegin);
	ExitPadHitBox->OnComponentEndOverlap.AddDynamic(this, &AExitPad::OnHitboxOverlapEnd);
}


// Called when the game starts or when spawned
void AExitPad::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AExitPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AExitPad::OnHitboxOverlapBegin(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComponent,
	int32 OtherBodyIndex,bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor && OtherActor != this)
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlap Has Begun"));
		TArray<AActor*> Actors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(),AActor::StaticClass(),Actors);
		for (AActor* Actor:Actors)
		{
			if(Cast<ASlidingDoor>(Actor))
			{
				Cast<ASlidingDoor>(Actor)->PadActivated=true;
			}
		}
	}
}

void AExitPad::OnHitboxOverlapEnd(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	if(OtherActor && OtherActor != this)
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlap Has ended"));
		TArray<AActor*> Actors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(),AActor::StaticClass(),Actors);
		for (AActor* Actor:Actors)
		{
			if(Cast<ASlidingDoor>(Actor))
			{
				Cast<ASlidingDoor>(Actor)->PadActivated=false;
			}
		}
	}
	
}

