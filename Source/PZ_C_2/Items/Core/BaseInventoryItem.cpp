#include "BaseInventoryItem.h"
#include "BaseItem.h"

void UBaseInventoryItem::SetAmount(const int NewAmount)
{
	Amount = FMath::Clamp(
		NewAmount,
		0,
		GetStackLimit()
	);
}

void UBaseInventoryItem::ModifyAmount(const int ModValue)
{
	return SetAmount(Amount + ModValue);
}

UBaseInventoryItem::UBaseInventoryItem()
{
	VisualActorClass = ABaseItem::StaticClass();
	Amount = 1;
	Icon = nullptr;
}

/*
UBaseInventoryItem::UBaseInventoryItem(EInventorySlot Slot = EInventorySlot::None,
                                       TSubclassOf<ABaseItem> ActorClass = nullptr, int32 InitAmount = 1,
                                       FString ItemName = FString(""), UTexture* IconTexture = nullptr)
	: Amount(InitAmount), VisualActorClass(ActorClass), Icon(IconTexture), Name(ItemName), SlotType(Slot)
{
}
*/

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
