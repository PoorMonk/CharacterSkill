// Fill out your copyright notice in the Description page of Project Settings.


#include "SCSkillMist.h"
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
#include "TimerManager.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/PointLightComponent.h"
#include "SCPlayerController.h"
#include "SCSkillAcquiredWidget.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInstanceConstant.h"

ASCSkillMist::ASCSkillMist()
{
	Dias->SetRelativeScale3D(FVector(1.5f, 0.3f, 0.8f));

	SkillMesh->SetRelativeScale3D(FVector(0.4));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SkillMeshObj(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (SkillMeshObj.Succeeded())
	{
		SkillMesh->SetStaticMesh(SkillMeshObj.Object);
	}

	Skill_VFX->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
	Skill_VFX->SetRelativeScale3D(FVector(3.f));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> SkillVFXObj(TEXT("ParticleSystem'/Game/MatroidVania/Particles/P_WMagic.P_WMagic'"));
	if (SkillVFXObj.Succeeded())
	{
		Skill_VFX->SetTemplate(SkillVFXObj.Object);
	}

	Skill_Light->LightColor = FColor(0.7f, 0.7f, 1.f);

	Candles_01->SetVisibility(true);
	Candles_02->SetVisibility(true);
	Candles_03->SetVisibility(true);
	Candles_04->SetVisibility(true);

	CandleLight_01->bAutoActivate = true;
	CandleLight_02->bAutoActivate = true;
	CandleLight_03->bAutoActivate = true;
	CandleLight_04->bAutoActivate = true;

	Candles_Dias_01->SetVisibility(true);
	Candles_Dias_02->SetVisibility(true);

	static ConstructorHelpers::FObjectFinder<UMaterial> texture(TEXT("Material'/Game/MatroidVania/Materials/Decal_Mist.Decal_Mist'"));
	Decal_Skill->SetDecalMaterial(texture.Object);	

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StatueMeshObj(TEXT("StaticMesh'/Game/InfinityBladeIceLands/Environments/Ice/EXO_RockyRuins/StaticMesh/SM_Statue01.SM_Statue01'"));
	if (StatueMeshObj.Succeeded())
	{
		Statue->SetStaticMesh(StatueMeshObj.Object);
	}
	Statue->SetRelativeLocation(FVector(0.f, -120.f, -10.f));
	Statue->SetRelativeScale3D(FVector(1.2f, 1.f, 0.9f));
	Statue->SetVisibility(true);

	ReceiveMessage = FText::FromString("Learn White Magic");

	UMaterialInstanceConstant * SkillMeshMat = ConstructorHelpers::FObjectFinderOptional<UMaterialInstanceConstant>(TEXT("MaterialInstanceConstant'/Game/MatroidVania/Materials/MI_WMagic.MI_WMagic'")).Get();
	SkillMeshMI = UMaterialInstanceDynamic::Create(SkillMeshMat, SkillMesh);
}

void ASCSkillMist::BeginPlay()
{
	Super::BeginPlay();

	InteractiveBox->OnComponentBeginOverlap.AddDynamic(this, &ASCSkillMist::OnOverlapBegin);
	InteractiveBox->OnComponentEndOverlap.AddDynamic(this, &ASCSkillMist::OverlapEnd);

	UMaterialInstanceConstant * SkillMeshMat = LoadObject<UMaterialInstanceConstant>(nullptr, TEXT("MaterialInstanceConstant'/Game/MatroidVania/Materials/MI_WMagic.MI_WMagic'"));
	SkillMeshMI = UMaterialInstanceDynamic::Create(SkillMeshMat, SkillMesh);
	SkillMesh->SetMaterial(0, SkillMeshMI);
}

void ASCSkillMist::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor)
	{
		if (Player)
		{
			Player->bCanInteract = true;
			if (!Player->bHasWMagicMist && !Player->bIsUsingMist)
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

void ASCSkillMist::OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
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
					GetWorldTimerManager().SetTimer(DelayHandle, this, &ASCSkillMist::WidgetAnimEnd, WidgetAnim->GetEndTime());
				}
			}
		}
	}
}

void ASCSkillMist::WidgetAnimEnd()
{
	WBP_SCNotify->RemoveFromParent();
	Player->bCanInteract = false;
}

void ASCSkillMist::ObjectInteractedWith()
{
	if (!Player->bHasWMagicMist)
	{
		Player->bHasWMagicMist = true;
		Player->SkillAcquired = E_Skills::ES_Mist;
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
