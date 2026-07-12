// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SlashAnimInstance.h"
#include "Characters/SlashCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Animations "BeginPlay" Function
void USlashAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	// Holt den Pawn und castet ihn zu ASlashCharacter 
	SlashCharacter = Cast<ASlashCharacter>(TryGetPawnOwner());
	
	if (SlashCharacter)
	{
		SlashCharacterMovement = SlashCharacter->GetCharacterMovement();
	}
}

// Animations "Tick"-Function
void USlashAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	
	if (SlashCharacterMovement)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(SlashCharacterMovement->Velocity);
		IsFalling = SlashCharacterMovement->IsFalling();
	}
}
