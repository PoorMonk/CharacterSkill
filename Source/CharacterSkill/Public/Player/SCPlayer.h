// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"
#include "Public/Data/SCDataObject.h"
#include "SCPlayer.generated.h"

class UParticleSystemComponent;
class UArrowComponent;
class USpringArmComponent;
class UCameraComponent;
class USCMainWidget;
class UFloatingPawnMovement;
class ASCElectroSparkActor;
class ASCPlayerController;
class UWidgetComponent;
class UMaterialInstanceDynamic;
class UTimelineComponent;
//class TEnumAsByte;
class UCurveFloat;
//class ASCSecretWall;
//class ASCSecretPlatform;

DECLARE_DELEGATE(FToggleWMagicReadyMsg);

UCLASS()
class CHARACTERSKILL_API ASCPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCPlayer();

	void UpdateHealth(float DamageValue);
	void UpdateMana(float ManaCostValue);
	FORCEINLINE float GetHealth() { return HealthCurrent; }
	FORCEINLINE float GetMana() { return ManaCurrent; }
	FORCEINLINE float GetMaxMana() { return ManaMax; }

	void ElectroSparkON();
	void ElectroSparkOFF();

	void OnLearnSkillBtnClicked();

	void OnBackDashBtnClicked();

	// 按键使用BMagic
	void OnBlackMagicBtnClicked();
	void UseFireball();
	void UseElectroSpark();

	//按键使用WMagic
	void OnWhiteMagicBtnClicked();
	void UseLensOfTruth();
	void UseMist();

	void LensFX_ON();
	void LensFX_OFF();

	void MistFX_ON();
	void MistFX_OFF();

	// 弹出BMagic菜单
	void OnBMagicMenuBtnClicked();
	void OnBMagicMenuBtnReleased();
	// 弹出WMagic菜单
	void OnWMagicMenuBtnClicked();
	void OnWMagicMenuBtnReleased();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void JumpStart();
	void JumpEnd();
	void JumpFX();
	virtual void Landed(const FHitResult& Hit) override;

	void MoveRight(float Value);
	void MoveUp(float Value);

	void DelayMove();
	void BackDashCD();

	void ElectroSparkTimer();

	UFUNCTION(BlueprintImplementableEvent)
	void UsingElectroSpark(ASCElectroSparkActor* ESActor);
	//virtual void UsingElectroSpark_Implementation();

	UFUNCTION()
	void CameraTimelineProgress(float val);

	UFUNCTION()
	void WMagicActiveProgress(float ActiveTime);
	UFUNCTION()
	void WMagicActiveFinished();

	UFUNCTION()
	void WMagicCooldownProgress(float CooldownTime);
	UFUNCTION()
	void WMagicCooldownFinished();

	void WMagicTimerStop();
	void WMagicTimerUp();
	void WMagicDeactivated();

protected:
	FTimerHandle DelayMoveHandle;
	//FTimerHandle DelayBackDash;
	float BackDashCDTime;
	float BackDashSpeed = 5.f;
	float BackDashTime = 0.f;

	float FireballTimer = 0.f;
	float UsedFireballTimer = 0.f;

	float ElectroSparkCDTimer = 0.f;
	float UsedElectroSparkTimer = 0.f;

	float LensPercent = 0.f;

	UPROPERTY()
	FTimeline CameraTimeline;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Timeline")
	UCurveFloat* CameraFloatCurve;

	UPROPERTY()
	FTimeline WMagicActiveTimeline;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Timeline")
	UCurveFloat* WMagicActiveFloatCurve;

	UPROPERTY()
	FTimeline WMagicCooldownTimeline;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Timeline")
	UCurveFloat* WMagicCooldownFloatCurve;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:

	FToggleWMagicReadyMsg ToggleReady;

#pragma region PlayerInfo
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	E_BMagic BMagicSlotted;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	E_WMagic WMagicSlotted;
	UPROPERTY(VisibleAnywhere)
	E_Skills SkillAcquired;
	bool bWMagicReady;
#pragma endregion

#pragma region Skill
	bool bHasDoubleJump;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	bool bIsDoubleJumping;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	int32 JumpCount;
	FVector DoubleJumpVelocity;

	float PlayerAcceleration;
	float JumpScaler;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill | DoubleJump")
	FSTR_SkillData DoubleJumpData;

	bool bHasBackDash;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	bool bIsBackDashing;
	bool bBackDashingReady;
	float BackDashCooldown;
	FVector BackDashLeftAmount;
	FVector BackDashRightAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill | BackDash")
	FSTR_SkillData BackDashData;
#pragma endregion

#pragma region BlackMagic
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	bool bUsedFireball;
	bool bFireballReady;
	bool bHasBMagicFireball;
	float FireballCooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BMagic | Fireball")
	FSTR_MagicAttr FireballAttributes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BMagic | Fireball")
	FSTR_SkillData FireballData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	bool bUsedElectroSpark;
	bool bElectroSparkReady;
	bool bHasBMagicElectroSpark;
	float ElectroSparkCooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BMagic | ElectroSpark")
	FSTR_MagicAttr ElectroSparkAttributes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BMagic | ElectroSpark")
	FSTR_SkillData ElectroSparkData;

	bool bHasBMagicBloodlust;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BMagic | Bloodlust")
	FSTR_MagicAttr BloodlustAttributes;

	bool bHasBMagicArcticBlast;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BMagic | ArcticBlast")
	FSTR_MagicAttr ArcticBlastAttributes;

	UPROPERTY(VisibleAnywhere)
	class USoundCue* SFXInsufficientMana;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ASCFireballActor> FireballClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ASCElectroSparkActor> ElectroSparkClass;
#pragma endregion

#pragma region WhiteMagic
	bool bIsUsingLensOfTruth;
	bool bHasWMagicLensOfTruth;
	float LensOfTruthCooldown;
	float LensOfTruthActiveTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WMagic | LensOfTruth")
	FSTR_MagicAttr LensOfTruthAttributes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WMagic | LensOfTruth")
	FSTR_SkillData LensOfTruthData;

	bool bIsUsingMist;
	bool bHasWMagicMist;
	float MistCooldown;
	float MistActiveTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WMagic | Mist")
	FSTR_MagicAttr MistAttributes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WMagic | Mist")
	FSTR_SkillData MistData;

	bool bHasWMagicShield;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WMagic | Shield")
	FSTR_MagicAttr ShieldAttributes;

	bool bHasWMagicTimeSlow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WMagic | TimeSlow")
	FSTR_MagicAttr TimeSlowAttributes;
#pragma endregion

#pragma region Interact
	bool bCanInteract;
	int InteractTraceLength;
#pragma endregion

protected:
	UPROPERTY()
	USCMainWidget* MainWidget;

	UPROPERTY()
	ASCPlayerController* SCPlayerCtl;

	UPROPERTY()
	ASCElectroSparkActor* SCEsActor;

	FTimerHandle ElectroSparkHandle;

	UPROPERTY()
	TArray<AActor*> SCSecretWalls;

	UPROPERTY()
	TArray<AActor*> SCSecretPlatforms;

	UPROPERTY()
	UMaterialInstanceDynamic* LensOfTruthMat;
	UPROPERTY()
	UMaterialInstanceDynamic* BrickWallMat;
	//ASCGameMode

#pragma region Components
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component")
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component")
	UArrowComponent* JumpVFXPoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component")
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component")
	UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component")
	UArrowComponent* FireballSpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component")
	UWidgetComponent* WMagicWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component")
	UAudioComponent* SFX_WMagic_LensActive;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component")
	UParticleSystemComponent* VFX_WMagic_MistActive;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component")
	UAudioComponent* SFX_WMagic_MistActive;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component")
	UFloatingPawnMovement* MistPawnMovement;
#pragma endregion

#pragma region AttributeInfo
	float HealthMax;
	float HealthCurrent;
	float HealthPercentage;
	float ManaMax;
	float ManaCurrent;
	float ManaPercentage;
#pragma endregion

private:
	void InitTimeline();
	void InitInfo();
	void InitPlayerAttributes();
	void InitSkillInfo();
	void InitWMagicInfo();
	void InitBMagicInfo();
};
