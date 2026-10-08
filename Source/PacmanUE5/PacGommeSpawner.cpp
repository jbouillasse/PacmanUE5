#include "PacGommeSpawner.h"
#include "PacmanNode.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"

void APacGommeSpawner::BeginPlay()
{
    Super::BeginPlay();
    
    // Génère automatiquement les pac-gommes au lancement du niveau
    GenererGommes();
}

APacGommeSpawner::APacGommeSpawner()
{
    PrimaryActorTick.bCanEverTick = false;
}

void APacGommeSpawner::GenererGommes()
{
    if (!PacGommeClass || !SuperPacGommeClass) return;

    UWorld* World = GetWorld();
    if (!World) return;

    TArray<FVector> PositionsPossibles;

    // 1. On parcourt tous les noeuds de la carte
    for (TActorIterator<APacmanNode> It(World); It; ++It)
    {
        APacmanNode* Node = *It;

        // On place OBLIGATOIREMENT une gomme sur l'intersection
        PositionsPossibles.AddUnique(Node->GetActorLocation());

        // Remplissage du couloir vers la DROITE
        if (Node->NodeRight)
        {
            FVector Start = Node->GetActorLocation();
            FVector End = Node->NodeRight->GetActorLocation();
            float Distance = FVector::Dist(Start, End);

            if (Distance < 1000.f) // On ignore les murs de téléportation
            {
                FVector Direction = (End - Start).GetSafeNormal();
                
                // On calcule combien de segments parfaits on peut faire
                int32 NbSegments = FMath::RoundToInt(Distance / Espacement);
                
                if (NbSegments > 1)
                {
                    // On recalcule l'espacement parfait pour CE couloir précis
                    float EspacementReel = Distance / NbSegments;
                    
                    // On place les gommes intermédiaires
                    for (int32 i = 1; i < NbSegments; i++)
                    {
                        PositionsPossibles.AddUnique(Start + (Direction * (i * EspacementReel)));
                    }
                }
            }
        }

        // Remplissage du couloir vers le BAS
        if (Node->NodeDown)
        {
            FVector Start = Node->GetActorLocation();
            FVector End = Node->NodeDown->GetActorLocation();
            float Distance = FVector::Dist(Start, End);

            if (Distance < 1000.f)
            {
                FVector Direction = (End - Start).GetSafeNormal();
                int32 NbSegments = FMath::RoundToInt(Distance / Espacement);
                
                if (NbSegments > 1)
                {
                    float EspacementReel = Distance / NbSegments;
                    for (int32 i = 1; i < NbSegments; i++)
                    {
                        PositionsPossibles.AddUnique(Start + (Direction * (i * EspacementReel)));
                    }
                }
            }
        }
    }

    // 2. On choisit aléatoirement les emplacements des Super Pac-Gommes
    for (int32 i = 0; i < NombreSuperGommes && PositionsPossibles.Num() > 0; i++)
    {
        int32 RandomIndex = FMath::RandRange(0, PositionsPossibles.Num() - 1);
        World->SpawnActor<AActor>(SuperPacGommeClass, PositionsPossibles[RandomIndex], FRotator::ZeroRotator);
        
        PositionsPossibles.RemoveAt(RandomIndex);
    }

    // 3. On remplit tous les emplacements restants avec des Pac-Gommes classiques
    for (const FVector& Loc : PositionsPossibles)
    {
        World->SpawnActor<AActor>(PacGommeClass, Loc, FRotator::ZeroRotator);
    }
}

void APacGommeSpawner::NettoyerGommes()
{
    UWorld* World = GetWorld();
    if (!World) return;

    TArray<AActor*> FoundActors;
    
    // Nettoyer les pac-gommes classiques
    if (PacGommeClass)
    {
        UGameplayStatics::GetAllActorsOfClass(World, PacGommeClass, FoundActors);
        for (AActor* Actor : FoundActors) Actor->Destroy();
    }
    
    // Nettoyer les super pac-gommes
    if (SuperPacGommeClass)
    {
        FoundActors.Empty();
        UGameplayStatics::GetAllActorsOfClass(World, SuperPacGommeClass, FoundActors);
        for (AActor* Actor : FoundActors) Actor->Destroy();
    }
}