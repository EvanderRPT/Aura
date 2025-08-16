#pragma once
#include "GameplayEffectTypes.h"

USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{	
	GENERATED_BODY()
public:
	bool IsCriticalHit() const { return bIsBlockedHit; }
	bool IsBlockedHit() const { return bIsBlockedHit; }
	void SetIsCriticalHit(bool bInIsCriticalHit ) { bIsBlockedHit = bInIsCriticalHit; }
	void SetIsBlockedHit(bool bInIsBlockedHit) { bIsBlockedHit = bInIsBlockedHit; }
	
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FAuraGameplayEffectContext::GetScriptStruct();
	}

	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;

protected:

	UFUNCTION()
	bool bIsBlockedHit = false;
	
	UPROPERTY()
	bool bIsCriticalHit = false;
};