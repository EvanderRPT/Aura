// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#define CUSTOM_DEPTH_RED 250
#define ECC_Projectile ECollisionChannel::ECC_GameTraceChannel1

namespace AuraDebug
{
	inline  void Print(const FString& Message) 
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, Message);
		}
	}
}
