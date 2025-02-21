// Shooter Project Game. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "STUBaseCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USTUHealthComponent;
class UTextRenderComponent;


UCLASS()
class SHOOTER_API ASTUBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASTUBaseCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USTUHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UTextRenderComponent* HealthTextComponent;

	UPROPERTY(EditDefaultsOnly, Category="Animation")
	UAnimMontage* DeathAnimMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	float sprintSpeed = 900.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	float defaultSpeed = 600.0f;
	
	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float LifeSpanOnDeath = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	FVector2D LandedDamageVelocity = FVector2D(900.0f, 1200.0f);

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	FVector2D LandedDamage = FVector2D(10.0f, 100.0f);


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category="Movement")
	bool IsRunning() const;

	UFUNCTION(BlueprintCallable, Category="Movement")
	float CalculateCharacterDirection() const;

private:
	UFUNCTION()
	void OnDeath();

	UFUNCTION()
	void OnHealthChanged(float Health);

	bool WantsToRun;
	bool IsMovingForward;

	void MoveForward(float value);
	void MoveRight(float value);

	void StartSprinting();
	void StopSprinting();

	UFUNCTION()
	void OnPlayerLanded(const FHitResult& Hit);
};
