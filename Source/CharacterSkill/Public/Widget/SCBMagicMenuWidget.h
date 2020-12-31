// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SCBMagicMenuWidget.generated.h"

class ASCPlayer;
class ASCPlayerController;
class UTextBlock;
class UImage;
class UBorder;
class UProgressBar;
class USoundCue;

UENUM(BlueprintType)
enum class E_BMagicSelection : uint8
{
	EBMS_None,
	EBMS_Fireball,
	EBMS_ElectroSpark,
	EBMS_Bloodlust,
	EBMS_ArcticBlast
};

/**
 * 
 */
UCLASS()
class CHARACTERSKILL_API USCBMagicMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

	E_BMagicSelection SelectMagic();
	void UnSelectAllMagicFontEffect();
	void UnSelectMagicFontEffect(UTextBlock* MagicName);
	void SelectMagicFontEffect(UTextBlock* MagicName);

	void UpdateInfo();
	void UpdateFireballInfo();
	void UpdateElectroSparkInfo();
	void UpdateBloodlustInfo();
	void UpdateArcticBlastInfo();
	void UpdateBMagicInfo();
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
	UBorder* Border_BMagic;

	UPROPERTY(Meta = (BindWidget))
	UProgressBar* PB_Mana;

	UPROPERTY(Meta = (BindWidget))
	UImage* Img_BMagic;

	UPROPERTY(Meta = (BindWidget))
	UTextBlock* Text_Mana;

	UPROPERTY(Meta = (BindWidget))
	UTextBlock* Text_BMagic_ManaCost;

	UPROPERTY(Meta = (BindWidget))
	UImage* Img_NoMagicSelected;

	UPROPERTY(Meta = (BindWidget))
	UImage* Img_ElectroSpark;
	UPROPERTY(Meta = (BindWidget))
	UImage* Img_ElectroSparkSelected;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* Text_ElectroSparkName;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* Text_ElectroSparkManaCost;

	UPROPERTY(Meta = (BindWidget))
	UImage* Img_Fireball;
	UPROPERTY(Meta = (BindWidget))
	UImage* Img_FireballSelected;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* Text_FireballName;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* Text_FireballManaCost;

	UPROPERTY(Meta = (BindWidget))
	UImage* Img_Bloodlust;
	UPROPERTY(Meta = (BindWidget))
	UImage* Img_BloodlustSelected;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* Text_BloodlustName;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* Text_BloodlustManaCost;

	UPROPERTY(Meta = (BindWidget))
	UImage* Img_ArcticBlast;
	UPROPERTY(Meta = (BindWidget))
	UImage* Img_ArcticBlastSelected;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* Text_ArcticBlastName;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* Text_ArcticBlastManaCost;
};
