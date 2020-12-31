// Fill out your copyright notice in the Description page of Project Settings.


#include "SCWMagicTimerWidget.h"
#include "SCPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "TextBlock.h"
#include "ProgressBar.h"

void USCWMagicTimerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SCPlayer = Cast<ASCPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	UpdateName();
}

void USCWMagicTimerWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

}

void USCWMagicTimerWidget::UpdateName()
{
	if (SCPlayer)
	{
		switch (SCPlayer->WMagicSlotted)
		{
		case E_WMagic::EWM_None:
			break;
		case E_WMagic::EWM_LensOfTruth:
			Text_Name->SetText(SCPlayer->LensOfTruthAttributes.MagicName);
			break;
		case E_WMagic::EWM_Mist:
			Text_Name->SetText(SCPlayer->MistAttributes.MagicName);
			break;
		case E_WMagic::EWM_Shield:
			break;
		case E_WMagic::EWM_TimeSlow:
			break;
		default:
			break;
		}
	}
}

void USCWMagicTimerWidget::UpdatePBTimer(float Percent)
{
	if (Percent >= 0.f)
	{
		PB_Timer->SetPercent(Percent);
	}
}
