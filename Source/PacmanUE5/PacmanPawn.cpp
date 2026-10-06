#include "PacmanPawn.h"
#include "Math/UnrealMathUtility.h"

APacmanPawn::APacmanPawn()
{
    PrimaryActorTick.bCanEverTick = true;
}

void APacmanPawn::BeginPlay()
{
    Super::BeginPlay();
    
    // Initialisation
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
    // On utilise un seuil (0.5) pour éviter les micro-mouvements des joysticks de manette
    if (Value > 0.5f)
    {
        NextDirection = EPacmanDirection::Up; // Touche Z ou Joystick Haut
    }
    else if (Value < -0.5f)
    {
        NextDirection = EPacmanDirection::Down; // Touche S ou Joystick Bas
    }
}

void APacmanPawn::MoveRightInput(float Value)
{
    if (Value > 0.5f)
    {
        NextDirection = EPacmanDirection::Right; // Touche D ou Joystick Droite
    }
    else if (Value < -0.5f)
    {
        NextDirection = EPacmanDirection::Left; // Touche Q ou Joystick Gauche
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

// Mise à jour de la fonction Tick
void APacmanPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!CurrentNode) return;

    // NOUVEAU : Vérifier le demi-tour instantané AVANT d'atteindre le point
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

    // Le reste de ton code original ne change pas
    // Si on a atteint la cible ou si on est à l'arrêt...
    if (!TargetNode || FVector::Dist(GetActorLocation(), TargetNode->GetActorLocation()) < 5.0f)
    {
        if (TargetNode)
        {
            CurrentNode = TargetNode; 
            SetActorLocation(CurrentNode->GetActorLocation());
        }

        APacmanNode* PossibleNextNode = GetNodeInDirection(CurrentNode, NextDirection);
        
        if (PossibleNextNode)
        {
            TargetNode = PossibleNextNode;
            CurrentDirection = NextDirection; 
        }
        else
        {
            PossibleNextNode = GetNodeInDirection(CurrentNode, CurrentDirection);
            if (PossibleNextNode)
            {
                TargetNode = PossibleNextNode;
            }
            else
            {
                TargetNode = nullptr; 
                CurrentDirection = EPacmanDirection::None;
            }
        }
    }

    // Le déplacement continu
    if (TargetNode)
    {
        FVector NewLocation = FMath::VInterpConstantTo(GetActorLocation(), TargetNode->GetActorLocation(), DeltaTime, MoveSpeed);
        SetActorLocation(NewLocation);
    }
}