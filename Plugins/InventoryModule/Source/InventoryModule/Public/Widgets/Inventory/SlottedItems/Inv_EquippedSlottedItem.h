// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Inv_SlottedItem.h"
#include "Inv_EquippedSlottedItem.generated.h"

class UInv_EquippedSlottedItem;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEquippedSlottedItemClicked, UInv_EquippedSlottedItem*, SlottedItem);


UCLASS()
class INVENTORYMODULE_API UInv_EquippedSlottedItem : public UInv_SlottedItem
{
	GENERATED_BODY()

public:

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	void SetEquipmentTypeTag(const FGameplayTag& Tag) { this->EquipmentTypeTag = Tag; }
	FGameplayTag GetEquipmentTypeTag() const { return EquipmentTypeTag; }


	FEquippedSlottedItemClicked OnEquippedSlottedItemClicked;

private:
	
	UPROPERTY()
	FGameplayTag EquipmentTypeTag;
};
