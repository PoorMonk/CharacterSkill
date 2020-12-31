// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Widget/SCMainWidget.h"
#include "ProgressBar.h"
#include "TextBlock.h"
#include "Image.h"
#include "Public/Player/SCPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/WidgetAnimation.h"

bool USCMainWidget::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	SCPlayer = Cast<ASCPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (SCPlayer)
	{
		UpdateHealth(SCPlayer->GetHealth());
		UpdateMana(SCPlayer->GetMana());
		SCPlayer->ToggleReady.BindUObject(this, &USCMainWidget::ShowReadyText);
	}

	UTexture* EptTex = LoadObject<UTexture>(nullptr, TEXT("Texture2D'/Game/MatroidVania/Textures/T_Magic_Empty.T_Magic_Empty'"));
	if (EptTex)
	{
		MagicEmpty.SetResourceObject(EptTex);
	}

	WidgetAnims.Empty();
	UProperty* Prop = GetClass()->PropertyLink;
	/*遍历所有的UProperty*/
	while (Prop)
	{
		if (Prop->GetClass() == UObjectProperty::StaticClass())
		{
			UObjectProperty* ObjProp = Cast<UObjectProperty>(Prop);
			/*寻找UWidgetAnimation*/
			if (ObjProp->PropertyClass == UWidgetAnimation::StaticClass())
			{
				UObject* Obj = ObjProp->GetObjectPropertyValue_InContainer(this);
				UWidgetAnimation* WidgetAnimation = Cast<UWidgetAnimation>(Obj);
				if (WidgetAnimation && WidgetAnimation->MovieScene)
				{
					/*以动画名为Key存储引用*/
					FName AnimName = WidgetAnimation->MovieScene->GetFName();
					WidgetAnims.Add(AnimName, WidgetAnimation);
				}
			}
		}
		Prop = Prop->PropertyLinkNext;
	}

	return true;
}

void USCMainWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	bShow = true;

	UpdateWMagicInfo();
	UpdateBMagicInfo();
	UpdateCooldownProgressBar(0.f);
	ShowReadyText();
}

void USCMainWidget::UpdateHealth(float HealthValue)
{
	Text_Health->SetText(FText::FromString(FString::SanitizeFloat(HealthValue, 0)));
}

void USCMainWidget::UpdateHealthProgressBar(float HealthRate)
{
	PB_Health->SetPercent(HealthRate);
}

void USCMainWidget::UpdateMana(float ManaValue)
{
	Text_Mana->SetText(FText::FromString(FString::SanitizeFloat(ManaValue, 0)));
}

void USCMainWidget::UpdateManaProgressBar(float ManaRate)
{
	PB_Mana->SetPercent(ManaRate);
}

void USCMainWidget::SetWMagicImg(FSlateBrush WMBrush)
{
	Img_WMagic->SetBrush(WMBrush);
}

void USCMainWidget::SetWMagicManaCost(float WManaCostValue)
{
	Text_WMagic_ManaCost->SetText(FText::FromString(FString::SanitizeFloat(WManaCostValue)));
}

void USCMainWidget::SetBMagicImg(FSlateBrush BMBrush)
{
	Img_BMagic->SetBrush(BMBrush);
}

void USCMainWidget::SetBMagicManaCost(float BManaCostValue)
{
	Text_BMagic_ManaCost->SetText(FText::FromString(FString::SanitizeFloat(BManaCostValue)));
}

void USCMainWidget::UpdateCooldownProgressBar(float Percent)
{
	PB_WMagic_Cooldown->SetPercent(Percent);
}

void USCMainWidget::UpdateBMagicInfo()
{
	if (SCPlayer)
	{
		switch (SCPlayer->BMagicSlotted)
		{
		case E_BMagic::EBM_None:
			Img_BMagic->SetBrush(MagicEmpty);
			Text_BMagic_ManaCost->SetText(FText::FromString(FString::FromInt(0)));
			break;
		case E_BMagic::EBM_Fireball:
			Img_BMagic->SetBrush(SCPlayer->FireballAttributes.IconSlate);
			Text_BMagic_ManaCost->SetText(FText::FromString(FString::FromInt(SCPlayer->FireballAttributes.ManaCost)));
			break;
		case E_BMagic::EBM_ElectroSpark:
			Img_BMagic->SetBrush(SCPlayer->ElectroSparkAttributes.IconSlate);
			Text_BMagic_ManaCost->SetText(FText::FromString(FString::FromInt(SCPlayer->ElectroSparkAttributes.ManaCost)));
			break;
		case E_BMagic::EBM_Bloodlust:
			Img_BMagic->SetBrush(SCPlayer->BloodlustAttributes.IconSlate);
			Text_BMagic_ManaCost->SetText(FText::FromString(FString::FromInt(SCPlayer->BloodlustAttributes.ManaCost)));
			break;
		case E_BMagic::EBM_ArcticBlast:
			Img_BMagic->SetBrush(SCPlayer->ArcticBlastAttributes.IconSlate);
			Text_BMagic_ManaCost->SetText(FText::FromString(FString::FromInt(SCPlayer->ArcticBlastAttributes.ManaCost)));
			break;
		default:
			break;
		}
	}
}

void USCMainWidget::UpdateWMagicInfo()
{
	if (SCPlayer)
	{
		switch (SCPlayer->WMagicSlotted)
		{
		case E_WMagic::EWM_None:
			Img_WMagic->SetBrush(MagicEmpty);
			Text_WMagic_ManaCost->SetText(FText::FromString(FString::FromInt(0)));
			break;
		case E_WMagic::EWM_LensOfTruth:
			Img_WMagic->SetBrush(SCPlayer->LensOfTruthAttributes.IconSlate);
			Text_WMagic_ManaCost->SetText(FText::FromString(FString::FromInt(SCPlayer->LensOfTruthAttributes.ManaCost)));
			break;
		case E_WMagic::EWM_Mist:
			Img_WMagic->SetBrush(SCPlayer->MistAttributes.IconSlate);
			Text_WMagic_ManaCost->SetText(FText::FromString(FString::FromInt(SCPlayer->MistAttributes.ManaCost)));
			break;
		case E_WMagic::EWM_Shield:
			Img_WMagic->SetBrush(SCPlayer->ShieldAttributes.IconSlate);
			Text_WMagic_ManaCost->SetText(FText::FromString(FString::FromInt(SCPlayer->ShieldAttributes.ManaCost)));
			break;
		case E_WMagic::EWM_TimeSlow:
			Img_WMagic->SetBrush(SCPlayer->TimeSlowAttributes.IconSlate);
			Text_WMagic_ManaCost->SetText(FText::FromString(FString::FromInt(SCPlayer->TimeSlowAttributes.ManaCost)));
			break;
		default:
			break;
		}
	}
}

void USCMainWidget::ShowReadyText()
{
	if (!bShow)
	{
		Text_WMagic->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		Text_WMagic->SetVisibility(ESlateVisibility::Visible);
		UWidgetAnimation* ReadyAnim = *WidgetAnims.Find(FName("WMagicReady"));
		if (ReadyAnim)
		{
			PlayAnimation(ReadyAnim);
		}
	}
	bShow = !bShow;
}

