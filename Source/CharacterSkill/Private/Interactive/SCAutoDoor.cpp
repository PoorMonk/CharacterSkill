// Fill out your copyright notice in the Description page of Project Settings.


#include "SCAutoDoor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TimelineComponent.h"
#include "Components/SceneComponent.h"
#include "ConstructorHelpers.h"

// Sets default values
ASCAutoDoor::ASCAutoDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	DoorMesh->SetRelativeScale3D(FVector(1.5f, 1.f, 2.f));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> DoorObj(TEXT("StaticMesh'/Game/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Ruins/StaticMesh/SM_Plains_CastleArch_Iron_Gate_01.SM_Plains_CastleArch_Iron_Gate_01'"));
	if (DoorObj.Succeeded())
	{
		DoorMesh->SetStaticMesh(DoorObj.Object);
	}
	DoorMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UCurveFloat> CurveObj(TEXT("CurveFloat'/Game/MatroidVania/Blueprints/Actors/SC/RaiseDoor.RaiseDoor'"));
	if (CurveObj.Succeeded())
	{
		RaiseDoorFloatCurve = CurveObj.Object;
	}
}

void ASCAutoDoor::RaiseDoorProgress(float val)
{
	FVector NewLocation = InitPosition + FVector(0.f, 0.f, val);
	SetActorLocation(NewLocation);
}

void ASCAutoDoor::RaiseDoor()
{
	RaiseDoorTimeline.PlayFromStart();
}

// Called when the game starts or when spawned
void ASCAutoDoor::BeginPlay()
{
	Super::BeginPlay();
	
	InitPosition = GetActorLocation();

	FOnTimelineFloat RaiseDoorTimelineProgress;
	RaiseDoorTimelineProgress.BindUFunction(this, FName("RaiseDoorProgress"));
	RaiseDoorTimeline.AddInterpFloat(RaiseDoorFloatCurve, RaiseDoorTimelineProgress);
}

// Called every frame
void ASCAutoDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RaiseDoorTimeline.TickTimeline(DeltaTime);
}

