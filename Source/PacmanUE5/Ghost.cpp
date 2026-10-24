#include "Ghost.h"
#include "AIControllerBase.h"
#include "PacmanPawn.h"
#include "Kismet/GameplayStatics.h"
#include "Components/PrimitiveComponent.h"
#include "TimerManager.h"

AGhost::AGhost()
{
    AIControllerClass = AAIControllerBase::StaticClass();
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

    IsDead = false;
    IsFrightened = false;
}

void AGhost::BeginPlay()
{
    Super::BeginPlay();

    InitialGhostLocation = GetActorLocation();
    if (PawnMovement) BaseSpeed = PawnMovement->MaxSpeed;

    TArray<UPrimitiveComponent*> Components;
    GetComponents<UPrimitiveComponent>(Components);
    for (UPrimitiveComponent* Comp : Components)
    {
        if (Comp->IsCollisionEnabled())
        {
            Comp->OnComponentBeginOverlap.AddDynamic(this, &AGhost::OnOverlap);
            break;
        }
    }

    SetAliveMode();

    // Au tick suivant, pour que l'IA ait démarré son arbre avant de le mettre en pause
    if (bStartsInHouse)
    {
        GetWorldTimerManager().SetTimerForNextTick(this, &AGhost::HoldInHouse);
    }
}

void AGhost::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AGhost::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor || OtherActor == this) return;

    APacmanPawn* PacmanPlayer = Cast<APacmanPawn>(OtherActor);
    if (!PacmanPlayer) return;

    if (IsDead) return;               // un fantôme qui rentre à la base est inoffensif

    if (IsFrightened)
    {
        PacmanPlayer->Score += PacmanPlayer->ConsumeGhostScore();   // 200 / 400 / 800 / 1600
        SetDeadMode();
    }
    else
    {
        PacmanPlayer->PerdreVie();
    }
}

void AGhost::ApplySpeedFactor(float Factor)
{
    if (PawnMovement && BaseSpeed > 0.f)
    {
        PawnMovement->MaxSpeed = BaseSpeed * Factor;
    }
}

void AGhost::SyncBlackboard()
{
    AAIControllerBase* AICon = Cast<AAIControllerBase>(GetController());
    if (AICon) AICon->UpdateBlackboardStates(IsDead, IsFrightened);
}

void AGhost::SetAliveMode()
{
    GetWorldTimerManager().ClearTimer(FrightenTimerHandle);

    IsDead = false;
    IsFrightened = false;
    ApplySpeedFactor(1.f);
    SyncBlackboard();
    OnModeChanged(IsDead, IsFrightened);
}

void AGhost::SetDeadMode()
{
    GetWorldTimerManager().ClearTimer(FrightenTimerHandle);

    IsDead = true;
    IsFrightened = false;
    ApplySpeedFactor(DeadSpeedFactor);
    SyncBlackboard();
    OnModeChanged(IsDead, IsFrightened);
}

void AGhost::SetFrightenMode()
{
    if (IsDead) return;

    IsFrightened = true;
    ApplySpeedFactor(FrightenSpeedFactor);
    SyncBlackboard();
    OnModeChanged(IsDead, IsFrightened);

    // Relance les 10 s si déjà effrayé
    GetWorldTimerManager().SetTimer(FrightenTimerHandle, this, &AGhost::EndFrighten, FrightenDuration, false);
}

void AGhost::EndFrighten()
{
    // Garde-fou : on ne réanime pas un fantôme mort
    if (IsDead || !IsFrightened) return;
    SetAliveMode();
}

void AGhost::HoldInHouse()
{
    if (!bStartsInHouse || bWaitingInHouse) return;

    // Déjà assez de gommes mangées : il sort directement (Pinky au début de la partie)
    APacmanPawn* Pacman = Cast<APacmanPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
    if (Pacman && Pacman->GetDotsEaten() >= GetReleaseThreshold(Pacman->HasLostLife()))
    {
        return;
    }

    if (AAIControllerBase* AICon = Cast<AAIControllerBase>(GetController()))
    {
        AICon->StopMovement();
        AICon->PauseBehavior();
        bWaitingInHouse = true;
    }
}

void AGhost::ReleaseFromHouse()
{
    if (!bWaitingInHouse) return;

    bWaitingInHouse = false;
    if (AAIControllerBase* AICon = Cast<AAIControllerBase>(GetController()))
    {
        AICon->ResumeBehavior();
    }
}

void AGhost::ResetToStart()
{
    if (AController* AICon = GetController())
    {
        AICon->StopMovement();   // annule le Move To en cours
    }

    SetActorLocation(InitialGhostLocation);
    SetAliveMode();              // annule aussi le timer de fuite

    // Retour dans la maison pour ceux qui doivent y attendre
    HoldInHouse();
}