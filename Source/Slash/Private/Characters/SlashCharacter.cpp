// Fill out your copyright notice in the Description page of Project Settings.
#include "Characters/SlashCharacter.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GroomComponent.h"
#include "Items/Weapons/Weapon.h"
#include "Animation/AnimMontage.h"
#include "Components/BoxComponent.h"

// Sets default values
ASlashCharacter::ASlashCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 300.f;
	SpringArm->bUsePawnControlRotation = true; // Kamera/SpringArm um Charakter drehen, ohen dass dieser sich mitbewegt

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(SpringArm);

	HairGroom = CreateDefaultSubobject<UGroomComponent>(TEXT("HairGroom"));
	HairGroom->SetupAttachment(GetMesh());
	HairGroom->AttachmentName = FString("head");

	Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
	Eyebrows->SetupAttachment(GetMesh());
	Eyebrows->AttachmentName = FString("head");
}

void ASlashCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(SlashInputMappingContext, 0);
		}
	}
}

void ASlashCharacter::Move(const FInputActionValue& Value)
{
	if (ActionState != EActionState::EAS_Unoccupied) return;

	const FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, MovementVector.Y);

	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void ASlashCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerPitchInput(LookAxisVector.Y);
	AddControllerYawInput(LookAxisVector.X);
}

void ASlashCharacter::EKeyPressed(const FInputActionValue& Value)
{
	// If weapon to pick up is nearby
	if (AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem))
	{
		OverlappingWeapon->Equip(GetMesh(), FName("RightHandSocket"));
		CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
		OverlappingItem = nullptr; // Prevents equipping a weapon, thats already equipped
		EquippedWeapon = OverlappingWeapon;
	}
	// If the Player can disarm
	else if (ActionState == EActionState::EAS_Unoccupied && CharacterState != ECharacterState::ECS_Unequipped)
	{
		PlayEquipMontage(FName("Unequip"));
		CharacterState = ECharacterState::ECS_Unequipped;
		ActionState = EActionState::EAS_EquippingWeapon;
	}
	// If the player can equip the weapon 
	else if (ActionState == EActionState::EAS_Unoccupied && CharacterState == ECharacterState::ECS_Unequipped &&
		EquippedWeapon)
	{
		PlayEquipMontage(FName("Equip"));
		CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
		ActionState = EActionState::EAS_EquippingWeapon;
	}
}

void ASlashCharacter::Attack(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Display, TEXT("Attacking"));

	if (ActionState == EActionState::EAS_Unoccupied && CharacterState != ECharacterState::ECS_Unequipped)
	{
		PlayAttackMontage();
		ActionState = EActionState::EAS_Attacking;
	}
}

void ASlashCharacter::Jump()
{
	Super::Jump();
}

void ASlashCharacter::PlayAttackMontage()
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		if (AttackMontage)
		{
			// Play Montage
			AnimInstance->Montage_Play(AttackMontage);
			// Generate Random Number to select played montage
			const int32 Selection = FMath::RandRange(0, 1);

			FName SectionName = FName();
			switch (Selection)
			{
			case 0:
				SectionName = FName("Attack 1");
				break;
			case 1:
				SectionName = FName("Attack 2");
				break;
			default:
				break;
			}
			AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
		}
	}
}

void ASlashCharacter::PlayEquipMontage(FName SectionName)
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		if (EquipMontage)
		{
			// Play Montage
			AnimInstance->Montage_Play(EquipMontage);
			AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
		}
	}
}

// Function will be called from the AnimationBlueprint in response to the AnimNotify
void ASlashCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

//Will be called by ABP_Echo through AnimNotify_Disarm
void ASlashCharacter::Disarm()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
	}
}

//Will be called by ABP_Echo through AnimNotify_Arm
void ASlashCharacter::Arm()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
	}
}

//Will be called by ABP_Echo through AnimNotify_FinishEquipping
void ASlashCharacter::FinishEquipping()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void ASlashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASlashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MovementInputAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Move);
		EnhancedInputComponent->BindAction(LookInputAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Look);
		EnhancedInputComponent->BindAction(JumpInputAction, ETriggerEvent::Started, this, &ASlashCharacter::Jump);
		EnhancedInputComponent->BindAction(IA_EKeyPressed, ETriggerEvent::Started, this, &ASlashCharacter::EKeyPressed);
		EnhancedInputComponent->BindAction(AttackInputAction, ETriggerEvent::Started, this, &ASlashCharacter::Attack);
	}
}

void ASlashCharacter::SetWeaponCollision(ECollisionEnabled::Type CollisionType)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionType);
	}
}
