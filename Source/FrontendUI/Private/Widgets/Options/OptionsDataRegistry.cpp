// Dmytro Chesniuk All Rights Reserved


#include "Widgets/Options/OptionsDataRegistry.h"
#include "Widgets/Options/DataObjects/ListDataObject_Collection.h"
#include "Widgets/Options/DataObjects/ListDataObject_String.h"
#include "Widgets/Options/OptionsDataInteractionHelper.h"
#include "FrontendSettings/FrontendGameUserSettings.h"
#include "FrontendFunctionLibrary.h"
#include "FrontendGameplayTags.h"

#define MAKE_OPTIONS_DATA_CONTROL(SetterOrGetterFuncName) \
	MakeShared<FOptionsDataInteractionHelper>(GET_FUNCTION_NAME_STRING_CHECKED(UFrontendGameUserSettings, SetterOrGetterFuncName))

void UOptionsDataRegistry::InitOptionsDataRegistry(ULocalPlayer* InOwningLocalPlayer)
{
	InitGameplayCollectionTab();
	InitAudioCollectionTab();
	InitVideoCollectionTab();
	InitControlCollectionTab();
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

	return FoundTabCollection->GetAllChildListData();
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

	RegisteredOptionsTabCollections.Add(AudioTabCollection);
}

void UOptionsDataRegistry::InitVideoCollectionTab()
{
	UListDataObject_Collection* VideoTabCollection = NewObject<UListDataObject_Collection>();
	VideoTabCollection->SetDataID(FName("VideoTabCollection"));
	VideoTabCollection->SetDataDisplayName(FText::FromString(TEXT("Video")));

	RegisteredOptionsTabCollections.Add(VideoTabCollection);
}

void UOptionsDataRegistry::InitControlCollectionTab()
{
	UListDataObject_Collection* ControlTabCollection = NewObject<UListDataObject_Collection>();
	ControlTabCollection->SetDataID(FName("ControlTabCollection"));
	ControlTabCollection->SetDataDisplayName(FText::FromString(TEXT("Control")));

	RegisteredOptionsTabCollections.Add(ControlTabCollection);
}
