// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

class UInv_HUDWidget;
class UInv_InventoryComponent;
class UDamageTextComponent;
class USplineComponent;
class UAuraAbilitySystemComponent;
class UAuraInputConfig;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
class IEnemyInterface;
/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	
	AAuraPlayerController();
	virtual void PlayerTick(float DeltaTime) override;

	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter);

	UFUNCTION(BlueprintCallable)
	void ToggleInventory();
	
protected:
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;
private:

	UPROPERTY()
	APawn* ControllerPawn;

	
	UPROPERTY(EditAnywhere, Category=Input)
	TObjectPtr<UInputMappingContext> AuraContext;

	UPROPERTY(EditAnywhere, Category=Input)
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category=Input)
	TObjectPtr<UInputAction> ShiftAction;
	

	void Move(const FInputActionValue& InputActionValue);
	void CursorTrace();
	void ShiftPressed() { bShiftKeyDown = true; }
	void ShiftReleased() { bShiftKeyDown = false; }
	bool bShiftKeyDown = false;

	IEnemyInterface* LastActor;
	IEnemyInterface* ThisActor;
	FHitResult CursorHit;

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);
	
	UPROPERTY(EditDefaultsOnly, Category=Input)
	TObjectPtr<UAuraInputConfig> InputConfig;

	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

	UAuraAbilitySystemComponent* GetASC();

	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime = 0.f;
	float ShortPressThreshold = 0.5f;
	bool bAutoRunning = false;
	bool bTargeting = false;
	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

	void AutoRun();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;



	/*
	 * Inventory System
	 */
	void PrimaryInteract();
	
	void CreateHUDWidget();
	void TraceForItem();

	
	TWeakObjectPtr<UInv_InventoryComponent> InventoryComponent;
	


	UPROPERTY(EditDefaultsOnly, Category="Inventory")
	TObjectPtr<UInputAction> PrimaryInteractAction;

	UPROPERTY(EditDefaultsOnly, Category="Inventory")
	TObjectPtr<UInputAction> ToggleInventoryAction;
	
	
	UPROPERTY(EditDefaultsOnly, Category="Inventory")
	TSubclassOf<UInv_HUDWidget> HUDWidgetClass;

	UPROPERTY()
	TObjectPtr<UInv_HUDWidget> HUDWidget;

	UPROPERTY(EditDefaultsOnly, Category="Inventory")
	double TraceLength;

	UPROPERTY(EditDefaultsOnly, Category="Inventory")
	TEnumAsByte<ECollisionChannel> ItemTraceChannel;

	
	TWeakObjectPtr<AActor> ThisTraceActor;
	TWeakObjectPtr<AActor> LastTraceActor;
};


