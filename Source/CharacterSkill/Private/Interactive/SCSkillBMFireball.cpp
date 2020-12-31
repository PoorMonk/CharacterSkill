// Fill out your copyright notice in the Description page of Project Settings.


#include "SCSkillBMFireball.h"
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

ASCSkillBMFireball::ASCSkillBMFireball()
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
	static ConstructorHelpers::FObjectFinder<UParticleSystem> SkillVFXObj(TEXT("ParticleSystem'/Game/MatroidVania/Particles/P_BMagic.P_BMagic'"));
	if (SkillVFXObj.Succeeded())
	{
		Skill_VFX->SetTemplate(SkillVFXObj.Object);
	}

	Skill_Light->LightColor = FColor(0.08f, 0.005f, 0.034f);

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

	static ConstructorHelpers::FObjectFinder<UMaterial> texture(TEXT("Material'/Game/MatroidVania/Materials/Decal_Fireball.Decal_Fireball'"));
	Decal_Skill->SetDecalMaterial(texture.Object);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StatueMeshObj(TEXT("StaticMesh'/Game/SoulCave/Environment/Meshes/Building/SM_Statue_Crumble_RK.SM_Statue_Crumble_RK'"));
	if (StatueMeshObj.Succeeded())
	{
		Statue->SetStaticMesh(StatueMeshObj.Object);
	}
	Statue->SetRelativeLocation(FVector(0.f, -150.f, 200.f));
	Statue->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	Statue->SetVisibility(true);

	ReceiveMessage = FText::FromString("Learn Black Magic");
}

void ASCSkillBMFireball::BeginPlay()
{
	Super::BeginPlay();

	InteractiveBox->OnComponentBeginOverlap.AddDynamic(this, &ASCSkillBMFireball::OnOverlapBegin);
	InteractiveBox->OnComponentEndOverlap.AddDynamic(this, &ASCSkillBMFireball::OverlapEnd);

	UMaterialInstanceConstant * SkillMeshMat = LoadObject<UMaterialInstanceConstant>(nullptr, TEXT("MaterialInstanceConstant'/Game/MatroidVania/Materials/MI_BMagic.MI_BMagic'"));
	SkillMeshMI = UMaterialInstanceDynamic::Create(SkillMeshMat, SkillMesh);
	SkillMesh->SetMaterial(0, SkillMeshMI);
}

void ASCSkillBMFireball::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor)
	{
		if (Player)
		{
			Player->bCanInteract = true;
			if (!Player->bHasBMagicFireball && !Player->bIsUsingMist)
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

void ASCSkillBMFireball::OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
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
					GetWorldTimerManager().SetTimer(DelayHandle, this, &ASCSkillBMFireball::WidgetAnimEnd, WidgetAnim->GetEndTime());
				}
			}
		}
	}
}

void ASCSkillBMFireball::WidgetAnimEnd()
{
	WBP_SCNotify->RemoveFromParent();
	Player->bCanInteract = false;
}

void ASCSkillBMFireball::ObjectInteractedWith()
{
	if (!Player->bHasBMagicFireball)
	{
		Player->bHasBMagicFireball = true;
		Player->SkillAcquired = E_Skills::ES_Fireball;
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
