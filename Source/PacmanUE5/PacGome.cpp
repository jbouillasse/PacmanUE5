#include "PacGome.h"
#include "PacmanPawn.h"
#include "Kismet/GameplayStatics.h"

void APacGome::OnEat()
{
	// On récupère le joueur et on augmente son score
	APacmanPawn* Joueur = Cast<APacmanPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (Joueur)
	{
		Joueur->Score += 10;
	}

	Super::OnEat(); // Appelle le Destroy()
}