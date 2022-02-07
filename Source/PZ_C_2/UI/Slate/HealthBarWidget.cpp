#include "HealthBarWidget.h"

#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SHealthBarWidget::Construct(const FArguments& Arguments)
{
	WidgetOwner = Arguments._WidgetArea;

	ChildSlot
		.VAlign(VAlign_Top)
		.HAlign(HAlign_Left);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
