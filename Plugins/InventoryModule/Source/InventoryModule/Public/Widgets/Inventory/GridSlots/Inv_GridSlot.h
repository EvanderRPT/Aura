// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VisualizeTexture.h"
#include "Blueprint/UserWidget.h"
#include "Inv_GridSlot.generated.h"

class UInv_ItemPopUp;
class UInv_InventoryItem;
class UImage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGridSlotEvent, int32, GridIndex, const FPointerEvent& ,MouseEvent);


UENUM(BlueprintType)
enum class EInv_GridSlotState : uint8
{
	Unoccupied,
	Occupied,
	Selected,
	GrayedOut
};

UCLASS()
class INVENTORYMODULE_API UInv_GridSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	
	int32 GetTileIndex() const { return TileIndex; }
	void SetTileIndex(const int32 TileIndexNum) { this->TileIndex = TileIndexNum; }
	EInv_GridSlotState GetGridSlotState() const { return GetSlotState; }
	TWeakObjectPtr<UInv_InventoryItem> GetInventoryItem() const { return InventoryItem; }
	void SetInventoryItem(UInv_InventoryItem* Item);
	int32 GetIndex() const { return TileIndex; }
	void SetIndex(const int32 Index) { TileIndex = Index; }
	int32 GetUpperLeftIndex() const { return UpperLeftIndex; }
	void SetUpperLeftIndex(const int32 Index) { UpperLeftIndex = Index; }
	void SetStackCount(const int32 Count) { StackCount = Count; }
	int32 GetStackCount() const { return StackCount; }
	bool IsAvailable() const { return bAvailable; }
	void SetAvailable(const bool Available) { bAvailable = Available; }
	void SetItemPopUp(UInv_ItemPopUp* PopUp);
	UInv_ItemPopUp* GetItemPopUp() const;
	
	void SetOccupiedTexture();
	void SetUnoccupiedTexture();
	void SetSelectedTexture();
	void SetGrayedOutTexture();

	FGridSlotEvent GridSlotClicked;
	FGridSlotEvent GridSlotHovered;
	FGridSlotEvent GridSlotUnHovered;
	
private:

	int32 TileIndex{INDEX_NONE};
	int32 UpperLeftIndex {INDEX_NONE};
	int32 StackCount{0};
	bool bAvailable{true};
	TWeakObjectPtr<UInv_InventoryItem> InventoryItem;
	TWeakObjectPtr<UInv_ItemPopUp> ItemPopUp;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_GridSlot;

	UPROPERTY(EditAnywhere, Category = Inventory)
	FSlateBrush Brush_Unoccupied;

	UPROPERTY(EditAnywhere, Category = Inventory)
	FSlateBrush Brush_Occupied;

	UPROPERTY(EditAnywhere, Category = Inventory)
	FSlateBrush Brush_Selected;

	UPROPERTY(EditAnywhere, Category = Inventory)
	FSlateBrush Brush_GrayedOut;

	EInv_GridSlotState GetSlotState;

	UFUNCTION()
	void OnItemPopUpDestruct(UUserWidget* Menu);
};
