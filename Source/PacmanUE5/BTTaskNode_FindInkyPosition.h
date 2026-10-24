#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BTTaskNode_FindInkyPosition.generated.h"

UCLASS()
class PACMANUE5_API UBTTaskNode_FindInkyPosition : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTaskNode_FindInkyPosition();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, Category = "AI")
	struct FBlackboardKeySelector TargetLocation;

	// Coin visé en mode Scatter (Inky : bas-droite)
	UPROPERTY(EditAnywhere, Category = "AI")
	FVector ScatterLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "AI")
	float TileSize = 100.f;

	// Le pivot est à N cases devant Pac-Man (2 dans l'original)
	UPROPERTY(EditAnywhere, Category = "AI")
	float PivotTilesAhead = 2.f;

	UPROPERTY(EditAnywhere, Category = "AI")
	bool bReproduceOverflowBug = true;
};