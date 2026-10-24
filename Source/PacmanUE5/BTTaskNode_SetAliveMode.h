#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_SetAliveMode.generated.h"

UCLASS()
class PACMANUE5_API UBTTaskNode_SetAliveMode : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTaskNode_SetAliveMode();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};