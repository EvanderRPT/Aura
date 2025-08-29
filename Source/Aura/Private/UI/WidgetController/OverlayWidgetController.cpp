// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Player/AuraPlayerState.h"

void UOverlayWidgetController::BroadcastInitialValue()
{

	OnHealthChange.Broadcast(GetAuraAS()->GetHealth());
	OnMaxHealthChange.Broadcast(GetAuraAS()->GetMaxHealth());
	OnManaChange.Broadcast(GetAuraAS()->GetMana());
	OnMaxManaChange.Broadcast(GetAuraAS()->GetMaxMana());
	
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{

	GetAuraPS()->OnXPChangedDelegate.AddUObject(this, &ThisClass::OnXPChanged);
	GetAuraPS()->OnLevelChangedDelegate.AddLambda(
	[this](int32 NewLevel)
	{
		OnPlayerLevelChanged.Broadcast(NewLevel);
	});
		

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuraAS()->GetHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
	{
		OnHealthChange.Broadcast(Data.NewValue);
	});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuraAS()->GetMaxHealthAttribute()).AddLambda( [this](const FOnAttributeChangeData& Data)
	{
		OnMaxHealthChange.Broadcast(Data.NewValue);
	});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuraAS()->GetManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
	{
		OnManaChange.Broadcast(Data.NewValue);
	});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuraAS()->GetMaxManaAttribute()).AddLambda( [this](const FOnAttributeChangeData& Data)
	{
		OnMaxManaChange.Broadcast(Data.NewValue);
	});
	


	if (GetAuraASC()->bStartupAbilitiesGiven)
	{
		BroadcastAbilityInfo();
	}
	else
	{
		GetAuraASC()->AbilitiesGivenDelegate.AddUObject(this, &UAuraWidgetController::BroadcastAbilityInfo);
	}
			
	GetAuraASC()->EffectAssetTags.AddLambda([&](const FGameplayTagContainer& AssetTags)
	{
		for (const FGameplayTag& Tag : AssetTags)
		{

			FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
			if (Tag.MatchesTag(MessageTag))
			{
				if (const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag))
				{
					MessageWidgetRowDelegate.Broadcast(*Row);
				}
			}
		}
	});
	
}




void UOverlayWidgetController::OnXPChanged(int32 NewXP) 
{
	const ULevelUpInfo* LevelUpInfo = GetAuraPS()->LevelUpInfo;

	checkf(LevelUpInfo, TEXT("Unable to find LevelUpInfo, Please fill out AuraPlayerState Blueprint"));

	const int32 Level = LevelUpInfo->FindLevelForXP(NewXP);
	const int32 MaxLevel = LevelUpInfo->LevelUpInformation.Num();

	if (Level <= MaxLevel && Level > 0)
	{
		const int32 LevelUpRequirement = LevelUpInfo->LevelUpInformation[Level].LevelUpRequirement;
		const int32 PreviousLevelUpRequirement = LevelUpInfo->LevelUpInformation[Level - 1].LevelUpRequirement;

		const int32 DeltaLevelRequirement = LevelUpRequirement - PreviousLevelUpRequirement;
		const int32 XPForThisLevel = NewXP - PreviousLevelUpRequirement;

		const float XPBarPercent = static_cast<float>(XPForThisLevel) / static_cast<float>(DeltaLevelRequirement);

		OnXPPercentChangedDelegate.Broadcast(XPBarPercent);
		
	}
}
