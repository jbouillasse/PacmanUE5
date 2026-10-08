#include "PacEntity.h"

APacEntity::APacEntity()
{
	PrimaryActorTick.bCanEverTick = true;

	// Initialisation du champ PawnMovement
	PawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("PawnMovement"));

	// Initialisation de la direction à zéro pour ne pas bouger au démarrage
	CurrentDirection = FVector::ZeroVector;
}

void APacEntity::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &APacEntity::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APacEntity::MoveRight);
}

void APacEntity::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!CurrentDirection.IsNearlyZero())
	{
		AddMovementInput(CurrentDirection, 1.0f);
	}
}

void APacEntity::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		CurrentDirection = FVector(Value > 0 ? 1.0f : -1.0f, 0.0f, 0.0f);

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
		CurrentDirection = FVector(0.0f, Value > 0 ? 1.0f : -1.0f, 0.0f);
		
		if (PawnMovement)
		{
			PawnMovement->Velocity.X = 0.0f;
		}
	}
}