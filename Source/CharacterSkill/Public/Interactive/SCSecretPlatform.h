// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactive/SCBaseInteractiveActor.h"
#include "SCSecretPlatform.generated.h"

class UStaticMeshComponent;

/**
 * 
 */
UCLASS()
class CHARACTERSKILL_API ASCSecretPlatform : public ASCBaseInteractiveActor
{
	GENERATED_BODY()
	
public:
	ASCSecretPlatform();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Mesh;
};
