// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactive/SCSkillBase.h"
#include "SCSkillDoubleJump.generated.h"

/**
 * 
 */
UCLASS()
class CHARACTERSKILL_API ASCSkillDoubleJump : public ASCSkillBase
{
	GENERATED_BODY()
	
public:

	ASCSkillDoubleJump();
	
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	UFUNCTION()
	virtual void OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void WidgetAnimEnd();

	void ObjectInteractedWith() override;

protected:
	FTimerHandle DelayHandle;
};
