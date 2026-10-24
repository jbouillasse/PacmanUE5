#include "Eatable.h"
#include "PacmanPawn.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

AEatable::AEatable()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	RootComponent = CollisionBox;
	CollisionBox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

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
	if (OtherActor && OtherActor->IsA(APacmanPawn::StaticClass()))
	{
		OnEat();
	}
}
//test

void AEatable::OnEat()
{
	// Compte la gomme pour la sortie des fantômes de la maison
	if (APacmanPawn* Pacman = Cast<APacmanPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
	{
		Pacman->NotifyDotEaten();
	}

	Destroy();
}