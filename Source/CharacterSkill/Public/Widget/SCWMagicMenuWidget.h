// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SCWMagicMenuWidget.generated.h"

class ASCPlayer;
class ASCPlayerController;
class UTextBlock;
class UImage;
class UBorder;
class UProgressBar;
class USoundCue;

UENUM(BlueprintType)
enum class E_WMagicSelection : uint8
{
	EWMS_None,
	EWMS_LensOfTruth,
	EWMS_Mist,
	EWMS_Shield,
	EWMS_TimeSlow
};

/**
 * 
 */
UCLASS()
class CHARACTERSKILL_API USCWMagicMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

	E_WMagicSelection SelectMagic();
	void UnSelectAllMagicFontEffect();
	//void UnSelectMagicFontEffect(UTextBlock* MagicName);
	void SelectMagicFontEffect(UTextBlock* MagicName);

	void UpdateInfo();
	void UpdateLensInfo();
	void UpdateMistInfo();
	void UpdateShieldInfo();
	void UpdateTimeSlowInfo();
	void UpdateWMagicInfo();
	void UpdateManaInfo();

	void SetMagicName(UTextBlock* TextName, FText Name);
	void SetMagicImage(UImage* ImgName, FSlateBrush ImgBrush);
	void SetMagicManaCost(UTextBlock* TextName, FText Mana);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	ASCPlayer* SCPlayer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	ASCPlayerController* SCPlayerCtl;

	FSlateBrush MagicEmpty;

	bool bSelectionMade;
	bool bPlaySound;

	UPROPERTY()
	USoundCue* SelectSC;

	UPROPERTY()
	TArray<UTextBlock*> MagicNames;

	UPROPERTY(Meta = (BindWidget))
	UImage* Img_WMagic;

	UPROPERTY(Meta = (BindWidget))
	UProgressBar* PB_Mana;

	UPROPERTY(Meta = (BindWidget))
	UTextBlock* Text_Mana;

	UPROPERTY(Meta = (BindWidget))
	UTextBlock* Text_WMagic_ManaCost;

	UPROPERTY(Meta = (BindWidget))
	UImage* Img_NoMagicSelected;

	UPROPERTY(Meta = (BindWidget))
	UImage* Img_Mist;
	UPROPERTY(Meta = (BindWidget))
	UImage* Img_MistSelected;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* Text_MistName;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* Text_MistManaCost;

	UPROPERTY(Meta = (BindWidget))
	UImage* Img_LensOfTruth;
	UPROPERTY(Meta = (BindWidget))
	UImage* Img_LensOfTruthSelected;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* Text_LensOfTruthName;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* Text_LensOfTruthManaCost;

	UPROPERTY(Meta = (BindWidget))
	UImage* Img_Shield;
	UPROPERTY(Meta = (BindWidget))
	UImage* Img_ShieldSelected;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* Text_ShieldName;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* Text_ShieldManaCost;

	UPROPERTY(Meta = (BindWidget))
	UImage* Img_TimeSlow;
	UPROPERTY(Meta = (BindWidget))
	UImage* Img_TimeSlowSelected;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* Text_TimeSlowName;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* Text_TimeSlowManaCost;
};
