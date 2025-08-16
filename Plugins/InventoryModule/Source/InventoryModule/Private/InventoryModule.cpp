// Copyright Epic Games, Inc. All Rights Reserved.

#include "InventoryModule.h"

#define LOCTEXT_NAMESPACE "FInventoryModuleModule"

DEFINE_LOG_CATEGORY(LogInventory);

void FInventoryModuleModule::StartupModule()
{
}

void FInventoryModuleModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FInventoryModuleModule, InventoryModule)