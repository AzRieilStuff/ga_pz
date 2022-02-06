#pragma once


class SHealthBarWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SHealthBarWidget)
		{
		}

		SLATE_ARGUMENT(TWeakObjectPtr<class ABaseHUD>, WidgetArea)

		SLATE_ARGUMENT(TWeakObjectPtr<class UWorld>, WorldContex)

	SLATE_END_ARGS()

	void Construct(const FArguments& Arguments);


private:
	TWeakObjectPtr<class ABaseHUD> WidgetOwner;
};
