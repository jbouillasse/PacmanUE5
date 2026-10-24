#pragma once

#include "CoreMinimal.h"
#include "Engine/TimerHandle.h"
#include "PacEntity.h"
#include "Ghost.generated.h"

class UBehaviorTree;

UCLASS()
class PACMANUE5_API AGhost : public APacEntity
{
	GENERATED_BODY()

public:
	AGhost();

protected:
	virtual void BeginPlay() override;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void SetAliveMode();
	void SetDeadMode();
	void SetFrightenMode();

	// Replace le fantôme au départ (respawn de Pac-Man)
	void ResetToStart();

	// Utilisés par les tâches BT et SuperPacGome
	FVector GetInitialLocation() const { return InitialGhostLocation; }
	float GetFrightenDuration() const { return FrightenDuration; }

	// Sortie de la maison
	bool IsWaitingInHouse() const { return bWaitingInHouse; }
	int32 GetReleaseThreshold(bool bAfterDeath) const { return bAfterDeath ? DotsToReleaseAfterDeath : DotsToRelease; }
	void ReleaseFromHouse();

	// Appelé à chaque changement d'état : à implémenter dans le Blueprint pour changer l'apparence
	UFUNCTION(BlueprintImplementableEvent, Category = "Ghost|Appearance")
	void OnModeChanged(bool bDead, bool bFrightened);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ghost|State")
	bool IsDead;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ghost|State")
	bool IsFrightened;

	// Durée du mode fuite (secondes)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ghost|State")
	float FrightenDuration = 10.f;

	// Multiplicateurs de vitesse selon l'état
	UPROPERTY(EditAnywhere, Category = "Ghost|Movement")
	float FrightenSpeedFactor = 0.66f;

	UPROPERTY(EditAnywhere, Category = "Ghost|Movement")
	float DeadSpeedFactor = 2.0f;

	// --- Maison des fantômes ---
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ghost|House")
	bool bStartsInHouse = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ghost|House")
	int32 DotsToRelease = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ghost|House")
	int32 DotsToReleaseAfterDeath = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ghost|AI")
	UBehaviorTree* TreeAsset;

private:
	FVector InitialGhostLocation;
	FTimerHandle FrightenTimerHandle;
	float BaseSpeed = 0.f;
	bool bWaitingInHouse = false;

	void EndFrighten();
	void ApplySpeedFactor(float Factor);
	void SyncBlackboard();
	void HoldInHouse();
};