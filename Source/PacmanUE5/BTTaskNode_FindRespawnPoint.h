#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BTTaskNode_FindRespawnPoint.generated.h"

UCLASS()
class PACMANUE5_API UBTTaskNode_FindRespawnPoint : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTaskNode_FindRespawnPoint();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetLocationKey;
};