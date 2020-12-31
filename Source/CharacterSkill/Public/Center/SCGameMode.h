// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SCGameMode.generated.h"

class ASCAutoDoor;

/**
 * 
 */
UCLASS()
class CHARACTERSKILL_API ASCGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	ASCGameMode();

	void RaiseDoor();

	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door")
	ASCAutoDoor* AutoDoor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door")
	TSubclassOf<ASCAutoDoor> DoorClass;

	FVector DoorPosition;
};
