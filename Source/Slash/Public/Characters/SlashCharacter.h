// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include  "InputActionValue.h"
#include "SlashCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class UCameraComponent;
class USpringArmComponent;
class UGroomComponent;

UCLASS()
class SLASH_API ASlashCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASlashCharacter();
	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;
	
	/* Inputs */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* SlashInputMappingContext;  
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MovementInputAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Input")
	UInputAction* LookInputAction; 
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Input")
	UInputAction* JumpInputAction; 
	
	/* Input Functions */
	void Move(const FInputActionValue& Value);
	
	void Look(const FInputActionValue& Value);
	
	// Jump Function inherited from ACharacter
	virtual void Jump() override;
	
private:
	
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;
	
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;
	
	UPROPERTY(VisibleAnywhere, Category= Hair)
	UGroomComponent* HairGroom;

	UPROPERTY(VisibleAnywhere, Category = Hair)
	UGroomComponent* Eyebrows;
};
