// Dmytro Chesniuk All Rights Reserved


#include "Widgets/Options/DataObjects/ListDataObject_StringResolution.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Widgets/Options/OptionsDataInteractionHelper.h"
#include "FrontendSettings/FrontendGameUserSettings.h"

#include "FrontendDebugHelper.h"

void UListDataObject_StringResolution::InitResolutionValues()
{
	TArray<FIntPoint> AvailableResolutions;
	
	UKismetSystemLibrary::GetSupportedFullscreenResolutions(AvailableResolutions);

	AvailableResolutions.Sort(
		[](const FIntPoint& A, const FIntPoint& B)->bool
		{
			return A.SizeSquared() < B.SizeSquared();
		}
	);
	
	for (const FIntPoint& Resolution : AvailableResolutions)
	{
		AddDynamicOption(ResToValueString(Resolution), ResToDisplayText(Resolution));
	}

	MaximumAllowedResolution = ResToValueString(AvailableResolutions.Last());
	
	SetDefaultValueFromString(MaximumAllowedResolution);
}

void UListDataObject_StringResolution::OnDataObjectInitialized()
{
	Super::OnDataObjectInitialized();

	if (!TrySetDisplayTextFromStringValue(CurrentStringValue))
	{
		CurrentDisplayText = ResToDisplayText(UFrontendGameUserSettings::Get()->GetScreenResolution());
	}
}

FString UListDataObject_StringResolution::ResToValueString(const FIntPoint& InResolution) const
{
	return FString::Printf(TEXT("(X=%i,Y=%i)"), InResolution.X, InResolution.Y);
}

FText UListDataObject_StringResolution::ResToDisplayText(const FIntPoint& InResolution) const
{
	const FString DisplayString = FString::Printf(TEXT("%i x %i"), InResolution.X, InResolution.Y);
	
	return FText::FromString(DisplayString);
}
