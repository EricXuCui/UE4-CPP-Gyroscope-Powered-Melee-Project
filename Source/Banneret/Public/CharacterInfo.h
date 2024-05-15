// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "CharacterInfo.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum ESoliderTypes
{
	EST_Knight = 0	UMETA(DisplayName = "Knight"),
	EST_Cavalry = 1 UMETA(DisplayName = "Cavalry"),
	EST_Archer = 2 UMETA(DisplayName = "Archer"),

};
UENUM(BlueprintType)
enum EFaction
{
	EF_Silver = 0 UMETA(DisplayName = "Silver"),
	EF_Red = 1 UMETA(DisplayName = "Red"),
};

UCLASS()
class BANNERET_API UCharacterInfo : public UUserDefinedEnum
{
	GENERATED_BODY()
	
};
