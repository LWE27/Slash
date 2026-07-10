// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
#include "Slash/DebugMacros.h"

AItem::AItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	int32 Average = Avg<int32>(1, 3);
	UE_LOG(LogTemp, Error, TEXT("Average: %d"), Average);

	float AverageFloat = Avg<float>(526.f, 7.34f);
	UE_LOG(LogTemp, Error, TEXT("Average: %f"), AverageFloat);
}

float AItem::TransformedSin()
{
	return Amplitude * FMath::Sin(RunningTime * TimeConstant);
}

float AItem::TransformedCos()
{
	return Amplitude * FMath::Cos(RunningTime * TimeConstant);
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RunningTime += DeltaTime; // Kinda like a stopwatch
	// Amplitude Height .25f & Speed 5.f
	//float DeltaZ = Amplitude * FMath::Sin(RunningTime * TimeConstant);

	//AddActorWorldOffset(FVector(0.0f, 0.0f, DeltaZ));

	DRAW_SPHERE_SingleFrame(GetActorLocation());
	DRAW_VECTOR_SingleFrame(GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 100.f);
}
