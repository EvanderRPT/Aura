#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h"

USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	bool IsCriticalHit() const { return bIsCriticalHit; }
	bool IsBlockedHit()  const { return bIsBlockedHit;  }

	void SetIsCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }
	void SetIsBlockedHit(bool bInIsBlockedHit)   { bIsBlockedHit  = bInIsBlockedHit;  }

	virtual UScriptStruct* GetScriptStruct() const override
	{
		return StaticStruct();
	}

	// Custom net-serialize: remember to enable WithNetSerializer in traits below
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;

	virtual FAuraGameplayEffectContext* Duplicate() const override
	{
		FAuraGameplayEffectContext* NewContext = new FAuraGameplayEffectContext();
		*NewContext = *this;

		if (GetHitResult())
		{
			// deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), /*bReplace=*/true);
		}
		return NewContext;
	}

protected:
	UPROPERTY()
	bool bIsBlockedHit = false;

	UPROPERTY()
	bool bIsCriticalHit = false;
};

// NOTE: The base here MUST be your struct type, not FGameplayEffectContext.
template<>
struct TStructOpsTypeTraits<FAuraGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FAuraGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};
