// Dmytro Chesniuk All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/DataObjects/ListDataObject_String.h"
#include "ListDataObject_StringResolution.generated.h"

UCLASS()
class FRONTENDUI_API UListDataObject_StringResolution : public UListDataObject_String
{
	GENERATED_BODY()

public:
	void InitResolutionValues();

protected:
	virtual void OnDataObjectInitialized() override;

private:
	FString ResToValueString(const FIntPoint& InResolution) const;
	FText ResToDisplayText(const FIntPoint& InResolution) const;

	FString MaximumAllowedResolution;

public:
	FORCEINLINE FString GetMaximumAllowedResolution() const { return MaximumAllowedResolution; }
};
