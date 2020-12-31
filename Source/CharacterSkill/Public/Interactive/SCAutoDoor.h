// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "SCAutoDoor.generated.h"

class UStaticMeshComponent;
struct FTimeline;

UCLASS()
class CHARACTERSKILL_API ASCAutoDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASCAutoDoor();

	UFUNCTION()
	void RaiseDoorProgress(float val);

	void RaiseDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* DoorMesh;

	UPROPERTY()
	FTimeline RaiseDoorTimeline;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Timeline")
	UCurveFloat* RaiseDoorFloatCurve;

	FVector InitPosition;
};
