// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SlashAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SLASH_API USlashAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	// Äquivalent zum BP-Event: Blueprint Initialize Animation
	virtual void NativeInitializeAnimation() override;

	// Äquivalent zum BP-Event: Blueprint Update Animation
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly) // Zeigt Variable links bei den BPs an
	class ASlashCharacter* SlashCharacter;

	UPROPERTY(BlueprintReadOnly, Category=Movement) // Zeigt Variable links bei den BPs an
	class UCharacterMovementComponent* SlashCharacterMovement;

	UPROPERTY(BlueprintReadOnly, Category=Movement) // Zeigt Variable links bei den BPs an
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category=Movement) // Zeigt Variable links bei den BPs an
	bool IsFalling;
};
