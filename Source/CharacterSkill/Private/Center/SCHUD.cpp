// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Center/SCHUD.h"
#include "Public/Widget/SCMainWidget.h"

void ASCHUD::BeginPlay()
{
	Super::BeginPlay();

	if (SCMainWidgetClass)
	{
		SCMainWidget = CreateWidget<USCMainWidget>(GetWorld(), SCMainWidgetClass);
		SCMainWidget->AddToViewport();
	}
}
