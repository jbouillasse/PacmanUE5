#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Engine/TimerHandle.h"
#include "PacmanNode.h" 
#include "PacmanPawn.generated.h"

class USoundBase;
class UAudioComponent;

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
    
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void Respawn();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UUserWidget> GameOverWidgetClass;

	// Widget affiché quand toutes les gommes sont mangées
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UUserWidget> VictoryWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UUserWidget> CountdownWidgetClass;

	UFUNCTION(BlueprintCallable, Category = "Game")
	void LancerDecompte();

	UFUNCTION(BlueprintCallable, Category = "Game")
	void FinDecompte();

	// Test : tape "Victoire" dans la console pendant le jeu pour afficher l'écran de victoire
	UFUNCTION(Exec, Category = "Debug")
	void Victoire();

	// Le mode Scatter n'existe plus : les fantômes chassent en permanence
	bool IsScatterMode() const { return false; }

	// Appelé quand une Super Pac-Gomme est mangée
	void OnSuperPacGomeEaten(float FrightenDuration);

	// Score du fantôme mangé : 200, 400, 800, 1600
	int32 ConsumeGhostScore();

	// --- Sortie des fantômes de la maison ---
	void NotifyDotEaten();
	int32 GetDotsEaten() const { return DotsEaten; }
	bool HasLostLife() const { return bLostLife; }

	bool IsDying() const { return bIsDying; }

	// --- Mort : événements à implémenter dans BP_Pacman ---
	// Lance l'animation de mort (Duration = durée totale en secondes)
	UFUNCTION(BlueprintImplementableEvent, Category = "Death")
	void OnPlayerDeath(float Duration);

	// Remet l'apparence normale de Pac-Man (échelle, rotation...) au respawn
	UFUNCTION(BlueprintImplementableEvent, Category = "Death")
	void OnPlayerRespawn();
	
	FRotator InitialRotation;

	// --- Sons (à assigner dans BP_Pacman) ---
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	USoundBase* DotSound;          // 03 Eating The Pac-dots (le fichier de 12 s)

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	USoundBase* SuperPelletSound;  // son joué une fois quand une Super Pac-Gomme est mangée

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	USoundBase* GhostEatenSound;   // 13 Eating The Ghost

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	USoundBase* DeathSound;        // 15 Fail

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	USoundBase* SirenSound;        // 06 Ghost - Normal Move

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	USoundBase* FrightenSound;     // 12 Ghost - Turn to Blue

	// Délai sans gomme mangée avant que le son des gommes s'arrête (secondes)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	float DotStopDelay = 0.35f;

	// Durée de la séquence de mort (secondes). 0 = durée du son de mort.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Death")
	float DeathDuration = 0.f;

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

	void MoveForwardInput(float Value);
	void MoveRightInput(float Value);

	APacmanNode* GetNodeInDirection(APacmanNode* Node, EPacmanDirection Direction);

private:
	FVector InitialLocation;
    
	UPROPERTY()
	APacmanNode* InitialNode;

	int32 GhostCombo = 0;

	// Sortie de la maison
	int32 DotsEaten = 0;
	bool bLostLife = false;

	void ReleaseGhostsByDots();

	// Victoire
	// -1 = pas encore compté. Calculé à la première gomme mangée.
	int32 RemainingEatables = -1;
	bool bIsWon = false;
	void TriggerVictory();

	// Mort
	bool bIsDying = false;
	FTimerHandle DeathTimer;
	void FinishDeath();

	// Protection : aucun contact ne retire de vie pendant le respawn et le décompte
	bool bInvulnerable = false;

	// Sons
	UPROPERTY()
	UAudioComponent* SirenComp = nullptr;

	UPROPERTY()
	UAudioComponent* FrightenComp = nullptr;

	UPROPERTY()
	UAudioComponent* DotComp = nullptr;

	FTimerHandle FrightenSoundTimer;
	FTimerHandle DotStopTimer;

	bool bSirenWanted = false;
	bool bFrightenSoundActive = false;

	void StartSiren();
	void StopLoops();
	void EndFrightenSound();
	void StopDotSound();
	void OnLoopSoundFinished(UAudioComponent* FinishedComp);
};