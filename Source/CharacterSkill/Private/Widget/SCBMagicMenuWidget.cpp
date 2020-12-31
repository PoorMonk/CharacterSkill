// Fill out your copyright notice in the Description page of Project Settings.


#include "SCBMagicMenuWidget.h"
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

bool USCBMagicMenuWidget::Initialize()
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

	MagicNames.Add(Text_ArcticBlastName);
	MagicNames.Add(Text_FireballName);
	MagicNames.Add(Text_ElectroSparkName);
	MagicNames.Add(Text_BloodlustName);

	return true;
}

void USCBMagicMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SCPlayer = Cast<ASCPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	SCPlayerCtl = Cast<ASCPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	UpdateInfo();
}

void USCBMagicMenuWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);

	bPlaySound = false;

	switch (SelectMagic())
	{
	case E_BMagicSelection::EBMS_None:
		Img_FireballSelected->SetVisibility(ESlateVisibility::Hidden);
		Img_ElectroSparkSelected->SetVisibility(ESlateVisibility::Hidden);
		Img_BloodlustSelected->SetVisibility(ESlateVisibility::Hidden);
		Img_ArcticBlastSelected->SetVisibility(ESlateVisibility::Hidden);
		Img_NoMagicSelected->SetVisibility(ESlateVisibility::Visible);
		UnSelectAllMagicFontEffect();
		SCPlayer->BMagicSlotted = E_BMagic::EBM_None;
		bPlaySound = false;
		break;
	case E_BMagicSelection::EBMS_Fireball:
		Img_NoMagicSelected->SetVisibility(ESlateVisibility::Hidden);
		Img_FireballSelected->SetVisibility(ESlateVisibility::Visible);
		SelectMagicFontEffect(Text_FireballName);
		SCPlayer->BMagicSlotted = E_BMagic::EBM_Fireball;
		break;
	case E_BMagicSelection::EBMS_ElectroSpark:
		Img_NoMagicSelected->SetVisibility(ESlateVisibility::Hidden);
		Img_ElectroSparkSelected->SetVisibility(ESlateVisibility::Visible);
		SelectMagicFontEffect(Text_ElectroSparkName);
		SCPlayer->BMagicSlotted = E_BMagic::EBM_ElectroSpark;
		break;
	case E_BMagicSelection::EBMS_Bloodlust:
		Img_NoMagicSelected->SetVisibility(ESlateVisibility::Hidden);
		Img_BloodlustSelected->SetVisibility(ESlateVisibility::Visible);
		SelectMagicFontEffect(Text_BloodlustName);
		SCPlayer->BMagicSlotted = E_BMagic::EBM_Bloodlust;	
		break;
	case E_BMagicSelection::EBMS_ArcticBlast:
		Img_NoMagicSelected->SetVisibility(ESlateVisibility::Hidden);
		Img_ArcticBlastSelected->SetVisibility(ESlateVisibility::Visible);
		SelectMagicFontEffect(Text_ArcticBlastName);
		SCPlayer->BMagicSlotted = E_BMagic::EBM_ArcticBlast;
		break;
	default:
		break;
	}

	if (bPlaySound && SelectSC)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SelectSC);
		UpdateBMagicInfo();
		bPlaySound = false;
	}
}

E_BMagicSelection USCBMagicMenuWidget::SelectMagic()
{
	if (SCPlayerCtl->IsInputKeyDown(FKey("D")))
	{
		if (!bSelectionMade)
		{
			bPlaySound = true;
			bSelectionMade = true;
		}
		if (SCPlayer->bHasBMagicFireball)
		{
			return E_BMagicSelection::EBMS_Fireball;
		}
	}
	else if (SCPlayerCtl->IsInputKeyDown(FKey("W")))
	{
		if (!bSelectionMade)
		{
			bPlaySound = true;
			bSelectionMade = true;
		}
		if (SCPlayer->bHasBMagicBloodlust)
		{
			return E_BMagicSelection::EBMS_Bloodlust;
		}
	}
	else if (SCPlayerCtl->IsInputKeyDown(FKey("A")))
	{
		if (!bSelectionMade)
		{
			bPlaySound = true;
			bSelectionMade = true;
		}
		if (SCPlayer->bHasBMagicElectroSpark)
		{
			return E_BMagicSelection::EBMS_ElectroSpark;
		}
	}
	else if (SCPlayerCtl->IsInputKeyDown(FKey("S")))
	{
		if (!bSelectionMade)
		{
			bPlaySound = true;
			bSelectionMade = true;
		}
		if (SCPlayer->bHasBMagicArcticBlast)
		{
			return E_BMagicSelection::EBMS_ArcticBlast;
		}
	}
	else
	{
		bSelectionMade = false;
	}
	return E_BMagicSelection::EBMS_None;
}

void USCBMagicMenuWidget::UnSelectAllMagicFontEffect()
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

void USCBMagicMenuWidget::SelectMagicFontEffect(UTextBlock* MagicName)
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

void USCBMagicMenuWidget::UpdateInfo()
{
	if (SCPlayer)
	{
		UpdateFireballInfo();
		UpdateElectroSparkInfo();
		UpdateBloodlustInfo();
		UpdateArcticBlastInfo();
		UpdateBMagicInfo();
		UpdateManaInfo();
	}
}

void USCBMagicMenuWidget::UpdateFireballInfo()
{
	if (SCPlayer->bHasBMagicFireball)
	{
		Text_FireballName->SetVisibility(ESlateVisibility::Visible);
		Text_FireballManaCost->SetVisibility(ESlateVisibility::Visible);
		SetMagicName(Text_FireballName, SCPlayer->FireballAttributes.MagicName);
		SetMagicImage(Img_Fireball, SCPlayer->FireballAttributes.IconSlate);
		SetMagicManaCost(Text_FireballManaCost, FText::FromString(FString::FromInt(SCPlayer->FireballAttributes.ManaCost)));
	}
	else
	{
		Text_FireballName->SetVisibility(ESlateVisibility::Hidden);
		Text_FireballManaCost->SetVisibility(ESlateVisibility::Hidden);
		SetMagicImage(Img_Fireball, MagicEmpty);
	}
}

void USCBMagicMenuWidget::UpdateElectroSparkInfo()
{
	if (SCPlayer->bHasBMagicElectroSpark)
	{
		Text_ElectroSparkName->SetVisibility(ESlateVisibility::Visible);
		Text_ElectroSparkManaCost->SetVisibility(ESlateVisibility::Visible);
		SetMagicName(Text_ElectroSparkName, SCPlayer->ElectroSparkAttributes.MagicName);
		SetMagicImage(Img_ElectroSpark, SCPlayer->ElectroSparkAttributes.IconSlate);
		SetMagicManaCost(Text_ElectroSparkManaCost, FText::FromString(FString::FromInt(SCPlayer->ElectroSparkAttributes.ManaCost)));
	}
	else
	{
		Text_ElectroSparkName->SetVisibility(ESlateVisibility::Hidden);
		Text_ElectroSparkManaCost->SetVisibility(ESlateVisibility::Hidden);
		SetMagicImage(Img_ElectroSpark, MagicEmpty);
	}
}

void USCBMagicMenuWidget::UpdateBloodlustInfo()
{
	if (SCPlayer->bHasBMagicBloodlust)
	{
		Text_BloodlustName->SetVisibility(ESlateVisibility::Visible);
		Text_BloodlustManaCost->SetVisibility(ESlateVisibility::Visible);
		SetMagicName(Text_BloodlustName, SCPlayer->BloodlustAttributes.MagicName);
		SetMagicImage(Img_Bloodlust, SCPlayer->BloodlustAttributes.IconSlate);
		SetMagicManaCost(Text_BloodlustManaCost, FText::FromString(FString::FromInt(SCPlayer->BloodlustAttributes.ManaCost)));
	}
	else
	{
		Text_BloodlustName->SetVisibility(ESlateVisibility::Hidden);
		Text_BloodlustManaCost->SetVisibility(ESlateVisibility::Hidden);
		SetMagicImage(Img_Bloodlust, MagicEmpty);
	}
}

void USCBMagicMenuWidget::UpdateArcticBlastInfo()
{
	if (SCPlayer->bHasBMagicArcticBlast)
	{
		Text_ArcticBlastName->SetVisibility(ESlateVisibility::Visible);
		Text_ArcticBlastManaCost->SetVisibility(ESlateVisibility::Visible);
		SetMagicName(Text_ArcticBlastName, SCPlayer->ArcticBlastAttributes.MagicName);
		SetMagicImage(Img_ArcticBlast, SCPlayer->ArcticBlastAttributes.IconSlate);
		SetMagicManaCost(Text_ArcticBlastManaCost, FText::FromString(FString::FromInt(SCPlayer->ArcticBlastAttributes.ManaCost)));
	}
	else
	{
		Text_ArcticBlastName->SetVisibility(ESlateVisibility::Hidden);
		Text_ArcticBlastManaCost->SetVisibility(ESlateVisibility::Hidden);
		SetMagicImage(Img_ArcticBlast, MagicEmpty);
	}
}

void USCBMagicMenuWidget::UpdateBMagicInfo()
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

void USCBMagicMenuWidget::UpdateManaInfo()
{
	Text_Mana->SetText(FText::FromString(FString::FromInt(FMath::RoundToInt(SCPlayer->GetMana()))));
	PB_Mana->SetPercent(SCPlayer->GetMana() / SCPlayer->GetMaxMana());
}

void USCBMagicMenuWidget::SetMagicName(UTextBlock* TextName, FText Name)
{
	if (TextName)
	{
		TextName->SetText(Name);
	}
}

void USCBMagicMenuWidget::SetMagicImage(UImage* ImgName, FSlateBrush ImgBrush)
{
	if (ImgName)
	{
		ImgName->SetBrush(ImgBrush);
	}
}

void USCBMagicMenuWidget::SetMagicManaCost(UTextBlock* TextName, FText Mana)
{
	if (TextName)
	{
		TextName->SetText(Mana);
	}
}

void USCBMagicMenuWidget::UnSelectMagicFontEffect(UTextBlock* MagicName)
{
	FFontOutlineSettings FontOS;
	FontOS.OutlineSize = 2;
	UMaterial* OutlineMat = LoadObject<UMaterial>(nullptr, TEXT("Material'/Game/MatroidVania/Materials/M_TextHighlight.M_TextHighlight'"));
	if (OutlineMat)
	{
		FontOS.OutlineMaterial = OutlineMat;
	}

	FSlateFontInfo SlateFI;
	UFont* Font = LoadObject<UFont>(nullptr, TEXT("Font'/Game/MatroidVania/Fonts/FT_Gothic_Letters_Font.FT_Gothic_Letters_Font'"));
	if (Font)
	{
		SlateFI = FSlateFontInfo(Font, 35, NAME_None, FontOS);
	}
	MagicName->SetFont(SlateFI);
}
