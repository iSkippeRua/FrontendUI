// Dmytro Chesniuk All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/DataObjects/ListDataObject_Base.h"
#include "CommonInputTypeEnum.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "ListDataObject_KeyRemap.generated.h"

class UEnhancedInputUserSettings;
class UEnhancedPlayerMappableKeyProfile;

UCLASS()
class FRONTENDUI_API UListDataObject_KeyRemap : public UListDataObject_Base
{
	GENERATED_BODY()

public:
	void InitKeyRemapData(UEnhancedInputUserSettings* InOwningInputUserSettings, UEnhancedPlayerMappableKeyProfile* InKeyProfile, ECommonInputType InDesiredInputKeyType, const FPlayerKeyMapping& InOwningPlayerKeyMapping);

	FSlateBrush GetIconFromCurrentKey() const;

	void FindNewInputKey(const FKey& InNewKey);

	virtual bool HasDefaultValue() const override;
	virtual bool CanResetBackToDefaultValue() const override;
	virtual bool TryResetBackToDefaultValue() override;
	
private:
	FPlayerKeyMapping* GetOwningKeyMapping() const;
	
	UPROPERTY(Transient)
	UEnhancedInputUserSettings* CachedOwningInputUserSettings;

	UPROPERTY(Transient)
	UEnhancedPlayerMappableKeyProfile* CachedOwningKeyProfile;

	ECommonInputType CachedDesiredInputKeyType;

	FName CachedOwningMappingName;

	EPlayerMappableKeySlot CachedOwningMappableKeySlot;

public:
	FORCEINLINE ECommonInputType GetDesiredInputKeyType() const { return CachedDesiredInputKeyType; }
};
