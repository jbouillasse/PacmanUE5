#include "BTTaskNode_FindFrightenPosition.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

UBTTaskNode_FindFrightenPosition::UBTTaskNode_FindFrightenPosition()
{
	NodeName = "Find Frighten Position";
	TargetLocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTaskNode_FindFrightenPosition, TargetLocationKey));
	TargetLocationKey.SelectedKeyName = TEXT("TargetLocation");
}

EBTNodeResult::Type UBTTaskNode_FindFrightenPosition::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AI = OwnerComp.GetAIOwner();
	APawn* Ghost = AI ? AI->GetPawn() : nullptr;
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (!Ghost || !BB || !NavSys) return EBTNodeResult::Failed;

	const FVector GhostLoc = Ghost->GetActorLocation();

	// Fuite aléatoire, comme dans l'original
	for (int32 Try = 0; Try < 8; ++Try)
	{
		FNavLocation Result;
		if (NavSys->GetRandomReachablePointInRadius(GhostLoc, SearchRadius, Result)
			&& FVector::Dist2D(GhostLoc, Result.Location) >= MinDistanceFromGhost)
		{
			BB->SetValueAsVector(TargetLocationKey.SelectedKeyName, Result.Location);
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}