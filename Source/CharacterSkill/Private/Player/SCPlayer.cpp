// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Player/SCPlayer.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Public/Center/SCGameMode.h"
#include "Public/Center/SCHUD.h"
#include "Public/Widget/SCMainWidget.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/AudioComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TimelineComponent.h"
#include "InteractInterface.h"
#include "Sound/SoundCue.h"
#include "SCPlayerController.h"
#include "TimerManager.h"
#include "BMagic/SCFireballActor.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "BMagic/SCCameraShake.h"
#include "BMagic/SCElectroSparkActor.h"
#include "Engine/Engine.h"
#include "SCWMagicTimerWidget.h"
#include "SCSecretWall.h"
#include "SCSecretPlatform.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Curves/CurveFloat.h"

#define ECC_Mist ECC_GameTraceChannel3

// Sets default values
ASCPlayer::ASCPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;

	// 跳跃键按下的时间会影响跳跃的高度
	JumpMaxHoldTime = 0.2f;
	// 连续跳跃次数
	JumpMaxCount = 2;

	// 设置人物骨骼和位置、旋转
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> mesh(TEXT("SkeletalMesh'/Game/ParagonCountess/Characters/Heroes/Countess/Meshes/SM_Countess.SM_Countess'"));
	GetMesh()->SetSkeletalMesh(mesh.Object);
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -95.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	// 跳跃的时候粒子特效生成的位置
	JumpVFXPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("JumpVFXPoint"));
	//JumpVFXPoint->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	JumpVFXPoint->SetupAttachment(GetMesh());

	// 弹簧臂属性设置
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bAbsoluteRotation = true;	//把相机固定在观察视角，不会跟随人物转向变化
	SpringArm->SetWorldRotation(FRotator(0.f, -90.f, 0.f));
	SpringArm->SocketOffset = FVector(0.f, 0.f, 75.f);
	SpringArm->TargetArmLength = 700.0f;
	SpringArm->bDoCollisionTest = false;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArm);

	// 发射火球时的初始位置
	FireballSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("FireballSpawnPoint"));
	FireballSpawnPoint->SetRelativeLocation(FVector(70.f, 0.f, 20.f));
	FireballSpawnPoint->SetupAttachment(RootComponent);

	WMagicWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("WMagicWidget"));
	WMagicWidget->SetupAttachment(RootComponent);
	WMagicWidget->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
	WMagicWidget->SetWidgetSpace(EWidgetSpace::Screen);
	WMagicWidget->SetVisibility(false);

	SFX_WMagic_LensActive = CreateDefaultSubobject<UAudioComponent>(TEXT("SFX_WMagic_LensActive"));
	SFX_WMagic_LensActive->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<USoundBase> LensAct(TEXT("SoundWave'/Game/MatroidVania/Audio/WMagic_LensActive.WMagic_LensActive'"));
	if (LensAct.Succeeded())
	{
		SFX_WMagic_LensActive->Sound = LensAct.Object;
		SFX_WMagic_LensActive->VolumeMultiplier = 2.f;
	}
	SFX_WMagic_LensActive->bAutoActivate = false;

	VFX_WMagic_MistActive = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("VFX_WMagic_MistActive"));
	VFX_WMagic_MistActive->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UParticleSystem> MistPS(TEXT("ParticleSystem'/Game/MatroidVania/Particles/P_Mist.P_Mist'"));
	if (MistPS.Succeeded())
	{
		VFX_WMagic_MistActive->SetTemplate(MistPS.Object);
	}
	VFX_WMagic_MistActive->SetVisibility(false);

	SFX_WMagic_MistActive = CreateDefaultSubobject<UAudioComponent>(TEXT("SFX_WMagic_MistActive"));
	SFX_WMagic_MistActive->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<USoundBase> MistAct(TEXT("SoundWave'/Game/MatroidVania/Audio/WMagic_MistActive.WMagic_MistActive'"));
	if (MistAct.Succeeded())
	{
		SFX_WMagic_MistActive->Sound = MistAct.Object;
		SFX_WMagic_MistActive->VolumeMultiplier = 2.f;
	}
	SFX_WMagic_MistActive->bAutoActivate = false;

	// 移动时的属性设置
	GetCharacterMovement()->GravityScale = 1.7f;
	GetCharacterMovement()->GroundFriction = 10.0f;
	GetCharacterMovement()->MaxWalkSpeed = 700.f;
	GetCharacterMovement()->JumpZVelocity = 900.f;
	GetCharacterMovement()->AirControl = 0.75;
	GetCharacterMovement()->RotationRate = FRotator(0.f, -1.f, 0.f); // 人物转向的速度
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.f, 1.f, 0.f)); // 固定不能横向移动

	MistPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MistPawnMovement"));
	MistPawnMovement->MaxSpeed = 300.f;
	MistPawnMovement->bAutoActivate = false;

	InitTimeline();

	// 初始化属性
	InitInfo();

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void ASCPlayer::UpdateHealth(float DamageValue)
{
	HealthCurrent = FMath::Clamp(HealthCurrent - DamageValue, 0.f, HealthMax);
	HealthPercentage = HealthCurrent / HealthMax;

	// 更新UI
	MainWidget->UpdateHealth(HealthCurrent);
	MainWidget->UpdateHealthProgressBar(HealthPercentage);
}

void ASCPlayer::UpdateMana(float ManaCostValue)
{
	ManaCurrent = FMath::Clamp(ManaCurrent - ManaCostValue, 0.f, ManaMax);
	ManaPercentage = ManaCurrent / ManaMax;

	MainWidget->UpdateMana(ManaCurrent);
	MainWidget->UpdateManaProgressBar(ManaPercentage);
}

void ASCPlayer::ElectroSparkON()
{
	if (GetCharacterMovement()->IsFalling())
	{
		MistPawnMovement->Activate();
		GetCharacterMovement()->Deactivate();
	}
	DisableInput(SCPlayerCtl);
}

void ASCPlayer::ElectroSparkOFF()
{
	EnableInput(SCPlayerCtl);
	MistPawnMovement->Deactivate();
	GetCharacterMovement()->Activate();
}

void ASCPlayer::OnLearnSkillBtnClicked()
{
	FVector StartPosition = GetCapsuleComponent()->GetComponentLocation();
	FVector EndPosition = StartPosition + GetCapsuleComponent()->GetForwardVector() * InteractTraceLength;
	
	FCollisionQueryParams TranceParams(FName(TEXT("Interact")));
	
	FHitResult hit(ForceInit);
	if (GetWorld()->LineTraceSingleByChannel(hit, StartPosition, EndPosition, ECC_Visibility))
	{
		bCanInteract = true;
		if (hit.Actor.IsValid())
		{			
			// 技能学习
			Cast<IInteractInterface>(hit.Actor)->ObjectInteractedWith();
		}
	}
}

void ASCPlayer::OnBackDashBtnClicked()
{
	if (bHasBackDash && bBackDashingReady && !GetCharacterMovement()->IsFalling())
	{
		bBackDashingReady = false;
		bIsBackDashing = true;
		
		UParticleSystem* BackDashPS = LoadObject<UParticleSystem>(nullptr, TEXT("ParticleSystem'/Game/MatroidVania/Particles/P_Backdash.P_Backdash'"));
		if (BackDashPS)
		{
			if (GetCapsuleComponent()->GetComponentRotation().Yaw >= 0.f)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BackDashPS, GetCapsuleComponent()->GetComponentLocation());
			}
			else
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BackDashPS, GetCapsuleComponent()->GetComponentLocation(), FRotator(0.f, 180.f, 0.f));
			}
		}
		USoundCue* BackDashSC = LoadObject<USoundCue>(nullptr, TEXT("SoundCue'/Game/MatroidVania/Audio/SFX_BackDash_Cue.SFX_BackDash_Cue'"));
		if (BackDashSC)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), BackDashSC, GetCapsuleComponent()->GetComponentLocation());
		}	
	}
}

void ASCPlayer::OnBlackMagicBtnClicked()
{
	switch (BMagicSlotted)
	{
	case E_BMagic::EBM_None:
		break;
	case E_BMagic::EBM_Fireball:
		UseFireball();
		break;
	case E_BMagic::EBM_ElectroSpark:
		UseElectroSpark();
		break;
	case E_BMagic::EBM_Bloodlust:
		break;
	case E_BMagic::EBM_ArcticBlast:
		break;
	default:
		break;
	}
}

void ASCPlayer::UseFireball()
{
	if (FireballAttributes.ManaCost <= ManaCurrent)
	{
		if (bFireballReady && !bIsUsingMist)
		{
			UpdateMana(FireballAttributes.ManaCost);
			bUsedFireball = true;
			bFireballReady = false;
			if (FireballClass)
			{
				GetWorld()->SpawnActor<ASCFireballActor>(FireballClass, FireballSpawnPoint->GetComponentTransform());
			}
			USoundCue* fireCue = LoadObject<USoundCue>(nullptr, TEXT("SoundCue'/Game/MatroidVania/Audio/SFX_Fireball_Cue.SFX_Fireball_Cue'"));
			if (fireCue)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), fireCue, FireballSpawnPoint->GetComponentLocation());
			}		
		}
		else
		{
			UGameplayStatics::PlaySound2D(GetWorld(), SFXInsufficientMana);
		}
	}
}

void ASCPlayer::UseElectroSpark()
{
	if (ElectroSparkAttributes.ManaCost <= ManaCurrent)
	{
		if (bElectroSparkReady && !bIsUsingMist)
		{
			UpdateMana(ElectroSparkAttributes.ManaCost);
			bUsedElectroSpark = true;
			bElectroSparkReady = false;

			ElectroSparkON();
			GetWorldTimerManager().SetTimer(ElectroSparkHandle, this, &ASCPlayer::ElectroSparkTimer, 0.01f);
		}
		else
		{
			UGameplayStatics::PlaySound2D(GetWorld(), SFXInsufficientMana);
		}
	}
}

void ASCPlayer::UseLensOfTruth()
{
	if (!bIsUsingLensOfTruth)
	{
		if (LensOfTruthAttributes.ManaCost <= ManaCurrent && LensOfTruthCooldown == 0.f)
		{
			UpdateMana(LensOfTruthAttributes.ManaCost);
			ToggleReady.ExecuteIfBound();
			LensFX_ON();
			CameraTimeline.PlayFromStart();
			WMagicActiveTimeline.PlayFromStart();
		}
		else
		{
			UGameplayStatics::PlaySound2D(GetWorld(), SFXInsufficientMana);
		}
	}
	else
	{
		WMagicTimerUp();
	}
}

void ASCPlayer::UseMist()
{
	if (!bIsUsingMist)
	{
		if (MistAttributes.ManaCost <= ManaCurrent && MistCooldown == 0.f)
		{
			UpdateMana(MistAttributes.ManaCost);
			ToggleReady.ExecuteIfBound();
			MistFX_ON();
			CameraTimeline.PlayFromStart();
			WMagicActiveTimeline.PlayFromStart();
		}
		else
		{
			UGameplayStatics::PlaySound2D(GetWorld(), SFXInsufficientMana);
		}
	}
	else
	{
		WMagicTimerUp();
	}
}

void ASCPlayer::LensFX_ON()
{
	bIsUsingLensOfTruth = true;
	USoundCue* LensSC = LoadObject<USoundCue>(nullptr, TEXT("SoundCue'/Engine/EditorSounds/GamePreview/EjectFromPlayer_Cue.EjectFromPlayer_Cue'"));
	if (LensSC)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), LensSC, 3.f);
	}
	WMagicWidget->SetVisibility(true);
	SFX_WMagic_LensActive->Activate();
	for (auto wall : SCSecretWalls)
	{
		ASCSecretWall* SecretWall = Cast<ASCSecretWall>(wall);
		if (SecretWall)
		{
			SecretWall->Mesh->SetMaterial(0, LensOfTruthMat);
			SecretWall->Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}

	for (auto platform : SCSecretPlatforms)
	{
		ASCSecretPlatform* SecretPlatform = Cast<ASCSecretPlatform>(platform);
		if (SecretPlatform)
		{
			SecretPlatform->Mesh->SetVisibility(true);
			SecretPlatform->Mesh->SetMaterial(0, LensOfTruthMat);
			SecretPlatform->Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			SecretPlatform->Mesh->SetCollisionObjectType(ECC_WorldDynamic);
			SecretPlatform->Mesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
		}
	}
}

void ASCPlayer::LensFX_OFF()
{
	bIsUsingLensOfTruth = false;
	USoundCue* LensSC = LoadObject<USoundCue>(nullptr, TEXT("SoundCue'/Engine/EditorSounds/GamePreview/EjectFromPlayer_Cue.EjectFromPlayer_Cue'"));
	if (LensSC)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), LensSC);
	}
	WMagicWidget->SetVisibility(false);
	SFX_WMagic_LensActive->FadeOut(1.f, 0.f);

	for (auto wall : SCSecretWalls)
	{
		ASCSecretWall* SecretWall = Cast<ASCSecretWall>(wall);
		if (SecretWall)
		{
			SecretWall->Mesh->SetMaterial(0, BrickWallMat);
			SecretWall->Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			SecretWall->Mesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
		}
	}

	for (auto platform : SCSecretPlatforms)
	{
		ASCSecretPlatform* SecretPlatform = Cast<ASCSecretPlatform>(platform);
		if (SecretPlatform)
		{
			SecretPlatform->Mesh->SetVisibility(false);
			SecretPlatform->Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			
		}
	}
}

void ASCPlayer::MistFX_ON()
{
	bIsUsingMist = true;
	USoundCue* MistSC = LoadObject<USoundCue>(nullptr, TEXT("SoundCue'/Game/MatroidVania/Audio/WMagic_MistTransform_Cue.WMagic_MistTransform_Cue'"));
	if (MistSC)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), MistSC);
	}
	GetMesh()->SetVisibility(false);
	WMagicWidget->SetVisibility(true);
	VFX_WMagic_MistActive->SetVisibility(true);
	MistPawnMovement->Activate();
	SFX_WMagic_LensActive->Activate();
	GetCharacterMovement()->Deactivate();
	GetCapsuleComponent()->SetCapsuleHalfHeight(45.f);
	GetCapsuleComponent()->SetCollisionObjectType(ECC_Mist);
}

void ASCPlayer::MistFX_OFF()
{
	bIsUsingMist = false;
	USoundCue* MistSC = LoadObject<USoundCue>(nullptr, TEXT("SoundCue'/Game/MatroidVania/Audio/WMagic_MistTransform_Cue.WMagic_MistTransform_Cue'"));
	if (MistSC)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), MistSC);
	}
	UParticleSystem* MistPS = LoadObject<UParticleSystem>(nullptr, TEXT("ParticleSystem'/Game/MatroidVania/Particles/P_MistTransfor.P_MistTransfor'"));
	if (MistPS)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MistPS, GetCapsuleComponent()->GetComponentLocation(), FRotator::ZeroRotator);
	}
	GetMesh()->SetVisibility(true);
	WMagicWidget->SetVisibility(false);
	VFX_WMagic_MistActive->SetVisibility(false);
	GetCharacterMovement()->Activate();
	MistPawnMovement->Deactivate();
	SFX_WMagic_MistActive->FadeOut(1.f, 0.f);
	GetCapsuleComponent()->SetCapsuleHalfHeight(96.f);
	GetCapsuleComponent()->SetCollisionObjectType(ECC_Pawn);
}

void ASCPlayer::OnBMagicMenuBtnClicked()
{
	if (SCPlayerCtl)
	{
		SCPlayerCtl->ShowBMagicMenu();
	}
}

void ASCPlayer::OnBMagicMenuBtnReleased()
{
	if (SCPlayerCtl)
	{
		SCPlayerCtl->HideBMagicMenu();
	}
}

void ASCPlayer::OnWMagicMenuBtnClicked()
{
	if (SCPlayerCtl)
	{
		SCPlayerCtl->ShowWMagicMenu();
	}
}

void ASCPlayer::OnWMagicMenuBtnReleased()
{
	if (SCPlayerCtl)
	{
		SCPlayerCtl->HideWMagicMenu();
	}
}

void ASCPlayer::OnWhiteMagicBtnClicked()
{
	switch (WMagicSlotted)
	{
	case E_WMagic::EWM_None:
		break;
	case E_WMagic::EWM_LensOfTruth:
		UseLensOfTruth();
		break;
	case E_WMagic::EWM_Mist:
		UseMist();
		break;
	case E_WMagic::EWM_Shield:
		break;
	case E_WMagic::EWM_TimeSlow:
		break;
	default:
		break;
	}
}

float ASCPlayer::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	UpdateHealth(DamageAmount);

	return DamageAmount;
}

// Called when the game starts or when spawned
void ASCPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	ASCHUD* hud = Cast<ASCHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if (hud)
	{
		MainWidget = hud->SCMainWidget;
	}

	SCPlayerCtl = Cast<ASCPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	UClass* WMWidget = LoadClass<USCWMagicTimerWidget>(nullptr, TEXT("WidgetBlueprint'/Game/MatroidVania/Widgets/SC/WBP_SCWMagicTimer.WBP_SCWMagicTimer_C'"));
	if (WMWidget)
	{
		WMagicWidget->SetWidgetClass(WMWidget);
	}

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASCSecretPlatform::StaticClass(), SCSecretPlatforms);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASCSecretWall::StaticClass(), SCSecretWalls);

	UMaterial* LensMat = LoadObject<UMaterial>(nullptr, TEXT("Material'/Game/MatroidVania/Materials/M_LensOfTruth_Wall.M_LensOfTruth_Wall'"));
	if (LensMat)
	{
		LensOfTruthMat = UMaterialInstanceDynamic::Create(LensMat, nullptr);
	}
	UMaterial* BrickMat = LoadObject<UMaterial>(nullptr, TEXT("Material'/Game/StarterContent/Materials/M_Brick_Clay_Old.M_Brick_Clay_Old'"));
	if (BrickMat)
	{
		BrickWallMat = UMaterialInstanceDynamic::Create(BrickMat, nullptr);
	}

	if (CameraFloatCurve)
	{
		FOnTimelineFloat CameraTimelineProgress;
		CameraTimelineProgress.BindUFunction(this, FName("CameraTimelineProgress"));
		CameraTimeline.AddInterpFloat(CameraFloatCurve, CameraTimelineProgress);
	}

	if (WMagicActiveFloatCurve)
	{
		FOnTimelineFloat WMagicTimelineProgress;
		WMagicTimelineProgress.BindUFunction(this, FName("WMagicActiveProgress"));
		WMagicActiveTimeline.AddInterpFloat(WMagicActiveFloatCurve, WMagicTimelineProgress);
	}

	if (WMagicCooldownFloatCurve)
	{
		FOnTimelineFloat WMagicCDTimelineProgress;
		WMagicCDTimelineProgress.BindUFunction(this, FName("WMagicCooldownProgress"));
		WMagicCooldownTimeline.AddInterpFloat(WMagicCooldownFloatCurve, WMagicCDTimelineProgress);
	}
}

void ASCPlayer::JumpStart()
{
	if (!bIsUsingMist)
	{
		if (bHasDoubleJump)
		{
			if (JumpCount == 0)
			{
				if (!GetCharacterMovement()->IsFalling())
				{
					bPressedJump = true;
					JumpFX();
					JumpCount++;
				}
			}
			else if (JumpCount == 1) //第二次跳
			{
				JumpCount++;
				bIsDoubleJumping = true;
				
				GetCharacterMovement()->Velocity = FVector(GetCharacterMovement()->Velocity.X, GetCharacterMovement()->Velocity.Y, 0.f);
				LaunchCharacter(DoubleJumpVelocity, false, false);
				USoundCue* DJSoundCue = LoadObject<USoundCue>(nullptr, TEXT("SoundCue'/Engine/EditorSounds/GamePreview/StartSimulate_Cue.StartSimulate_Cue'"));
				if (DJSoundCue)
				{
					UGameplayStatics::PlaySound2D(GetWorld(), DJSoundCue);
				}
				UParticleSystem* DJParticle = LoadObject<UParticleSystem>(nullptr, TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Forging/P_HammerSparks.P_HammerSparks'"));
				if (DJParticle)
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DJParticle, JumpVFXPoint->GetComponentLocation(), FRotator::ZeroRotator, FVector(5.f));
				}
				JumpFX();
				bIsDoubleJumping = false;
			}
		}
		else
		{
			if (!GetCharacterMovement()->IsFalling())
			{
				bPressedJump = true;
				JumpFX();
			}
		}
	}
}

void ASCPlayer::JumpEnd()
{
	bPressedJump = false;
}

void ASCPlayer::JumpFX()
{
	if (!GetCharacterMovement()->IsFalling())
	{
		USoundCue* JumpSC = LoadObject<USoundCue>(nullptr, TEXT("SoundCue'/Game/ParagonCountess/Characters/Heroes/Countess/Sounds/SoundCues/Countess_Effort_Jump.Countess_Effort_Jump'"));
		if (JumpSC)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), JumpSC, 0.5f);
		}
		UParticleSystem* JumpPS = LoadObject<UParticleSystem>(nullptr, TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Monsters/FX_Monster_Sentinel/P_Sentinel_Leap_Launch_Trail.P_Sentinel_Leap_Launch_Trail'"));
		if (JumpPS)
		{
			UGameplayStatics::SpawnEmitterAttached(JumpPS, JumpVFXPoint, NAME_None, FVector::ZeroVector, FRotator::ZeroRotator, FVector(0.5f));
		}
	}
}

void ASCPlayer::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	USoundCue* LandedSC = LoadObject<USoundCue>(nullptr, TEXT("SoundCue'/Game/ParagonCountess/Characters/Heroes/Countess/Sounds/SoundCues/Countess_Effort_Land.Countess_Effort_Land'"));
	if (LandedSC)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), LandedSC, JumpVFXPoint->GetComponentLocation(), 0.5f);
	}
	float LandSpeed = FMath::Clamp(GetCapsuleComponent()->GetComponentVelocity().Z * JumpScaler, 0.5f, 3.f);
	UParticleSystem* LandedPS = LoadObject<UParticleSystem>(nullptr, TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Monsters/FX_Monster_Gruntling/Master/P_MasterGrunt_Drag_Dust.P_MasterGrunt_Drag_Dust'"));
	if (LandedPS)
	{
		UParticleSystemComponent* LandedPsc = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), LandedPS, JumpVFXPoint->GetComponentLocation());
		LandedPsc->SetWorldScale3D(FVector(LandSpeed));
	}
	Cast<ASCPlayerController>(GetController())->PlayDynamicForceFeedback(LandSpeed / 3.f, 0.1f, true, true, true, true);
	GetCharacterMovement()->DisableMovement();
	GetWorldTimerManager().SetTimer(DelayMoveHandle, this, &ASCPlayer::DelayMove, 0.2f);
}

void ASCPlayer::MoveRight(float Value)
{
	AddMovementInput(FVector::ForwardVector, Value);
}

void ASCPlayer::MoveUp(float Value)
{
	if (bIsUsingMist)
	{
		AddMovementInput(FVector::UpVector, Value);
	}
}

void ASCPlayer::DelayMove()
{
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	JumpCount = 0;
}

void ASCPlayer::BackDashCD()
{
	bBackDashingReady = true;
}

void ASCPlayer::ElectroSparkTimer()
{
	bUsedElectroSpark = false;
	UClass* CamShake = LoadClass<USCCameraShake>(nullptr, TEXT("Blueprint'/Game/MatroidVania/Blueprints/Misc/BP_SCCameraShake.BP_SCCameraShake_C'"));
	if (CamShake)
	{
		UGameplayStatics::PlayWorldCameraShake(GetWorld(), CamShake, GetCapsuleComponent()->GetComponentLocation(), 0.f, 1000.f);
	}

	SCEsActor= GetWorld()->SpawnActor<ASCElectroSparkActor>(ElectroSparkClass, GetCapsuleComponent()->GetComponentLocation() + FVector(0.f, 0.f, 150.f), FRotator::ZeroRotator);
	if (SCEsActor)
	{
		UsingElectroSpark(SCEsActor);
	}
}

void ASCPlayer::CameraTimelineProgress(float val)
{
	float Fov = FMath::Lerp(90.f, 100.f, val);
	CameraComp->SetFieldOfView(Fov);
	if (!bIsUsingMist)
	{
		float Intensity = FMath::Lerp(0.f, 0.8f, val);
		FPostProcessSettings PostPS;
		PostPS.VignetteIntensity = Intensity;
		CameraComp->PostProcessSettings = PostPS;
	}
	else
	{
		float Intensity = FMath::Lerp(0.f, 0.6f, val);
		float BlueSize = FMath::Lerp(0.f, 15.f, val);
		FPostProcessSettings PostPS;		
		//PostPS.DepthOfFieldMethod = EDepthOfFieldMethod::DOFM_Gaussian;
		PostPS.DepthOfFieldMethod = EDepthOfFieldMethod::DOFM_CircleDOF;
		PostPS.VignetteIntensity = Intensity;
		PostPS.DepthOfFieldNearBlurSize = BlueSize;
		PostPS.DepthOfFieldFarBlurSize = BlueSize;
		PostPS.bOverride_DepthOfFieldMethod = true;
		PostPS.bOverride_DepthOfFieldNearBlurSize = true;
		PostPS.bOverride_DepthOfFieldFarBlurSize = true;
		CameraComp->PostProcessSettings = PostPS;
		FVector ScaleVec = FMath::Lerp(FVector(0.f), FVector(1.f), val);
		VFX_WMagic_MistActive->SetWorldScale3D(ScaleVec);
	}
}

void ASCPlayer::WMagicActiveProgress(float ActiveTime)
{
	LensOfTruthActiveTime = ActiveTime;
	USCWMagicTimerWidget* wid = Cast<USCWMagicTimerWidget>(WMagicWidget->GetUserWidgetObject());
	if (wid)
	{
		wid->UpdatePBTimer(ActiveTime);
	}
}

void ASCPlayer::WMagicActiveFinished()
{
	WMagicTimerUp();
}

void ASCPlayer::WMagicCooldownProgress(float CooldownTime)
{
	if (bIsUsingMist)
	{
		MistCooldown = CooldownTime;
	}
	else
	{
		LensOfTruthCooldown = CooldownTime;
	}
	MainWidget->UpdateCooldownProgressBar(CooldownTime);
}

void ASCPlayer::WMagicCooldownFinished()
{
	ToggleReady.ExecuteIfBound();
}

void ASCPlayer::WMagicTimerStop()
{
	WMagicActiveTimeline.Stop();
}

void ASCPlayer::WMagicTimerUp()
{
	WMagicDeactivated();
	WMagicTimerStop();
	if (bIsUsingMist)
	{
		MistFX_OFF();
	}
	else
	{
		LensFX_OFF();
	}
	WMagicCooldownTimeline.PlayFromStart();
}

void ASCPlayer::WMagicDeactivated()
{
	CameraTimeline.Reverse();
}

// Called every frame
void ASCPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bBackDashingReady)
	{
		BackDashCDTime += DeltaTime;
		if (BackDashCDTime >= BackDashCooldown)
		{
			bBackDashingReady = true;
			BackDashCDTime = 0.f;
		}
	}

	if (!bFireballReady)
	{
		FireballTimer += DeltaTime;
		if (FireballTimer >= FireballCooldown)
		{
			bFireballReady = true;
			FireballTimer = 0.f;
		}
	}

	if (bUsedFireball)
	{
		UsedFireballTimer += DeltaTime;
		if (UsedFireballTimer >= 0.1f)
		{
			bUsedFireball = false;
			UsedFireballTimer = 0.f;
		}
	}

	if (!bElectroSparkReady)
	{
		ElectroSparkCDTimer += DeltaTime;
		if (ElectroSparkCDTimer >= ElectroSparkCooldown)
		{
			bElectroSparkReady = true;
			ElectroSparkCDTimer = 0.f;
			ElectroSparkOFF();
		}
	}

	if (bIsBackDashing)
	{
		// 面向前方
		if (GetCapsuleComponent()->GetComponentRotation().Yaw >= 0.f)
		{
			FVector NewLocation = FMath::Lerp(GetCapsuleComponent()->GetComponentLocation(),
				GetCapsuleComponent()->GetComponentLocation() + BackDashLeftAmount, BackDashSpeed * BackDashTime);
			GetCapsuleComponent()->SetRelativeLocation(NewLocation);
			
		}
		// 面向后方
		else
		{
			FVector NewLocation = FMath::Lerp(GetCapsuleComponent()->GetComponentLocation(),
				GetCapsuleComponent()->GetComponentLocation() + BackDashRightAmount, BackDashSpeed * BackDashTime);
			GetCapsuleComponent()->SetRelativeLocation(NewLocation);
		}

		BackDashTime += DeltaTime;
		if (BackDashTime >= 0.2f)
		{
			bIsBackDashing = false;
			BackDashTime = 0.f;
		}
	}

	CameraTimeline.TickTimeline(DeltaTime);
	WMagicActiveTimeline.TickTimeline(DeltaTime);
	WMagicCooldownTimeline.TickTimeline(DeltaTime);
}

// Called to bind functionality to input
void ASCPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASCPlayer::JumpStart);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ASCPlayer::JumpEnd);
	PlayerInputComponent->BindAction("LearnSkill", IE_Pressed, this, &ASCPlayer::OnLearnSkillBtnClicked);
	PlayerInputComponent->BindAction("BackDash", IE_Pressed, this, &ASCPlayer::OnBackDashBtnClicked);
	PlayerInputComponent->BindAction("BlackMagic", IE_Pressed, this, &ASCPlayer::OnBlackMagicBtnClicked);
	PlayerInputComponent->BindAction("WhiteMagic", IE_Pressed, this, &ASCPlayer::OnWhiteMagicBtnClicked);
	PlayerInputComponent->BindAction("ShowBMMenu", IE_Pressed, this, &ASCPlayer::OnBMagicMenuBtnClicked).bExecuteWhenPaused = true;
	PlayerInputComponent->BindAction("ShowBMMenu", IE_Released, this, &ASCPlayer::OnBMagicMenuBtnReleased).bExecuteWhenPaused = true;
	PlayerInputComponent->BindAction("ShowWMMenu", IE_Pressed, this, &ASCPlayer::OnWMagicMenuBtnClicked).bExecuteWhenPaused = true;
	PlayerInputComponent->BindAction("ShowWMMenu", IE_Released, this, &ASCPlayer::OnWMagicMenuBtnReleased).bExecuteWhenPaused = true;

	PlayerInputComponent->BindAxis("MoveRight", this, &ASCPlayer::MoveRight);
	PlayerInputComponent->BindAxis("MoveUp", this, &ASCPlayer::MoveUp);
}

void ASCPlayer::InitTimeline()
{
	static ConstructorHelpers::FObjectFinder<UCurveFloat> CameraCurveObj(TEXT("CurveFloat'/Game/MatroidVania/Blueprints/Actors/SC/CameraFX.CameraFX'"));
	if (CameraCurveObj.Succeeded())
	{
		CameraFloatCurve = CameraCurveObj.Object;
	}

	static ConstructorHelpers::FObjectFinder<UCurveFloat> LensCurveObj(TEXT("CurveFloat'/Game/MatroidVania/Blueprints/Actors/SC/LensActive.LensActive'"));
	if (LensCurveObj.Succeeded())
	{
		WMagicActiveFloatCurve = LensCurveObj.Object;
	}

	static ConstructorHelpers::FObjectFinder<UCurveFloat> LensCDObj(TEXT("CurveFloat'/Game/MatroidVania/Blueprints/Actors/SC/LensCooldown.LensCooldown'"));
	if (LensCDObj.Succeeded())
	{
		WMagicCooldownFloatCurve = LensCDObj.Object;
	}

	FOnTimelineEventStatic WMagicTimeLineEvent;
	WMagicTimeLineEvent.BindUFunction(this, TEXT("WMagicActiveFinished"));
	WMagicActiveTimeline.SetTimelineFinishedFunc(WMagicTimeLineEvent);

	FOnTimelineEventStatic WMagicCDTimeLineEvent;
	WMagicCDTimeLineEvent.BindUFunction(this, TEXT("WMagicCooldownFinished"));
	WMagicCooldownTimeline.SetTimelineFinishedFunc(WMagicCDTimeLineEvent);
}

void ASCPlayer::InitInfo()
{	
	InitPlayerAttributes();
	InitSkillInfo();
	InitBMagicInfo();
	InitWMagicInfo();
}

void ASCPlayer::InitPlayerAttributes()
{
	HealthMax = 100;
	HealthCurrent = 100;
	HealthPercentage = 1.f;
	ManaMax = 150;
	ManaCurrent = 150;
	ManaPercentage = 1.f;
	bCanInteract = false;
	InteractTraceLength = 100;

	BMagicSlotted = E_BMagic::EBM_None;
	WMagicSlotted = E_WMagic::EWM_None;
	SkillAcquired = E_Skills::ES_None;
	bWMagicReady = true;

	BackDashCDTime = 0.f;
}

void ASCPlayer::InitSkillInfo()
{
	JumpScaler = -0.001f;
	PlayerAcceleration = 0.f;

	bHasDoubleJump = false;
	bIsDoubleJumping = false;
	JumpCount = 0;
	DoubleJumpVelocity = FVector(0.f, 0.f, 1200.f);
	DoubleJumpData.Title = FText::FromString("Learned Double Jump!");
	DoubleJumpData.Description = FText::FromString("Allows a 2nd jump while character is airborne");
	static ConstructorHelpers::FObjectFinder<UTexture> ABtn(TEXT("Texture2D'/Game/MatroidVania/Textures/T_Button_A.T_Button_A'"));
	DoubleJumpData.InputButton.SetResourceObject(ABtn.Object);
	static ConstructorHelpers::FObjectFinder<UTexture> DJIconTex(TEXT("Texture2D'/Game/MatroidVania/Textures/T_Icon_DoubleJump.T_Icon_DoubleJump'"));
	DoubleJumpData.SkillIcon.SetResourceObject(DJIconTex.Object);
	DoubleJumpData.SkillIcon.SetImageSize(FVector2D(200.f, 200.f));
	static ConstructorHelpers::FObjectFinder<UTexture> DJImgTex(TEXT("Texture2D'/Game/MatroidVania/Textures/T_Image_DoubleJump.T_Image_DoubleJump'"));
	DoubleJumpData.SkillImage.SetResourceObject(DJImgTex.Object);
	DoubleJumpData.SkillImage.SetImageSize(FVector2D(516.f, 363.f));
	DoubleJumpData.SkillBorderColor.TintColor = FSlateColor(FLinearColor(0.15f, 0.15f, 0.f));
	DoubleJumpData.SkillFontColor = FSlateColor(FLinearColor(0.15f, 0.15f, 0.f));
	DoubleJumpData.MagicSkill = false;

	bHasBackDash = false;
	bIsBackDashing = false;
	bBackDashingReady = true;
	BackDashCooldown = 0.35f;
	BackDashLeftAmount = FVector(-20.f, 0.f, 0.f);
	BackDashRightAmount = FVector(20.f, 0.f, 0.f);
	BackDashData.Title = FText::FromString("Learned Back Dash!");
	BackDashData.Description = FText::FromString("Character rapid back dashes away from danger!");
	static ConstructorHelpers::FObjectFinder<UTexture> YBtn(TEXT("Texture2D'/Game/MatroidVania/Textures/T_Button_Y.T_Button_Y'"));
	BackDashData.InputButton.SetResourceObject(YBtn.Object);
	static ConstructorHelpers::FObjectFinder<UTexture> BDIconTex(TEXT("Texture2D'/Game/MatroidVania/Textures/T_Icon_BackDash.T_Icon_BackDash'"));
	BackDashData.SkillIcon.SetResourceObject(BDIconTex.Object);
	BackDashData.SkillIcon.SetImageSize(FVector2D(200.f, 200.f));
	static ConstructorHelpers::FObjectFinder<UTexture> BDImgTex(TEXT("Texture2D'/Game/MatroidVania/Textures/T_Image_BackDash.T_Image_BackDash'"));
	BackDashData.SkillImage.SetResourceObject(BDImgTex.Object);
	BackDashData.SkillImage.SetImageSize(FVector2D(531.f, 350.f));
	BackDashData.SkillBorderColor.TintColor = FSlateColor(FLinearColor(0.15f, 0.15f, 0.f));
	BackDashData.SkillFontColor = FSlateColor(FLinearColor(0.15f, 0.15f, 0.f));
	BackDashData.MagicSkill = false;
}

void ASCPlayer::InitWMagicInfo()
{
	bHasWMagicLensOfTruth = false;
	bIsUsingLensOfTruth = false;
	LensOfTruthActiveTime = 1.f;
	LensOfTruthCooldown = 0.f;
	LensOfTruthAttributes.MagicName = FText::FromString("Lens Of Truth");
	static ConstructorHelpers::FObjectFinder<UTexture> LIconTex(TEXT("Texture2D'/Game/MatroidVania/Textures/T_LensOfTruth.T_LensOfTruth'"));
	LensOfTruthAttributes.IconSlate.SetResourceObject(LIconTex.Object);
	LensOfTruthAttributes.ManaCost = 10;
	LensOfTruthAttributes.DamageValue = 0.f;
	LensOfTruthAttributes.DamageType = nullptr;
	LensOfTruthData.Title = FText::FromString("Learned LensOfTruth!");
	LensOfTruthData.Description = FText::FromString("Previews passageways or platforms when activated.");
	static ConstructorHelpers::FObjectFinder<UTexture> LTBtn(TEXT("Texture2D'/Game/MatroidVania/Textures/T_LTrigger_01.T_LTrigger_01'"));
	LensOfTruthData.InputButton.SetResourceObject(LTBtn.Object);
	static ConstructorHelpers::FObjectFinder<UTexture> LensIconTex(TEXT("Texture2D'/Game/MatroidVania/Textures/T_Icon_LensOfTruth.T_Icon_LensOfTruth'"));
	LensOfTruthData.SkillIcon.SetResourceObject(LensIconTex.Object);
	LensOfTruthData.SkillIcon.SetImageSize(FVector2D(150.f, 150.f));
	static ConstructorHelpers::FObjectFinder<UTexture> LensImgTex(TEXT("Texture2D'/Game/MatroidVania/Textures/T_Image_LensOfTruth.T_Image_LensOfTruth'"));
	LensOfTruthData.SkillImage.SetResourceObject(LensImgTex.Object);
	LensOfTruthData.SkillImage.SetImageSize(FVector2D(64.f, 64.f));
	LensOfTruthData.SkillBorderColor.TintColor = FSlateColor(FLinearColor(0.7f, 0.7f, 1.f));
	LensOfTruthData.SkillFontColor = FSlateColor(FLinearColor(0.7f, 0.7f, 1.f));
	LensOfTruthData.MagicSkill = true;

	bHasWMagicMist = false;
	bIsUsingMist = false;
	MistActiveTime = 1.f;
	MistCooldown = 0.f;
	MistAttributes.MagicName = FText::FromString("Mist");
	static ConstructorHelpers::FObjectFinder<UTexture> MIconTex(TEXT("Texture2D'/Game/MatroidVania/Textures/T_Mist.T_Mist'"));
	MistAttributes.IconSlate.SetResourceObject(MIconTex.Object);
	MistAttributes.ManaCost = 10;
	MistAttributes.DamageValue = 0.f;
	MistAttributes.DamageType = nullptr;
	MistData.Title = FText::FromString("Learned Mist!");
	MistData.Description = FText::FromString("Transforms you into mist! This allows you to pass through locked gates!");
	MistData.InputButton.SetResourceObject(LTBtn.Object);
	static ConstructorHelpers::FObjectFinder<UTexture> MistIconTex(TEXT("Texture2D'/Game/MatroidVania/Textures/T_Icon_Mist.T_Icon_Mist'"));
	MistData.SkillIcon.SetResourceObject(MistIconTex.Object);
	MistData.SkillIcon.SetImageSize(FVector2D(200.f, 200.f));
	static ConstructorHelpers::FObjectFinder<UTexture> MistImgTex(TEXT("Texture2D'/Game/MatroidVania/Textures/T_Image_Mist.T_Image_Mist'"));
	MistData.SkillImage.SetResourceObject(MistImgTex.Object);
	MistData.SkillImage.SetImageSize(FVector2D(615.f, 443.f));
	MistData.SkillBorderColor.TintColor = FSlateColor(FLinearColor(0.7f, 0.7f, 1.f));
	MistData.SkillFontColor = FSlateColor(FLinearColor(0.7f, 0.7f, 1.f));
	MistData.MagicSkill = true;

	bHasWMagicShield = true;
	ShieldAttributes.MagicName = FText::FromString("Shield");
	static ConstructorHelpers::FObjectFinder<UTexture> SIconTex(TEXT("Texture2D'/Game/MatroidVania/Textures/T_Shield.T_Shield'"));
	ShieldAttributes.IconSlate.SetResourceObject(SIconTex.Object);
	ShieldAttributes.ManaCost = 10;
	ShieldAttributes.DamageValue = 0.f;
	ShieldAttributes.DamageType = nullptr;

	bHasWMagicTimeSlow = true;
	TimeSlowAttributes.MagicName = FText::FromString("TimeSlow");
	static ConstructorHelpers::FObjectFinder<UTexture> TIconTex(TEXT("Texture2D'/Game/MatroidVania/Textures/T_TimeSlow.T_TimeSlow'"));
	TimeSlowAttributes.IconSlate.SetResourceObject(TIconTex.Object);
	TimeSlowAttributes.ManaCost = 10;
	TimeSlowAttributes.DamageValue = 0.f;
	TimeSlowAttributes.DamageType = nullptr;
}

void ASCPlayer::InitBMagicInfo()
{
	bUsedFireball = false;
	bHasBMagicFireball = false;
	bFireballReady = true;
	FireballCooldown = 1.13f;
	FireballAttributes.MagicName = FText::FromString("Fireball");
	static ConstructorHelpers::FObjectFinder<UTexture> FIconTex(TEXT("Texture2D'/Game/MatroidVania/Textures/T_Fireball.T_Fireball'"));
	FireballAttributes.IconSlate.SetResourceObject(FIconTex.Object);
	FireballAttributes.ManaCost = 5;
	FireballAttributes.DamageValue = 50.f;
	static ConstructorHelpers::FClassFinder<UDamageType> DmgType(TEXT("Blueprint'/Game/MatroidVania/Blueprints/DamageTypes/BP_DamageFire.BP_DamageFire_C'"));
	FireballAttributes.DamageType = DmgType.Class;
	FireballData.Title = FText::FromString("Learned Fireball!");
	FireballData.Description = FText::FromString("Cast out a ranged fireball.");
	static ConstructorHelpers::FObjectFinder<UTexture> RTBtn(TEXT("Texture2D'/Game/MatroidVania/Textures/T_RTrigger_01.T_RTrigger_01'"));
	FireballData.InputButton.SetResourceObject(RTBtn.Object);
	static ConstructorHelpers::FObjectFinder<UTexture> FbIconTex(TEXT("Texture2D'/Game/MatroidVania/Textures/T_Icon_Fireball.T_Icon_Fireball'"));
	FireballData.SkillIcon.SetResourceObject(FbIconTex.Object);
	FireballData.SkillIcon.SetImageSize(FVector2D(200.f, 200.f));
	static ConstructorHelpers::FObjectFinder<UTexture> FbImgTex(TEXT("Texture2D'/Game/MatroidVania/Textures/T_Image_Fireball.T_Image_Fireball'"));
	FireballData.SkillImage.SetResourceObject(FbImgTex.Object);
	FireballData.SkillImage.SetImageSize(FVector2D(64.f, 64.f));
	FireballData.SkillBorderColor.TintColor = FSlateColor(FLinearColor(0.08f, 0.005f, 0.035f));
	FireballData.SkillFontColor = FSlateColor(FLinearColor(0.08f, 0.005f, 0.035f));
	FireballData.MagicSkill = true;

	bUsedElectroSpark = false;
	bHasBMagicElectroSpark = false;
	bElectroSparkReady = true;
	ElectroSparkCooldown = 2.01f;
	ElectroSparkAttributes.MagicName = FText::FromString("Electro Spark");
	static ConstructorHelpers::FObjectFinder<UTexture> ESIconTex(TEXT("Texture2D'/Game/MatroidVania/Textures/T_ElectroSpark.T_ElectroSpark'"));
	ElectroSparkAttributes.IconSlate.SetResourceObject(ESIconTex.Object);
	ElectroSparkAttributes.ManaCost = 15;
	ElectroSparkAttributes.DamageValue = 100.f;
	static ConstructorHelpers::FClassFinder<UDamageType> ElecDmgType(TEXT("Blueprint'/Game/MatroidVania/Blueprints/DamageTypes/BP_DamageLightning.BP_DamageLightning_C'"));
	ElectroSparkAttributes.DamageType = ElecDmgType.Class;
	ElectroSparkData.Title = FText::FromString("Learned ElectroSpark!");
	ElectroSparkData.Description = FText::FromString("Sends out an arc of electricity in all directional.");
	ElectroSparkData.InputButton.SetResourceObject(RTBtn.Object);
	static ConstructorHelpers::FObjectFinder<UTexture> ElecIconTex(TEXT("Texture2D'/Game/MatroidVania/Textures/T_Icon_ElectroSpark.T_Icon_ElectroSpark'"));
	ElectroSparkData.SkillIcon.SetResourceObject(ElecIconTex.Object);
	ElectroSparkData.SkillIcon.SetImageSize(FVector2D(200.f, 200.f));
	static ConstructorHelpers::FObjectFinder<UTexture> ElecImgTex(TEXT("Texture2D'/Game/MatroidVania/Textures/T_Image_ElectroSpark.T_Image_ElectroSpark'"));
	ElectroSparkData.SkillImage.SetResourceObject(ElecImgTex.Object);
	ElectroSparkData.SkillImage.SetImageSize(FVector2D(64.f, 64.f));
	ElectroSparkData.SkillBorderColor.TintColor = FSlateColor(FLinearColor(0.08f, 0.005f, 0.035f));
	ElectroSparkData.SkillFontColor = FSlateColor(FLinearColor(0.08f, 0.005f, 0.035f));
	ElectroSparkData.MagicSkill = true;

	bHasBMagicBloodlust = true;
	BloodlustAttributes.MagicName = FText::FromString("Bloodlust");
	static ConstructorHelpers::FObjectFinder<UTexture> BlIconTex(TEXT("Texture2D'/Game/MatroidVania/Textures/T_Bloodlust.T_Bloodlust'"));
	BloodlustAttributes.IconSlate.SetResourceObject(BlIconTex.Object);
	BloodlustAttributes.ManaCost = 5;
	BloodlustAttributes.DamageValue = 50.f;
	BloodlustAttributes.DamageType = nullptr;

	bHasBMagicArcticBlast = true;
	ArcticBlastAttributes.MagicName = FText::FromString("ArcticBlast");
	static ConstructorHelpers::FObjectFinder<UTexture> AbIconTex(TEXT("Texture2D'/Game/MatroidVania/Textures/T_ArcticBlast.T_ArcticBlast'"));
	ArcticBlastAttributes.IconSlate.SetResourceObject(AbIconTex.Object);
	ArcticBlastAttributes.ManaCost = 5.f;
	ArcticBlastAttributes.DamageValue = 50.f;
	ArcticBlastAttributes.DamageType = nullptr;

	static ConstructorHelpers::FObjectFinder<USoundCue> LackManaCue(TEXT("SoundCue'/Engine/VREditor/Sounds/VR_negative_Cue.VR_negative_Cue'"));
	if (LackManaCue.Succeeded())
	{
		SFXInsufficientMana = LackManaCue.Object;
	}

	static ConstructorHelpers::FClassFinder<ASCFireballActor> FbCls(TEXT("Blueprint'/Game/MatroidVania/Blueprints/Actors/SC/BP_SCFireballActor.BP_SCFireballActor_C'"));
	if (FbCls.Succeeded())
	{
		FireballClass = FbCls.Class;
	}

	static ConstructorHelpers::FClassFinder<ASCElectroSparkActor> ESCls(TEXT("Blueprint'/Game/MatroidVania/Blueprints/Actors/SC/BP_SCElectroSparkActor.BP_SCElectroSparkActor_C'"));
	if (ESCls.Succeeded())
	{
		ElectroSparkClass = ESCls.Class;
	}
}

