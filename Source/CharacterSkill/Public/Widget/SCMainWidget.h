// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SCMainWidget.generated.h"

class UProgressBar;
class UTextBlock;
class UImage;
class UTexture2D;
class ASCPlayer;
class UWidgetAnimation;

/**
 * 
 */
UCLASS()
class CHARACTERSKILL_API USCMainWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;

	void UpdateHealth(float HealthValue);
	void UpdateHealthProgressBar(float HealthRate);
	void UpdateMana(float ManaValue);
	void UpdateManaProgressBar(float ManaRate);
	void SetWMagicImg(FSlateBrush WMBrush);
	void SetWMagicManaCost(float WManaCostValue);
	void SetBMagicImg(FSlateBrush BMBrush);
	void SetBMagicManaCost(float BManaCostValue);

	void UpdateCooldownProgressBar(float Percent);

	void UpdateBMagicInfo();
	void UpdateWMagicInfo();

	UFUNCTION()
	void ShowReadyText();

protected:
	UPROPERTY()
	ASCPlayer* SCPlayer;

	UPROPERTY()
	TMap<FName, UWidgetAnimation*> WidgetAnims;

	FSlateBrush MagicEmpty;

	bool bShow;

#pragma region Widget
	UPROPERTY(Meta = (BindWidget))
	UProgressBar* PB_Health;

	UPROPERTY(Meta = (BindWidget))
	UTextBlock* Text_Health;

	UPROPERTY(Meta = (BindWidget))
	UProgressBar* PB_Mana;

	UPROPERTY(Meta = (BindWidget))
	UTextBlock* Text_Mana;

	UPROPERTY(Meta = (BindWidget))
	UProgressBar* PB_WMagic_Cooldown;

	UPROPERTY(Meta = (BindWidget))
	UTextBlock* Text_WMagic_ManaCost;

	UPROPERTY(Meta = (BindWidget))
	UTextBlock* Text_BMagic_ManaCost;

	UPROPERTY(Meta = (BindWidget))
	UImage* Img_WMagic;

	UPROPERTY(Meta = (BindWidget))
	UImage* Img_BMagic;

	UPROPERTY(Meta = (BindWidget))
	UTextBlock* Text_WMagic;
#pragma endregion
};
