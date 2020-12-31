// Fill out your copyright notice in the Description page of Project Settings.


#include "SCSecretPlatform.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ConstructorHelpers.h"

ASCSecretPlatform::ASCSecretPlatform()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(InteractiveBox);
	Mesh->SetRelativeLocation(FVector(-100.f, 0.f, 0.f));
	Mesh->SetRelativeScale3D(FVector(0.2f, 0.2f, 0.1f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("StaticMesh'/Game/InfinityBladeFireLands/Environments/Fire/Env_Fire_Rocky/StaticMesh/SM_Env_Fire_Cliffs_CliffWall_03c.SM_Env_Fire_Cliffs_CliffWall_03c'"));
	if (MeshObj.Succeeded())
	{
		Mesh->SetStaticMesh(MeshObj.Object);
	}
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetVisibility(false);

	InteractiveBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
