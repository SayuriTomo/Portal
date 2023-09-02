// Fill out your copyright notice in the Description page of Project Settings.


#include "DoCloseRangeAssault.h"
#include "EnemyAIController.h"

EBTNodeResult::Type UDoCloseRangeAssault::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBehaviorTreeComponent* Component = &OwnerComp;
	if(!Component)
		return EBTNodeResult::Failed;
	AEnemyAIController* MyController = Cast<AEnemyAIController>(Component->GetOwner());
	if(!MyController)
		return EBTNodeResult::Failed;
	MyController->CloseRangeAssault();
	return EBTNodeResult::Succeeded;
}