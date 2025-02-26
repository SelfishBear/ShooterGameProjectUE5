// Shooter Project Game. All rights reserved.


#include "Weapon/STUProjectile.h"

#include "Components/SphereComponent.h"

ASTUProjectile::ASTUProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere collision"));
	CollisionComponent->InitSphereRadius(5.0f);
	SetRootComponent(CollisionComponent);
}

void ASTUProjectile::BeginPlay()
{
	Super::BeginPlay();
}
