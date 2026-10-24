#include "BTTaskNode_ChasePlayer.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AITypes.h"
#include "Navigation/PathFollowingComponent.h"
#include "NavigationSystem.h"

UBTTaskNode_ChasePlayer::UBTTaskNode_ChasePlayer()
{
	NodeName = TEXT("Chase Player");
}

EBTNodeResult::Type UBTTaskNode_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// 1. Récupérer le contrôleur de l'IA et le Blackboard
	AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	if (AIController != nullptr && BlackboardComp != nullptr)
	{
		// 2. Lire la position cible depuis le Blackboard (celle trouvée par la Task précédente)
		FVector Destination = BlackboardComp->GetValueAsVector(TargetLocation.SelectedKeyName);

		// 3. Ordonner au fantôme de se déplacer vers cette position via le NavMesh
		// L'EPathFollowingRequestResult nous indique si la requête de mouvement a été acceptée
		FAIMoveRequest MoveRequest(Destination);
		MoveRequest.SetUsePathfinding(true);

		FPathFollowingRequestResult Result = AIController->MoveTo(MoveRequest);

		if (Result.Code == EPathFollowingRequestResult::RequestSuccessful || Result.Code == EPathFollowingRequestResult::AlreadyAtGoal)
		{
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}