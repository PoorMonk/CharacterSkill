// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactive/SCBaseInteractiveActor.h"
#include "SCGateActor.generated.h"

class UStaticMeshComponent;

/**
 * 
 */
UCLASS()
class CHARACTERSKILL_API ASCGateActor : public ASCBaseInteractiveActor
{
	GENERATED_BODY()
	
public:
	ASCGateActor();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* GateArch;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Gate;
};
