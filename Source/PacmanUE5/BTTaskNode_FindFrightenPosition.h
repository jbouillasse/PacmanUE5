#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BTTaskNode_FindFrightenPosition.generated.h"

UCLASS()
class PACMANUE5_API UBTTaskNode_FindFrightenPosition : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTaskNode_FindFrightenPosition();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetLocationKey;

	// Rayon dans lequel choisir une destination aléatoire
	UPROPERTY(EditAnywhere, Category = "Frighten")
	float SearchRadius = 500.f;

	// Distance minimale entre le fantôme et la destination
	UPROPERTY(EditAnywhere, Category = "Frighten")
	float MinDistanceFromGhost = 200.f;
};