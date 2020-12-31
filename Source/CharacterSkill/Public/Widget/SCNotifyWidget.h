// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SCNotifyWidget.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class CHARACTERSKILL_API USCNotifyWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;

	void UpdateMessage(FText Message);
	
public:
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* Text_Message;

	UPROPERTY()
	TMap<FName, UWidgetAnimation*> WidgetAnims;
};
