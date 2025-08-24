// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValue()
{
	if (const UAuraAttributeSet* AS = Cast<UAuraAttributeSet>(AttributeSet))
	{
		OnHealthChange.Broadcast(AS->GetHealth());
		OnMaxHealthChange.Broadcast(AS->GetMaxHealth());
		OnManaChange.Broadcast(AS->GetMana());
		OnMaxManaChange.Broadcast(AS->GetMaxMana());
	}
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	if (const UAuraAttributeSet* AS = Cast<UAuraAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
		{
			OnHealthChange.Broadcast(Data.NewValue);
		});
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetMaxHealthAttribute()).AddLambda( [this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChange.Broadcast(Data.NewValue);
		});
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
		{
			OnManaChange.Broadcast(Data.NewValue);
		});
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetMaxManaAttribute()).AddLambda( [this](const FOnAttributeChangeData& Data)
		{
			OnMaxManaChange.Broadcast(Data.NewValue);
		});

		if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
		{
			if (AuraASC->bStartupAbilitiesGiven)
			{
				OnInitializeStartupAbilities(AuraASC);
			}
			else
			{
				AuraASC->AbilitiesGivenDelegate.AddUObject(this, &ThisClass::OnInitializeStartupAbilities);
			}
			
			AuraASC->EffectAssetTags.AddLambda([&](const FGameplayTagContainer& AssetTags)
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

		
	}
		
}

void UOverlayWidgetController::OnInitializeStartupAbilities(UAuraAbilitySystemComponent* AuraASC)
{
	//TODO: Get Information about all given abilities, look up their Ability Info, and broadcast it to widgets
	if (!AuraASC->bStartupAbilitiesGiven) return;

	
}



