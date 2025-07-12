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
	UFrontendGameUserSettings();
	
	static UFrontendGameUserSettings* Get();

	// <--- Gameplay Tab Start --->
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
	// <--- Gameplay Tab End --->

	// <--- Audio Tab Start --->
	UFUNCTION()
	float GetOverallVolume() const { return OverallVolume; }

	UFUNCTION()
	void SetOverallVolume(float InVolume);

	UFUNCTION()
	float GetMusicVolume() const { return MusicVolume; }

	UFUNCTION()
	void SetMusicVolume(float InVolume);

	UFUNCTION()
	float GetSFXVolume() const { return SFXVolume; }

	UFUNCTION()
	void SetSFXVolume(float InVolume);
	// <--- Audio Tab End --->
	
private:
	// <--- Gameplay Tab Start --->
	UPROPERTY(Config)
	FString CurrentGameDifficulty;

	UPROPERTY(Config)
	FString CurrentGameAudioLanguage;

	UPROPERTY(Config)
	FString CurrentSwitchTargetStatus;
	// <--- Gameplay Tab End --->

	// <--- Audio Tab Start --->
	UPROPERTY(Config)
	float OverallVolume;

	UPROPERTY(Config)
	float MusicVolume;

	UPROPERTY(Config)
	float SFXVolume;
	// <--- Audio Tab End --->
	
};
