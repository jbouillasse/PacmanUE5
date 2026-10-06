#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "PacEntity.generated.h"

UCLASS()
class PACMANUE5_API APacEntity : public APawn
{
	GENERATED_BODY()

public:
	APacEntity();

	// Surcharge de la fonction Tick pour le mouvement continu
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override; 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	UFloatingPawnMovement* PawnMovement;

	void MoveForward(float Value);
	void MoveRight(float Value);

	// Stocke la direction actuelle
	FVector CurrentDirection;
};