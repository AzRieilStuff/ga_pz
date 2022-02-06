#include "BaseHUD.h"

#include "Blueprint/UserWidget.h"


bool ABaseHUD::SetActiveWidget(TSubclassOf<UUserWidget> NewWidget)
{
	if (CurrentWidget != nullptr)
	{
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
	}

	if (NewWidget != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidget);
		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
			return true;
		}
	}

	return false;
}

void ABaseHUD::BeginPlay()
{
	Super::BeginPlay();

	if (StartWidget)
	{
		//SetActiveWidget(StartWidget);
	}

}

void ABaseHUD::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	//SAssignNew(HealthBarWidget, SHealthBarWidget).WidgetOwnerArg(this);
}
