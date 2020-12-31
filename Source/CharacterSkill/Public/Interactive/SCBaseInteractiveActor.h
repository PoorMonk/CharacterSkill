// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Public/Data/InteractInterface.h"
#include "SCBaseInteractiveActor.generated.h"

class UBoxComponent;
class ASCPlayer;
class USCNotifyWidget;
class USoundCue;

UCLASS()
class CHARACTERSKILL_API ASCBaseInteractiveActor : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASCBaseInteractiveActor();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interactive")
	ASCPlayer* Player;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interactive")
	USCNotifyWidget* WBP_SCNotify;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactive")
	TSubclassOf<USCNotifyWidget> SCNotifyClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactive")
	FText ReceiveMessage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactive")
	USoundCue* SFXInteractive;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* InteractiveBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* SceneComp;
};
