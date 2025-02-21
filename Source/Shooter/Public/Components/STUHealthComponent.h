// Shooter Project Game. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUHealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDeath);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SHOOTER_API USTUHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USTUHealthComponent();

	FOnDeath OnDeath;
	FOnHealthChanged OnHealthChanged;

	float GetHealth() const { return Health; }

	UFUNCTION(BlueprintCallable, Category="Health")
	bool IsDead() const { return FMath::IsNearlyZero(Health); }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Heal")
	bool bAutoHeal = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Heal", meta=(EditCondition="bAutoHeal"))
	float HealUpdateTime = 0.3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Heal", meta=(EditCondition="bAutoHeal"))
	float HealDelay = 3.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Heal", meta=(EditCondition="bAutoHeal"))
	float HealModifier = 1.0f;

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (ClampMin = "0", ClampMax = "100"))
	float MaxHealth = 100.0f;

private:
	float Health = 0.0f;

	UFUNCTION()
	void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
	                     class AController* InstigatedBy, AActor* DamageCauser);

	void AutoHeal();
	void SetHealth(float NewHealth);

	FTimerHandle ActivateHealTimer;
};
