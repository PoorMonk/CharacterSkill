// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Interactive/SCBaseInteractiveActor.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SCPlayer.h"
#include "SCNotifyWidget.h"
#include "ConstructorHelpers.h"
#include "Sound/SoundCue.h"

// Sets default values
ASCBaseInteractiveActor::ASCBaseInteractiveActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	RootComponent = SceneComp;

	InteractiveBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractiveBox"));
	InteractiveBox->SetupAttachment(RootComponent);
	InteractiveBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractiveBox->SetCollisionObjectType(ECC_WorldDynamic);
	InteractiveBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	InteractiveBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block); 

	static ConstructorHelpers::FObjectFinder<USoundCue> sc(TEXT("SoundCue'/Engine/VREditor/Sounds/UI/Floating_UI_Close_Cue.Floating_UI_Close_Cue'"));
	if (sc.Succeeded())
	{
		SFXInteractive = sc.Object;
	}
	static ConstructorHelpers::FClassFinder<USCNotifyWidget> NotifyClass(TEXT("WidgetBlueprint'/Game/MatroidVania/Widgets/SC/WBP_SCNotify.WBP_SCNotify_C'"));
	if (NotifyClass.Succeeded())
	{
		SCNotifyClass = NotifyClass.Class;
	}
}

// Called when the game starts or when spawned
void ASCBaseInteractiveActor::BeginPlay()
{
	Super::BeginPlay();
	
	Player = Cast<ASCPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (SCNotifyClass)
	{
		WBP_SCNotify = CreateWidget<USCNotifyWidget>(GetWorld(), SCNotifyClass);
	}
}



