// Dmytro Chesniuk All Rights Reserved


#include "Widgets/Options/DataObjects/ListDataObject_Base.h"

void UListDataObject_Base::InitDataObject()
{
	OnDataObjectInitialized();
}

void UListDataObject_Base::OnDataObjectInitialized()
{
}

void UListDataObject_Base::NotifyListDataModified(UListDataObject_Base* ModifiedData,
	EOptionsListDataModifyReason ModifiedReason)
{
	OnListDataModified.Broadcast(ModifiedData, ModifiedReason);
}
