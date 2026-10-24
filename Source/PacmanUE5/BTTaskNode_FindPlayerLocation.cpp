#include "BTTaskNode_FindPlayerLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"

UBTTaskNode_FindPlayerLocation::UBTTaskNode_FindPlayerLocation()
{
	NodeName = TEXT("Find Player Location");
}

EBTNodeResult::Type UBTTaskNode_FindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (PlayerPawn != nullptr)
	{
		UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
		
		if (BlackboardComp != nullptr)
		{
			BlackboardComp->SetValueAsVector(TargetLocation.SelectedKeyName, PlayerPawn->GetActorLocation());
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}