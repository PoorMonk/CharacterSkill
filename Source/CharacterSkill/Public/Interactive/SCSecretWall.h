// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactive/SCBaseInteractiveActor.h"
#include "SCSecretWall.generated.h"

class UStaticMeshComponent;
class UMaterialInstanceDynamic;

/**
 * 
 */
UCLASS()
class CHARACTERSKILL_API ASCSecretWall : public ASCBaseInteractiveActor
{
	GENERATED_BODY()
	
public:
	ASCSecretWall();

	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Materials")
	UMaterialInstanceDynamic* MeshMat;
};
