#include "PacmanPawn.h"
#include "Math/UnrealMathUtility.h"

APacmanPawn::APacmanPawn()
{
    PrimaryActorTick.bCanEverTick = true;
}

void APacmanPawn::BeginPlay()
{
    Super::BeginPlay();

    TargetNode = CurrentNode; 
    if (CurrentNode)
    {
        SetActorLocation(CurrentNode->GetActorLocation());
    }
}

void APacmanPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    
    // Les noms doivent correspondre exactement à tes Axis Mappings
    PlayerInputComponent->BindAxis("MoveForward", this, &APacmanPawn::MoveForwardInput);
    PlayerInputComponent->BindAxis("MoveRight", this, &APacmanPawn::MoveRightInput);
}

void APacmanPawn::MoveForwardInput(float Value)
{
    if (Value > 0.5f)
    {
        NextDirection = EPacmanDirection::Up;
    }
    else if (Value < -0.5f)
    {
        NextDirection = EPacmanDirection::Down;
    }
}

void APacmanPawn::MoveRightInput(float Value)
{
    if (Value > 0.5f)
    {
        NextDirection = EPacmanDirection::Right;
    }
    else if (Value < -0.5f)
    {
        NextDirection = EPacmanDirection::Left;
    }
}

APacmanNode* APacmanPawn::GetNodeInDirection(APacmanNode* Node, EPacmanDirection Direction)
{
    if (!Node) return nullptr;
    
    switch (Direction)
    {
        case EPacmanDirection::Up: return Node->NodeUp;
        case EPacmanDirection::Down: return Node->NodeDown;
        case EPacmanDirection::Left: return Node->NodeLeft;
        case EPacmanDirection::Right: return Node->NodeRight;
        default: return nullptr;
    }
}

// Implémentation de la détection d'opposition
bool APacmanPawn::IsOppositeDirection(EPacmanDirection Dir1, EPacmanDirection Dir2)
{
    if (Dir1 == EPacmanDirection::Up && Dir2 == EPacmanDirection::Down) return true;
    if (Dir1 == EPacmanDirection::Down && Dir2 == EPacmanDirection::Up) return true;
    if (Dir1 == EPacmanDirection::Left && Dir2 == EPacmanDirection::Right) return true;
    if (Dir1 == EPacmanDirection::Right && Dir2 == EPacmanDirection::Left) return true;
    return false;
}

void APacmanPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!CurrentNode) return;

    if (TargetNode && CurrentDirection != EPacmanDirection::None && NextDirection != CurrentDirection)
    {
        if (IsOppositeDirection(CurrentDirection, NextDirection))
        {
            // On échange le point de départ et le point d'arrivée
            APacmanNode* Temp = CurrentNode;
            CurrentNode = TargetNode;
            TargetNode = Temp;
            
            // On met à jour la direction actuelle
            CurrentDirection = NextDirection;
        }
    }

    if (!TargetNode || FVector::Dist(GetActorLocation(), TargetNode->GetActorLocation()) < 5.0f)
    {
        if (TargetNode)
        {
            CurrentNode = TargetNode; 
            SetActorLocation(CurrentNode->GetActorLocation()); // Centre parfaitement le joueur
        }

        // On essaie d'abord d'aller dans la NOUVELLE direction demandée par le joueur
        APacmanNode* PossibleNextNode = GetNodeInDirection(CurrentNode, NextDirection);
        
        if (PossibleNextNode)
        {
            TargetNode = PossibleNextNode;
            CurrentDirection = NextDirection; 
        }
        else
        {
            // Si le virage est impossible, on essaie de continuer tout droit
            PossibleNextNode = GetNodeInDirection(CurrentNode, CurrentDirection);
            if (PossibleNextNode)
            {
                TargetNode = PossibleNextNode;
            }
            else
            {
                // Mur droit devant et aucune nouvelle direction valide : on s'arrête
                TargetNode = nullptr; 
                CurrentDirection = EPacmanDirection::None;
            }
        }
    }
    
    if (TargetNode)
    {
        float DistanceToTarget = FVector::Dist(GetActorLocation(), TargetNode->GetActorLocation());

        if (DistanceToTarget > 1500.f) 
        {
            SetActorLocation(TargetNode->GetActorLocation());
        }
        else
        {
            FVector NewLocation = FMath::VInterpConstantTo(GetActorLocation(), TargetNode->GetActorLocation(), DeltaTime, MoveSpeed);
            SetActorLocation(NewLocation);
        }
    }
    
    if (CurrentDirection != EPacmanDirection::None)
    {
        float TargetYaw = 0.f;
        
        switch (CurrentDirection)
        {
            case EPacmanDirection::Right: TargetYaw = 90.f; break;
            case EPacmanDirection::Down:  TargetYaw = 180.f; break;
            case EPacmanDirection::Left:  TargetYaw = -90.f; break;
            case EPacmanDirection::Up:    TargetYaw = 0.f; break;
        }

        // On récupère sa rotation actuelle pour ne pas écraser son inclinaison "à plat"
        FRotator CurrentRot = GetActorRotation();
        
        // On modifie uniquement l'axe Z
        CurrentRot.Yaw = TargetYaw;

        // On applique la nouvelle rotation
        SetActorRotation(CurrentRot);
    }
}

void APacmanPawn::PerdreVie()
{
    if (Vies > 0)
    {
        Vies--; 
        
        if (Vies <= 0)
        {

            if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("GAME OVER !"));

        }
        else
        {

        }
    }
}