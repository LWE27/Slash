// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"

class USoundBase;
class UBoxComponent;

/**
 * 
 */
UCLASS()
class SLASH_API AWeapon : public AItem
{
	GENERATED_BODY()
	
public:
	AWeapon();
	
	TArray<AActor*> IgnoreActors;
	
	void AttachMeshToSocket(USceneComponent* InParent, FName InSocketName);
	void Equip(USceneComponent* InParent, const FName InSocketName, AActor* NewOwner, APawn* NewInstigator);
	
	FORCEINLINE UBoxComponent* GetWeaponBox() const { return WeaponBox; }
	
protected:
	
	virtual void BeginPlay() override;
	
	// Sphere Overlap
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	// Calls the Function through Blueprint and handles the rest in Blueprints
	UFUNCTION(BlueprintImplementableEvent)
	void CreateFields(const FVector& FieldLocation);
	
private:
	//Variable for MetaSoundsSource
	UPROPERTY(EditAnywhere, Category= "Weapon Properties")
	USoundBase* EquipSound;
	
	UPROPERTY(EditAnywhere, Category= "Weapon Properties")
	UBoxComponent* WeaponBox;
	
	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceStart;
	
	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceEnd;
	
	UPROPERTY(EditAnywhere)
	float Damage = 20.f;
};
