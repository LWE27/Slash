// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"

#include "Components/AttributeComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "HUD/HealthBarComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Slash/DebugMacros.h" // Own Class with Macros for debugging


AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	//Setup Collision to get hit by sword trace
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	// Activate "Generate Overlap Events"
	GetMesh()->SetGenerateOverlapEvents(true);
	// Configure Camera to ignore Mesh (prevents camera blocking)
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	AttributeComponent = CreateDefaultSubobject<UAttributeComponent>(TEXT("AttributeComponent"));
	// Man muss es nirgends anbringen/attach, da es kein Mesh oder ähnliches besitzt

	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>("HealthBarWidget");
	HealthBarWidget->SetupAttachment(GetRootComponent());
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(false);
	}
}

void AEnemy::Die()
{
	
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		if (DeathMontage)
		{
			// Play Montage
			AnimInstance->Montage_Play(DeathMontage);
			// Generate Random Number to select played montage
			const int32 Selection = FMath::RandRange(0, 3);

			FName SectionName = FName();
			switch (Selection)
			{
			case 0:
				SectionName = FName("Death1");
				DeathPose = EDeathPose::EDP_Death1;
				break;
			case 1:
				SectionName = FName("Death2");
				DeathPose = EDeathPose::EDP_Death2;
				break;
			case 2:
				SectionName = FName("Death3");
				DeathPose = EDeathPose::EDP_Death3;
				break;
			case 3:
				SectionName = FName("Death4");
				DeathPose = EDeathPose::EDP_Death4;
				break;
			default:
				break;
			}
			AnimInstance->Montage_JumpToSection(SectionName, DeathMontage);
		}
	}

	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(false);
	}
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetLifeSpan(3.f);
}

void AEnemy::PlayHitReactMontage(const FName& SectionName)
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		if (HitReactMontage)
		{
			// Play Montage
			AnimInstance->Montage_Play(HitReactMontage);
			AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
		}
	}
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CombatTarget)
	{
		const double DistanceToTarget = (CombatTarget->GetActorLocation() - GetActorLocation()).Size();

		if (DistanceToTarget > CombatRadius)
		{
			CombatTarget = nullptr;
			if (HealthBarWidget)
			{
				HealthBarWidget->SetVisibility(false);
			}
		}
	}
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemy::DirectionalHitReact(const FVector& ImpactLocation)
{
	const FVector ForwardVector = GetActorForwardVector();
	// Lower Impact Point to the Enemy's Actor Location Z
	const FVector ImpactLowered(ImpactLocation.X, ImpactLocation.Y, GetActorLocation().Z);
	// Get normalized vector from Actor to ImpactLocation 
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal(); //GetSafeNormal prevents division by 0

	// Dot Product -> Forward * ToHit = |Forward| |ToHit| * cos(theta)
	const double CosTheta = FVector::DotProduct(ForwardVector, ToHit);

	//Inverse Cosine (Arc-Cosine) of cos(theta) to get theta
	double Theta = FMath::Acos(CosTheta);

	// convert from radiance to degrees
	Theta = FMath::RadiansToDegrees(Theta);

	// If CrossProduct points down, Theta should be negative
	const FVector CrossProduct = FVector::CrossProduct(ForwardVector, ToHit);
	if (CrossProduct.Z < 0)
	{
		Theta = -Theta;
	}

	FName Section("FromBack"); // Default Section
	if (Theta >= -45.f && Theta < 45.f)
	{
		Section = FName("FromFront");
	}
	else if (Theta >= -135.f && Theta < -45.f)
	{
		Section = FName("FromLeft");
	}
	else if (Theta >= 45.f && Theta < 135.f)
	{
		Section = FName("FromRight");
	}
	PlayHitReactMontage(Section);


	/* 
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(),
	                                     GetActorLocation() + CrossProduct * 100.f, 5.f, FColor::Blue, 5.f);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green,
		                                 FString::Printf(TEXT("Theta: %f"), Theta));
	}
	// Debug Stuff // Draws Arrows
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(),
	                                     GetActorLocation() + ForwardVector * 60.f, 5.f, FColor::Red, 5.f);

	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(),
	                                     GetActorLocation() + ToHit * 60.f, 5.f, FColor::Green, 5.f);
	                                     
	*/
}

void AEnemy::GetHit_Implementation(const FVector& ImpactLocation)
{
	//DRAW_SPHERE_COLOR(ImpactLocation, FColor::Blue); // Own Macro
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(true);
	}

	if (AttributeComponent && AttributeComponent->IsAlive())
	{
		DirectionalHitReact(ImpactLocation);
	}
	else
	{
		Die();
	}


	if (HitReactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitReactSound, ImpactLocation);
	}
	if (HitReactParticleSystem && GetWorld())
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitReactParticleSystem,
			ImpactLocation
		);
	}
}

float AEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator,
                         AActor* DamageCauser)
{
	if (AttributeComponent && HealthBarWidget)
	{
		AttributeComponent->ReceiveDamage(DamageAmount);

		HealthBarWidget->SetHealthPercent(AttributeComponent->GetHealthPercent());
	}
	CombatTarget = EventInstigator->GetPawn();

	return DamageAmount;

	//Remove this line, if the parent functionality shall not be applied
	//return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}
