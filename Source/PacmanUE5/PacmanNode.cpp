// PacmanNode.cpp
#include "PacmanNode.h"
#include "Components/SphereComponent.h"

APacmanNode::APacmanNode()
{
	PrimaryActorTick.bCanEverTick = false; // Pas besoin de Tick pour un simple point

	DebugSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DebugSphere"));
	RootComponent = DebugSphere;
	DebugSphere->SetSphereRadius(20.f);
	DebugSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}