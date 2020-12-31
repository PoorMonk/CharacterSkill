// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Center/SCGameMode.h"
#include "Public/Center/SCHUD.h"
#include "Public/Center/SCPlayerController.h"
#include "ConstructorHelpers.h"
#include "Public/Player/SCPlayer.h"
#include "SCAutoDoor.h"
#include "Engine/World.h"

ASCGameMode::ASCGameMode()
{
	static ConstructorHelpers::FClassFinder<ASCHUD> HudCls(TEXT("Blueprint'/Game/MatroidVania/Blueprints/Framework/BP_SCHUD.BP_SCHUD_C'"));
	if (HudCls.Succeeded())
	{
		HUDClass = HudCls.Class;
	}

	static ConstructorHelpers::FClassFinder<ASCPlayerController> PCCls(TEXT("Blueprint'/Game/MatroidVania/Blueprints/Framework/BP_SCPlayerController.BP_SCPlayerController_C'"));
	if (PCCls.Succeeded())
	{
		PlayerControllerClass = PCCls.Class;
	}

	static ConstructorHelpers::FClassFinder<ASCPlayer> Player(TEXT("/Game/MatroidVania/Blueprints/Characters/BP_SCPlayer"));
	if (Player.Succeeded())
	{
		DefaultPawnClass = Player.Class;
	}

	static ConstructorHelpers::FClassFinder<ASCAutoDoor> DoorCls(TEXT("Blueprint'/Game/MatroidVania/Blueprints/Actors/SC/BP_SCAutoDoor.BP_SCAutoDoor_C'"));
	if (DoorCls.Succeeded())
	{
		DoorClass = DoorCls.Class;
	}

	DoorPosition = FVector(-400.f, 0.f, 50.f);
}

void ASCGameMode::RaiseDoor()
{
	if (AutoDoor)
	{
		AutoDoor->RaiseDoor();
	}
}

void ASCGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (DoorClass)
	{
		AutoDoor = GetWorld()->SpawnActor<ASCAutoDoor>(DoorClass, DoorPosition, FRotator::ZeroRotator);
	}
}
