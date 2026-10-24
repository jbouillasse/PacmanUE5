#include "BTTaskNode_FindAmbushPosition.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "PacmanPawn.h"

UBTTaskNode_FindAmbushPosition::UBTTaskNode_FindAmbushPosition()
{
	NodeName = TEXT("Find Ambush Position");
}

EBTNodeResult::Type UBTTaskNode_FindAmbushPosition::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APacmanPawn* Pacman = Cast<APacmanPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!Pacman || !BlackboardComp) return EBTNodeResult::Failed;

	const FVector PlayerLocation = Pacman->GetActorLocation();
	FVector Raw;

	if (Pacman->IsScatterMode())
	{
		Raw = ScatterLocation;
	}
	else
	{
		// yaw 0 = haut = +X (voir APacmanPawn::Tick)
		const FVector Forward = Pacman->GetActorForwardVector().GetSafeNormal2D();
		Raw = PlayerLocation + Forward * (TileSize * TilesAhead);

		if (bReproduceOverflowBug && Forward.X > 0.5f)
		{
			Raw -= FVector::RightVector * (TileSize * TilesAhead);
		}
	}

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	FNavLocation Projected;
	FVector Final = PlayerLocation;
	if (NavSys && NavSys->ProjectPointToNavigation(Raw, Projected, FVector(1000.f, 1000.f, 500.f)))
	{
		Final = Projected.Location;
	}

	BlackboardComp->SetValueAsVector(TargetLocation.SelectedKeyName, Final);
	return EBTNodeResult::Succeeded;
}