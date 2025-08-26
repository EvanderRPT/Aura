// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "AuraPlayerState.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChanged, int32)
/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AAuraPlayerState();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }
	int32 GetXP() const { return XP; }
	int32 GetPlayerLevel() const { return Level; }

	void AddToXP(int32 NewXP);
	void AddToLevel(int32 NewLevel);
	
	void SetXP(int32 NewXP);
	void SetLevel(int32 NewLevel);

	FOnPlayerStatChanged OnXPChangedDelegate;
	FOnPlayerStatChanged OnLevleChangedDelegate;

protected:

	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

private:
	UPROPERTY(ReplicatedUsing=OnRep_Level, VisibleAnywhere)
	int32 Level = 1;

	UPROPERTY(ReplicatedUsing=OnRep_XP, VisibleAnywhere)
	int32 XP = 1;
	
	UFUNCTION()
	void OnRep_Level(int32 OldLevel);

	UFUNCTION()
	void OnRep_XP(int32 OldXP);

public:
};



