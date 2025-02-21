// Shooter Project Game. All rights reserved.


#include "Player/STUBaseCharacter.h"

#include "STUBaseWeapon.h"
#include "STUHealthComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/STUCharacterMovementComponent.h"
#include "Components/STUHealthComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/SpectatorPawn.h"

DEFINE_LOG_CATEGORY_STATIC(BaseCharacterLog, All, All);

// Sets default values
ASTUBaseCharacter::ASTUBaseCharacter(const FObjectInitializer& ObjectInitializer) : Super(
	ObjectInitializer.SetDefaultSubobjectClass<USTUCharacterMovementComponent>(
		ACharacter::CharacterMovementComponentName))
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArm);

	HealthComponent = CreateDefaultSubobject<USTUHealthComponent>(TEXT("HealthComponent"));

	HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("HealthTextComponent"));
	HealthTextComponent->SetupAttachment(GetRootComponent());
}

void ASTUBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	OnHealthChanged(HealthComponent->GetHealth());
	HealthComponent->OnDeath.AddUObject(this, &ASTUBaseCharacter::OnDeath);
	HealthComponent->OnHealthChanged.AddUObject(this, &ASTUBaseCharacter::OnHealthChanged);
	
	check(HealthComponent);
	check(HealthTextComponent);
	check(GetCharacterMovement());

	LandedDelegate.AddDynamic(this, &ASTUBaseCharacter::OnPlayerLanded);

	SpawnWeapon();
}

void ASTUBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASTUBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent)
	
	PlayerInputComponent->BindAxis("MoveForward", this, &ASTUBaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASTUBaseCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ASTUBaseCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &ASTUBaseCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASTUBaseCharacter::Jump);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASTUBaseCharacter::StartSprinting);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASTUBaseCharacter::StopSprinting);
}

bool ASTUBaseCharacter::IsRunning() const
{
	return WantsToRun && IsMovingForward && !GetVelocity().IsZero();
}

float ASTUBaseCharacter::CalculateCharacterDirection() const
{
	if (GetVelocity().IsZero()) return 0.0f;
	const auto VelocityNormal = GetVelocity().GetSafeNormal();
	const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
	const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);
	const auto Degrees = FMath::RadiansToDegrees(AngleBetween);
	return CrossProduct.IsZero() ? Degrees : Degrees * FMath::Sign(CrossProduct.Z);
}

void ASTUBaseCharacter::OnDeath()
{
	PlayAnimMontage(DeathAnimMontage);

	GetCharacterMovement()->DisableMovement();

	SetLifeSpan(LifeSpanOnDeath);

	if (Controller)
	{
		Controller->ChangeState(NAME_Spectating); 
	}
}

void ASTUBaseCharacter::OnHealthChanged(float Health)
{
	HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));
}


void ASTUBaseCharacter::MoveForward(float value)
{
	IsMovingForward = value > 0.0f;
	if (value == 0.0f) return;
	AddMovementInput(GetActorForwardVector(), value);
}

void ASTUBaseCharacter::MoveRight(float value)
{
	if (value == 0.0f) return;
	AddMovementInput(GetActorRightVector(), value);
}

void ASTUBaseCharacter::StartSprinting()
{
	WantsToRun = true;
}

void ASTUBaseCharacter::StopSprinting()
{
	WantsToRun = false;
}

void ASTUBaseCharacter::OnPlayerLanded(const FHitResult& Hit)
{
	const auto FallVelocityZ = -GetVelocity().Z;
	UE_LOG(BaseCharacterLog, Warning, TEXT("FALL VELOCITY: %f"), FallVelocityZ);
	if (FallVelocityZ < LandedDamageVelocity.X) return;

	const auto FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVelocityZ);

	TakeDamage(FinalDamage, FDamageEvent{}, nullptr, this);
}

void ASTUBaseCharacter::SpawnWeapon()
{
	if (!GetWorld()) return;
	const auto Weapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(WeaponClass);
	if (Weapon)
	{
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
		Weapon->AttachToComponent(GetMesh(), AttachmentRules, "WeaponSocket");
	}
}


