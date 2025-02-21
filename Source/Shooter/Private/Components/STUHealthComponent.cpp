// Shooter Project Game. All rights reserved.


#include "Components/STUHealthComponent.h"


DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All);

USTUHealthComponent::USTUHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USTUHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	SetHealth(MaxHealth);
	OnHealthChanged.Broadcast(Health);
	AActor* ComponentOwner = GetOwner();
	if (ComponentOwner)
	{
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamage);
	}
}

void USTUHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
                                          class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f || IsDead()) return;

	SetHealth(Health - Damage);
	GetWorld()->GetTimerManager().ClearTimer(ActivateHealTimer);
	
	if (IsDead())
	{
		OnDeath.Broadcast();
	}
	else if (bAutoHeal && GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(ActivateHealTimer, this, &USTUHealthComponent::AutoHeal, HealUpdateTime, true , HealDelay);
	}
}

void USTUHealthComponent::AutoHeal()
{
	SetHealth(Health + HealModifier);

	if (FMath::IsNearlyEqual(Health, MaxHealth) && GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(ActivateHealTimer);
	}
}

void USTUHealthComponent::SetHealth(float NewHealth)
{
	Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
	OnHealthChanged.Broadcast(Health);
}
