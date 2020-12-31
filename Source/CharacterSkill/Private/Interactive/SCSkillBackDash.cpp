// Fill out your copyright notice in the Description page of Project Settings.


#include "SCSkillBackDash.h"
#include "Public/Interactive/SCSkillBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/DecalComponent.h"
#include "ConstructorHelpers.h"
#include "Materials/Material.h"
#include "Components/BoxComponent.h"
#include "Public/Player/SCPlayer.h"
#include "Public/Widget/SCNotifyWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/PointLightComponent.h"
#include "SCPlayerController.h"
#include "SCSkillAcquiredWidget.h"
#include "TimerManager.h"

ASCSkillBackDash::ASCSkillBackDash()
{
	Candles_03->SetVisibility(true);
	Candles_04->SetVisibility(true);

	static ConstructorHelpers::FObjectFinder<UMaterial> texture(TEXT("Material'/Game/MatroidVania/Materials/Decal_BackDash.Decal_BackDash'"));
	Decal_Skill->SetDecalMaterial(texture.Object);

	ReceiveMessage = FText::FromString("Learn Skill");
}

void ASCSkillBackDash::BeginPlay()
{
	Super::BeginPlay();

	InteractiveBox->OnComponentBeginOverlap.AddDynamic(this, &ASCSkillBackDash::OnOverlapBegin);
	InteractiveBox->OnComponentEndOverlap.AddDynamic(this, &ASCSkillBackDash::OverlapEnd);
}

void ASCSkillBackDash::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor)
	{
		if (Player)
		{
			Player->bCanInteract = true;
			if (!Player->bHasBackDash && !Player->bIsUsingMist)
			{
				if (!WBP_SCNotify->IsInViewport())
				{
					WBP_SCNotify->UpdateMessage(ReceiveMessage);
					WBP_SCNotify->AddToViewport();
				}
				if (WBP_SCNotify->WidgetAnims.Contains(FName("Intro")))
				{
					WBP_SCNotify->PlayAnimation(WBP_SCNotify->WidgetAnims[FName("Intro")]);
				}
				UGameplayStatics::PlaySound2D(this, SFXInteractive);
			}
		}
	}
}

void ASCSkillBackDash::OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		if (Player)
		{
			if (WBP_SCNotify->WidgetAnims.Contains(FName("Intro")))
			{
				UWidgetAnimation* WidgetAnim = WBP_SCNotify->WidgetAnims[FName("Intro")];
				if (WidgetAnim)
				{
					WBP_SCNotify->PlayAnimation(WidgetAnim, 0.f, 1, EUMGSequencePlayMode::Reverse);
					GetWorldTimerManager().SetTimer(DelayHandle, this, &ASCSkillBackDash::WidgetAnimEnd, WidgetAnim->GetEndTime());
				}
			}
		}
	}
}

void ASCSkillBackDash::ObjectInteractedWith()
{
	if (!Player->bHasBackDash)
	{
		Player->bHasBackDash = true;
		Player->SkillAcquired = E_Skills::ES_BackDash;
		PlayAcquiredSkillFX();
		ASCPlayerController* SCPC = Cast<ASCPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (SCPC)
		{
			if (SCPC->SCSkillAcquiredClass)
			{
				SCPC->WBP_SCSkillAcquired = CreateWidget<USCSkillAcquiredWidget>(GetWorld(), SCPC->SCSkillAcquiredClass);
			}
			SCPC->ShowAcquiredSkill();
		}
	}
}

void ASCSkillBackDash::WidgetAnimEnd()
{
	WBP_SCNotify->RemoveFromParent();
	Player->bCanInteract = false;
}
