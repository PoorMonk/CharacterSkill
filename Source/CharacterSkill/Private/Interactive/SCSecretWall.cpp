// Fill out your copyright notice in the Description page of Project Settings.


#include "SCSecretWall.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ConstructorHelpers.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstanceDynamic.h"

#define ECC_Mist ECC_GameTraceChannel3

ASCSecretWall::ASCSecretWall()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(InteractiveBox);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("StaticMesh'/Engine/EngineMeshes/Cube.Cube'"));
	if (MeshObj.Succeeded())
	{
		Mesh->SetStaticMesh(MeshObj.Object);
	}

	InteractiveBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	InteractiveBox->SetCollisionObjectType(ECC_WorldDynamic);
	InteractiveBox->SetCollisionResponseToAllChannels(ECR_Overlap);
	InteractiveBox->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	InteractiveBox->SetCollisionResponseToChannel(ECC_Mist, ECR_Block);
}

void ASCSecretWall::BeginPlay()
{
	Super::BeginPlay();

	// 材质设置放到构造函数不会生效
	UMaterial* texture = LoadObject<UMaterial>(nullptr, TEXT("Material'/Game/StarterContent/Materials/M_Brick_Clay_Old.M_Brick_Clay_Old'"));
	MeshMat = UMaterialInstanceDynamic::Create(texture, Mesh);
	Mesh->SetMaterial(0, MeshMat);
}
