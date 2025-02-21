// Shooter Project Game. All rights reserved.


#include "Weapon/STUBaseWeapon.h"

ASTUBaseWeapon::ASTUBaseWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASTUBaseWeapon::BeginPlay()
{
	Super::BeginPlay();
}
