#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GeminiVoiceAssistantSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGeminiText, const FString&, Text);

UCLASS(BlueprintType)
class ALIENDOCGEMINIVA_API UGeminiVoiceAssistantSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UGeminiVoiceAssistantSubsystem();

	UFUNCTION(BlueprintCallable, Category="GeminiVA")
	void StartListening();

	UFUNCTION(BlueprintCallable, Category="GeminiVA")
	void StopAndSend();

    UFUNCTION(BlueprintCallable, Category="GeminiVA")
    void SpeakText(const FString& Text);

	UPROPERTY(BlueprintAssignable, Category="GeminiVA")
	FOnGeminiText OnGeminiResponse;

private:
	void BuildSilentWav(TArray<uint8>& OutWav);
	void SendToGemini(const TArray<uint8>& Wav);

    bool bCapturing = false;
    TArray<float> CapturedPCM;
    int32 SampleRate = 16000;
    int32 NumChannels = 1;
};



