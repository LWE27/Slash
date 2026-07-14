#pragma once

UENUM(BlueprintType) // Reflection-System und als BlueprintTyp zur Verfügung stellen
enum class ECharacterState : uint8
{
	ECS_Unequipped UMETA(DisplayName="Unequipped"),
	ECS_EquippedOneHandedWeapon UMETA(DisplayName="Equipped One-Handed Weapon"),
	ECS_EquippedTwoHandedWeapon UMETA(DisplayName="Equipped Two-Handed Weapon")
};