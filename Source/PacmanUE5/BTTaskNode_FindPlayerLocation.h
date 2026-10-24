#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_FindPlayerLocation.generated.h"

UCLASS()
class PACMANUE5_API UBTTaskNode_FindPlayerLocation : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTaskNode_FindPlayerLocation();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, Category = "AI")
	float NavArea; 

	UPROPERTY(EditAnywhere, Category = "AI")
	struct FBlackboardKeySelector TargetLocation; 
};