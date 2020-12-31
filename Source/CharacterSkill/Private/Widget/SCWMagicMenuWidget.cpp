// Fill out your copyright notice in the Description page of Project Settings.


#include "SCWMagicMenuWidget.h"
#include "SCPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "TextBlock.h"
#include "Image.h"
#include "Border.h"
#include "Overlay.h"
#include "SCPlayer.h"
#include "ProgressBar.h"
#include "Engine/Texture.h"
#include "Sound/SoundCue.h"
#include "Materials/Material.h"
#include "Engine/Font.h"
#include "SCPlayerController.h"

bool USCWMagicMenuWidget::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	UTexture* EptTex = LoadObject<UTexture>(nullptr, TEXT("Texture2D'/Game/MatroidVania/Textures/T_Magic_Empty.T_Magic_Empty'"));
	if (EptTex)
	{
		MagicEmpty.SetResourceObject(EptTex);
	}

	USoundCue* PlaySC = LoadObject<USoundCue>(nullptr, TEXT("SoundCue'/Engine/VREditor/Sounds/VR_grab_Cue.VR_grab_Cue'"));
	if (PlaySC)
	{
		SelectSC = PlaySC;
	}

	bSelectionMade = false;
	bPlaySound = false;

	MagicNames.Add(Text_LensOfTruthName);
	MagicNames.Add(Text_MistName);
	MagicNames.Add(Text_ShieldName);
	MagicNames.Add(Text_TimeSlowName);

	return true;
}

void USCWMagicMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SCPlayer = Cast<ASCPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	SCPlayerCtl = Cast<ASCPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	UpdateInfo();
}

void USCWMagicMenuWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);

	bPlaySound = false;

	switch (SelectMagic())
	{
	case E_WMagicSelection::EWMS_None:
		Img_LensOfTruthSelected->SetVisibility(ESlateVisibility::Hidden);
		Img_MistSelected->SetVisibility(ESlateVisibility::Hidden);
		Img_ShieldSelected->SetVisibility(ESlateVisibility::Hidden);
		Img_TimeSlowSelected->SetVisibility(ESlateVisibility::Hidden);
		Img_NoMagicSelected->SetVisibility(ESlateVisibility::Visible);
		UnSelectAllMagicFontEffect();
		SCPlayer->WMagicSlotted = E_WMagic::EWM_None;
		bPlaySound = false;
		break;
	case E_WMagicSelection::EWMS_LensOfTruth:
		Img_NoMagicSelected->SetVisibility(ESlateVisibility::Hidden);
		Img_LensOfTruthSelected->SetVisibility(ESlateVisibility::Visible);
		SelectMagicFontEffect(Text_LensOfTruthName);
		SCPlayer->WMagicSlotted = E_WMagic::EWM_LensOfTruth;
		break;
	case E_WMagicSelection::EWMS_Mist:
		Img_NoMagicSelected->SetVisibility(ESlateVisibility::Hidden);
		Img_MistSelected->SetVisibility(ESlateVisibility::Visible);
		SelectMagicFontEffect(Text_MistName);
		SCPlayer->WMagicSlotted = E_WMagic::EWM_Mist;
		break;
	case E_WMagicSelection::EWMS_Shield:
		Img_NoMagicSelected->SetVisibility(ESlateVisibility::Hidden);
		Img_ShieldSelected->SetVisibility(ESlateVisibility::Visible);
		SelectMagicFontEffect(Text_ShieldName);
		SCPlayer->WMagicSlotted = E_WMagic::EWM_Shield;
		break;
	case E_WMagicSelection::EWMS_TimeSlow:
		Img_NoMagicSelected->SetVisibility(ESlateVisibility::Hidden);
		Img_TimeSlowSelected->SetVisibility(ESlateVisibility::Visible);
		SelectMagicFontEffect(Text_TimeSlowName);
		SCPlayer->WMagicSlotted = E_WMagic::EWM_TimeSlow;
		break;
	default:
		break;
	}

	if (bPlaySound && SelectSC)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SelectSC);
		UpdateWMagicInfo();
		bPlaySound = false;
	}
}

E_WMagicSelection USCWMagicMenuWidget::SelectMagic()
{
	if (SCPlayerCtl->IsInputKeyDown(FKey("D")))
	{
		if (!bSelectionMade)
		{
			bPlaySound = true;
			bSelectionMade = true;
		}
		if (SCPlayer->bHasWMagicLensOfTruth)
		{
			return E_WMagicSelection::EWMS_LensOfTruth;
		}
	}
	else if (SCPlayerCtl->IsInputKeyDown(FKey("W")))
	{
		if (!bSelectionMade)
		{
			bPlaySound = true;
			bSelectionMade = true;
		}
		if (SCPlayer->bHasWMagicShield)
		{
			return E_WMagicSelection::EWMS_Shield;
		}
	}
	else if (SCPlayerCtl->IsInputKeyDown(FKey("A")))
	{
		if (!bSelectionMade)
		{
			bPlaySound = true;
			bSelectionMade = true;
		}
		if (SCPlayer->bHasWMagicMist)
		{
			return E_WMagicSelection::EWMS_Mist;
		}
	}
	else if (SCPlayerCtl->IsInputKeyDown(FKey("S")))
	{
		if (!bSelectionMade)
		{
			bPlaySound = true;
			bSelectionMade = true;
		}
		if (SCPlayer->bHasWMagicTimeSlow)
		{
			return E_WMagicSelection::EWMS_TimeSlow;
		}
	}
	else
	{
		bSelectionMade = false;
	}
	return E_WMagicSelection::EWMS_None;
}

void USCWMagicMenuWidget::UnSelectAllMagicFontEffect()
{
	for (auto font : MagicNames)
	{
		FFontOutlineSettings FontOS;
		FontOS.OutlineSize = 2;

		FSlateFontInfo SlateFI;
		UFont* Font = LoadObject<UFont>(nullptr, TEXT("Font'/Game/MatroidVania/Fonts/FT_Gothic_Letters_Font.FT_Gothic_Letters_Font'"));
		if (Font)
		{
			SlateFI = FSlateFontInfo(Font, 35, NAME_None, FontOS);
		}
		font->SetFont(SlateFI);
	}
}

void USCWMagicMenuWidget::SelectMagicFontEffect(UTextBlock* MagicName)
{
	FFontOutlineSettings FontOS;
	FontOS.OutlineSize = 7;
	UMaterial* OutlineMat = LoadObject<UMaterial>(nullptr, TEXT("Material'/Game/MatroidVania/Materials/M_TextHighlight.M_TextHighlight'"));
	if (OutlineMat)
	{
		FontOS.OutlineMaterial = OutlineMat;
	}

	FSlateFontInfo SlateFI;
	UFont* Font = LoadObject<UFont>(nullptr, TEXT("Font'/Game/MatroidVania/Fonts/FT_Gothic_Letters_Font.FT_Gothic_Letters_Font'"));
	if (Font)
	{
		SlateFI = FSlateFontInfo(Font, 50, NAME_None, FontOS);
	}
	MagicName->SetFont(SlateFI);
}

void USCWMagicMenuWidget::UpdateInfo()
{
	if (SCPlayer)
	{
		UpdateLensInfo();
		UpdateMistInfo();
		UpdateShieldInfo();
		UpdateTimeSlowInfo();
		UpdateWMagicInfo();
		UpdateManaInfo();
	}
}

void USCWMagicMenuWidget::UpdateLensInfo()
{
	if (SCPlayer->bHasWMagicLensOfTruth)
	{
		Text_LensOfTruthName->SetVisibility(ESlateVisibility::Visible);
		Text_LensOfTruthManaCost->SetVisibility(ESlateVisibility::Visible);
		SetMagicName(Text_LensOfTruthName, SCPlayer->LensOfTruthAttributes.MagicName);
		SetMagicImage(Img_LensOfTruth, SCPlayer->LensOfTruthAttributes.IconSlate);
		SetMagicManaCost(Text_LensOfTruthManaCost, FText::FromString(FString::FromInt(SCPlayer->LensOfTruthAttributes.ManaCost)));
	}
	else
	{
		Text_LensOfTruthName->SetVisibility(ESlateVisibility::Hidden);
		Text_LensOfTruthManaCost->SetVisibility(ESlateVisibility::Hidden);
		SetMagicImage(Img_LensOfTruth, MagicEmpty);
	}
}

void USCWMagicMenuWidget::UpdateMistInfo()
{
	if (SCPlayer->bHasWMagicMist)
	{
		Text_MistName->SetVisibility(ESlateVisibility::Visible);
		Text_MistManaCost->SetVisibility(ESlateVisibility::Visible);
		SetMagicName(Text_MistName, SCPlayer->MistAttributes.MagicName);
		SetMagicImage(Img_Mist, SCPlayer->MistAttributes.IconSlate);
		SetMagicManaCost(Text_MistManaCost, FText::FromString(FString::FromInt(SCPlayer->MistAttributes.ManaCost)));
	}
	else
	{
		Text_MistName->SetVisibility(ESlateVisibility::Hidden);
		Text_MistManaCost->SetVisibility(ESlateVisibility::Hidden);
		SetMagicImage(Img_Mist, MagicEmpty);
	}
}

void USCWMagicMenuWidget::UpdateShieldInfo()
{
	if (SCPlayer->bHasWMagicShield)
	{
		Text_ShieldName->SetVisibility(ESlateVisibility::Visible);
		Text_ShieldManaCost->SetVisibility(ESlateVisibility::Visible);
		SetMagicName(Text_ShieldName, SCPlayer->ShieldAttributes.MagicName);
		SetMagicImage(Img_Shield, SCPlayer->ShieldAttributes.IconSlate);
		SetMagicManaCost(Text_ShieldManaCost, FText::FromString(FString::FromInt(SCPlayer->ShieldAttributes.ManaCost)));
	}
	else
	{
		Text_ShieldName->SetVisibility(ESlateVisibility::Hidden);
		Text_ShieldManaCost->SetVisibility(ESlateVisibility::Hidden);
		SetMagicImage(Img_Shield, MagicEmpty);
	}
}

void USCWMagicMenuWidget::UpdateTimeSlowInfo()
{
	if (SCPlayer->bHasWMagicTimeSlow)
	{
		Text_TimeSlowName->SetVisibility(ESlateVisibility::Visible);
		Text_TimeSlowManaCost->SetVisibility(ESlateVisibility::Visible);
		SetMagicName(Text_TimeSlowName, SCPlayer->TimeSlowAttributes.MagicName);
		SetMagicImage(Img_TimeSlow, SCPlayer->TimeSlowAttributes.IconSlate);
		SetMagicManaCost(Text_TimeSlowManaCost, FText::FromString(FString::FromInt(SCPlayer->TimeSlowAttributes.ManaCost)));
	}
	else
	{
		Text_TimeSlowName->SetVisibility(ESlateVisibility::Hidden);
		Text_TimeSlowManaCost->SetVisibility(ESlateVisibility::Hidden);
		SetMagicImage(Img_TimeSlow, MagicEmpty);
	}
}

void USCWMagicMenuWidget::UpdateWMagicInfo()
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

void USCWMagicMenuWidget::UpdateManaInfo()
{
	Text_Mana->SetText(FText::FromString(FString::FromInt(FMath::RoundToInt(SCPlayer->GetMana()))));
	PB_Mana->SetPercent(SCPlayer->GetMana() / SCPlayer->GetMaxMana());
}

void USCWMagicMenuWidget::SetMagicName(UTextBlock* TextName, FText Name)
{
	if (TextName)
	{
		TextName->SetText(Name);
	}
}

void USCWMagicMenuWidget::SetMagicImage(UImage* ImgName, FSlateBrush ImgBrush)
{
	if (ImgName)
	{
		ImgName->SetBrush(ImgBrush);
	}
}

void USCWMagicMenuWidget::SetMagicManaCost(UTextBlock* TextName, FText Mana)
{
	if (TextName)
	{
		TextName->SetText(Mana);
	}
}
