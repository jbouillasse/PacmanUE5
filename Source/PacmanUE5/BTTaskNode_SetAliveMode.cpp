#include "BTTaskNode_SetAliveMode.h"
#include "AIController.h"
#include "Ghost.h"

UBTTaskNode_SetAliveMode::UBTTaskNode_SetAliveMode()
{
	NodeName = "Set Alive Mode";
}

EBTNodeResult::Type UBTTaskNode_SetAliveMode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AI = OwnerComp.GetAIOwner();
	AGhost* Ghost = AI ? Cast<AGhost>(AI->GetPawn()) : nullptr;
	if (!Ghost) return EBTNodeResult::Failed;

	Ghost->SetAliveMode();
	return EBTNodeResult::Succeeded;
}