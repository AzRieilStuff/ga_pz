#include "BaseInventoryItem.h"
#include "BaseItem.h"

bool UBaseInventoryItem::UseItem(AArcher* Target)
{
	return false;
}

bool UBaseInventoryItem::CanUsedBy(AArcher* Character)
{
	return true;
}

bool UBaseInventoryItem::CanUsedOn(AActor* Target)
{
	return true;
}


UBaseInventoryItem::UBaseInventoryItem()
{
	VisualActorClass = ABaseItem::StaticClass();
}
