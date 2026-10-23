// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Characters/ECS_CharacterStates.h"
#include "Enemy.generated.h"

class UHealthBarComponent;
class UAnimMontage;
class UAttributeComponent;

UCLASS()
class SLASH_API AEnemy : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	AEnemy();
	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void DirectionalHitReact(const FVector& ImpactLocation);

	virtual void GetHit_Implementation(const FVector& ImpactLocation);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
private:
	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* AttributeComponent;
	
	UPROPERTY(EditAnywhere)
	UHealthBarComponent* HealthBarWidget;
	
	/* Animation Montages */
	UPROPERTY(EditDefaultsOnly, Category= Montages)
	UAnimMontage* HitReactMontage;
	
	UPROPERTY(EditDefaultsOnly, Category= Montages)
	UAnimMontage* DeathMontage;
	
	// Sounds
	UPROPERTY(EditAnywhere, Category = Sounds)
	USoundBase* HitReactSound;
	
	// Data type for Cascade-System
	UPROPERTY(EditAnywhere, Category = VisualEffects)
	UParticleSystem* HitReactParticleSystem;
	
	UPROPERTY(VisibleAnywhere, Category = Combat)
	AActor* CombatTarget;
	
	UPROPERTY(EditAnywhere, Category=Combat)
	double CombatRadius = 500.f;
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadOnly)
	EDeathPose DeathPose = EDeathPose::EDP_Alive;
	
	void Die();
	
	/**
	* Play Montage Functions
	*/
	void PlayHitReactMontage(const FName& SectionName);
};
