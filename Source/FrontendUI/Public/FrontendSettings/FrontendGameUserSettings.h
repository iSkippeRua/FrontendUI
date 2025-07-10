// Dmytro Chesniuk All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "FrontendGameUserSettings.generated.h"

UCLASS()
class FRONTENDUI_API UFrontendGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:
	static UFrontendGameUserSettings* Get();

	UFUNCTION()
	FString GetCurrentGameDifficulty() const { return CurrentGameDifficulty; }

	UFUNCTION()
	void SetCurrentGameDifficulty(const FString& InNewDifficulty) { CurrentGameDifficulty = InNewDifficulty; }

	UFUNCTION()
	FString GetCurrentGameAudioLanguage() const { return CurrentGameAudioLanguage; }
	
	UFUNCTION()
	void SetCurrentGameAudioLanguage(const FString& InNewAudioLanguage) { CurrentGameAudioLanguage = InNewAudioLanguage; }

	UFUNCTION()
	FString GetCurrentSwitchTargetStatus() const { return CurrentSwitchTargetStatus; }
	
	UFUNCTION()
	void SetCurrentSwitchTargetStatus(const FString& InNewSwitchTargetStatus) { CurrentSwitchTargetStatus = InNewSwitchTargetStatus; }
	
private:
	UPROPERTY(Config)
	FString CurrentGameDifficulty;

	UPROPERTY(Config)
	FString CurrentGameAudioLanguage;

	UPROPERTY(Config)
	FString CurrentSwitchTargetStatus;
	
};
