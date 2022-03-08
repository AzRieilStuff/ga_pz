#include "BaseInventoryItem.h"
#include "BaseItem.h"

UBaseInventoryItem::UBaseInventoryItem()
{
	VisualActorClass = ABaseItem::StaticClass();
	Amount = 1;
	Icon = nullptr;
}

bool UBaseInventoryItem::UseItem(AArcher* Target)
{
	return false;
}

bool UBaseInventoryItem::CanUsedBy(AArcher* Character) const
{
	return true;
}

bool UBaseInventoryItem::CanUsedOn(AActor* Target) const
{
	return true;
}

ABaseItem* UBaseInventoryItem::GetItemDefaultObject() const
{
	return VisualActorClass ? VisualActorClass.GetDefaultObject() : nullptr;
}
