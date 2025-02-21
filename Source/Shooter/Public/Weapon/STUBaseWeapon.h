// Shooter Project Game. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUBaseWeapon.generated.h"

UCLASS()
class SHOOTER_API ASTUBaseWeapon : public AActor
{
	GENERATED_BODY()

public:
	ASTUBaseWeapon();

protected:
	virtual void BeginPlay() override;
};
