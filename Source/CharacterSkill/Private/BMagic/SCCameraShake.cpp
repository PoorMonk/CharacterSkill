// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/BMagic/SCCameraShake.h"

USCCameraShake::USCCameraShake()
{
	OscillationDuration = 0.25f;
	RotOscillation.Pitch.Amplitude = 5.f;
	RotOscillation.Pitch.Frequency = 50.f;
	RotOscillation.Yaw.Amplitude = 5.f;
	RotOscillation.Yaw.Frequency = 50.f;

	LocOscillation.X.Amplitude = 50.f;
	LocOscillation.X.Frequency = 1.f;
	LocOscillation.Z.Amplitude = 10.f;
	LocOscillation.Z.Frequency = 50.f;
}
