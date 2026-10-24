#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BTTaskNode_FindAmbushPosition.generated.h"

UCLASS()
class PACMANUE5_API UBTTaskNode_FindAmbushPosition : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTaskNode_FindAmbushPosition();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, Category = "AI")
	float NavArea;

	UPROPERTY(EditAnywhere, Category = "AI")
	struct FBlackboardKeySelector TargetLocation;

	// Coin visé en mode Scatter (Pinky : haut-gauche)
	UPROPERTY(EditAnywhere, Category = "AI")
	FVector ScatterLocation = FVector::ZeroVector;

	// Taille d'une case du labyrinthe (unités Unreal)
	UPROPERTY(EditAnywhere, Category = "AI")
	float TileSize = 100.f;

	// Nombre de cases devant Pac-Man (4 dans l'original)
	UPROPERTY(EditAnywhere, Category = "AI")
	float TilesAhead = 4.f;

	// Bug de l'original : si Pac-Man va vers le haut, la cible est aussi décalée vers la gauche
	UPROPERTY(EditAnywhere, Category = "AI")
	bool bReproduceOverflowBug = true;
};