#pragma once

#include "CoreMinimal.h"
#include "Eatable.h"
#include "PacGome.generated.h"

UCLASS()
class PACMANUE5_API APacGome : public AEatable
{
	GENERATED_BODY()
    
public:
	virtual void OnEat() override;
};