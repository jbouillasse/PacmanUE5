#include "BTTaskNode_FindClydePosition.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "NavigationSystem.h"
#include "PacmanPawn.h"

UBTTaskNode_FindClydePosition::UBTTaskNode_FindClydePosition()
{
	NodeName = TEXT("Find Clyde Position");
}

EBTNodeResult::Type UBTTaskNode_FindClydePosition::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APacmanPawn* Pacman = Cast<APacmanPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* Ghost = AIController ? AIController->GetPawn() : nullptr;
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!Pacman || !Ghost || !BlackboardComp) return EBTNodeResult::Failed;

	const FVector PlayerLocation = Pacman->GetActorLocation();
	FVector Raw;

	if (Pacman->IsScatterMode())
	{
		Raw = ScatterLocation;
	}
	else
	{
		// Loin : il attaque. Trop près : il file dans son coin.
		const float Dist = FVector::Dist2D(PlayerLocation, Ghost->GetActorLocation());
		Raw = (Dist > FleeDistanceThreshold) ? PlayerLocation : ScatterLocation;
	}

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	FNavLocation Projected;
	FVector Final = Raw;
	if (NavSys && NavSys->ProjectPointToNavigation(Raw, Projected, FVector(500.f, 500.f, 500.f)))
	{
		Final = Projected.Location;
	}

	BlackboardComp->SetValueAsVector(TargetLocation.SelectedKeyName, Final);
	return EBTNodeResult::Succeeded;
}