// Fill out your copyright notice in the Description page of Project Settings.


#include "SCPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Public/Widget/SCSkillAcquiredWidget.h"
#include "SCHUD.h"
#include "SCBMagicMenuWidget.h"
#include "SCWMagicMenuWidget.h"
#include "SCMainWidget.h"
#include "Sound/SoundCue.h"

void ASCPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SCHud = Cast<ASCHUD>(GetHUD());

	UClass* BMMenuCls = LoadClass<USCBMagicMenuWidget>(nullptr, TEXT("WidgetBlueprint'/Game/MatroidVania/Widgets/SC/WBP_SCBMagicMenu.WBP_SCBMagicMenu_C'"));
	if (BMMenuCls)
	{
		SCBMagicMenuClass = BMMenuCls;
	}
	if (SCBMagicMenuClass)
	{
		SCBMagicMenu = CreateWidget<USCBMagicMenuWidget>(GetWorld(), SCBMagicMenuClass);
	}

	UClass* WMMenuCls = LoadClass<USCWMagicMenuWidget>(nullptr, TEXT("WidgetBlueprint'/Game/MatroidVania/Widgets/SC/WBP_SCWMagicMenu.WBP_SCWMagicMenu_C'"));
	if (WMMenuCls)
	{
		SCWMagicMenuClass = WMMenuCls;
	}
	if (SCWMagicMenuClass)
	{
		SCWMagicMenu = CreateWidget<USCWMagicMenuWidget>(GetWorld(), SCWMagicMenuClass);
	}
}

void ASCPlayerController::ShowAcquiredSkill()
{
	UGameplayStatics::SetGamePaused(this, true);
	if (WBP_SCSkillAcquired)
	{
		WBP_SCSkillAcquired->AddToViewport();
	}
}

void ASCPlayerController::SwitchInputMode(bool IsUI)
{
	if (IsUI)
	{
		FInputModeGameAndUI IMGameAndUI;
		IMGameAndUI.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		SetInputMode(IMGameAndUI);

		bShowMouseCursor = true;
	}
	else
	{
		FInputModeGameOnly IMGameOnly;
		IMGameOnly.SetConsumeCaptureMouseDown(true);
		SetInputMode(IMGameOnly);

		bShowMouseCursor = false;
	}
}

void ASCPlayerController::ShowBMagicMenu()
{
	if (SCHud && SCHud->SCMainWidget)
	{
		SCHud->SCMainWidget->RemoveFromParent();
	}
	if (SCBMagicMenu)
	{
		SCBMagicMenu->AddToViewport();
	}
	UGameplayStatics::PlaySound2D(GetWorld(), SFX_MenuON);
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	SwitchInputMode(true);
}

void ASCPlayerController::HideBMagicMenu()
{
	if (SCHud && SCHud->SCMainWidget)
	{
		SCHud->SCMainWidget->AddToViewport();
	}
	if (SCBMagicMenu)
	{
		SCBMagicMenu->RemoveFromParent();
	}
	UGameplayStatics::PlaySound2D(GetWorld(), SFX_MenuOFF);
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	SwitchInputMode(false);
}

void ASCPlayerController::ShowWMagicMenu()
{
	if (SCHud && SCHud->SCMainWidget)
	{
		SCHud->SCMainWidget->RemoveFromParent();
	}
	if (SCWMagicMenu)
	{
		SCWMagicMenu->AddToViewport();
	}
	UGameplayStatics::PlaySound2D(GetWorld(), SFX_MenuON);
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	SwitchInputMode(true);
}

void ASCPlayerController::HideWMagicMenu()
{
	if (SCHud && SCHud->SCMainWidget)
	{
		SCHud->SCMainWidget->AddToViewport();
	}
	if (SCWMagicMenu)
	{
		SCWMagicMenu->RemoveFromParent();
	}
	UGameplayStatics::PlaySound2D(GetWorld(), SFX_MenuOFF);
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	SwitchInputMode(false);
}
