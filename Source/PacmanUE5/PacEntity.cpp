#include "PacEntity.h"

APacEntity::APacEntity()
{
	// On s'assure que le Pawn peut tick
	PrimaryActorTick.bCanEverTick = true;

	// Initialisation du champ PawnMovement
	PawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("PawnMovement"));

	// Initialisation de la direction à zéro pour ne pas bouger au démarrage
	CurrentDirection = FVector::ZeroVector;
}

void APacEntity::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Liaison des axes configurés dans Unreal Engine avec nos fonctions C++
	PlayerInputComponent->BindAxis("MoveForward", this, &APacEntity::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APacEntity::MoveRight);
}

void APacEntity::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Applique le mouvement en continu tant qu'une direction est définie
	if (!CurrentDirection.IsNearlyZero())
	{
		AddMovementInput(CurrentDirection, 1.0f);
	}
}

void APacEntity::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// Force la direction sur l'axe X (1.0 ou -1.0)
		CurrentDirection = FVector(Value > 0 ? 1.0f : -1.0f, 0.0f, 0.0f);
        
		// Annule complètement la vélocité sur l'axe Y pour empêcher la glissade
		if (PawnMovement)
		{
			PawnMovement->Velocity.Y = 0.0f;
		}
	}
}

void APacEntity::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// Force la direction sur l'axe Y (1.0 ou -1.0)
		CurrentDirection = FVector(0.0f, Value > 0 ? 1.0f : -1.0f, 0.0f);
        
		// Annule complètement la vélocité sur l'axe X pour empêcher la glissade
		if (PawnMovement)
		{
			PawnMovement->Velocity.X = 0.0f;
		}
	}
}