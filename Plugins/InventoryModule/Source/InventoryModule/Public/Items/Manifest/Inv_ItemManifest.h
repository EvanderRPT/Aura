#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Items/Fragments/Inv_ItemFragment.h"
#include "Types/Inv_GridTypes.h"
#include "StructUtils/InstancedStruct.h"
#include "Inv_ItemManifest.generated.h"

/*
 * The Item Manifest contains all of the necessary data
 * for creating a new Inventory Item
 */

class UInv_CompositeBase;
struct FInv_ItemFragment;
enum class EInv_ItemCategory : uint8;

USTRUCT(BlueprintType)
struct INVENTORYMODULE_API FInv_ItemManifest
{
	GENERATED_BODY()


	TArray<TInstancedStruct<FInv_ItemFragment>>& GetFragmentsMutable()  { return  Fragments; }
	UInv_InventoryItem* Manifest(UObject* NewOuter);
	EInv_ItemCategory GetItemCategory() const{ return ItemCategory; }
	FGameplayTag GetItemType() const{ return ItemType; }
	void AssimilateInventoryFragments(UInv_CompositeBase* Composite) const;


	template<typename T> requires std::derived_from<T, FInv_ItemFragment>
	const T* GetFragmentByTag(const FGameplayTag& FragmentTag) const;


	template<typename T> requires std::derived_from<T, FInv_ItemFragment>
	const T* GetFragmentByType() const;

	template<typename T> requires std::derived_from<T, FInv_ItemFragment>
	T* GetFragmentByTypeMutable();

	template<typename T> requires std::derived_from<T, FInv_ItemFragment>
	TArray<const T*> GetAllFragmentsOfType() const;

	void SpawnPickupActor(const UObject* WorldContextObject, const FVector& SpawnLocation, const FRotator& SpawnRotation);
	
private:

	UPROPERTY(EditAnywhere, Category="Inventory", meta=(ExcludeBaseStruct))
	TArray<TInstancedStruct<FInv_ItemFragment>> Fragments;

	UPROPERTY(EditAnywhere, Category="Inventory")
	EInv_ItemCategory ItemCategory {EInv_ItemCategory::None};

	UPROPERTY(EditAnywhere, Category="Inventory", meta=(Categories="GameItems"))
	FGameplayTag ItemType;

	UPROPERTY(EditAnywhere, Category="Inventory")
	TSubclassOf<AActor> PickupActorClass;

	void ClearFragments();
	
};

template<typename T>  requires std::derived_from<T, FInv_ItemFragment>
const T* FInv_ItemManifest::GetFragmentByTag(const FGameplayTag& FragmentTag) const
{
	for (const TInstancedStruct<FInv_ItemFragment>& Fragment : Fragments)
	{
		if (const T* FragmentPtr = Fragment.GetPtr<T>())
		{
			if (!FragmentPtr->GetFragmentTag().MatchesTagExact(FragmentTag)) continue;
			
			return FragmentPtr;
		}
	}
	return  nullptr;
}

template <typename T> requires std::derived_from<T, FInv_ItemFragment>
const T* FInv_ItemManifest::GetFragmentByType() const
{
	for (const TInstancedStruct<FInv_ItemFragment>& Fragment : Fragments)
	{
		if (const T* FragmentPtr = Fragment.GetPtr<T>())
		{
			return FragmentPtr;
		}
	}
	return  nullptr;
}

template <typename T> requires std::derived_from<T, FInv_ItemFragment>
T* FInv_ItemManifest::GetFragmentByTypeMutable()
{
	for ( TInstancedStruct<FInv_ItemFragment>& Fragment : Fragments)
	{
		if (T* FragmentPtr = Fragment.GetMutablePtr<T>())
		{
			return FragmentPtr;
		}
	}
	return  nullptr;
}

template <typename T> requires std::derived_from<T, FInv_ItemFragment>
TArray<const T*> FInv_ItemManifest::GetAllFragmentsOfType() const
{
	TArray<const T*> Result;
	for (const TInstancedStruct<FInv_ItemFragment>& Fragment : Fragments)
	{
		if (const T* FragmentPtr = Fragment.GetPtr<T>())
		{
			Result.Add(FragmentPtr);
		}
	}
	return Result;
}

