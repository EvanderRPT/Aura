// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/Manifest/Inv_ItemManifest.h"
#include "Inv_EquipmentComponent.generated.h"


struct FInv_EquipmentFragment;
class AInv_EquipActor;
class UInv_InventoryItem;
class UInv_InventoryComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class INVENTORYMODULE_API UInv_EquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	void SetOwningSkeletalMesh(USkeletalMeshComponent* OwningMesh);
	void SetIsProxy(bool bProxy) { bIsProxy = bProxy; }
	void InitializeOwner(APlayerController* PlayerController);
	
protected:
	virtual void BeginPlay() override;

private:
	
	TWeakObjectPtr<UInv_InventoryComponent> InventoryComponent;
	TWeakObjectPtr<APlayerController> OwningPlayerController;
	TWeakObjectPtr<USkeletalMeshComponent> OwningSkeletalMesh;
	
	UFUNCTION()
	void OnItemEquipped(UInv_InventoryItem* EquippedItem);

	UFUNCTION()
	void OnItemUnequipped(UInv_InventoryItem* EquippedItem);

	void InitInventoryComponent();
	void InitPlayerController();

	AInv_EquipActor* SpawnEquippedActor(FInv_EquipmentFragment* EquipmentFragment, const FInv_ItemManifest& Manifest, USkeletalMeshComponent* AttachMesh);

	UPROPERTY()
	TArray<TObjectPtr<AInv_EquipActor>> EquippedActors;

	
	AInv_EquipActor* FindEquippedActor(const FGameplayTag& EquipmentTypeTag);
	void RemoveEquippedActor(const FGameplayTag& EquipmentTypeTag);

	UFUNCTION()
	void OnPossessedPawnChange(APawn* OldPawn, APawn* NewPawn);

	bool bIsProxy{false};
};





