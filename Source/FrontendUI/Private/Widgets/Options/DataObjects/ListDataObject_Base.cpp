// Dmytro Chesniuk All Rights Reserved


#include "Widgets/Options/DataObjects/ListDataObject_Base.h"
#include "FrontendSettings/FrontendGameUserSettings.h"

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

	if (bShouldApplyChangeImmediately)
	{
		UFrontendGameUserSettings::Get()->ApplySettings(true);
	}
}
