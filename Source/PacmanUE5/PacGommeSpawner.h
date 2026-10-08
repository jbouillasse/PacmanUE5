#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PacGommeSpawner.generated.h"

class APacmanNode;

UCLASS()
class PACMANUE5_API APacGommeSpawner : public AActor
{
	GENERATED_BODY()
    
public: 
	APacGommeSpawner();

	UPROPERTY(EditAnywhere, Category = "Generation")
	TSubclassOf<AActor> PacGommeClass;

	UPROPERTY(EditAnywhere, Category = "Generation")
	TSubclassOf<AActor> SuperPacGommeClass;

	UPROPERTY(EditAnywhere, Category = "Generation")
	float Espacement = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Generation")
	int32 NombreSuperGommes = 4;

	UFUNCTION(CallInEditor, Category = "Generation")
	void GenererGommes();

	UFUNCTION(CallInEditor, Category = "Generation")
	void NettoyerGommes();
	
protected:
	virtual void BeginPlay() override;
};