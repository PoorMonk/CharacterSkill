// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactive/SCBaseInteractiveActor.h"
#include "SCSkillBase.generated.h"

class UStaticMeshComponent;
class UParticleSystemComponent;
class UPointLightComponent;
class UDecalComponent;
class USoundCue;

/**
 * 
 */
UCLASS()
class CHARACTERSKILL_API ASCSkillBase : public ASCBaseInteractiveActor
{
	GENERATED_BODY()
	
public:
	ASCSkillBase();

	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

	// 播放技能学习特效
	void PlayAcquiredSkillFX();		

private:

	void SwapLocation(FVector& VecStart, FVector& VecEnd);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Dias;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* SkillMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UParticleSystemComponent* Skill_VFX;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UPointLightComponent* Skill_Light;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* Candles_01;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UParticleSystemComponent* CandleLight_01;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* Candles_02;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UParticleSystemComponent* CandleLight_02;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* Candles_03;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UParticleSystemComponent* CandleLight_03;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* Candles_04;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UParticleSystemComponent* CandleLight_04;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Candles_Dias_01;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Candles_Dias_02;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UDecalComponent* Decal_Skill;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Statue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UMaterialInstanceDynamic* CandleMat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundCue* SFX_SkillAcquire;

	UPROPERTY(EditAnywhere, meta = (MakeEditWidget = true), Category = "Floating")
	FVector SkillBeginPosition;
	UPROPERTY(EditAnywhere, meta = (MakeEditWidget = true), Category = "Floating")
	FVector SkillEndPosition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floating")
	float InterpSpeed;

};
