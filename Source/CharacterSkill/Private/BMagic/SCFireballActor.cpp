// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/BMagic/SCFireballActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ConstructorHelpers.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "BMagic/SCCameraShake.h"
#include "SCPlayer.h"

// Sets default values
ASCFireballActor::ASCFireballActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	RootComponent = Collision;
	Collision->SetSphereRadius(20.f);
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Collision->SetCollisionObjectType(ECC_WorldDynamic);
	Collision->SetCollisionResponseToAllChannels(ECR_Block);
	Collision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	SM_Fireball = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_Fireball"));
	SM_Fireball->SetRelativeLocation(FVector(25.f, 0.f, 0.f));
	SM_Fireball->SetRelativeScale3D(FVector(2.f));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> fireballObj(TEXT("StaticMesh'/Game/InfinityBladeEffects/Effects/FX_Meshes/Fire/SM_FireBall.SM_FireBall'"));
	if (fireballObj.Succeeded())
	{
		SM_Fireball->SetStaticMesh(fireballObj.Object);
	}

	UMaterial * MaterialPtr = ConstructorHelpers::FObjectFinderOptional<UMaterial>(TEXT("Material'/Game/InfinityBladeEffects/Effects/FX_Materials/Mobile/M_FireBlast_Add_FireBall_MeshEmit.M_FireBlast_Add_FireBall_MeshEmit'")).Get();
	dynamicMaterialInstanceDynamic = UMaterialInstanceDynamic::Create(MaterialPtr, SM_Fireball);
	
	SM_Fireball->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SM_Fireball->SetupAttachment(Collision);

	VFX_Fireball = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("VFX_Fireball"));
	VFX_Fireball->SetRelativeLocation(FVector(5.f, 0.f, 0.f));
	VFX_Fireball->SetRelativeScale3D(FVector(1.5f));

	static ConstructorHelpers::FObjectFinder<UParticleSystem> fireballPS(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Mobile/Fire/combat/P_FireBall_Strong.P_FireBall_Strong'"));
	if (fireballPS.Succeeded())
	{
		VFX_Fireball->SetTemplate(fireballPS.Object);
	}
	VFX_Fireball->SetupAttachment(Collision);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 1500.f;
	ProjectileMovement->MaxSpeed = 1500.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

// Called when the game starts or when spawned
void ASCFireballActor::BeginPlay()
{
	Super::BeginPlay();
	
	SM_Fireball->SetMaterial(0, dynamicMaterialInstanceDynamic);
}

// Called every frame
void ASCFireballActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASCFireballActor::NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	UParticleSystem* HitPS = LoadObject<UParticleSystem>(nullptr, TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Mobile/Fire/combat/P_FireBall_Strong_MuzzFlash_01.P_FireBall_Strong_MuzzFlash_01'"));
	if (HitPS)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitPS, HitLocation);
	}

	USoundCue* ExplosionCue = LoadObject<USoundCue>(nullptr, TEXT("SoundCue'/Game/StarterContent/Audio/Explosion_Cue.Explosion_Cue'"));
	if (ExplosionCue)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionCue, HitLocation);
	}

	UClass* CamSkClass = LoadClass<UCameraShake>(nullptr, TEXT("Blueprint'/Game/MatroidVania/Blueprints/Misc/BP_SCCameraShake.BP_SCCameraShake_C'"));
	if (CamSkClass)
	{
		UGameplayStatics::PlayWorldCameraShake(GetWorld(), CamSkClass, HitLocation, 0.f, 1000.f);
	}
	ASCPlayer* Player = Cast<ASCPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Player)
	{
		UGameplayStatics::ApplyDamage(Other, Player->FireballAttributes.DamageValue, nullptr, nullptr, Player->FireballAttributes.DamageType);
	}

	Destroy();
}

