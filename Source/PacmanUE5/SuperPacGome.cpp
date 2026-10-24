#include "SuperPacGome.h"
#include "PacmanPawn.h"
#include "Ghost.h"
#include "Kismet/GameplayStatics.h"

void ASuperPacGome::OnEat()
{
	APacmanPawn* Joueur = Cast<APacmanPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (Joueur)
	{
		Joueur->Score += 100;
	}

	// Tous les fantômes passent en mode fuite
	float Duration = 0.f;
	TArray<AActor*> Ghosts;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGhost::StaticClass(), Ghosts);
	for (AActor* Actor : Ghosts)
	{
		if (AGhost* Ghost = Cast<AGhost>(Actor))
		{
			Ghost->SetFrightenMode();
			Duration = FMath::Max(Duration, Ghost->GetFrightenDuration());
		}
	}

	// Combo de score remis à 200 + cycle Scatter/Chase en pause pendant la fuite
	if (Joueur)
	{
		Joueur->OnSuperPacGomeEaten(Duration);
	}

	// Doit rester en dernier : compte la gomme et détruit l'objet
	Super::OnEat();
}