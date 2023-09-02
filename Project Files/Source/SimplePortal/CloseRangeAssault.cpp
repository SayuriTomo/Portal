// Fill out your copyright notice in the Description page of Project Settings.


#include "CloseRangeAssault.h"

#include "SimplePortalCharacter.h"

// Sets default values
ACloseRangeAssault::ACloseRangeAssault()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(RootComponent);

	BaseMesh->OnComponentBeginOverlap.AddDynamic(this, &ACloseRangeAssault::AssaultHitBegin);
	BaseMesh->OnComponentEndOverlap.AddDynamic(this, &ACloseRangeAssault::AssaultHitEnd);
	Parent = nullptr;
	MaximumLifetime = 1;
	CurrentLifetime = 0;
}

// Called when the game starts or when spawned
void ACloseRangeAssault::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACloseRangeAssault::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CurrentLifetime += DeltaTime;
	if(CurrentLifetime >= MaximumLifetime)
	{
		Destroy();
	}

}

void ACloseRangeAssault::AssaultHitBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor && OtherActor != this)
	{
		if(!Parent)return;
		if(Parent == OtherActor)return;

		Destroy();

		if(Cast<ASimplePortalCharacter>(OtherActor))
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit player"));
			OtherActor->Destroy();
		}
	}
}

void ACloseRangeAssault::AssaultHitEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	if(OtherActor && OtherActor != this)
	{
		if(!Parent)return;
		if(Parent == OtherActor)return;

		Destroy();

		if(Cast<ASimplePortalCharacter>(OtherActor))
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit player"));
			OtherActor->Destroy();
		}
	}
}



