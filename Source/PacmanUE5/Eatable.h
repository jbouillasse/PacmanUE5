#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Eatable.generated.h"

class UBoxComponent;

UCLASS()
class PACMANUE5_API AEatable : public AActor
{
	GENERATED_BODY()
    
public:	
	AEatable();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* CollisionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMesh;

public:
	UFUNCTION()
	virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void OnEat();
};