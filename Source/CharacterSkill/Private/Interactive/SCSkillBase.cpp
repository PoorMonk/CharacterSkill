// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Interactive/SCSkillBase.h"
#include "Public/Interactive/SCBaseInteractiveActor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/DecalComponent.h"
#include "Components/PointLightComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "ConstructorHelpers.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/Material.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "Sound/SoundCue.h"
#include "Public/Widget/SCNotifyWidget.h"
#include "Materials/MaterialInstanceConstant.h"

ASCSkillBase::ASCSkillBase()
{
	PrimaryActorTick.bCanEverTick = true;

	Dias = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Dias"));
	Dias->SetRelativeScale3D(FVector(0.6f, 0.3f, 0.8f));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(TEXT("StaticMesh'/Game/InfinityBladeFireLands/Environments/Fire/Fire_Fortress/SM/Env_Fire_MelancholicTroll03.Env_Fire_MelancholicTroll03'"));
	Dias->SetStaticMesh(mesh.Object);
	Dias->SetupAttachment(RootComponent);

	InteractiveBox->SetRelativeLocation(FVector(0.f, 270.f, 95.f));
	InteractiveBox->SetBoxExtent(FVector(100.f, 300.f, 100.f));	
	
	SkillMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SkillMesh"));
	SkillMesh->SetRelativeLocation(FVector(0.f, 0.f, 160.f));
	SkillMesh->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> skillMesh(TEXT("StaticMesh'/Game/MatroidVania/Meshes/SM_MoveSkill.SM_MoveSkill'"));
	SkillMesh->SetStaticMesh(skillMesh.Object);
	SkillMesh->SetupAttachment(RootComponent);

	Skill_VFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Skill_VFX"));
	Skill_VFX->SetRelativeLocation(FVector(0.f, 0.f, 40.f));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> skill_vfx(TEXT("ParticleSystem'/Game/MatroidVania/Particles/P_Skill.P_Skill'"));
	Skill_VFX->SetTemplate(skill_vfx.Object);
	Skill_VFX->SetupAttachment(SkillMesh);

	Skill_Light = CreateDefaultSubobject<UPointLightComponent>(TEXT("Skill_Light"));
	Skill_Light->SetLightColor(FLinearColor(FVector(0.1f, 0.1f, 0.f)));
	Skill_Light->Intensity = 25;
	Skill_Light->SetupAttachment(SkillMesh);

#pragma region Candles
	static ConstructorHelpers::FObjectFinder<UStaticMesh> candleMesh(TEXT("StaticMesh'/Game/InfinityBladeEffects/Effects/FX_Meshes/Mobile/SM_CandleStand_03.SM_CandleStand_03'"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> candleLightMesh(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Ambient/Lighting/P_LightCorona.P_LightCorona'"));

	Candles_01 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Candles_01"));
	Candles_01->SetRelativeLocation(FVector(195.f, 0.f, 0.f));
	Candles_01->SetStaticMesh(candleMesh.Object);
	Candles_01->SetVisibility(false);
	
	Candles_01->SetupAttachment(RootComponent);

	CandleLight_01 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("CandleLight_01"));
	CandleLight_01->SetRelativeLocation(FVector(0.f, 0.f, 220.f));
	CandleLight_01->SetRelativeScale3D(FVector(1.5f, 1.5f, 1.5f));
	CandleLight_01->SetTemplate(candleLightMesh.Object);
	CandleLight_01->SetVisibility(false);
	CandleLight_01->SetAutoActivate(false);
	CandleLight_01->SetupAttachment(Candles_01);

	Candles_02 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Candles_02"));
	Candles_02->SetRelativeLocation(FVector(-195.f, 0.f, 0.f));
	Candles_02->SetStaticMesh(candleMesh.Object);
	Candles_02->SetVisibility(false);
	Candles_02->SetupAttachment(RootComponent);

	CandleLight_02 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("CandleLight_02"));
	CandleLight_02->SetRelativeLocation(FVector(0.f, 0.f, 220.f));
	CandleLight_02->SetRelativeScale3D(FVector(1.5f, 1.5f, 1.5f));
	CandleLight_02->SetTemplate(candleLightMesh.Object);
	CandleLight_02->SetVisibility(false);
	CandleLight_02->SetAutoActivate(false);
	CandleLight_02->SetupAttachment(Candles_02);

	Candles_03 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Candles_03"));
	Candles_03->SetRelativeLocation(FVector(-120.f, -100.f, 0.f));
	Candles_03->SetRelativeScale3D(FVector(1.2f));
	Candles_03->SetStaticMesh(candleMesh.Object);
	Candles_03->SetVisibility(false);
	Candles_03->SetupAttachment(RootComponent);

	CandleLight_03 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("CandleLight_03"));
	CandleLight_03->SetRelativeLocation(FVector(0.f, 0.f, 220.f));
	CandleLight_03->SetRelativeScale3D(FVector(1.5f, 1.5f, 1.5f));
	CandleLight_03->SetTemplate(candleLightMesh.Object);
	CandleLight_03->SetVisibility(false);
	CandleLight_03->SetAutoActivate(false);
	CandleLight_03->SetupAttachment(Candles_03);

	Candles_04 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Candles_04"));
	Candles_04->SetRelativeLocation(FVector(120.f, -100.f, 0.f));
	Candles_04->SetRelativeRotation(FRotator(0.f, -160.f, 0.f));
	Candles_04->SetRelativeScale3D(FVector(1.2f));
	Candles_04->SetStaticMesh(candleMesh.Object);
	Candles_04->SetVisibility(false);
	Candles_04->SetupAttachment(RootComponent);

	CandleLight_04 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("CandleLight_04"));
	CandleLight_04->SetRelativeLocation(FVector(0.f, 0.f, 220.f));
	CandleLight_04->SetRelativeRotation(FRotator(0.f, 50.f, 0.f));
	CandleLight_04->SetRelativeScale3D(FVector(1.5f, 1.5f, 1.5f));
	CandleLight_04->SetTemplate(candleLightMesh.Object);
	CandleLight_04->SetVisibility(false);
	CandleLight_04->SetAutoActivate(false);
	CandleLight_04->SetupAttachment(Candles_04);				 
#pragma endregion

	static ConstructorHelpers::FObjectFinder<UStaticMesh> diasMesh(TEXT("StaticMesh'/Game/InfinityBladeEffects/Effects/FX_Meshes/Mobile/SM_Candle_Drips_3.SM_Candle_Drips_3'"));
	
	Candles_Dias_01 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Candles_Dias_01"));
	Candles_Dias_01->SetRelativeLocation(FVector(-90.f, 24.f, 95.f));
	Candles_Dias_01->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	Candles_Dias_01->SetRelativeScale3D(FVector(3.f));
	Candles_Dias_01->SetStaticMesh(diasMesh.Object);
	Candles_Dias_01->SetVisibility(true);
	Candles_Dias_01->SetupAttachment(RootComponent);

	Candles_Dias_02 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Candles_Dias_02"));
	Candles_Dias_02->SetRelativeLocation(FVector(90.f, 24.f, 95.f));
	Candles_Dias_02->SetRelativeRotation(FRotator(0.f, -40.f, 0.f));
	Candles_Dias_02->SetRelativeScale3D(FVector(3.f));
	Candles_Dias_02->SetStaticMesh(diasMesh.Object);
	Candles_Dias_02->SetVisibility(true);
	Candles_Dias_02->SetupAttachment(RootComponent);

	Decal_Skill = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal_Skill"));
	Decal_Skill->SetRelativeLocation(FVector(0.f, 0.f, 120.f));
	Decal_Skill->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
	Decal_Skill->SetRelativeScale3D(FVector(0.1f));
	Decal_Skill->DecalSize = FVector(256.f, 300.f, 400.f);
	Decal_Skill->SetupAttachment(RootComponent);

	Statue = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Statue"));
	Statue->SetRelativeLocation(FVector(0.f, -120.f, -10.f));
	Statue->SetRelativeScale3D(FVector(1.2f, 1.0f, 0.9f));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StatueMesh(TEXT("StaticMesh'/Game/InfinityBladeIceLands/Environments/Ice/EXO_RockyRuins/StaticMesh/SM_Statue01.SM_Statue01'"));
	Statue->SetStaticMesh(StatueMesh.Object);
	Statue->SetVisibility(false);
	Statue->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<USoundCue> SkillAcquireSC(TEXT("SoundCue'/Game/MatroidVania/Audio/SFX_SkillAcquired_Cue.SFX_SkillAcquired_Cue'"));
	SFX_SkillAcquire = SkillAcquireSC.Object;

	SkillBeginPosition = FVector(0.f, 0.f, 150.f);
	SkillEndPosition = FVector(0.f, 0.f, 200.f);
	InterpSpeed = 0.8f;
}

void ASCSkillBase::BeginPlay()
{
	Super::BeginPlay();

	UMaterial* texture = LoadObject<UMaterial>(nullptr, TEXT("Material'/Game/InfinityBladeFireLands/Environments/Forge/Env_Forge/Materials/M_Candle_glowing.M_Candle_glowing'"));
	CandleMat = UMaterialInstanceDynamic::Create(texture, nullptr);
	Candles_01->SetMaterial(0, CandleMat);
	Candles_02->SetMaterial(0, CandleMat);
	Candles_03->SetMaterial(0, CandleMat);
	Candles_04->SetMaterial(0, CandleMat);

	Candles_Dias_01->SetMaterial(0, CandleMat);
	Candles_Dias_02->SetMaterial(0, CandleMat);
}

void ASCSkillBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (SkillMesh == nullptr)
	{
		return;
	}

	FVector CurrentLocation = SkillMesh->RelativeLocation;
	
	// 使用VInterpTo实现平滑移动
	FVector Interp = FMath::VInterpTo(CurrentLocation, SkillEndPosition, DeltaTime, InterpSpeed);
	
	SkillMesh->SetRelativeLocation(Interp);
	float Distance = (Interp - SkillEndPosition).Size();

	if (Distance < 5.f)
	{
		SwapLocation(SkillBeginPosition, SkillEndPosition);
	}
}

void ASCSkillBase::PlayAcquiredSkillFX()
{
	WBP_SCNotify->RemoveFromParent();
	UParticleSystem* Emit = LoadObject<UParticleSystem>(nullptr, TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Treasure/Resources/P_OrePile_Explosion_Success.P_OrePile_Explosion_Success'"));
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Emit, SkillMesh->GetComponentLocation(), FRotator::ZeroRotator, true);
	UGameplayStatics::PlaySound2D(GetWorld(), SFX_SkillAcquire);
	SkillMesh->DestroyComponent();
	Skill_VFX->DestroyComponent();
	Skill_Light->DestroyComponent();
	InteractiveBox->DestroyComponent();
}

void ASCSkillBase::SwapLocation(FVector& VecStart, FVector& VecEnd)
{
	FVector temp = VecStart;
	VecStart = VecEnd;
	VecEnd = temp;
}

