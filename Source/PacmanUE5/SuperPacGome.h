#pragma once

#include "CoreMinimal.h"
#include "Eatable.h"
#include "SuperPacGome.generated.h"

UCLASS()
class PACMANUE5_API ASuperPacGome : public AEatable
{
	GENERATED_BODY()
    
public:
	virtual void OnEat() override;
};