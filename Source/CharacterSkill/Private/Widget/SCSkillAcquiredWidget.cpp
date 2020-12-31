// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Widget/SCSkillAcquiredWidget.h"
#include "WidgetAnimation.h"
#include "SCPlayerController.h"
#include "SCPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "TextBlock.h"
#include "Image.h"
#include "Border.h"
#include "Overlay.h"
#include "SCPlayer.h"

bool USCSkillAcquiredWidget::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	WidgetAnims.Empty();
	UProperty* Prop = GetClass()->PropertyLink;
	/*遍历所有的UProperty*/
	while (Prop)
	{
		if (Prop->GetClass() == UObjectProperty::StaticClass())
		{
			UObjectProperty* ObjProp = Cast<UObjectProperty>(Prop);
			/*寻找我们的UWidgetAnimation*/
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

void USCSkillAcquiredWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UWidgetAnimation* InputPulseAnims = *WidgetAnims.Find(FName("InputPulse"));
	if (InputPulseAnims)
	{
		PlayAnimation(InputPulseAnims, 0.f, 0);
	}

	SCPlayerController = Cast<ASCPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (SCPlayerController)
	{
		SCPlayerController->SwitchInputMode(true);
	}

	SCPlayer = Cast<ASCPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	UpdateInfo();
}

void USCSkillAcquiredWidget::UpdateInfo()
{
	switch (SCPlayer->SkillAcquired)
	{
	case E_Skills::ES_DoubleJump:
		UpdateUIInfo(SCPlayer->DoubleJumpData);
		break;
	case E_Skills::ES_BackDash:
		UpdateUIInfo(SCPlayer->BackDashData);
		break;
	case E_Skills::ES_Fireball:
		UpdateUIInfo(SCPlayer->FireballData);
		Text_ManaValue->SetText(FText::FromString(FString::FromInt(SCPlayer->FireballAttributes.ManaCost)));
		break;
	case E_Skills::ES_ElectroSpark:
		UpdateUIInfo(SCPlayer->ElectroSparkData);
		Text_ManaValue->SetText(FText::FromString(FString::FromInt(SCPlayer->ElectroSparkAttributes.ManaCost)));
		break;
	case E_Skills::ES_LensOfTruth:
		UpdateUIInfo(SCPlayer->LensOfTruthData);
		Text_ManaValue->SetText(FText::FromString(FString::FromInt(SCPlayer->LensOfTruthAttributes.ManaCost)));
		break;
	case E_Skills::ES_Mist:
		UpdateUIInfo(SCPlayer->MistData);
		Text_ManaValue->SetText(FText::FromString(FString::FromInt(SCPlayer->MistAttributes.ManaCost)));
		break;
	default:
		break;
	}
}


void USCSkillAcquiredWidget::UpdateUIInfo(const FSTR_SkillData& SkillData)
{
	Text_SkillLearned->SetText(SkillData.Title);
	Text_SkillLearned->ColorAndOpacity = SkillData.SkillFontColor;
	Img_SkillBG->SetBrush(SkillData.SkillBorderColor);
	ScreenShot_Border->SetBrush(SkillData.SkillBorderColor);
	Img_SkillIconLeft->SetBrush(SkillData.SkillIcon);
	Img_SkillIconRight->SetBrush(SkillData.SkillIcon);
	Img_Screenshot->SetBrush(SkillData.SkillImage);
	Text_Description->SetText(SkillData.Description);
	Overlay_Attributes->SetVisibility(SkillData.MagicSkill ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}


void USCSkillAcquiredWidget::OnBackButtonClicked()
{
	RemoveFromParent();
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	SCPlayerController->SwitchInputMode(false);
}

void USCSkillAcquiredWidget::OnBackButtonHovered()
{
	UWidgetAnimation* BackHoverAnim = *WidgetAnims.Find(FName("BackHover"));
	if (BackHoverAnim)
	{
		PlayAnimation(BackHoverAnim);
	}
}

void USCSkillAcquiredWidget::OnBackButtonUnHovered()
{
	UWidgetAnimation* BackHoverAnim = *WidgetAnims.Find(FName("BackHover"));
	if (BackHoverAnim)
	{
		PlayAnimation(BackHoverAnim, 0.f, 1.f, EUMGSequencePlayMode::Reverse);
	}
}
