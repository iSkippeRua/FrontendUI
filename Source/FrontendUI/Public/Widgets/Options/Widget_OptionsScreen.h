// Dmytro Chesniuk All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Widget_ActivatableBase.h"
#include "Widget_OptionsScreen.generated.h"

class UFrontendTabListWidgetBase;
class UOptionsDataRegistry;
class UFrontendCommonListView;

UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class FRONTENDUI_API UWidget_OptionsScreen : public UWidget_ActivatableBase
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	
private:
	UOptionsDataRegistry* GetOrCreateDataRegistry();
	
	void OnResetBoundActionTriggered();
	void OnBackBoundActionTriggered();

	UFUNCTION()
	void OnOptionsTabSelected(FName TabID);

	void OnListViewItemHovered(UObject* InHoveredItem, bool bWasHovered);
	void OnListViewItemSelected(UObject* InSelectedItem);

	UPROPERTY(meta = (BindWidget))
	UFrontendTabListWidgetBase* TabListWidget_OptionsTabs;
	
	UPROPERTY(meta = (BindWidget))
	UFrontendCommonListView* CommonListView_OptionsList;

	// Handle the creation of data in the options screen.
	UPROPERTY(Transient)
	UOptionsDataRegistry* CreatedOwningDataRegistry;
	
	UPROPERTY(EditDefaultsOnly, Category = "Frontend Options Screen", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle ResetAction;
	
	FUIActionBindingHandle ResetActionHandle;
};
