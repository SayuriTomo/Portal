// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "CloseRangeAssault.h"
#include "SimplePortalCharacter.h"

AEnemyAIController::AEnemyAIController()
{
	PrimaryActorTick.bCanEverTick = true;
	SightConfiguration = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Configuration"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));

	SightConfiguration->SightRadius = SightRadius;
	SightConfiguration->LoseSightRadius = LoseSightRadius;
	SightConfiguration->PeripheralVisionAngleDegrees = FieldOfView;
	SightConfiguration->SetMaxAge(SightAge);
	SightConfiguration->DetectionByAffiliation.bDetectEnemies = true;
	SightConfiguration->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfiguration->DetectionByAffiliation.bDetectNeutrals = true;

	GetPerceptionComponent()->SetDominantSense(*SightConfiguration->GetSenseImplementation());
    GetPerceptionComponent()->ConfigureSense(*SightConfiguration);
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnSensesUpdated);
	
	TargetPlayer = nullptr;
	
	static ConstructorHelpers::FObjectFinder<UBlueprint> CloseAssaultBlueprint
	(TEXT("Blueprint'/Game/Assignment3/Blueprints/BP_CloseRangeAssault.BP_CloseRangeAssault'"));
	
	if (CloseAssaultBlueprint.Object){
		CloseAssaultClass = CloseAssaultBlueprint.Object->GeneratedClass;
	}
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	NavigationSystem = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem());
	if(!AIBlackboard)return;
	if(!ensure(BehaviorTree))return;

	UseBlackboard(AIBlackboard, BlackboardComponent);
	RunBehaviorTree(BehaviorTree);
}

void AEnemyAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(TargetPlayer)
	{
		BlackboardComponent->SetValueAsVector("PlayerPosition", TargetPlayer->GetActorLocation());
		DistanceBetweenPlayer = GetPawn()->GetDistanceTo(TargetPlayer);
		if(DistanceBetweenPlayer<=200)
		{
			BlackboardComponent->SetValueAsBool("ClosePlayer", true);
			UE_LOG(LogTemp, Warning, TEXT("Actor is close"));
		}
		else
		{
			BlackboardComponent->ClearValue("ClosePlayer");
		}
	}
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

FRotator AEnemyAIController::GetControlRotation() const
{
	if(GetPawn())
	{
		return FRotator(0, GetPawn()->GetActorRotation().Yaw, 0);
	}
	return FRotator::ZeroRotator;
}

/*
void AEnemyAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if(NavigationSystem)
	{
		FNavLocation ReturnLocation;
		NavigationSystem->GetRandomPointInNavigableRadius(GetPawn()->GetActorLocation(),
	   2000,ReturnLocation);
		MoveToLocation(ReturnLocation.Location);
	}
}*/

void AEnemyAIController::GenerateNewRandomLocation()
{
	if(NavigationSystem)
	{
		FNavLocation ReturnLocation;
		NavigationSystem->GetRandomPointInNavigableRadius(GetPawn()->GetActorLocation(), 2000, ReturnLocation);
		BlackboardComponent->SetValueAsVector("PatrolPoint", ReturnLocation.Location);
	}
}

void AEnemyAIController::OnSensesUpdated(AActor* UpdatedActor, FAIStimulus Stimulus)
{
	APawn* TemporaryPawn = Cast<APawn>(UpdatedActor);
	
	
	if(TemporaryPawn && TemporaryPawn->IsPlayerControlled())
	{
		if(Stimulus.WasSuccessfullySensed())
		{
			UE_LOG(LogTemp, Warning, TEXT("Set Actor Location"));
			TargetPlayer = TemporaryPawn;
			BlackboardComponent->SetValueAsBool("ChasePlayer", true);
			BlackboardComponent->
			SetValueAsVector("PlayerPosition", TargetPlayer->GetActorLocation());
		} else
		{
			TargetPlayer = nullptr;
			BlackboardComponent->ClearValue("ChasePlayer");
		}
	}
}

void AEnemyAIController::CloseRangeAssault()
{
	FVector EnemyLocation= GetPawn()->GetActorLocation();
	FVector StartPosition = EnemyLocation + FVector(0,0,20);
	ACloseRangeAssault* SpawnedAssault = (ACloseRangeAssault*) GetWorld()
	->SpawnActor(CloseAssaultClass, &StartPosition);
	SpawnedAssault->Parent = this;
	
}




