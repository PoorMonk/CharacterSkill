// Fill out your copyright notice in the Description page of Project Settings.


#include "SCEenmy.h"
#include "SCDamageFire.h"
#include "ConstructorHelpers.h"
#include "SCDamageLightning.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SCPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "SCGameMode.h"

#define ECC_Enemy ECC_GameTraceChannel2

// Sets default values
ASCEenmy::ASCEenmy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<USCDamageFire> VulDmgCls(TEXT("Blueprint'/Game/MatroidVania/Blueprints/DamageTypes/BP_DamageFire.BP_DamageFire_C'"));
	if (VulDmgCls.Succeeded())
	{
		DamageVulnerableClass = VulDmgCls.Class;
	}

	static ConstructorHelpers::FClassFinder<USCDamageLightning> ResistDmgCls(TEXT("Blueprint'/Game/MatroidVania/Blueprints/DamageTypes/BP_DamageLightning.BP_DamageLightning_C'"));
	if (ResistDmgCls.Succeeded())
	{
		DamageResistantClass = ResistDmgCls.Class;
	}

	VulnerableMultiplier = 1.5f;
	ResistantMultiplier = 0.5f;
	Health = 300.f;
	IncomingBaseDamage = 0.0f;
	AttackDamage = 5.f;
	bCanHit = true;
	HitTimer = 0.f;

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetCollisionObjectType(ECC_Enemy);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);

	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshObj(TEXT("SkeletalMesh'/Game/ParagonCountess/Characters/Heroes/Countess/Skins/Tier2/Shogun/Meshes/SM_Countess_Shogun.SM_Countess_Shogun'"));
	if (MeshObj.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshObj.Object);
	}
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimCls(TEXT("AnimBlueprint'/Game/MatroidVania/Animations/Countess_AnimBP.Countess_AnimBP_C'"));
	if (AnimCls.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimCls.Class);
	}

	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.f, 1.f, 0.f)); // 固定不能横向移动

}

// Called when the game starts or when spawned
void ASCEenmy::BeginPlay()
{
	Super::BeginPlay();
	
	SCGameMode = Cast<ASCGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
}

// Called every frame
void ASCEenmy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bCanHit)
	{
		HitTimer += DeltaTime;
		if (HitTimer >= 0.5f)
		{
			bCanHit = true;
			HitTimer = 0.f;
		}
	}
}

void ASCEenmy::NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	if (Other)
	{
		ASCPlayer* Player = Cast<ASCPlayer>(Other);
		if (Player)
		{
			if (bCanHit)
			{
				UGameplayStatics::ApplyDamage(Player, AttackDamage, nullptr, nullptr, nullptr);
			}
		}
	}
}

float ASCEenmy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	UParticleSystem* VulnerablePS = LoadObject<UParticleSystem>(nullptr, TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Monsters/FX_Monster_Gruntling/FlameThrower/P_FlameExplode.P_FlameExplode'"));
	UParticleSystem* ResistantPS = LoadObject<UParticleSystem>(nullptr, TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Mobile/Lightning/P_LineToPoint_Blast_Lightning_00.P_LineToPoint_Blast_Lightning_00'"));
	UParticleSystem* NormalPS = LoadObject<UParticleSystem>(nullptr, TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));
	FVector PSLocation = GetMesh()->GetComponentLocation();

	E_DmgType = CalcuteDamage(DamageAmount, DamageEvent.DamageTypeClass);
	switch (E_DmgType)
	{
	case E_TakeDamage::ETD_ResistantDamaged:
		if (ResistantPS)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ResistantPS, PSLocation, FRotator::ZeroRotator, FVector(0.75f));
		}
		break;
	case E_TakeDamage::ETD_ResistantDeath:
		SCGameMode->RaiseDoor();
		if (ResistantPS)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ResistantPS, PSLocation, FRotator::ZeroRotator, FVector(1.5f));
			Destroy();
		}
		break;
	case E_TakeDamage::ETD_VulnerableDamaged:
		if (VulnerablePS)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), VulnerablePS, PSLocation, FRotator::ZeroRotator);
		}
		break;
	case E_TakeDamage::ETD_VulnerableDeath:
		SCGameMode->RaiseDoor();
		if (VulnerablePS)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), VulnerablePS, PSLocation, FRotator::ZeroRotator, FVector(2.0f));
			Destroy();
		}
		break;
	case E_TakeDamage::ETD_NormalDamaged:
		if (NormalPS)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), NormalPS, PSLocation, FRotator::ZeroRotator);
		}
		break;
	case E_TakeDamage::ETD_NormalDeath:
		SCGameMode->RaiseDoor();
		if (NormalPS)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), NormalPS, PSLocation, FRotator::ZeroRotator, FVector(2.0f));
			Destroy();
		}
		break;
	default:
		break;
	}
	return DamageAmount;
}

E_TakeDamage ASCEenmy::CalcuteDamage(float DamageAmount, TSubclassOf<UDamageType> DamageType)
{
	IncomingBaseDamage = DamageAmount;
	if (DamageType == DamageVulnerableClass)
	{
		Health -= IncomingBaseDamage * VulnerableMultiplier;
		if (Health <= 0.f)
		{
			return E_TakeDamage::ETD_VulnerableDeath;
		}
		else
		{
			return E_TakeDamage::ETD_VulnerableDamaged;
		}
	}
	else if (DamageType == DamageResistantClass)
	{
		Health -= IncomingBaseDamage * ResistantMultiplier;
		if (Health <= 0.f)
		{
			return E_TakeDamage::ETD_ResistantDeath;
		}
		else
		{
			return E_TakeDamage::ETD_ResistantDamaged;
		}
	}
	else
	{
		Health -= IncomingBaseDamage;
		if (Health <= 0.f)
		{
			return E_TakeDamage::ETD_NormalDeath;
		}
		else
		{
			return E_TakeDamage::ETD_NormalDamaged;
		}
	}
}

// Called to bind functionality to input
void ASCEenmy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

