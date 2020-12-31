// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Public/Data/SCDataObject.h"
#include "SCSkillAcquiredWidget.generated.h"

class ASCPlayerController;
class ASCPlayer;
class UTextBlock;
class UImage;
class UBorder;
class UOverlay;

/**
 * 
 */
UCLASS()
class CHARACTERSKILL_API USCSkillAcquiredWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual bool Initialize() override;

	virtual void NativeConstruct() override;

	void UpdateInfo();

	UFUNCTION(BlueprintCallable)
	void OnBackButtonClicked();

	UFUNCTION(BlueprintCallable)
	void OnBackButtonHovered();

	UFUNCTION(BlueprintCallable)
	void OnBackButtonUnHovered();


public:
	UPROPERTY()
	TMap<FName, UWidgetAnimation*> WidgetAnims;

	UPROPERTY()
	ASCPlayerController* SCPlayerController;

	UPROPERTY()
	ASCPlayer* SCPlayer;

protected:
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* Text_SkillLearned;

	UPROPERTY(Meta = (BindWidget))
	UImage* Img_SkillBG;

	UPROPERTY(Meta = (BindWidget))
	UImage* Img_SkillIconLeft;

	UPROPERTY(Meta = (BindWidget))
	UImage* Img_SkillIconRight;

	UPROPERTY(Meta = (BindWidget))
	UBorder* ScreenShot_Border;

	UPROPERTY(Meta = (BindWidget))
	UImage* Img_Screenshot;

	UPROPERTY(Meta = (BindWidget))
	UImage* Img_ButtonInput;
	
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* Text_Description;

	UPROPERTY(Meta = (BindWidget))
	UTextBlock* Text_ManaValue;

	UPROPERTY(Meta = (BindWidget))
	UOverlay* Overlay_Attributes;

private:
	void UpdateUIInfo(const FSTR_SkillData& SkillData);
};
