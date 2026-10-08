#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PacmanNode.h" 
#include "PacmanPawn.generated.h"

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
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score")
	int32 Score = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	int32 Vies = 3;

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void PerdreVie();
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MoveSpeed = 400.f;
	
	UPROPERTY(EditInstanceOnly, Category = "Movement")
	APacmanNode* CurrentNode;

	APacmanNode* TargetNode;
	EPacmanDirection CurrentDirection = EPacmanDirection::None;
	EPacmanDirection NextDirection = EPacmanDirection::None;
	
	bool IsOppositeDirection(EPacmanDirection Dir1, EPacmanDirection Dir2);

protected:
	void MoveForwardInput(float Value);
	void MoveRightInput(float Value);

	APacmanNode* GetNodeInDirection(APacmanNode* Node, EPacmanDirection Direction);
};