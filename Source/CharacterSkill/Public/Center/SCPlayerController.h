// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SCPlayerController.generated.h"

class USCSkillAcquiredWidget;
class USCBMagicMenuWidget;
class USCWMagicMenuWidget;
class ASCHUD;
class USoundCue;

/**
 * 
 */
UCLASS()
class CHARACTERSKILL_API ASCPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	void ShowAcquiredSkill();

	void SwitchInputMode(bool IsUI);

	void ShowBMagicMenu();
	void HideBMagicMenu();

	void ShowWMagicMenu();
	void HideWMagicMenu();
	
public:
	UPROPERTY(EditAnywhere)
	USCSkillAcquiredWidget* WBP_SCSkillAcquired;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<USCSkillAcquiredWidget> SCSkillAcquiredClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<USCBMagicMenuWidget> SCBMagicMenuClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<USCWMagicMenuWidget> SCWMagicMenuClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	USCBMagicMenuWidget* SCBMagicMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	USCWMagicMenuWidget* SCWMagicMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundCue* SFX_MenuON;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundCue* SFX_MenuOFF;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundCue* SFX_ActionDisallowed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD")
	ASCHUD* SCHud;
};
