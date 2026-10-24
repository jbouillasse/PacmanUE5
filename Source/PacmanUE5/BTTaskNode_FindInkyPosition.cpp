#include "BTTaskNode_FindInkyPosition.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "PacmanPawn.h"

UBTTaskNode_FindInkyPosition::UBTTaskNode_FindInkyPosition()
{
    NodeName = TEXT("Find Inky Position");
}

EBTNodeResult::Type UBTTaskNode_FindInkyPosition::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    APacmanPawn* Pacman = Cast<APacmanPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!Pacman || !BlackboardComp) return EBTNodeResult::Failed;

    const FVector PlayerLocation = Pacman->GetActorLocation();
    FVector Raw = PlayerLocation;   // fallback : Pac-Man

    if (Pacman->IsScatterMode())
    {
        Raw = ScatterLocation;
    }
    else
    {
        TArray<AActor*> FoundActors;
        UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Blinky"), FoundActors);

        if (FoundActors.Num() > 0 && FoundActors[0] != nullptr)
        {
            const FVector Forward = Pacman->GetActorForwardVector().GetSafeNormal2D();
            FVector Pivot = PlayerLocation + Forward * (TileSize * PivotTilesAhead);

            if (bReproduceOverflowBug && Forward.X > 0.5f)
            {
                Pivot -= FVector::RightVector * (TileSize * PivotTilesAhead);
            }

            // Vecteur Blinky -> pivot, doublé
            const FVector BlinkyLoc = FoundActors[0]->GetActorLocation();
            Raw = BlinkyLoc + (Pivot - BlinkyLoc) * 2.f;
        }
        else if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("ERREUR : Tag 'Blinky' introuvable !"));
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