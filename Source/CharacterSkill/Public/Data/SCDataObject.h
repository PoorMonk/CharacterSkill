// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SlateBrush.h"
#include "GameFramework/DamageType.h"
#include "SCDataObject.generated.h"


USTRUCT(BlueprintType)
struct FSTR_MagicAttr
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere)
	int32 ManaCost;

	UPROPERTY(EditAnywhere)
	float DamageValue;

	UPROPERTY(EditAnywhere)
	FText MagicName;

	UPROPERTY(EditAnywhere)
	FSlateBrush IconSlate;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UDamageType> DamageType;
};

USTRUCT(BlueprintType)
struct FSTR_SkillData
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere)
	FText Title;

	UPROPERTY(EditAnywhere)
	FText Description;

	UPROPERTY(EditAnywhere)
	FSlateBrush InputButton;

	UPROPERTY(EditAnywhere)
	FSlateBrush SkillIcon;

	UPROPERTY(EditAnywhere)
	FSlateBrush SkillImage;

	UPROPERTY(EditAnywhere)
	FSlateBrush SkillBorderColor;

	UPROPERTY(EditAnywhere)
	FSlateColor SkillFontColor;
	
	UPROPERTY(EditAnywhere)
	bool MagicSkill;
};

UENUM(BlueprintType)
enum class E_BMagic : uint8
{
	EBM_None,
	EBM_Fireball,
	EBM_ElectroSpark,
	EBM_Bloodlust,
	EBM_ArcticBlast
};

UENUM(BlueprintType)
enum class E_WMagic : uint8
{
	EWM_None,
	EWM_LensOfTruth,
	EWM_Mist,
	EWM_Shield,
	EWM_TimeSlow
};

UENUM(BlueprintType)
enum class E_Skills : uint8
{
	ES_None,
	ES_DoubleJump,
	ES_BackDash,
	ES_Fireball,
	ES_ElectroSpark,
	ES_LensOfTruth,
	ES_Mist
};

/**
 * 
 */
UCLASS()
class CHARACTERSKILL_API USCDataObject : public UObject
{
	GENERATED_BODY()
	
};
