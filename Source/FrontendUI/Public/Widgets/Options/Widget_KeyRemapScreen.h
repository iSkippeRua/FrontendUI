// Dmytro Chesniuk All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Widget_ActivatableBase.h"
#include "CommonInputTypeEnum.h"
#include "Widget_KeyRemapScreen.generated.h"

class UCommonRichTextBlock;
class FKeyRemapScreenInputPreprocessor;

UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class FRONTENDUI_API UWidget_KeyRemapScreen : public UWidget_ActivatableBase
{
	GENERATED_BODY()

public:
	void SetDesiredInputTypeToFilter(ECommonInputType InDesiredInputType);

	DECLARE_DELEGATE_OneParam(FOnKeyRemapScreenKeyPressedDelegate, const FKey& /* PressedKey */)
	FOnKeyRemapScreenKeyPressedDelegate OnKeyRemapScreenKeyPressed;

	DECLARE_DELEGATE_OneParam(FOnKeyRemapScreenKeySelectCanceledDelegate, const FString& /* CanceledReason */)
	FOnKeyRemapScreenKeySelectCanceledDelegate OnKeyRemapScreenKeySelectCanceled;
	
protected:
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	
private:
	void OnValidKeyPressedDetected(const FKey& PressedKey);
	void OnKeySelectCanceled(const FString& CanceledReason);

	// This function should be used to make sure that the input key is captured properly before deactivating the widget
	void RequestDeactivateWidget(TFunction<void()> PreDeactivateCallback);
	
	UPROPERTY(meta = (BindWidget))
	UCommonRichTextBlock* CommonRichText_RemapMessage;

	TSharedPtr<FKeyRemapScreenInputPreprocessor> CachedInputPreprocessor;

	ECommonInputType CachedDesiredInputType;
	
};
