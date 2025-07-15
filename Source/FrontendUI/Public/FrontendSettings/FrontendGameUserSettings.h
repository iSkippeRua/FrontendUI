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

	UFUNCTION()
	bool GetAllowBackgroundAudio() const { return bAllowBackgroundAudio; }

	UFUNCTION()
	void SetAllowBackgroundAudio(bool bIsAllowed);

	UFUNCTION()
	bool GetUseHDRAudioMode() const { return bUseHDRAudioMode; }

	UFUNCTION()
	void SetUseHDRAudioMode(bool bIsUsed);
	// <--- Audio Tab End --->

	// <--- Video Tab Start --->
	UFUNCTION()
	float GetCurrentDisplayGamma() const;

	UFUNCTION()
	void SetCurrentDisplayGamma(float InNewGamma);
	// <--- Video Tab End --->
	
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

	UPROPERTY(Config)
	bool bAllowBackgroundAudio;

	UPROPERTY(Config)
	bool bUseHDRAudioMode;
	// <--- Audio Tab End --->

	// <--- Video Tab Start --->
	
	// <--- Video Tab End --->
	
};
