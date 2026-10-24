#include "PacmanPawn.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "TimerManager.h"
#include "Ghost.h"
#include "Eatable.h"

APacmanPawn::APacmanPawn()
{
    PrimaryActorTick.bCanEverTick = true;
}

void APacmanPawn::BeginPlay()
{
    Super::BeginPlay();

    InitialNode = CurrentNode; 
    InitialLocation = GetActorLocation();
    InitialRotation = GetActorRotation();

    TargetNode = CurrentNode; 
    if (CurrentNode)
    {
        SetActorLocation(CurrentNode->GetActorLocation());
    }

    LancerDecompte();
}

void APacmanPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    
    PlayerInputComponent->BindAxis("MoveForward", this, &APacmanPawn::MoveForwardInput);
    PlayerInputComponent->BindAxis("MoveRight", this, &APacmanPawn::MoveRightInput);
}

void APacmanPawn::MoveForwardInput(float Value)
{
    if (Value > 0.5f) NextDirection = EPacmanDirection::Up;
    else if (Value < -0.5f) NextDirection = EPacmanDirection::Down;
}

void APacmanPawn::MoveRightInput(float Value)
{
    if (Value > 0.5f) NextDirection = EPacmanDirection::Right;
    else if (Value < -0.5f) NextDirection = EPacmanDirection::Left;
}

APacmanNode* APacmanPawn::GetNodeInDirection(APacmanNode* Node, EPacmanDirection Direction)
{
    if (!Node) return nullptr;
    
    switch (Direction)
    {
        case EPacmanDirection::Up: return Node->NodeUp;
        case EPacmanDirection::Down: return Node->NodeDown;
        case EPacmanDirection::Left: return Node->NodeLeft;
        case EPacmanDirection::Right: return Node->NodeRight;
        default: return nullptr;
    }
}

bool APacmanPawn::IsOppositeDirection(EPacmanDirection Dir1, EPacmanDirection Dir2)
{
    if (Dir1 == EPacmanDirection::Up && Dir2 == EPacmanDirection::Down) return true;
    if (Dir1 == EPacmanDirection::Down && Dir2 == EPacmanDirection::Up) return true;
    if (Dir1 == EPacmanDirection::Left && Dir2 == EPacmanDirection::Right) return true;
    if (Dir1 == EPacmanDirection::Right && Dir2 == EPacmanDirection::Left) return true;
    return false;
}

// ---------------- Sons ----------------

// Un son "voulu" qui se termine repart au début (comme une boucle),
// sans dépendre de la case Looping du Sound Wave.
void APacmanPawn::OnLoopSoundFinished(UAudioComponent* FinishedComp)
{
    if (!FinishedComp) return;

    if (FinishedComp == SirenComp)
    {
        if (bSirenWanted && !bFrightenSoundActive) FinishedComp->Play();
    }
    else if (FinishedComp == FrightenComp)
    {
        if (bFrightenSoundActive) FinishedComp->Play();
    }
}

void APacmanPawn::StartSiren()
{
    // Pas de sirène pendant la fuite
    if (!SirenSound || bFrightenSoundActive) return;

    bSirenWanted = true;

    if (!SirenComp)
    {
        SirenComp = UGameplayStatics::SpawnSound2D(this, SirenSound, 1.f, 1.f, 0.f, nullptr, false, false);
        if (SirenComp)
        {
            SirenComp->OnAudioFinishedNative.AddUObject(this, &APacmanPawn::OnLoopSoundFinished);
        }
    }
    if (SirenComp && !SirenComp->IsPlaying())
    {
        SirenComp->Play();
    }
}

void APacmanPawn::StopLoops()
{
    GetWorldTimerManager().ClearTimer(FrightenSoundTimer);
    GetWorldTimerManager().ClearTimer(DotStopTimer);

    // Les "voulu" passent à faux AVANT le Stop, pour que rien ne redémarre
    bSirenWanted = false;
    bFrightenSoundActive = false;

    if (SirenComp) SirenComp->Stop();
    if (FrightenComp) FrightenComp->Stop();
    if (DotComp) DotComp->Stop();
}

void APacmanPawn::EndFrightenSound()
{
    bFrightenSoundActive = false;
    if (FrightenComp) FrightenComp->Stop();
    StartSiren();
}

void APacmanPawn::StopDotSound()
{
    if (DotComp) DotComp->Stop();
}

void APacmanPawn::OnSuperPacGomeEaten(float FrightenDuration)
{
    GhostCombo = 0;

    // Son joué une seule fois quand la Super Pac-Gomme est mangée
    if (SuperPelletSound)
    {
        UGameplayStatics::PlaySound2D(this, SuperPelletSound);
    }

    if (!FrightenSound) return;

    // La sirène normale s'arrête pendant la fuite
    bSirenWanted = false;
    if (SirenComp) SirenComp->Stop();

    bFrightenSoundActive = true;

    if (!FrightenComp)
    {
        FrightenComp = UGameplayStatics::SpawnSound2D(this, FrightenSound, 1.f, 1.f, 0.f, nullptr, false, false);
        if (FrightenComp)
        {
            FrightenComp->OnAudioFinishedNative.AddUObject(this, &APacmanPawn::OnLoopSoundFinished);
        }
    }
    if (FrightenComp && !FrightenComp->IsPlaying())
    {
        FrightenComp->Play();
    }

    // Relancé si une 2e Super Pac-Gomme est mangée
    GetWorldTimerManager().SetTimer(FrightenSoundTimer, this, &APacmanPawn::EndFrightenSound, FrightenDuration, false);
}

int32 APacmanPawn::ConsumeGhostScore()
{
    if (GhostEatenSound)
    {
        UGameplayStatics::PlaySound2D(this, GhostEatenSound);
    }

    const int32 Result = 200 << FMath::Min(GhostCombo, 3);   // 200, 400, 800, 1600
    ++GhostCombo;
    return Result;
}

void APacmanPawn::NotifyDotEaten()
{
    if (bIsWon) return;

    if (DotSound)
    {
        // Le son démarre à la première gomme, et repart au début s'il s'est terminé
        // alors que Pac-Man mange encore. Il ne se rejoue jamais par-dessus lui-même.
        if (!DotComp)
        {
            DotComp = UGameplayStatics::SpawnSound2D(this, DotSound, 1.f, 1.f, 0.f, nullptr, false, false);
        }
        if (DotComp && !DotComp->IsPlaying())
        {
            DotComp->Play();
        }

        // Chaque gomme repousse l'arrêt : le son se coupe quand Pac-Man arrête de manger
        GetWorldTimerManager().SetTimer(DotStopTimer, this, &APacmanPawn::StopDotSound, DotStopDelay, false);
    }

    ++DotsEaten;
    ReleaseGhostsByDots();

    // Victoire : on compte toutes les gommes (petites et super) à la première gomme mangée,
    // puis on décompte. Ce nombre inclut la gomme en cours de consommation.
    if (RemainingEatables < 0)
    {
        TArray<AActor*> Eatables;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEatable::StaticClass(), Eatables);
        RemainingEatables = Eatables.Num();
    }

    --RemainingEatables;
    if (RemainingEatables <= 0)
    {
        TriggerVictory();
    }
}

// ---------------- Victoire ----------------

void APacmanPawn::TriggerVictory()
{
    if (bIsWon) return;
    bIsWon = true;

    // Coupe tous les sons
    StopLoops();

    if (VictoryWidgetClass)
    {
        UUserWidget* VictoryWidget = CreateWidget<UUserWidget>(GetWorld(), VictoryWidgetClass);
        if (VictoryWidget)
        {
            VictoryWidget->AddToViewport();
        }
    }

    UGameplayStatics::SetGamePaused(GetWorld(), true);
}

void APacmanPawn::Victoire()
{
    TriggerVictory();
}

// ---------------- Fantômes ----------------

void APacmanPawn::ReleaseGhostsByDots()
{
    TArray<AActor*> FoundGhosts;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGhost::StaticClass(), FoundGhosts);
    for (AActor* Actor : FoundGhosts)
    {
        AGhost* Ghost = Cast<AGhost>(Actor);
        if (Ghost && Ghost->IsWaitingInHouse() && DotsEaten >= Ghost->GetReleaseThreshold(bLostLife))
        {
            Ghost->ReleaseFromHouse();
        }
    }
}

void APacmanPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Pendant la mort, Pac-Man ne bouge plus (l'animation est gérée par le Blueprint)
    if (bIsDying || bIsWon) return;

    if (!CurrentNode) return;

    if (TargetNode && CurrentDirection != EPacmanDirection::None && NextDirection != CurrentDirection)
    {
        if (IsOppositeDirection(CurrentDirection, NextDirection))
        {
            APacmanNode* Temp = CurrentNode;
            CurrentNode = TargetNode;
            TargetNode = Temp;
            CurrentDirection = NextDirection;
        }
    }

    if (!TargetNode || FVector::Dist(GetActorLocation(), TargetNode->GetActorLocation()) < 5.0f)
    {
        if (TargetNode)
        {
            CurrentNode = TargetNode; 
            SetActorLocation(CurrentNode->GetActorLocation());
        }

        APacmanNode* PossibleNextNode = GetNodeInDirection(CurrentNode, NextDirection);
        
        if (PossibleNextNode)
        {
            TargetNode = PossibleNextNode;
            CurrentDirection = NextDirection; 
        }
        else
        {
            PossibleNextNode = GetNodeInDirection(CurrentNode, CurrentDirection);
            if (PossibleNextNode) TargetNode = PossibleNextNode;
            else
            {
                TargetNode = nullptr; 
                CurrentDirection = EPacmanDirection::None;
            }
        }
    }
    
    if (TargetNode)
    {
        float DistanceToTarget = FVector::Dist(GetActorLocation(), TargetNode->GetActorLocation());

        if (DistanceToTarget > 1500.f) 
        {
            SetActorLocation(TargetNode->GetActorLocation());
        }
        else
        {
            FVector NewLocation = FMath::VInterpConstantTo(GetActorLocation(), TargetNode->GetActorLocation(), DeltaTime, MoveSpeed);
            SetActorLocation(NewLocation);
        }
    }
    
    if (CurrentDirection != EPacmanDirection::None)
    {
        float TargetYaw = 0.f;
        switch (CurrentDirection)
        {
            case EPacmanDirection::Right: TargetYaw = 90.f; break;
            case EPacmanDirection::Down:  TargetYaw = 180.f; break;
            case EPacmanDirection::Left:  TargetYaw = -90.f; break;
            case EPacmanDirection::Up:    TargetYaw = 0.f; break;
        }

        FRotator CurrentRot = GetActorRotation();
        CurrentRot.Yaw = TargetYaw;
        SetActorRotation(CurrentRot);
    }
}

// ---------------- Mort ----------------

void APacmanPawn::PerdreVie()
{
    // Déjà en train de mourir, protégé (respawn / décompte), partie gagnée ou terminée :
    // on ignore les contacts en plus
    if (bIsDying || bInvulnerable || bIsWon || Vies <= 0) return;

    Vies--;
    bIsDying = true;

    // Coupe les sons en boucle et joue le son de mort
    StopLoops();
    if (DeathSound)
    {
        UGameplayStatics::PlaySound2D(this, DeathSound);
    }

    // Fige les fantômes pendant l'animation
    TArray<AActor*> FoundGhosts;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGhost::StaticClass(), FoundGhosts);
    for (AActor* Actor : FoundGhosts)
    {
        if (Actor)
        {
            Actor->CustomTimeDilation = 0.0001f;
        }
    }

    // Durée : valeur réglée, sinon durée du son de mort
    float Wait = DeathDuration;
    if (Wait <= 0.f)
    {
        Wait = DeathSound ? DeathSound->GetDuration() : 1.5f;
        if (Wait <= 0.f || Wait > 30.f) Wait = 1.5f;   // son en boucle ou invalide
    }

    // Lance l'animation de mort dans le Blueprint
    OnPlayerDeath(Wait);

    GetWorldTimerManager().SetTimer(DeathTimer, this, &APacmanPawn::FinishDeath, Wait, false);
}

void APacmanPawn::FinishDeath()
{
    if (Vies <= 0)
    {
        bIsDying = false;

        if (GameOverWidgetClass)
        {
            UUserWidget* GameOverWidget = CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetClass);
            if (GameOverWidget)
            {
                GameOverWidget->AddToViewport();
            }
        }

        UGameplayStatics::SetGamePaused(GetWorld(), true);
    }
    else
    {
        // Respawn, puis le décompte démarre (LancerDecompte est appelé dans Respawn).
        // bIsDying reste vrai jusqu'à la fin de Respawn pour qu'aucun contact ne passe entre-temps.
        Respawn();
        bIsDying = false;
    }
}

void APacmanPawn::Respawn()
{
    // Protégé dès maintenant, jusqu'à la fin du décompte (voir FinDecompte)
    bInvulnerable = true;

    CurrentNode = InitialNode;
    TargetNode = InitialNode;
    CurrentDirection = EPacmanDirection::None;
    NextDirection = EPacmanDirection::None;

    if (CurrentNode) SetActorLocation(CurrentNode->GetActorLocation());
    else SetActorLocation(InitialLocation);
    
    SetActorRotation(InitialRotation);

    GhostCombo = 0;

    // Nouveaux compteurs de gommes (à faire AVANT ResetToStart, qui lit ces valeurs)
    DotsEaten = 0;
    bLostLife = true;

    TArray<AActor*> FoundGhosts;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGhost::StaticClass(), FoundGhosts); 
    
    for (AActor* Actor : FoundGhosts)
    {
        AGhost* Ghost = Cast<AGhost>(Actor);
        if (Ghost)
        {
            Ghost->ResetToStart();
        }
    }

    // Remet l'apparence normale (échelle, rotation) EN DERNIER :
    // Pac-Man est déjà au départ et les fantômes sont déjà à la maison,
    // donc il ne peut plus regrossir sur le fantôme qui l'a touché.
    OnPlayerRespawn();
    
    LancerDecompte();
}

void APacmanPawn::LancerDecompte()
{
    // Pas de perte de vie tant que le décompte n'est pas fini
    bInvulnerable = true;

    if (CountdownWidgetClass)
    {
        UUserWidget* CountdownWidget = CreateWidget<UUserWidget>(GetWorld(), CountdownWidgetClass);
        if (CountdownWidget)
        {
            CountdownWidget->AddToViewport();
        }
    }

    this->CustomTimeDilation = 0.0001f;

    TArray<AActor*> FoundGhosts;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGhost::StaticClass(), FoundGhosts); 
    for (AActor* Actor : FoundGhosts)
    {
        if (Actor)
        {
            Actor->CustomTimeDilation = 0.0001f;
        }
    }
}

void APacmanPawn::FinDecompte()
{
    this->CustomTimeDilation = 1.0f;

    TArray<AActor*> FoundGhosts;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGhost::StaticClass(), FoundGhosts); 
    for (AActor* Actor : FoundGhosts)
    {
        if (Actor)
        {
            Actor->CustomTimeDilation = 1.0f;
        }
    }

    // Fin de la protection : le jeu redevient normal
    bInvulnerable = false;

    // La sirène démarre quand le décompte est fini (au lancement et après chaque mort)
    StartSiren();
}