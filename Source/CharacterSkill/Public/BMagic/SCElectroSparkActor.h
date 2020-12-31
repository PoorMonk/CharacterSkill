// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SCElectroSparkActor.generated.h"

class USphereComponent;
class UParticleSystemComponent;

UCLASS()
class CHARACTERSKILL_API ASCElectroSparkActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASCElectroSparkActor();

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* Collision;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Particles")
	UParticleSystemComponent* VFX_StartUp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Particles")
	UParticleSystemComponent* VFX_Lightning_Right_01;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Particles")
	UParticleSystemComponent* VFX_Lightning_Right_02;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Particles")
	UParticleSystemComponent* VFX_Lightning_Left_01;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Particles")
	UParticleSystemComponent* VFX_Lightning_Left_02;
};
