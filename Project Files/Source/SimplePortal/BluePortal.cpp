// Fill out your copyright notice in the Description page of Project Settings.


#include "BluePortal.h"
#include "SpecficWall.h"
#include "SimplePortalCharacter.h"

ABluePortal::ABluePortal()
{
	CollisionHitBox->OnComponentBeginOverlap.AddDynamic(this, &ABluePortal::CollisionHitboxOverlapBegin);
	CollisionHitBox->OnComponentEndOverlap.AddDynamic(this, &ABluePortal::CollisionHitboxOverlapEnd);
}

void ABluePortal::CollisionHitboxOverlapBegin(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor && OtherActor != this && Cast<ASimplePortalCharacter>(OtherActor))
	{
		Cast<ASimplePortalCharacter>(OtherActor)->CancelCollision();
		CancelCollision();
	}

	if(OtherActor && OtherActor != this && Cast<ASpecficWall>(OtherActor))
	{
		Cast<ASpecficWall>(OtherActor)->HasBluePortal = true;
	}
}

void ABluePortal::CollisionHitboxOverlapEnd(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	if(OtherActor && OtherActor != this && Cast<ASimplePortalCharacter>(OtherActor))
	{
		Cast<ASimplePortalCharacter>(OtherActor)->ResetCollision();
		ResetCollision();
	}
	if(OtherActor && OtherActor != this && Cast<ASpecficWall>(OtherActor))
	{
		Cast<ASpecficWall>(OtherActor)->HasBluePortal = false;
	}
}