#include "BTTaskNode_FindRespawnPoint.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Ghost.h"

UBTTaskNode_FindRespawnPoint::UBTTaskNode_FindRespawnPoint()
{
	NodeName = "Find Respawn Point";
	TargetLocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTaskNode_FindRespawnPoint, TargetLocationKey));
	TargetLocationKey.SelectedKeyName = TEXT("TargetLocation");
}

EBTNodeResult::Type UBTTaskNode_FindRespawnPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AI = OwnerComp.GetAIOwner();
	AGhost* Ghost = AI ? Cast<AGhost>(AI->GetPawn()) : nullptr;
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!Ghost || !BB) return EBTNodeResult::Failed;

	BB->SetValueAsVector(TargetLocationKey.SelectedKeyName, Ghost->GetInitialLocation());
	return EBTNodeResult::Succeeded;
}