// Fill out your copyright notice in the Description page of Project Settings.


#include "SCGateActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "ConstructorHelpers.h"

#define ECC_Mist ECC_GameTraceChannel3

ASCGateActor::ASCGateActor()
{
	InteractiveBox->SetRelativeLocation(FVector(0.f, 0.f, 300.f));
	InteractiveBox->SetBoxExtent(FVector(100.f, 200.f, 300.f));
	InteractiveBox->SetCollisionResponseToChannel(ECC_Mist, ECR_Ignore);

	GateArch = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GateArch"));
	GateArch->SetupAttachment(RootComponent);
	GateArch->SetRelativeLocation(FVector(160.f, 32.f, 0.f));
	GateArch->SetRelativeScale3D(FVector(1.25f, 1.f, 1.2f));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> GAObj(TEXT("StaticMesh'/Game/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Ruins/StaticMesh/SM_Plains_CastleArch_01.SM_Plains_CastleArch_01'"));
	if (GAObj.Succeeded())
	{
		GateArch->SetStaticMesh(GAObj.Object);
	}

	Gate = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gate"));
	Gate->SetupAttachment(RootComponent);
	Gate->SetRelativeLocation(FVector(0.f, -3.f, 0.f));
	Gate->SetRelativeScale3D(FVector(1.25f, 1.f, 1.1f));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> GObj(TEXT("StaticMesh'/Game/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Ruins/StaticMesh/SM_Plains_CastleArch_Iron_Gate_01.SM_Plains_CastleArch_Iron_Gate_01'"));
	if (GObj.Succeeded())
	{
		Gate->SetStaticMesh(GObj.Object);
	}
	Gate->SetCollisionProfileName(FName("IgnoreMist"));
}
