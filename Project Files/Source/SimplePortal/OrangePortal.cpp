// Fill out your copyright notice in the Description page of Project Settings.


#include "OrangePortal.h"
#include "SpecficWall.h"
#include "SimplePortalCharacter.h"

AOrangePortal::AOrangePortal()
{
	CollisionHitBox->OnComponentBeginOverlap.AddDynamic(this, &AOrangePortal::CollisionHitboxOverlapBegin);
	CollisionHitBox->OnComponentEndOverlap.AddDynamic(this, &AOrangePortal::CollisionHitboxOverlapEnd);
}

void AOrangePortal::CollisionHitboxOverlapBegin(UPrimitiveComponent* OverlappedComponent,
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
		Cast<ASpecficWall>(OtherActor)->HasOrangePortal = true;
	}
}

void AOrangePortal::CollisionHitboxOverlapEnd(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	if(OtherActor && OtherActor != this && Cast<ASimplePortalCharacter>(OtherActor))
	{
		Cast<ASimplePortalCharacter>(OtherActor)->ResetCollision();
		ResetCollision();
	}
	if(OtherActor && OtherActor != this && Cast<ASpecficWall>(OtherActor))
	{
		Cast<ASpecficWall>(OtherActor)->HasOrangePortal = false;
	}
}