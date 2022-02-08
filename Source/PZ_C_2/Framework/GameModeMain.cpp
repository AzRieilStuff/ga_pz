// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameModeMain.h"

#include "EngineUtils.h"
#include "PlayerControllerBase.h"
#include "PlayerStateMain.h"
#include "PZ_C_2/Characters/Archer.h"
#include "Engine/AssetManager.h"
#include "Engine/ObjectLibrary.h"
#include "Engine/StreamableManager.h"

AGameModeMain::AGameModeMain()
{
	PlayerStateClass = APlayerStateMain::StaticClass();
	DefaultPawnClass = AArcher::StaticClass();
	PlayerControllerClass = APlayerControllerBase::StaticClass();
}

void AGameModeMain::BeginPlay()
{
	Super::BeginPlay();

	const FName TraceTag("Debug");
	GetWorld()->DebugDrawTraceTag = TraceTag;

	// load env
	auto ObjectLib = UObjectLibrary::CreateLibrary(UStaticMesh::StaticClass(), false, GIsEditor);
	ObjectLib->AddToRoot();

	ObjectLib->LoadAssetDataFromPath(TEXT("/Game/Environment"));
	ObjectLib->LoadAssetsFromAssetData();

	TArray<FAssetData> AssetItems;
	ObjectLib->GetAssetDataList(AssetItems);

	bool bIsAsync = !FMath::RandRange(0, 1);
	FStreamableManager& StreamManager = UAssetManager::GetStreamableManager();

	for (int32 i = 0; i < AssetItems.Num(); i++)
	{
		FSoftObjectPath Path = AssetItems[i].ToSoftObjectPath();
		if (!bIsAsync)
		{
			TSharedPtr<FStreamableHandle> Result = StreamManager.RequestSyncLoad(Path, false);

			if (Result.IsValid())
			{
				FStreamableHandle* AssetHandle = Result.Get();
				UStaticMesh* Mesh = Cast<UStaticMesh>(AssetHandle->GetLoadedAsset());

				if (IsValid(Mesh))
				{
					FString Debug = FString::Printf(TEXT("%s loaded sync. success."), *Mesh->GetFName().ToString());
					GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, Debug);
				}
				AssetHandle->ReleaseHandle();
			}
		}
		else
		{
			FAssetData AssetData = AssetItems[i];
			StreamManager.RequestAsyncLoad(Path, FStreamableDelegate::CreateLambda([AssetData]
			{
				if (AssetData.IsAssetLoaded())
				{
					UStaticMesh* Mesh = Cast<UStaticMesh>(AssetData.GetAsset());
					
					if (IsValid(Mesh))
					{
						FString Debug = FString::Printf(TEXT("%s loaded async. success."), *Mesh->GetFName().ToString());
						GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, Debug);
					}
				}
			}));
		}
	}
}
