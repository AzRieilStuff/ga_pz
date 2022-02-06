#include "HealthBarWidget.h"

#include "SlateOptMacros.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "PZ_C_2/Characters/Archer.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SHealthBarWidget::Construct(const FArguments& Arguments)
{
	WidgetOwner = Arguments._WidgetArea;

	// todo remove
	ChildSlot
		.VAlign(VAlign_Top)
		.HAlign(HAlign_Left)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			[
				SNew(SButton)
				.Content()
				[
					SNew(STextBlock)
					.Text(FText::FromString("Click me bro"))
				]
				.OnClicked_Lambda([this, Arguments]
				{
					AArcher* Player = Cast<AArcher>(
						UGameplayStatics::GetPlayerPawn(Arguments._WorldContex.Get(), 0));
					if (Player)
					{
						Player->SetRandomColor();
					}
					return FReply::Handled();
				})
			]
		];
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
