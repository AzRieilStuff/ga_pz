#pragma once

#include "GameFramework/HUD.h"
#include "BaseHUD.generated.h"

class UUserWidget;

UCLASS()
class ABaseHUD : public AHUD
{
	GENERATED_BODY()
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI", meta=(AllowPrivateAccess="true"))
	TSubclassOf<UUserWidget> StartWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta=(AllowPrivateAccess="true"))
	UUserWidget* CurrentWidget;

public:
	UFUNCTION(BlueprintCallable)
	bool SetActiveWidget(TSubclassOf<UUserWidget> NewWidget);

	virtual void BeginPlay() override;

	//TSharedPtr<class SHealthBarWidget> HealthBarWidget;

	virtual void PostInitializeComponents() override;
};
