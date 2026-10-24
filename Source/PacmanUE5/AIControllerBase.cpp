#include "AIControllerBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

AAIControllerBase::AAIControllerBase()
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	MyBlackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("MyBlackboard"));
}

void AAIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (InPawn != nullptr && BehaviorTreeAsset != nullptr)
	{
		if (BehaviorTreeAsset->BlackboardAsset != nullptr)
		{
			MyBlackboard->InitializeBlackboard(*(BehaviorTreeAsset->BlackboardAsset));
		}

		BehaviorTreeComponent->StartTree(*BehaviorTreeAsset);
	}
}

void AAIControllerBase::UpdateBlackboardStates(bool bIsDead, bool bIsFrightened)
{
	if (MyBlackboard)
	{
		MyBlackboard->SetValueAsBool(FName("IsDead"), bIsDead);
		MyBlackboard->SetValueAsBool(FName("IsFrightened"), bIsFrightened);
	}
}

void AAIControllerBase::PauseBehavior()
{
	if (BehaviorTreeComponent)
	{
		BehaviorTreeComponent->PauseLogic(TEXT("GhostHouse"));
	}
}

void AAIControllerBase::ResumeBehavior()
{
	if (BehaviorTreeComponent)
	{
		BehaviorTreeComponent->ResumeLogic(TEXT("GhostHouse"));
	}
}