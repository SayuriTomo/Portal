// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLEPORTAL_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AEnemyAIController();
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaSeconds) override;
	virtual FRotator GetControlRotation() const override;
	//void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=AI)
	float SightRadius = 500;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=AI)
	float SightAge = 3.5;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=AI)
	float LoseSightRadius = SightRadius + 30;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=AI)
	float FieldOfView = 45;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=AI)
	UAISenseConfig_Sight* SightConfiguration;

	UNavigationSystemV1* NavigationSystem;
	APawn* TargetPlayer;

	void GenerateNewRandomLocation();

	UFUNCTION()
    void OnSensesUpdated(AActor* UpdatedActor, FAIStimulus Stimulus);

	UPROPERTY(EditDefaultsOnly, Category = Blackboard)
	UBlackboardData* AIBlackboard;
	
	UPROPERTY(EditDefaultsOnly, Category = Blackboard)
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditDefaultsOnly, Category = Blackboard)
	UBlackboardComponent* BlackboardComponent;

	void CloseRangeAssault();
	
	UPROPERTY(EditAnywhere, Category = Assault)
	TSubclassOf<class ACloseRangeAssault> CloseAssaultClass;

	float DistanceBetweenPlayer;
};
