// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactive/SCSkillBase.h"
#include "SCSkillBMFireball.generated.h"

class UMaterialInstanceDynamic;

/**
 * 
 */
UCLASS()
class CHARACTERSKILL_API ASCSkillBMFireball : public ASCSkillBase
{
	GENERATED_BODY()
	
public:
	ASCSkillBMFireball();

	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	virtual void OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void WidgetAnimEnd();

	void ObjectInteractedWith() override;

protected:
	FTimerHandle DelayHandle;

	UPROPERTY(EditAnywhere)
	UMaterialInstanceDynamic* SkillMeshMI;
};
