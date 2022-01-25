// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class PZ_C_2_API UUserWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
	class APlayerControllerBase* PlayerController;
public:
	UPROPERTY(BlueprintReadWrite)
	bool bEnableCursor;

	virtual void BeginDestroy() override;

	virtual void SetVisibility(ESlateVisibility InVisibility) override;

	virtual bool Initialize() override;
};
