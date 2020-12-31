// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SCWMagicTimerWidget.generated.h"

class UTextBlock;
class UProgressBar;
class ASCPlayer;

/**
 * 
 */
UCLASS()
class CHARACTERSKILL_API USCWMagicTimerWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void UpdateName();
	void UpdatePBTimer(float Percent);

public:
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* Text_Name;

	UPROPERTY(Meta = (BindWidget))
	UProgressBar* PB_Timer;

	UPROPERTY()
	ASCPlayer* SCPlayer;
};
