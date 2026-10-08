#include "Eatable.h"
#include "PacmanPawn.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

AEatable::AEatable()
{
	PrimaryActorTick.bCanEverTick = false;

	// Création de la Hitbox
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	RootComponent = CollisionBox;
	CollisionBox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	// Création du Mesh (le visuel de la gomme)
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); 
}

void AEatable::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AEatable::OnOverlap);
}

void AEatable::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// On vérifie que l'acteur qui nous touche existe et que c'est bien Pac-Man
	if (OtherActor && OtherActor->IsA(APacmanPawn::StaticClass()))
	{
		OnEat();
	}
}

void AEatable::OnEat()
{
	// L'objet se détruit (disparaît de la carte)
	Destroy();
}