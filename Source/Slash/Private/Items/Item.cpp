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
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// 50 cm/s
	float MovementSpeed = 50.0f;
	// 45°/s
	float RotationSpeed = 45.0f;
	
	//Every frame 1 gets added to actors x value
	AddActorWorldOffset(FVector(MovementSpeed, 0.f, 0.f) * DeltaTime);
	AddActorWorldRotation(FRotator(0.f, RotationSpeed, 0.f) * DeltaTime);
	DRAW_SPHERE_SingleFrame(GetActorLocation());
	DRAW_VECTOR_SingleFrame(GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 100.f);
}
