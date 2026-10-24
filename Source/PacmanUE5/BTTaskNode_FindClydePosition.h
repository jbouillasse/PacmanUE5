#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BTTaskNode_FindClydePosition.generated.h"

UCLASS()
class PACMANUE5_API UBTTaskNode_FindClydePosition : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTaskNode_FindClydePosition();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, Category = "AI")
	struct FBlackboardKeySelector TargetLocation;

	// Distance de timidité : au-dessus il attaque, en dessous il va dans son coin.
	// 800 = 8 cases (Clyde). Mettre 0 pour Blinky : il vise toujours Pac-Man en Chase.
	UPROPERTY(EditAnywhere, Category = "AI")
	float FleeDistanceThreshold = 800.f;

	// Coin visé en Scatter (Clyde : bas-gauche, Blinky : haut-droite)
	UPROPERTY(EditAnywhere, Category = "AI")
	FVector ScatterLocation = FVector::ZeroVector;
};