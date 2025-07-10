// Dmytro Chesniuk All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "FrontendTypes/FrontendEnumTypes.h"
#include "Widgets/Widget_ActivatableBase.h"
#include "Widget_OptionsScreen.generated.h"

class UFrontendTabListWidgetBase;
class UOptionsDataRegistry;
class UFrontendCommonListView;
class UWidget_OptionsDetailsView;
class UListDataObject_Base;

UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class FRONTENDUI_API UWidget_OptionsScreen : public UWidget_ActivatableBase
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	
private:
	UOptionsDataRegistry* GetOrCreateDataRegistry();
	
	void OnResetBoundActionTriggered();
	void OnBackBoundActionTriggered();

	UFUNCTION()
	void OnOptionsTabSelected(FName TabID);

	void OnListViewItemHovered(UObject* InHoveredItem, bool bWasHovered);
	void OnListViewItemSelected(UObject* InSelectedItem);

	FString TryGetEntryWidgetClassName(UObject* InOwningListItem) const;

	void OnListViewListDataModified(UListDataObject_Base* ModifiedData, EOptionsListDataModifyReason ModifiedReason);

	UPROPERTY(meta = (BindWidget))
	UFrontendTabListWidgetBase* TabListWidget_OptionsTabs;
	
	UPROPERTY(meta = (BindWidget))
	UFrontendCommonListView* CommonListView_OptionsList;

	UPROPERTY(meta = (BindWidget))
	UWidget_OptionsDetailsView* DetailsView_ListEntryInfo;

	// Handle the creation of data in the options screen.
	UPROPERTY(Transient)
	UOptionsDataRegistry* CreatedOwningDataRegistry;
	
	UPROPERTY(EditDefaultsOnly, Category = "Frontend Options Screen", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle ResetAction;
	
	FUIActionBindingHandle ResetActionHandle;

	UPROPERTY(Transient)
	TArray<UListDataObject_Base*>  ResettableDataArray;

	bool bIsResettingData = false;
};
