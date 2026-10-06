// PacmanNode.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
// Le .generated.h doit obligatoirement rester le tout dernier #include
#include "PacmanNode.generated.h" 

UCLASS()
class PACMANUE5_API APacmanNode : public AActor
{
	GENERATED_BODY()

public:
	APacmanNode();

	// Pointeurs vers les noeuds voisins exposés dans l'éditeur
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Navigation")
	APacmanNode* NodeUp;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Navigation")
	APacmanNode* NodeDown;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Navigation")
	APacmanNode* NodeLeft;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Navigation")
	APacmanNode* NodeRight;

protected:
	// Composant visuel pour placer facilement le point dans l'éditeur
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USphereComponent* DebugSphere;

};