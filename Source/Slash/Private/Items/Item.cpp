// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"

AItem::AItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	// Logging to Output-Log
	UE_LOG(LogTemp, Warning, TEXT("BeginPlay"));

	//Log to Screen
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Red, FString("Screen Message!"));
	}
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	UE_LOG(LogTemp, Warning, TEXT("Ticket - DeltaTime: %f"), DeltaTime);
	
	if (GEngine)
	{
		FString Name = GetName();
		FString Message = FString::Printf(TEXT("DeltaTime: %s"), *Name);
		GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Red, Message);
	}
}
