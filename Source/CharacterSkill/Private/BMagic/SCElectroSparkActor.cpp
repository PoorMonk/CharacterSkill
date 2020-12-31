// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/BMagic/SCElectroSparkActor.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "ConstructorHelpers.h"
#include "SCPlayer.h"
#include "Kismet/GameplayStatics.h"

#define ECC_Enemy ECC_GameTraceChannel2

// Sets default values
ASCElectroSparkActor::ASCElectroSparkActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	RootComponent = Collision;
	Collision->SetSphereRadius(500.f);
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Collision->SetCollisionResponseToAllChannels(ECR_Block);
	Collision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	Collision->SetCollisionResponseToChannel(ECC_Enemy, ECR_Overlap);
	
	VFX_StartUp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("VFX_StartUp"));
	VFX_StartUp->SetupAttachment(RootComponent);
	VFX_StartUp->SetRelativeScale3D(FVector(3.f));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> StartUpPS(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Mobile/Fire/combat/P_MovingSpin_Fire_Charge_01.P_MovingSpin_Fire_Charge_01'"));
	if (StartUpPS.Succeeded())
	{
		VFX_StartUp->SetTemplate(StartUpPS.Object);
	}

	VFX_Lightning_Right_01 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("VFX_Lightning_Right_01"));
	VFX_Lightning_Right_01->SetupAttachment(RootComponent);
	VFX_Lightning_Right_01->SetRelativeRotation(FRotator(0.f, 0.f, 90.f));
	VFX_Lightning_Right_01->SetRelativeScale3D(FVector(2.f));
	VFX_Lightning_Right_01->CustomTimeDilation = 2.f;

	VFX_Lightning_Right_02 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("VFX_Lightning_Right_02"));
	VFX_Lightning_Right_02->SetupAttachment(RootComponent);
	VFX_Lightning_Right_02->SetRelativeRotation(FRotator(0.f, 0.f, 90.f));
	VFX_Lightning_Right_02->SetRelativeScale3D(FVector(1.5f));
	VFX_Lightning_Right_02->CustomTimeDilation = 1.f;

	VFX_Lightning_Left_01 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("VFX_Lightning_Left_01"));
	VFX_Lightning_Left_01->SetupAttachment(RootComponent);
	VFX_Lightning_Left_01->SetRelativeRotation(FRotator(0.f, 0.f, -90.f));
	VFX_Lightning_Left_01->SetRelativeScale3D(FVector(2.f));
	VFX_Lightning_Left_01->CustomTimeDilation = 2.f;

	VFX_Lightning_Left_02 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("VFX_Lightning_Left_02"));
	VFX_Lightning_Left_02->SetupAttachment(RootComponent);
	VFX_Lightning_Left_02->SetRelativeRotation(FRotator(0.f, 0.f, -90.f));
	VFX_Lightning_Left_02->SetRelativeScale3D(FVector(1.5f));
	VFX_Lightning_Left_02->CustomTimeDilation = 1.f;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> LightningPS(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Skill_Whirlwind/P_Whirlwind_Lightning_Veng_01.P_Whirlwind_Lightning_Veng_01'"));
	if (LightningPS.Succeeded())
	{
		VFX_Lightning_Right_01->SetTemplate(LightningPS.Object);
		VFX_Lightning_Right_02->SetTemplate(LightningPS.Object);
		VFX_Lightning_Left_01->SetTemplate(LightningPS.Object);
		VFX_Lightning_Left_02->SetTemplate(LightningPS.Object);
	}
}

// Called when the game starts or when spawned
void ASCElectroSparkActor::BeginPlay()
{
	Super::BeginPlay();

	Collision->OnComponentBeginOverlap.AddDynamic(this, &ASCElectroSparkActor::OnOverlapBegin);
}

void ASCElectroSparkActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor)
	{
		ASCPlayer* Player = Cast<ASCPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (Player)
		{
			UGameplayStatics::ApplyDamage(OtherActor, Player->ElectroSparkAttributes.DamageValue, nullptr, Player, Player->ElectroSparkAttributes.DamageType);
		}
	}
}

