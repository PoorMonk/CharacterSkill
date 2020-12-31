// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SCHUD.generated.h"

class USCMainWidget;

/**
 * 
 */
UCLASS()
class CHARACTERSKILL_API ASCHUD : public AHUD
{
	GENERATED_BODY()
	
public:

	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget")
	TSubclassOf<USCMainWidget> SCMainWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget")
	USCMainWidget* SCMainWidget;
};
