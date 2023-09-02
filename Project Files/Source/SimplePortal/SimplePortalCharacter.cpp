// Copyright Epic Games, Inc. All Rights Reserved.

#include "SimplePortalCharacter.h"
#include "SimplePortalProjectile.h"
#include "BluePortal.h"
#include "OrangePortal.h"
#include "CompanionCube.h"
#include "Interactable.h"
#include "SpecficWall.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Physics/ImmediatePhysics/ImmediatePhysicsShared/ImmediatePhysicsCore.h"


//////////////////////////////////////////////////////////////////////////
// ASimplePortalCharacter

ASimplePortalCharacter::ASimplePortalCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	
	
	// set our turn rates for input
	TurnRateGamepad = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	HaveHeld = false;
}

void ASimplePortalCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

}

//////////////////////////////////////////////////////////////////////////// Input

void ASimplePortalCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("MoveBluePortal", IE_Pressed, this, &ASimplePortalCharacter::MoveBluePortal);
	PlayerInputComponent->BindAction("MoveOrangePortal", IE_Pressed, this, &ASimplePortalCharacter::MoveOrangePortal);
	PlayerInputComponent->BindAction("HoldCube",IE_Pressed, this, &ASimplePortalCharacter::HoldCube);
	// Enable touchscreen input
	EnableTouchscreenMovement(PlayerInputComponent);

	// Bind movement events
	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &ASimplePortalCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &ASimplePortalCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "Mouse" versions handle devices that provide an absolute delta, such as a mouse.
	// "Gamepad" versions are for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &ASimplePortalCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &ASimplePortalCharacter::LookUpAtRate);
}


void ASimplePortalCharacter::OnPrimaryAction()
{
	// Trigger the OnItemUsed Event
	OnUseItem.Broadcast();
}


void ASimplePortalCharacter::MoveBluePortal()
{
	CallTrace(1);
}

void ASimplePortalCharacter::MoveOrangePortal()
{
	CallTrace(2);
}

void ASimplePortalCharacter::HoldCube()
{
	CallTrace(3);
}

void ASimplePortalCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		OnPrimaryAction();
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void ASimplePortalCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	TouchItem.bIsPressed = false;
}

void ASimplePortalCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void ASimplePortalCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ASimplePortalCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void ASimplePortalCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

bool ASimplePortalCharacter::EnableTouchscreenMovement(class UInputComponent* PlayerInputComponent)
{
	if (FPlatformMisc::SupportsTouchInput() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ASimplePortalCharacter::BeginTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &ASimplePortalCharacter::EndTouch);

		return true;
	}
	
	return false;
}


bool ASimplePortalCharacter::Trace(UWorld* World,
	TArray<AActor*>& ActorsToIgnore,
	const FVector& Start,
	const FVector& End,
	FHitResult& HitOut,
	ECollisionChannel CollisionChannel = ECC_Pawn,
	bool ReturnPhysMat = false)
{
	// The World parameter refers to our game world (map/level) If there is no World, abort
	if (!World)
	{
		return false;
	}
	
	// Set up our TraceParams object
	FCollisionQueryParams TraceParams(FName(TEXT("My Trace")), true, ActorsToIgnore[0]);

	// Should we simple or complex collision?
	TraceParams.bTraceComplex = true;

	// We don't need Physics materials
	TraceParams.bReturnPhysicalMaterial = ReturnPhysMat;

	// Add our ActorsToIgnore
	TraceParams.AddIgnoredActors(ActorsToIgnore);
	
	HitOut = FHitResult(ForceInit);
	World->LineTraceSingleByChannel (
		HitOut, //result
		Start, //start
		End, //end
		CollisionChannel, //collision channel
		TraceParams
	);
	return (HitOut.GetActor() != NULL);
}

void ASimplePortalCharacter::CallTrace(int Input)
{
	// Get the location of the camera (where we are looking from) and the direction we are looking in
	const FVector Start = FirstPersonCameraComponent->GetComponentLocation();
	const FVector ForwardVector = FirstPersonCameraComponent->GetForwardVector();

	// How for in front of our character do we want our trace to extend?
	// ForwardVector is a unit vector, so we multiply by the desired distance
	const FVector End = Start + ForwardVector * 1256;
	
	// Force clear the HitData which contains our results
	FHitResult HitData(ForceInit);
	
	// What Actors do we want our trace to Ignore?
	TArray<AActor*> ActorsToIgnore;
	
	//Ignore the player character - so you don't hit yourself!
	ActorsToIgnore.Add(this);

	// Call our Trace() function with the paramaters we have set up
	// If it Hits anything
	if(Trace(GetWorld(), ActorsToIgnore, Start, End, HitData, ECC_Visibility, false)) {
		// Process our HitData
		if (HitData.GetActor()) {
			UE_LOG(LogClass, Warning, TEXT("We Hit this Actor:. %s"), *HitData.GetActor()->GetName())
			BaseAction(HitData,Input);
		} else
		{
			// The trace did not return an Actor
			// An error has occurred
			// Record a message in the error log
		}
	} else
	{
		// We did not hit an Actor
	}
}

void ASimplePortalCharacter::BaseAction(FHitResult& HitOut, int Input)
{
	if (Cast<ASpecficWall>(HitOut.GetActor())&&Input!=3)
	{
		//IInteractable::Execute_Interact(HitOut.GetActor());
		TArray<AActor*> Actors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(),AActor::StaticClass(),Actors);
		FVector NewPortalLocation = HitOut.Location+HitOut.Normal;
		FVector NewPortalDistance = 10*HitOut.Normal;
		FRotator NewPortalDirection = HitOut.Normal.Rotation();
		for (AActor* Actor:Actors){
			if(Cast<APortal>(Actor)){
				if(Input == 1&&Cast<ABluePortal>(Actor) ){
					UE_LOG(LogClass, Warning, TEXT("We found Blue Portal"));
					Actor->SetActorLocation(NewPortalLocation);
					Actor->SetActorRotation(NewPortalDirection);
				}
				else if(Input == 2&&Cast<AOrangePortal>(Actor) ){
					UE_LOG(LogClass, Warning, TEXT("We found Orange Portal"));
					Actor->SetActorLocation(NewPortalLocation);
					Actor->SetActorRotation(NewPortalDirection);
				}
					
				if(Input == 1&&Cast<AOrangePortal>(Actor) ){
					Cast<AOrangePortal>(Actor) ->SetPair(NewPortalLocation,NewPortalDistance,NewPortalDirection);
					UE_LOG(LogClass, Warning, TEXT("Blue change and orange record"));
				}
				else if(Input == 2&&Cast<ABluePortal>(Actor) ){
					Cast<ABluePortal>(Actor) ->SetPair(NewPortalLocation,NewPortalDistance,NewPortalDirection);
					UE_LOG(LogClass, Warning, TEXT("Orange change and blue record"));
				}
			}
		}
	}
	else if(Cast<ACompanionCube>(HitOut.GetActor())&&Input==3)
	{
		if(Cast<ACompanionCube>(HitOut.GetActor())&&HitOut.Distance<=250)
		{
			if(!HaveHeld){
				HitOut.GetActor()->SetOwner(this);
				Cast<ACompanionCube>(HitOut.GetActor())->BeCatch = true;
				HaveHeld = true;
				UE_LOG(LogClass, Warning, TEXT("Catch a cube"));
			}
			else
			{
				HaveHeld = false;
				Cast<ACompanionCube>(HitOut.GetActor())->BeCatch = false;
				Cast<ACompanionCube>(HitOut.GetActor())->BePutDown();
				HitOut.GetActor()->SetOwner(nullptr);
				UE_LOG(LogClass, Warning, TEXT("Put down cube"));
			}
		}
	}
	else
	{
		UE_LOG(LogClass, Warning, TEXT
	   ("Actor is NOT Interactable!"));
	}
}

void ASimplePortalCharacter::Teleport(FVector& TPLocation, FVector& TpDistance, FRotator& EnterDirection,FRotator& TpDirection)
{
	
	if(EnterDirection.Yaw<=90.5&&EnterDirection.Yaw>=89.5&&TpDirection.Yaw<=90.5&&TpDirection.Yaw>=89.5)
	{
		AddControllerYawInput(360);
	}
	else if(EnterDirection.Yaw<=180.5&&EnterDirection.Yaw>=179.5&&TpDirection.Yaw<=180.5&&TpDirection.Yaw>=179.5)
	{
		AddControllerYawInput(360);
	}
	else if(EnterDirection.Yaw+TpDirection.Yaw<=-179.5&&EnterDirection.Yaw+TpDirection.Yaw>=-180.5)
    {
		AddControllerYawInput(-360);
    }
	else if(EnterDirection.Yaw<=0.5&&EnterDirection.Yaw>=-0.5&&TpDirection.Yaw<=0.5&&TpDirection.Yaw>=-0.5)
	{
		AddControllerYawInput(-360);
	}
	else if(EnterDirection.Yaw-TpDirection.Yaw<=90.5&&EnterDirection.Yaw-TpDirection.Yaw>=89.5){
		AddControllerYawInput(180);
	}
	else if(EnterDirection.Yaw-TpDirection.Yaw<=-269.5&&EnterDirection.Yaw-TpDirection.Yaw>=-270.5){
		AddControllerYawInput(180);
	}
	else if(EnterDirection.Yaw-TpDirection.Yaw<=-89.5&&EnterDirection.Yaw-TpDirection.Yaw>=-90.5){
		AddControllerYawInput(-180);
	}
	else if(EnterDirection.Yaw-TpDirection.Yaw<=270.5&&EnterDirection.Yaw-TpDirection.Yaw>=269.5){
		AddControllerYawInput(-180);
	}
	SetActorLocation(TPLocation+15*TpDistance);
	UE_LOG(LogClass, Warning, TEXT("Teleport succeed"));
}

void ASimplePortalCharacter::CancelCollision()
{
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),AActor::StaticClass(),Actors);
	for (AActor* Actor:Actors)
	{
		if(Cast<ASpecficWall>(Actor))
		{
			if(Cast<ASpecficWall>(Actor)->HasBluePortal||Cast<ASpecficWall>(Actor)->HasOrangePortal)
			{
				Cast<ASpecficWall>(Actor)->CancelCollision();
			}
		}
	}
}

void ASimplePortalCharacter::ResetCollision()
{
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),AActor::StaticClass(),Actors);
	for (AActor* Actor:Actors)
	{
		if(Cast<ASpecficWall>(Actor))
		{
			if(Cast<ASpecficWall>(Actor)->HasBluePortal||Cast<ASpecficWall>(Actor)->HasOrangePortal)
			{
				Cast<ASpecficWall>(Actor)->ResetCollision();
			}
		}
	}
}


