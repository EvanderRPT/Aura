// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "MovieSceneTracksComponentTypes.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "Input/AuraInputComponent.h"
#include "GameFramework/Character.h"
#include "UI/Widget/DamageTextComponent.h"
#include "Interaction/EnemyInterface.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Interaction/Inv_Highlightable.h"
#include "InventoryManagement/Components/Inv_InventoryComponent.h"
#include "Items/Components/Inv_ItemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/HUD/Inv_HUDWidget.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	
	TraceLength = 500.0;
	ItemTraceChannel = ECC_GameTraceChannel1;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
	AutoRun();
	TraceForItem();
}

void AAuraPlayerController::ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit)
{
	if (IsValid(TargetCharacter) && DamageTextComponentClass && IsLocalController())
	{
		UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageText->SetDamageText(DamageAmount, bBlockedHit, bCriticalHit);
	}
}


void AAuraPlayerController::AutoRun()
{
	if (!bAutoRunning) return;
	ControllerPawn = ControllerPawn == nullptr ? GetPawn<APawn>() : ControllerPawn;

	if (ControllerPawn)
	{
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControllerPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
		ControllerPawn->AddMovementInput(Direction);

		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if (DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}




void AAuraPlayerController::CursorTrace()
{

	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_CursorTrace))
	{
		if (LastActor) LastActor->UnHighlightActor();
		if (ThisActor) ThisActor->UnHighlightActor();
		LastActor = nullptr;
		ThisActor = nullptr;
		return;
	}
	
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());

	/*
	 * Line trace from cursor. There are several scenarios:
	 *	A. LastActor is null && ThisActor is null
	 *		-  Do nothing.
	 *	B. LastActor is null && ThisActor is valid
	 *		- Highlight ThisActor
	 *	C. LastActor is valid && ThisActor is null
	 *		- Unhighlight LastActor
	 *	D. Both actors are valid, but LastActor != ThisActor
	 *		- UnHighlight LastActor, and Highlight ThisActor
	 *	E. Both actors are valid, and are the same actor
	 *		- Do nothing
	 */

	/*if (LastActor == nullptr)
	{
		if (ThisActor == nullptr) //A. LastActor is null && ThisActor is null
		{
			
		}
		else //B. LastActor is null && ThisActor is valid
		{
			
			ThisActor->HighlightActor();
		}
	}
	else
	{
		if (ThisActor == nullptr) // C. LastActor is valid && ThisActor is null
		{
			LastActor->UnHighlightActor();
		}
		else
		{
			if (ThisActor != LastActor) // D. Both actors are valid, but LastActor != ThisActor
			{
				LastActor->UnHighlightActor();
				ThisActor->HighlightActor();
			}
			else // E. Both actors are valid, and are the same actor
			{
				
			}
		}
	}*/

	
	if (LastActor != ThisActor)
	{
		if (LastActor) LastActor->UnHighlightActor();
		if (ThisActor) ThisActor->HighlightActor();
	}
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputPressed))
	{
		return;
	}
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		bTargeting = ThisActor ? true : false;
		bAutoRunning = false;
	}
	if (GetASC()) GetASC()->AbilityInputTagPressed(InputTag);
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputReleased))
	{
		return;
	}
	
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);

		return;
	}

	if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);
	
	
	if (!bTargeting || !bShiftKeyDown)
	{
		ControllerPawn = ControllerPawn == nullptr ? GetPawn<APawn>() : ControllerPawn;

		if (FollowTime <= ShortPressThreshold && ControllerPawn)
		{
			if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControllerPawn->GetActorLocation(), CachedDestination))
			{
				Spline->ClearSplinePoints();
				for (const FVector& PointLoc : NavPath->PathPoints)
				{
					Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
				}
				if (NavPath->PathPoints.Num() > 0)
				{
					CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
				}
				bAutoRunning = true;
			}
			if (GetASC() && !GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputPressed))
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ClickNiagaraSystem, CachedDestination);
			}
		}
		FollowTime = 0.0f;
		bTargeting = false;
	}
	
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputHeld))
	{
		return;
	}

	
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_RMB))
	{
		if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
		return;
	}
	
	if (bTargeting || bShiftKeyDown)
	{
		if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
		
	}
	else
	{
		FollowTime += GetWorld()->GetDeltaSeconds();
		
		if (CursorHit.bBlockingHit)
		{
			CachedDestination = CursorHit.ImpactPoint;
		}

		if (APawn* PlayerPawn = GetPawn())
		{
			const FVector WorldDirection = (CachedDestination - PlayerPawn->GetActorLocation()).GetSafeNormal();
			PlayerPawn->AddMovementInput(WorldDirection);
		}
	}
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if (AuraAbilitySystemComponent == nullptr)
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return AuraAbilitySystemComponent;
}


void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(AuraContext);
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(AuraContext, 0);
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);


	InventoryComponent = FindComponentByClass<UInv_InventoryComponent>();
	
	CreateHUDWidget();
	
}



void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &ThisClass::ShiftPressed);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &ThisClass::ShiftReleased);
	AuraInputComponent->BindAction(PrimaryInteractAction, ETriggerEvent::Started, this, &ThisClass::PrimaryInteract);
	AuraInputComponent->BindAction(ToggleInventoryAction, ETriggerEvent::Started, this, &ThisClass::ToggleInventory);
	AuraInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
	
	
	
}


void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputPressed))
	{
		return;
	}
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation =  GetControlRotation();
	const FRotator YawRotation (0.f, Rotation.Yaw, 0.0f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);

	ControllerPawn = ControllerPawn == nullptr ? GetPawn<APawn>() : ControllerPawn;
	if (ControllerPawn)
	{
		ControllerPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControllerPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}



/*
 * Inventory System
 */

void AAuraPlayerController::ToggleInventory()
{
	if (!InventoryComponent.IsValid()) return;
	InventoryComponent->ToggleInventoryMenu();

	if (InventoryComponent->IsMenuOpen())
	{
		HUDWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		HUDWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void AAuraPlayerController::PrimaryInteract()
{
	if (!ThisTraceActor.IsValid()) return;

	UInv_ItemComponent* ItemComp = ThisTraceActor->FindComponentByClass<UInv_ItemComponent>();
	if (!IsValid(ItemComp) || !InventoryComponent.IsValid()) return;
	InventoryComponent->TryAddItem(ItemComp);
}

void AAuraPlayerController::CreateHUDWidget()
{
	if (!IsLocalController()) return;

	HUDWidget = CreateWidget<UInv_HUDWidget>(this, HUDWidgetClass);
	if (IsValid(HUDWidget))
	{
		HUDWidget->AddToViewport();
	}
}

void AAuraPlayerController::TraceForItem()
{
    APawn* MyPawn = GetPawn();
    if (!IsValid(MyPawn)) return;

    FVector PlayerLocation = MyPawn->GetActorLocation();
    float DetectionRadius = 200.f; // adjust as needed

    // Create collision shape (sphere around player)
    FCollisionShape Sphere = FCollisionShape::MakeSphere(DetectionRadius);

    TArray<FOverlapResult> Overlaps;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(MyPawn); // don’t detect yourself

    bool bHit = GetWorld()->OverlapMultiByChannel(
        Overlaps,
        PlayerLocation,
        FQuat::Identity,
        ItemTraceChannel, // same channel you use for items
        Sphere,
        QueryParams
    );

    AActor* NearestItem = nullptr;
    float ClosestDistSq = FLT_MAX;

    if (bHit)
    {
        for (auto& Result : Overlaps)
        {
            AActor* OverlappedActor = Result.GetActor();
            if (!IsValid(OverlappedActor)) continue;

            float DistSq = FVector::DistSquared(PlayerLocation, OverlappedActor->GetActorLocation());
            if (DistSq < ClosestDistSq)
            {
                ClosestDistSq = DistSq;
                NearestItem = OverlappedActor;
            }
        }
    }

    LastTraceActor = ThisTraceActor;
    ThisTraceActor = NearestItem;

    // Hide old
    if (!ThisTraceActor.IsValid() && IsValid(HUDWidget))
    {
        HUDWidget->HidePickupMessage();
    }

    if (ThisTraceActor == LastTraceActor) return;

    // Highlight new item
    if (ThisTraceActor.IsValid())
    {
        if (UActorComponent* Highlightable = ThisTraceActor->FindComponentByInterface(UInv_Highlightable::StaticClass()); IsValid(Highlightable))
        {
            IInv_Highlightable::Execute_Highlight(Highlightable);
        }

        if (UInv_ItemComponent* ItemComponent = ThisTraceActor->FindComponentByClass<UInv_ItemComponent>())
        {
            if (IsValid(HUDWidget))
                HUDWidget->ShowPickupMessage(ItemComponent->GetPickupMessage());
        }
    }

    // Unhighlight old item
    if (LastTraceActor.IsValid())
    {
        if (UActorComponent* Highlightable = LastTraceActor->FindComponentByInterface(UInv_Highlightable::StaticClass()); IsValid(Highlightable))
        {
            IInv_Highlightable::Execute_UnHighlight(Highlightable);
        }
    }
}












