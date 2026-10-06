#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PacmanNode.h" 
#include "PacmanPawn.generated.h"

// Énumération pour gérer les directions
UENUM(BlueprintType)
enum class EPacmanDirection : uint8
{
	None,
	Up,
	Down,
	Left,
	Right
};

UCLASS()
class PACMANUE5_API APacmanPawn : public APawn
{
	GENERATED_BODY()

public:
	APacmanPawn();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MoveSpeed = 400.f;

	// Le noeud de départ, à assigner obligatoirement dans l'éditeur !
	UPROPERTY(EditInstanceOnly, Category = "Movement")
	APacmanNode* CurrentNode;

	APacmanNode* TargetNode;
	EPacmanDirection CurrentDirection = EPacmanDirection::None;
	EPacmanDirection NextDirection = EPacmanDirection::None;
	
	bool IsOppositeDirection(EPacmanDirection Dir1, EPacmanDirection Dir2);

protected:
	// ... tes autres variables (CurrentNode, TargetNode, etc.) restent identiques

	// Nouvelles fonctions pour les Axes
	void MoveForwardInput(float Value);
	void MoveRightInput(float Value);

	// Fonction utilitaire pour trouver le voisin selon la direction
	APacmanNode* GetNodeInDirection(APacmanNode* Node, EPacmanDirection Direction);
};