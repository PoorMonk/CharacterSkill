// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCEenmy.generated.h"

class UDamageType;
class ASCGameMode;

UENUM(BlueprintType)
enum class E_TakeDamage : uint8
{
	ETD_VulnerableDeath,
	ETD_VulnerableDamaged,
	ETD_ResistantDeath,
	ETD_ResistantDamaged,
	ETD_NormalDeath,
	ETD_NormalDamaged
};

UCLASS()
class CHARACTERSKILL_API ASCEenmy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCEenmy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	E_TakeDamage CalcuteDamage(float DamageAmount, TSubclassOf<UDamageType> DamageType);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info")
	TSubclassOf<UDamageType> DamageVulnerableClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info")
	TSubclassOf<UDamageType> DamageResistantClass;

	UPROPERTY()
	ASCGameMode* SCGameMode;

	float VulnerableMultiplier;
	float ResistantMultiplier;
	float Health;
	float IncomingBaseDamage;
	float AttackDamage;
	bool bCanHit;
	float HitTimer;

	E_TakeDamage E_DmgType;
};
