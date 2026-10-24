#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_ChasePlayer.generated.h"

UCLASS()
class PACMANUE5_API UBTTaskNode_ChasePlayer : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTaskNode_ChasePlayer();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	// Champs demandés par le TP
	UPROPERTY(EditAnywhere, Category = "AI")
	float NavArea; 

	// La clé du Blackboard où on a sauvegardé la position de Pac-Man
	UPROPERTY(EditAnywhere, Category = "AI")
	struct FBlackboardKeySelector TargetLocation; 
};