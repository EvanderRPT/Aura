#include  "AuraGameplayTags.h"

#include "AuraAssetManager.h"
#include "GameplayTagsManager.h"

FAuraGameplayTags FAuraGameplayTags::GameplayTags;

void FAuraGameplayTags::InitializeNativeGameplayTags()
{

	/*
	 * Attributes Primary
	 */
	GameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Strength"), FString("Increases physical damage"));
	GameplayTags.Attributes_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Intelligence"), FString("Increases magical damage"));
	GameplayTags.Attributes_Primary_Resilience =  UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Resilience"), FString("Increases Armor and Armor Penetration "));
	GameplayTags.Attributes_Primary_Vigor =  UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Vigor"), FString("Increases Health"));

	/*
	 * Attributes Secondary
	 */
	GameplayTags.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Armor"), FString("Reduce damage taken, improves Block Chance"));
	
	GameplayTags.Attributes_Secondary_ArmorPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.ArmorPenetration"), 
		FString("Ignores a portion of target's armor when dealing damage")
	);

	GameplayTags.Attributes_Secondary_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.BlockChance"), 
		FString("Chance to block incoming attacks, reducing their damage")
	);

	GameplayTags.Attributes_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitChance"), 
		FString("Chance to land a critical hit, dealing extra damage")
	);

	GameplayTags.Attributes_Secondary_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitDamage"), 
		FString("Bonus damage dealt when a critical hit occurs")
	);

	GameplayTags.Attributes_Secondary_CriticalHitResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitResistance"), 
		FString("Reduces the chance or damage from incoming critical hits")
	);

	GameplayTags.Attributes_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.HealthRegeneration"), 
		FString("Amount of health restored over time")
	);

	GameplayTags.Attributes_Secondary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.ManaRegeneration"), 
		FString("Amount of mana restored over time")
	);

	GameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxHealth"), 
		FString("Maximum amount of health the character can have")
	);

	GameplayTags.Attributes_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxMana"), 
		FString("Maximum amount of mana the character can have")
	);

	/*
	 * Input Tags
	 */
	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("InputTag.LMB"), 
	FString("Input Tag for Left mouse Button")
	);
	
	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("InputTag.RMB"), 
	FString("Input Tag for Right mouse Button")
	);

	GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("InputTag.1"), 
	FString("Input Tag for 1 key Button")
	);
	GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("InputTag.2"), 
	FString("Input Tag for 2 key Button")
	);
	GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("InputTag.3"), 
	FString("Input Tag for 3 key Button")
	);
	GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("InputTag.4"), 
	FString("Input Tag for 4 key Button")
	);


	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Damage"), 
	FString("Damage")
	);

	/*
	 * Damage Types
	 */
	GameplayTags.Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Damage.Fire"), 
	FString("fire damage types")
	);
	GameplayTags.Damage_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Lightning"), 
		FString("Lightning damage types")
		);
	GameplayTags.Damage_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Damage.Arcane"), 
	FString("Arcane damage types")
	);
	GameplayTags.Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Physical"), 
		FString("Physical damage types")
		);


	/*
	 * Resistance
	 */
	GameplayTags.Attributes_Resistance_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance.Arcane"), 
		FString("Resistance to Arcane damage types")
		);
	GameplayTags.Attributes_Resistance_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance.Fire"), 
		FString("Resistance to Fire damage types")
		);
	GameplayTags.Attributes_Resistance_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Resistance.Lightning"), 
	FString("Resistance to Lightning damage types")
	);
	GameplayTags.Attributes_Resistance_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Resistance.Physical"), 
	FString("Resistance to Physical damage types")
	);


	/*
	 * Map of Damage Type to Resistance
	 */
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Arcane, GameplayTags.Attributes_Resistance_Arcane);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Physical, GameplayTags.Attributes_Resistance_Physical);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Lightning, GameplayTags.Attributes_Resistance_Lightning);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Fire, GameplayTags.Attributes_Resistance_Fire);

	
	/*
	 * Effects
	 */
	GameplayTags.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Effects.HitReact"), 
	FString("Hit React")
	);

	/**
	 * Abilities
	 */
	GameplayTags.Abilities_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Abilities.Attack"), 
	FString("Attack Abilities")
	);
}
