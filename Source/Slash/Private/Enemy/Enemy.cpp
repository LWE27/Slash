// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"

#include "Components/CapsuleComponent.h"
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
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
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
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemy::GetHit(const FVector& ImpactLocation)
{
	DRAW_SPHERE_COLOR(ImpactLocation, FColor::Blue); // Own Macro

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
	}else if (Theta >= -135.f && Theta < -45.f)
	{
		Section = FName("FromLeft");
	}else if (Theta >= 45.f && Theta < 135.f)
	{
		Section = FName("FromRight");
	}
	PlayHitReactMontage(Section);
	
	
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
}
