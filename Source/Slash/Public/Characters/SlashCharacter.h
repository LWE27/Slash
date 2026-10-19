// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ECS_CharacterStates.h"
#include "GameFramework/Character.h"
#include  "InputActionValue.h"
#include "SlashCharacter.generated.h"

class AWeapon;
class AItem;
class UInputMappingContext;
class UInputAction;
class UCameraComponent;
class USpringArmComponent;
class UGroomComponent;
class UAnimMontage;

UCLASS()
class SLASH_API ASlashCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASlashCharacter();
	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(BlueprintCallable)
	void SetWeaponCollision(ECollisionEnabled::Type CollisionType);

protected:
	virtual void BeginPlay() override;
	
	/* Input Actions */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* SlashInputMappingContext;  
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MovementInputAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Input")
	UInputAction* LookInputAction; 
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Input")
	UInputAction* JumpInputAction; 
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Input")
	UInputAction* IA_EKeyPressed;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Input")
	UInputAction* AttackInputAction;
	
	/* Input Functions */
	void Move(const FInputActionValue& Value);
	
	void Look(const FInputActionValue& Value);
	
	void EKeyPressed(const FInputActionValue& Value);	
	
	void Attack(const FInputActionValue& Value);	
	
	// Jump Function inherited from ACharacter
	virtual void Jump() override;
	
	// Default CharacterState
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;
	UPROPERTY(BlueprintReadWrite, meta=( AllowPrivateAccess=true) )
	EActionState ActionState = EActionState::EAS_Unoccupied;

	/**
	 * Play Montage Functions
	*/
	void PlayAttackMontage();
	
	void PlayEquipMontage(FName SectionName);
	
	UFUNCTION(BlueprintCallable)
	void AttackEnd();
	
	UFUNCTION(BlueprintCallable)
	void Disarm();
	
	UFUNCTION(BlueprintCallable)
	void Arm();
	
	UFUNCTION(BlueprintCallable)
	void FinishEquipping();
private:
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;
	
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;
	
	UPROPERTY(VisibleAnywhere, Category= Hair)
	UGroomComponent* HairGroom;

	UPROPERTY(VisibleAnywhere, Category = Hair)
	UGroomComponent* Eyebrows;
	
	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;
	
	/* Animation Montages */
	UPROPERTY(EditDefaultsOnly, Category=Montages)
	UAnimMontage* AttackMontage;
	
	UPROPERTY(EditDefaultsOnly, Category=Montages)
	UAnimMontage* EquipMontage;
	
	UPROPERTY(VisibleAnywhere, Category=Weapon)
	AWeapon* EquippedWeapon;
	
public:
	FORCEINLINE AItem* GetOverlappingItem() const { return OverlappingItem; }

	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
	
	FORCEINLINE void SetOverlappingItem(AItem* InOverlappingItem)
	{
		this->OverlappingItem = InOverlappingItem;
	}
};
