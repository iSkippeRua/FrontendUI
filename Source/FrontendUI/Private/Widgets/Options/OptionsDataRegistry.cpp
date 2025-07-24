// Dmytro Chesniuk All Rights Reserved


#include "Widgets/Options/OptionsDataRegistry.h"
#include "Widgets/Options/DataObjects/ListDataObject_Collection.h"
#include "Widgets/Options/DataObjects/ListDataObject_String.h"
#include "Widgets/Options/OptionsDataInteractionHelper.h"
#include "FrontendSettings/FrontendGameUserSettings.h"
#include "FrontendFunctionLibrary.h"
#include "FrontendGameplayTags.h"
#include "Widgets/Options/DataObjects/ListDataObject_Scalar.h"
#include "Widgets/Options/DataObjects/ListDataObject_StringResolution.h"
#include "Internationalization/StringTableRegistry.h"
#include "EnhancedInputSubsystems.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "Widgets/Options/DataObjects/ListDataObject_KeyRemap.h"

#include "FrontendDebugHelper.h"

#define MAKE_OPTIONS_DATA_CONTROL(SetterOrGetterFuncName) \
	MakeShared<FOptionsDataInteractionHelper>(GET_FUNCTION_NAME_STRING_CHECKED(UFrontendGameUserSettings, SetterOrGetterFuncName))

#define GET_DESCRIPTION(InKey) LOCTABLE("/Game/UI/StringTables/ST_OptionsScreenDescription.ST_OptionsScreenDescription", InKey)

void UOptionsDataRegistry::InitOptionsDataRegistry(ULocalPlayer* InOwningLocalPlayer)
{
	InitGameplayCollectionTab();
	InitAudioCollectionTab();
	InitVideoCollectionTab();
	InitControlCollectionTab(InOwningLocalPlayer);
}

TArray<UListDataObject_Base*> UOptionsDataRegistry::GetListSourceItemsBySelectedTabID(
	const FName& InSelectedTabID) const
{
	UListDataObject_Collection* const* FoundTabCollectionPointer = RegisteredOptionsTabCollections.FindByPredicate(
		[InSelectedTabID](UListDataObject_Collection* AvailableTabCollection)->bool
		{
			return AvailableTabCollection->GetDataID() == InSelectedTabID;
		}
	);

	checkf(FoundTabCollectionPointer, TEXT("No valid tab found under the ID %s"), *InSelectedTabID.ToString());

	UListDataObject_Collection* FoundTabCollection = *FoundTabCollectionPointer;

	TArray<UListDataObject_Base*> AllChildListItems;

	for (UListDataObject_Base* ChildListData : FoundTabCollection->GetAllChildListData())
	{
		if (!ChildListData)
			continue;

		AllChildListItems.Add(ChildListData);

		if (ChildListData->HasAnyChildListData())
		{
			FindChildListDataRecursively(ChildListData, AllChildListItems);
		}
	}

	return AllChildListItems;
}


void UOptionsDataRegistry::FindChildListDataRecursively(UListDataObject_Base* InParentData,
	TArray<UListDataObject_Base*>& OutFoundChildListData) const
{
	if (!InParentData || !InParentData->HasAnyChildListData())
		return;

	for (UListDataObject_Base* SubChildListData : InParentData->GetAllChildListData())
	{
		if (!SubChildListData)
			continue;

		OutFoundChildListData.Add(SubChildListData);

		if (SubChildListData->HasAnyChildListData())
		{
			FindChildListDataRecursively(SubChildListData, OutFoundChildListData);
		}
	}
}

void UOptionsDataRegistry::InitGameplayCollectionTab()
{
	UListDataObject_Collection* GameplayTabCollection = NewObject<UListDataObject_Collection>();
	GameplayTabCollection->SetDataID(FName("GameplayTabCollection"));
	GameplayTabCollection->SetDataDisplayName(FText::FromString(TEXT("Gameplay")));
	
	{
		UListDataObject_String* GameDifficulty = NewObject<UListDataObject_String>();
		GameDifficulty->SetDataID(FName("GameDifficulty"));
		GameDifficulty->SetDataDisplayName(FText::FromString(TEXT("Difficulty")));
		GameDifficulty->SetDescriptionRichText(FText::FromString("Adjusts the difficulty of the game experience.\n\n<Bold>Easy:</> Focuses on the story experience. Provides the most relaxing combat.\n\n<Bold>Normal:</> Offers slightly harder combat experience\n\n<Bold>Hard:</> Offers a much more challenging combat experience\n\n<Bold>Inevitable:</> Provides the most challenging combat experience. Not recommended for first play through."));
		GameDifficulty->AddDynamicOption(TEXT("Easy"), FText::FromString(TEXT("Easy")));
		GameDifficulty->AddDynamicOption(TEXT("Normal"), FText::FromString(TEXT("Normal")));
		GameDifficulty->AddDynamicOption(TEXT("Hard"), FText::FromString(TEXT("Hard")));
		GameDifficulty->AddDynamicOption(TEXT("Inevitable"), FText::FromString(TEXT("Inevitable")));

		GameDifficulty->SetDefaultValueFromString(TEXT("Normal"));
		
		GameDifficulty->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCurrentGameDifficulty));
		GameDifficulty->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetCurrentGameDifficulty));

		GameDifficulty->SetShouldApplySettingsImmediately(true);
		
		GameplayTabCollection->AddChildListData(GameDifficulty);
	}

	{
		UListDataObject_String* GameAudioLanguage = NewObject<UListDataObject_String>();
		GameAudioLanguage->SetDataID(FName("GameAudioLanguage"));
		GameAudioLanguage->SetDataDisplayName(FText::FromString(TEXT("Audio Language")));
		GameAudioLanguage->SetSoftDescriptionImage(UFrontendFunctionLibrary::GetOptionsSoftImageByTag(FrontendGameplayTags::Frontend_Image_TestImage));
		GameAudioLanguage->SetDescriptionRichText(FText::FromString(TEXT("Set the <Bold>Audio Language Track</> that would be used during the Gameplay.")));
		GameAudioLanguage->AddDynamicOption(TEXT("English"), FText::FromString(TEXT("English")));
		GameAudioLanguage->AddDynamicOption(TEXT("Korean"), FText::FromString(TEXT("Korean")));
		GameAudioLanguage->AddDynamicOption(TEXT("Ukrainian"), FText::FromString(TEXT("Ukrainian")));
		GameAudioLanguage->AddDynamicOption(TEXT("Polish"), FText::FromString(TEXT("Polish")));
		GameAudioLanguage->AddDynamicOption(TEXT("German"), FText::FromString(TEXT("German")));
		GameAudioLanguage->AddDynamicOption(TEXT("French"), FText::FromString(TEXT("French")));
		GameAudioLanguage->AddDynamicOption(TEXT("Italian"), FText::FromString(TEXT("Italian")));

		GameAudioLanguage->SetDefaultValueFromString(TEXT("English"));

		GameAudioLanguage->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCurrentGameAudioLanguage));
		GameAudioLanguage->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetCurrentGameAudioLanguage));

		GameAudioLanguage->SetShouldApplySettingsImmediately(true);

		GameplayTabCollection->AddChildListData(GameAudioLanguage);
	}

	{
		UListDataObject_String* SwitchTarget = NewObject<UListDataObject_String>();
		SwitchTarget->SetDataID(FName("SwitchTarget"));
		SwitchTarget->SetDataDisplayName(FText::FromString(TEXT("Auto-switch target on Enemy Death")));
		SwitchTarget->SetDescriptionRichText(FText::FromString(TEXT("Enable to automatically target a new available enemy if the currently targeted enemy died.\n\n<Bold>Off:</> Never target a new available Enemy.\n\n<Bold>Visible enemy only:</> Target a new Enemy only in case if there are any left inside Player FOV.\n\n<Bold>Always:</> Always target a new available Enemy.")));
		SwitchTarget->AddDynamicOption(TEXT("Off"), FText::FromString(TEXT("Off")));
		SwitchTarget->AddDynamicOption(TEXT("VisibleEnemyOnly"), FText::FromString(TEXT("Visible Enemy Only")));
		SwitchTarget->AddDynamicOption(TEXT("Always"), FText::FromString(TEXT("Always")));

		SwitchTarget->SetDefaultValueFromString(TEXT("VisibleEnemyOnly"));

		SwitchTarget->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCurrentSwitchTargetStatus));
		SwitchTarget->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetCurrentSwitchTargetStatus));
		
		SwitchTarget->SetShouldApplySettingsImmediately(true);

		GameplayTabCollection->AddChildListData(SwitchTarget);
	}

	{
		UListDataObject_String* TestItem = NewObject<UListDataObject_String>();
		TestItem->SetDataID(FName("TestItem"));
		TestItem->SetDataDisplayName(FText::FromString("Test Image Item"));
		TestItem->SetSoftDescriptionImage(UFrontendFunctionLibrary::GetOptionsSoftImageByTag(FrontendGameplayTags::Frontend_Image_TestImage));
		TestItem->SetDescriptionRichText(FText::FromString(TEXT("The Image to display can be specified in the project settings.")));

		GameplayTabCollection->AddChildListData(TestItem);
	}
	
	RegisteredOptionsTabCollections.Add(GameplayTabCollection);
}

void UOptionsDataRegistry::InitAudioCollectionTab()
{
	UListDataObject_Collection* AudioTabCollection = NewObject<UListDataObject_Collection>();
	AudioTabCollection->SetDataID(FName("AudioTabCollection"));
	AudioTabCollection->SetDataDisplayName(FText::FromString(TEXT("Audio")));

	//Volume Category
	{
		UListDataObject_Collection* VolumeCategoryCollection = NewObject<UListDataObject_Collection>();
		VolumeCategoryCollection->SetDataID(FName("VolumeCategoryCollection"));
		VolumeCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("Volume")));

		AudioTabCollection->AddChildListData(VolumeCategoryCollection);

		// Overall Volume
		{
			UListDataObject_Scalar* OverallVolume = NewObject<UListDataObject_Scalar>();
			OverallVolume->SetDataID(FName("OverallVolume"));
			OverallVolume->SetDataDisplayName(FText::FromString(TEXT("Overall Volume")));
			OverallVolume->SetDescriptionRichText(FText::FromString(TEXT("This setting determine the Overall Volume of the entire game.")));
			OverallVolume->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			OverallVolume->SetOutputValueRange(TRange<float>(0.f, 2.f));
			OverallVolume->SetSliderStepSize(0.01f);
			OverallVolume->SetDefaultValueFromString(LexToString(1.f));
			OverallVolume->SetDisplayNumericType(ECommonNumericType::Percentage);
			OverallVolume->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());

			OverallVolume->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetOverallVolume));
			OverallVolume->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetOverallVolume));

			OverallVolume->SetShouldApplySettingsImmediately(true);
			
			VolumeCategoryCollection->AddChildListData(OverallVolume);
		}

		// Music Volume
		{
			UListDataObject_Scalar* MusicVolume = NewObject<UListDataObject_Scalar>();
			MusicVolume->SetDataID(FName("MusicVolume"));
			MusicVolume->SetDataDisplayName(FText::FromString(TEXT("Music Volume")));
			MusicVolume->SetDescriptionRichText(FText::FromString(TEXT("This setting determine the volume of the Music, that would be played during the entire game experience.")));
			MusicVolume->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			MusicVolume->SetOutputValueRange(TRange<float>(0.f, 2.f));
			MusicVolume->SetSliderStepSize(0.01f);
			MusicVolume->SetDefaultValueFromString(LexToString(1.f));
			MusicVolume->SetDisplayNumericType(ECommonNumericType::Percentage);
			MusicVolume->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());

			MusicVolume->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetMusicVolume));
			MusicVolume->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetMusicVolume));

			MusicVolume->SetShouldApplySettingsImmediately(true);
			
			VolumeCategoryCollection->AddChildListData(MusicVolume);
		}

		// SFX Volume
		{
			UListDataObject_Scalar* SFXVolume = NewObject<UListDataObject_Scalar>();
			SFXVolume->SetDataID(FName("SFXVolume"));
			SFXVolume->SetDataDisplayName(FText::FromString(TEXT("Sound FX Volume")));
			SFXVolume->SetDescriptionRichText(FText::FromString(TEXT("This setting determine the volume of the Sound Effects, that would be played during the entire game experience.")));
			SFXVolume->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			SFXVolume->SetOutputValueRange(TRange<float>(0.f, 2.f));
			SFXVolume->SetSliderStepSize(0.01f);
			SFXVolume->SetDefaultValueFromString(LexToString(1.f));
			SFXVolume->SetDisplayNumericType(ECommonNumericType::Percentage);
			SFXVolume->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());

			SFXVolume->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetSFXVolume));
			SFXVolume->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetSFXVolume));

			SFXVolume->SetShouldApplySettingsImmediately(true);
			
			VolumeCategoryCollection->AddChildListData(SFXVolume);
		}
	}

	// Sound Category
	{
		UListDataObject_Collection* SoundCategoryCollection = NewObject<UListDataObject_Collection>();
		SoundCategoryCollection->SetDataID(FName("SoundCategoryCollection"));
		SoundCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("Sound")));

		AudioTabCollection->AddChildListData(SoundCategoryCollection);

		{
			UListDataObject_StringBool* AllowBackgroundAudio = NewObject<UListDataObject_StringBool>();
			AllowBackgroundAudio->SetDataID(FName("AllowBackgroundAudio"));
			AllowBackgroundAudio->SetDataDisplayName(FText::FromString(TEXT("Allow Background Audio")));
			AllowBackgroundAudio->SetDescriptionRichText(FText::FromString(TEXT("This setting is responsible for playing the Audio Track in the background while the game is folded.\n\n<Bold>Enabled:</> The Audio Track will be playing in the background while the game is folded.\n\n<Bold>Disabled:</> The Audio Track won't be playing in the background while the game is folded.")));
			
			AllowBackgroundAudio->OverrideTrueDisplayText(FText::FromString(TEXT("Enabled")));
			AllowBackgroundAudio->OverrideFalseDisplayText(FText::FromString(TEXT("Disabled")));
			AllowBackgroundAudio->SetFalseAsDefaultValue();
			
			AllowBackgroundAudio->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetAllowBackgroundAudio));
			AllowBackgroundAudio->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetAllowBackgroundAudio));

			AllowBackgroundAudio->SetShouldApplySettingsImmediately(true);

			SoundCategoryCollection->AddChildListData(AllowBackgroundAudio);
		}

		{
			UListDataObject_StringBool* UseHDRAudioMode = NewObject<UListDataObject_StringBool>();
			UseHDRAudioMode->SetDataID(FName("UseHDRAudioMode"));
			UseHDRAudioMode->SetDataDisplayName(FText::FromString(TEXT("Use HDR Audio Mode")));
			UseHDRAudioMode->SetDescriptionRichText(FText::FromString(TEXT("This settings is responsible for using of the High Dynamic Range Audio Mode.\n\n<Bold>Enabled:</> The game will be using High Dynamic Range Audio Mode. Can influence the perfomance of the game.\n\n<Bold>Disabled:</> The game won't be using High Dynamic Range Audio Mode.")));
			
			UseHDRAudioMode->OverrideTrueDisplayText(FText::FromString(TEXT("Enabled")));
			UseHDRAudioMode->OverrideFalseDisplayText(FText::FromString(TEXT("Disabled")));
			UseHDRAudioMode->SetTrueAsDefaultValue();
			
			UseHDRAudioMode->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetUseHDRAudioMode));
			UseHDRAudioMode->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetUseHDRAudioMode));

			UseHDRAudioMode->SetShouldApplySettingsImmediately(true);

			SoundCategoryCollection->AddChildListData(UseHDRAudioMode);
		}
	}
	
	RegisteredOptionsTabCollections.Add(AudioTabCollection);
}

void UOptionsDataRegistry::InitVideoCollectionTab()
{
	UListDataObject_Collection* VideoTabCollection = NewObject<UListDataObject_Collection>();
	VideoTabCollection->SetDataID(FName("VideoTabCollection"));
	VideoTabCollection->SetDataDisplayName(FText::FromString(TEXT("Video")));

	UListDataObject_StringEnum* CreatedWindowMode = nullptr;

	{
		UListDataObject_Collection* DisplayCategoryCollection = NewObject<UListDataObject_Collection>();
		DisplayCategoryCollection->SetDataID(FName("DisplayCategoryCollection"));
		DisplayCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("Display")));

		VideoTabCollection->AddChildListData(DisplayCategoryCollection);

		FOptionsDataEditConditionDescriptor PackagedBuildOnlyCondition;
		PackagedBuildOnlyCondition.SetEditConditionFunc(
			[]()->bool
			{
				const bool bIsInEditor = GIsEditor || GIsPlayInEditorWorld;

				return !bIsInEditor;
			}
		);
		PackagedBuildOnlyCondition.SetDisabledRichReason(TEXT("\n\n<Disabled>This setting can only be adjusted in a packaged build.</>"));
		
		{
			UListDataObject_StringEnum* WindowMode = NewObject<UListDataObject_StringEnum>();
			WindowMode->SetDataID(FName("WindowMode"));
			WindowMode->SetDataDisplayName(FText::FromString(TEXT("Window Mode")));
			WindowMode->SetDescriptionRichText(GET_DESCRIPTION("WindowModeDescKey"));
			WindowMode->AddEnumOption(EWindowMode::Fullscreen, FText::FromString(TEXT("Fullscreen Mode")));
			WindowMode->AddEnumOption(EWindowMode::WindowedFullscreen, FText::FromString(TEXT("Borderless Window")));
			WindowMode->AddEnumOption(EWindowMode::Windowed, FText::FromString(TEXT("Windowed")));
			
			WindowMode->SetDefaultValueFromEnumOption(EWindowMode::WindowedFullscreen);
			
			WindowMode->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetFullscreenMode));
			WindowMode->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetFullscreenMode));
			WindowMode->SetShouldApplySettingsImmediately(true);

			WindowMode->AddEditCondition(PackagedBuildOnlyCondition);

			CreatedWindowMode = WindowMode;
			
			DisplayCategoryCollection->AddChildListData(WindowMode);
		}

		{
			UListDataObject_StringResolution* ScreenResolution = NewObject<UListDataObject_StringResolution>();
			ScreenResolution->SetDataID(FName("ScreenResolution"));
			ScreenResolution->SetDataDisplayName(FText::FromString(TEXT("Screen Resolution")));
			ScreenResolution->SetDescriptionRichText(GET_DESCRIPTION("ScreenResolutionsDescKey"));
			ScreenResolution->InitResolutionValues();
			
			ScreenResolution->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetScreenResolution));
			ScreenResolution->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetScreenResolution));
			ScreenResolution->SetShouldApplySettingsImmediately(true);

			ScreenResolution->AddEditCondition(PackagedBuildOnlyCondition);

			FOptionsDataEditConditionDescriptor WindowModeEditCondition;
			WindowModeEditCondition.SetEditConditionFunc(
				[CreatedWindowMode]()->bool
				{
					const bool bIsBorderlessWindow = CreatedWindowMode->GetCurrentValueAsEnum<EWindowMode::Type>() == EWindowMode::WindowedFullscreen;

					return !bIsBorderlessWindow;
				}
			);
			WindowModeEditCondition.SetDisabledRichReason(TEXT("\n\n<Disabled>Screen Resolution is not adjustable when the 'Window Mode' is set to Borderless Window. The value must match with the maximum allowed resolution.</>"));
			WindowModeEditCondition.SetDisabledForcedStringValue(ScreenResolution->GetMaximumAllowedResolution());

			ScreenResolution->AddEditCondition(WindowModeEditCondition);

			ScreenResolution->AddEditDependencyData(CreatedWindowMode);
			
			DisplayCategoryCollection->AddChildListData(ScreenResolution);
		}
	}

	{
		UListDataObject_Collection* GraphicsCategoryCollection = NewObject<UListDataObject_Collection>();
		GraphicsCategoryCollection->SetDataID(FName("GraphicsCategoryCollection"));
		GraphicsCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("Graphics")));

		VideoTabCollection->AddChildListData(GraphicsCategoryCollection);

		{
			UListDataObject_Scalar* DisplayGamma = NewObject<UListDataObject_Scalar>();
			DisplayGamma->SetDataID(FName("DisplayGamma"));
			DisplayGamma->SetDataDisplayName(FText::FromString(TEXT("Brightness")));
			DisplayGamma->SetDescriptionRichText(GET_DESCRIPTION("DisplayGammaDescKey"));
			DisplayGamma->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			DisplayGamma->SetOutputValueRange(TRange<float>(1.7f, 2.7f));
			DisplayGamma->SetSliderStepSize(0.01f);
			DisplayGamma->SetDisplayNumericType(ECommonNumericType::Percentage);
			DisplayGamma->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());
			
			DisplayGamma->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCurrentDisplayGamma));
			DisplayGamma->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetCurrentDisplayGamma));
			
			DisplayGamma->SetDefaultValueFromString(LexToString(2.2f));
			DisplayGamma->SetShouldApplySettingsImmediately(true);

			GraphicsCategoryCollection->AddChildListData(DisplayGamma);
		}

		UListDataObject_StringInteger* CreatedOverallQuality = nullptr;
		
		{
			UListDataObject_StringInteger* OverallQuality = NewObject<UListDataObject_StringInteger>();
			OverallQuality->SetDataID(FName("OverallQuality"));
			OverallQuality->SetDataDisplayName(FText::FromString(TEXT("Overall Quality")));
			OverallQuality->SetDescriptionRichText(GET_DESCRIPTION("OverallQualityDescKey"));
			OverallQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
			OverallQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
			OverallQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
			OverallQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
			OverallQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));

			OverallQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetOverallScalabilityLevel));
			OverallQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetOverallScalabilityLevel));
			OverallQuality->SetShouldApplySettingsImmediately(true);

			GraphicsCategoryCollection->AddChildListData(OverallQuality);

			CreatedOverallQuality = OverallQuality;
		}

		{
			UListDataObject_Scalar* ResolutionScale = NewObject<UListDataObject_Scalar>();
			ResolutionScale->SetDataID(FName("ResolutionScale"));
			ResolutionScale->SetDataDisplayName(FText::FromString(TEXT("3D Resolution")));
			ResolutionScale->SetDescriptionRichText(GET_DESCRIPTION("ResolutionScaleDescKey"));
			ResolutionScale->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			ResolutionScale->SetOutputValueRange(TRange<float>(0.f, 1.f));
			ResolutionScale->SetSliderStepSize(0.01f);
			ResolutionScale->SetDisplayNumericType(ECommonNumericType::Percentage);
			ResolutionScale->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());

			ResolutionScale->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetResolutionScaleNormalized));
			ResolutionScale->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetResolutionScaleNormalized));
			ResolutionScale->SetShouldApplySettingsImmediately(true);

			ResolutionScale->AddEditDependencyData(CreatedOverallQuality);

			GraphicsCategoryCollection->AddChildListData(ResolutionScale);
		}

		{
			UListDataObject_StringInteger* GlobalIlluminationQuality = NewObject<UListDataObject_StringInteger>();
			GlobalIlluminationQuality->SetDataID(FName("GlobalIlluminationQuality"));
			GlobalIlluminationQuality->SetDataDisplayName(FText::FromString(TEXT("Global Illumination")));
			GlobalIlluminationQuality->SetDescriptionRichText(GET_DESCRIPTION("GlobalIlluminationQualityDescKey"));
			GlobalIlluminationQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
			GlobalIlluminationQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
			GlobalIlluminationQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
			GlobalIlluminationQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
			GlobalIlluminationQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));

			GlobalIlluminationQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetGlobalIlluminationQuality));
			GlobalIlluminationQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetGlobalIlluminationQuality));
			GlobalIlluminationQuality->SetShouldApplySettingsImmediately(true);

			GlobalIlluminationQuality->AddEditDependencyData(CreatedOverallQuality);

			CreatedOverallQuality->AddEditDependencyData(GlobalIlluminationQuality);

			GraphicsCategoryCollection->AddChildListData(GlobalIlluminationQuality);
		}

		{
			UListDataObject_StringInteger* ShadowQuality = NewObject<UListDataObject_StringInteger>();
			ShadowQuality->SetDataID(FName("ShadowQuality"));
			ShadowQuality->SetDataDisplayName(FText::FromString(TEXT("Shadow Quality")));
			ShadowQuality->SetDescriptionRichText(GET_DESCRIPTION("ShadowQualityDescKey"));
			ShadowQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
			ShadowQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
			ShadowQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
			ShadowQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
			ShadowQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));

			ShadowQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetShadowQuality));
			ShadowQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetShadowQuality));
			ShadowQuality->SetShouldApplySettingsImmediately(true);

			ShadowQuality->AddEditDependencyData(CreatedOverallQuality);

			CreatedOverallQuality->AddEditDependencyData(ShadowQuality);

			GraphicsCategoryCollection->AddChildListData(ShadowQuality);
		}

		{
			UListDataObject_StringInteger* AntiAliasing = NewObject<UListDataObject_StringInteger>();
			AntiAliasing->SetDataID(FName("AntiAliasing"));
			AntiAliasing->SetDataDisplayName(FText::FromString(TEXT("Anti-Aliasing")));
			AntiAliasing->SetDescriptionRichText(GET_DESCRIPTION("AntiAliasingDescKey"));
			AntiAliasing->AddIntegerOption(0, FText::FromString(TEXT("Low")));
			AntiAliasing->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
			AntiAliasing->AddIntegerOption(2, FText::FromString(TEXT("High")));
			AntiAliasing->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
			AntiAliasing->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));

			AntiAliasing->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetAntiAliasingQuality));
			AntiAliasing->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetAntiAliasingQuality));
			AntiAliasing->SetShouldApplySettingsImmediately(true);

			AntiAliasing->AddEditDependencyData(CreatedOverallQuality);

			CreatedOverallQuality->AddEditDependencyData(AntiAliasing);

			GraphicsCategoryCollection->AddChildListData(AntiAliasing);
		}

		{
			UListDataObject_StringInteger* ViewDistanceQuality = NewObject<UListDataObject_StringInteger>();
			ViewDistanceQuality->SetDataID(FName("ViewDistanceQuality"));
			ViewDistanceQuality->SetDataDisplayName(FText::FromString(TEXT("View Distance")));
			ViewDistanceQuality->SetDescriptionRichText(GET_DESCRIPTION("ViewDistanceDescKey"));
			ViewDistanceQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
			ViewDistanceQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
			ViewDistanceQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
			ViewDistanceQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
			ViewDistanceQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));

			ViewDistanceQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetViewDistanceQuality));
			ViewDistanceQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetViewDistanceQuality));
			ViewDistanceQuality->SetShouldApplySettingsImmediately(true);

			ViewDistanceQuality->AddEditDependencyData(CreatedOverallQuality);

			CreatedOverallQuality->AddEditDependencyData(ViewDistanceQuality);

			GraphicsCategoryCollection->AddChildListData(ViewDistanceQuality);
		}

		{
			UListDataObject_StringInteger* TextureQuality = NewObject<UListDataObject_StringInteger>();
			TextureQuality->SetDataID(FName("TextureQuality"));
			TextureQuality->SetDataDisplayName(FText::FromString(TEXT("Texture Quality")));
			TextureQuality->SetDescriptionRichText(GET_DESCRIPTION("TextureQualityDescKey"));
			TextureQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
			TextureQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
			TextureQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
			TextureQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
			TextureQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));

			TextureQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetTextureQuality));
			TextureQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetTextureQuality));
			TextureQuality->SetShouldApplySettingsImmediately(true);

			TextureQuality->AddEditDependencyData(CreatedOverallQuality);

			CreatedOverallQuality->AddEditDependencyData(TextureQuality);

			GraphicsCategoryCollection->AddChildListData(TextureQuality);
		}

		{
			UListDataObject_StringInteger* VisualEffectQuality = NewObject<UListDataObject_StringInteger>();
			VisualEffectQuality->SetDataID(FName("VisualEffectQuality"));
			VisualEffectQuality->SetDataDisplayName(FText::FromString(TEXT("Visual Effect")));
			VisualEffectQuality->SetDescriptionRichText(GET_DESCRIPTION("VisualEffectQualityDescKey"));
			VisualEffectQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
			VisualEffectQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
			VisualEffectQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
			VisualEffectQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
			VisualEffectQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));

			VisualEffectQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetVisualEffectQuality));
			VisualEffectQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetVisualEffectQuality));
			VisualEffectQuality->SetShouldApplySettingsImmediately(true);

			VisualEffectQuality->AddEditDependencyData(CreatedOverallQuality);

			CreatedOverallQuality->AddEditDependencyData(VisualEffectQuality);

			GraphicsCategoryCollection->AddChildListData(VisualEffectQuality);
		}

		{
			UListDataObject_StringInteger* ReflectionQuality = NewObject<UListDataObject_StringInteger>();
			ReflectionQuality->SetDataID(FName("ReflectionQuality"));
			ReflectionQuality->SetDataDisplayName(FText::FromString(TEXT("Reflection Quality")));
			ReflectionQuality->SetDescriptionRichText(GET_DESCRIPTION("ReflectionQualityDescKey"));
			ReflectionQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
			ReflectionQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
			ReflectionQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
			ReflectionQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
			ReflectionQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));

			ReflectionQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetReflectionQuality));
			ReflectionQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetReflectionQuality));
			ReflectionQuality->SetShouldApplySettingsImmediately(true);

			ReflectionQuality->AddEditDependencyData(CreatedOverallQuality);

			CreatedOverallQuality->AddEditDependencyData(ReflectionQuality);

			GraphicsCategoryCollection->AddChildListData(ReflectionQuality);
		}

		{
			UListDataObject_StringInteger* PostProcessingQuality = NewObject<UListDataObject_StringInteger>();
			PostProcessingQuality->SetDataID(FName("PostProcessingQuality"));
			PostProcessingQuality->SetDataDisplayName(FText::FromString(TEXT("Post-Processing")));
			PostProcessingQuality->SetDescriptionRichText(GET_DESCRIPTION("PostProcessingQualityDescKey"));
			PostProcessingQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
			PostProcessingQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
			PostProcessingQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
			PostProcessingQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
			PostProcessingQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));

			PostProcessingQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetPostProcessingQuality));
			PostProcessingQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetPostProcessingQuality));
			PostProcessingQuality->SetShouldApplySettingsImmediately(true);

			PostProcessingQuality->AddEditDependencyData(CreatedOverallQuality);

			CreatedOverallQuality->AddEditDependencyData(PostProcessingQuality);

			GraphicsCategoryCollection->AddChildListData(PostProcessingQuality);
		}
	}

	{
		UListDataObject_Collection* AdvancedGraphicsCollection = NewObject<UListDataObject_Collection>();
		AdvancedGraphicsCollection->SetDataID(FName("AdvancedGraphicsCollection"));
		AdvancedGraphicsCollection->SetDataDisplayName(FText::FromString(TEXT("Advanced Graphics")));

		VideoTabCollection->AddChildListData(AdvancedGraphicsCollection);

		{
			UListDataObject_StringBool* VerticalSync = NewObject<UListDataObject_StringBool>();
			VerticalSync->SetDataID(FName("VerticalSync"));
			VerticalSync->SetDataDisplayName(FText::FromString(TEXT("V-Sync")));
			VerticalSync->SetDescriptionRichText(GET_DESCRIPTION("VerticalSyncDescKey"));
			
			VerticalSync->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(IsVSyncEnabled));
			VerticalSync->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetVSyncEnabled));
			VerticalSync->SetFalseAsDefaultValue();

			VerticalSync->SetShouldApplySettingsImmediately(true);

			FOptionsDataEditConditionDescriptor FullscreenOnlyCondition;
			FullscreenOnlyCondition.SetEditConditionFunc(
				[CreatedWindowMode]()->bool
				{
					return CreatedWindowMode->GetCurrentValueAsEnum<EWindowMode::Type>() == EWindowMode::Fullscreen;
				}
			);
			FullscreenOnlyCondition.SetDisabledRichReason(TEXT("\n\n<Disabled>This feature works only in Fullscreen Mode.</>"));
			FullscreenOnlyCondition.SetDisabledForcedStringValue(TEXT("false"));

			VerticalSync->AddEditCondition(FullscreenOnlyCondition);

			AdvancedGraphicsCollection->AddChildListData(VerticalSync);
		}
		
		{
			UListDataObject_String* FrameRateLimit = NewObject<UListDataObject_String>();
			FrameRateLimit->SetDataID(FName("FrameRateLimit"));
			FrameRateLimit->SetDataDisplayName(FText::FromString(TEXT("Frame Rate Limit")));
			FrameRateLimit->SetDescriptionRichText(GET_DESCRIPTION("FrameRateLimitDescKey"));
			
			FrameRateLimit->AddDynamicOption(LexToString(30.f), FText::FromString(TEXT("30 FPS")));
			FrameRateLimit->AddDynamicOption(LexToString(60.f), FText::FromString(TEXT("60 FPS")));
			FrameRateLimit->AddDynamicOption(LexToString(90.f), FText::FromString(TEXT("90 FPS")));
			FrameRateLimit->AddDynamicOption(LexToString(120.f), FText::FromString(TEXT("120 FPS")));
			FrameRateLimit->AddDynamicOption(LexToString(144.f), FText::FromString(TEXT("144 FPS")));
			FrameRateLimit->AddDynamicOption(LexToString(0.f), FText::FromString(TEXT("No Limit")));
			FrameRateLimit->SetDefaultValueFromString(LexToString(60.f));

			FrameRateLimit->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetFrameRateLimit));
			FrameRateLimit->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetFrameRateLimit));
			FrameRateLimit->SetShouldApplySettingsImmediately(true);

			AdvancedGraphicsCollection->AddChildListData(FrameRateLimit);
		}
	}
	
	RegisteredOptionsTabCollections.Add(VideoTabCollection);
}

void UOptionsDataRegistry::InitControlCollectionTab(ULocalPlayer* InOwningLocalPlayer)
{
	UListDataObject_Collection* ControlTabCollection = NewObject<UListDataObject_Collection>();
	ControlTabCollection->SetDataID(FName("ControlTabCollection"));
	ControlTabCollection->SetDataDisplayName(FText::FromString(TEXT("Control")));

	UEnhancedInputLocalPlayerSubsystem* EISubsystem = InOwningLocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	check(EISubsystem);

	UEnhancedInputUserSettings* EIUserSettings = EISubsystem->GetUserSettings();

	check(EIUserSettings);
	
	{
		UListDataObject_Collection* KeyboardMouseCategoryCollection = NewObject<UListDataObject_Collection>();
		KeyboardMouseCategoryCollection->SetDataID(FName("KeyboardMouseCategoryCollection"));
		KeyboardMouseCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("Keyboard & Mouse")));

		ControlTabCollection->AddChildListData(KeyboardMouseCategoryCollection);
		
		{
			FPlayerMappableKeyQueryOptions KeyboardMouseOnly;
			KeyboardMouseOnly.KeyToMatch = EKeys::S;
			KeyboardMouseOnly.bMatchBasicKeyTypes = true;
			
			for (const TPair<FString, UEnhancedPlayerMappableKeyProfile*>& ProfilePair : EIUserSettings->GetAllAvailableKeyProfiles())
			{
				UEnhancedPlayerMappableKeyProfile* MappableKeyProfile = ProfilePair.Value;

				check(MappableKeyProfile);

				for (const TPair<FName, FKeyMappingRow>& MappingRowPair : MappableKeyProfile->GetPlayerMappingRows())
				{
					for (const FPlayerKeyMapping& KeyMapping : MappingRowPair.Value.Mappings)
					{
						if (MappableKeyProfile->DoesMappingPassQueryOptions(KeyMapping, KeyboardMouseOnly))
						{
							UListDataObject_KeyRemap* KeyRemapDataObject = NewObject<UListDataObject_KeyRemap>();
							KeyRemapDataObject->SetDataID(KeyMapping.GetMappingName());
							KeyRemapDataObject->SetDataDisplayName(KeyMapping.GetDisplayName());
							KeyRemapDataObject->InitKeyRemapData(EIUserSettings, MappableKeyProfile, ECommonInputType::MouseAndKeyboard, KeyMapping);

							KeyboardMouseCategoryCollection->AddChildListData(KeyRemapDataObject);
						}
					}
				}
			}
		}

		{
			UListDataObject_Collection* GamepadCategoryCollection = NewObject<UListDataObject_Collection>();
			GamepadCategoryCollection->SetDataID(FName("GamepadCategoryCollection"));
			GamepadCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("Gamepad")));

			ControlTabCollection->AddChildListData(GamepadCategoryCollection);

			{
				FPlayerMappableKeyQueryOptions GamepadOnly;
				GamepadOnly.KeyToMatch = EKeys::Gamepad_FaceButton_Bottom;
				GamepadOnly.bMatchBasicKeyTypes = true;

				for (const TPair<FString, UEnhancedPlayerMappableKeyProfile*>& ProfilePair : EIUserSettings->GetAllAvailableKeyProfiles())
				{
					UEnhancedPlayerMappableKeyProfile* MappableKeyProfile = ProfilePair.Value;

					check(MappableKeyProfile);

					for (const TPair<FName, FKeyMappingRow>& MappingRowPair : MappableKeyProfile->GetPlayerMappingRows())
					{
						for (const FPlayerKeyMapping& KeyMapping : MappingRowPair.Value.Mappings)
						{
							if (MappableKeyProfile->DoesMappingPassQueryOptions(KeyMapping, GamepadOnly))
							{
								UListDataObject_KeyRemap* KeyRemapDataObject = NewObject<UListDataObject_KeyRemap>();
								KeyRemapDataObject->SetDataID(KeyMapping.GetMappingName());
								KeyRemapDataObject->SetDataDisplayName(KeyMapping.GetDisplayName());
								KeyRemapDataObject->InitKeyRemapData(EIUserSettings, MappableKeyProfile, ECommonInputType::Gamepad, KeyMapping);

								GamepadCategoryCollection->AddChildListData(KeyRemapDataObject);
							}
						}
					}
				}
			}
		}
	}
	
	RegisteredOptionsTabCollections.Add(ControlTabCollection);
}

