#include "GeminiVoiceAssistantSubsystem.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Misc/Base64.h"
#include "Serialization/JsonSerializer.h"
#include "Dom/JsonObject.h"

UGeminiVoiceAssistantSubsystem::UGeminiVoiceAssistantSubsystem(){}

void UGeminiVoiceAssistantSubsystem::StartListening(){}

static void WriteWav(const TArray<int16>& PCM16, int32 SampleRate, int32 NumChannels, TArray<uint8>& Out)
{
	const int32 DataBytes = PCM16.Num() * sizeof(int16);
	Out.Reset();
	Out.AddUninitialized(44 + DataBytes);
	uint8* P = Out.GetData();
	auto W=[&](int32 Off,const void*Src,int32 Size){FMemory::Memcpy(P+Off,Src,Size);} ;
	W(0,"RIFF",4); uint32 Chunk=36+DataBytes; W(4,&Chunk,4); W(8,"WAVE",4);
	W(12,"fmt ",4); uint32 Sub1=16; W(16,&Sub1,4); uint16 Fmt=1; W(20,&Fmt,2);
	uint16 Ch=(uint16)NumChannels; W(22,&Ch,2); uint32 SR=(uint32)SampleRate; W(24,&SR,4);
	uint32 ByteRate=(uint32)(SampleRate*NumChannels*2); W(28,&ByteRate,4); uint16 Align=(uint16)(NumChannels*2); W(32,&Align,2);
	uint16 Bps=16; W(34,&Bps,2); W(36,"data",4); uint32 Sub2=(uint32)DataBytes; W(40,&Sub2,4);
	W(44,PCM16.GetData(),DataBytes);
}

void UGeminiVoiceAssistantSubsystem::BuildSilentWav(TArray<uint8>& OutWav)
{
	TArray<int16> S; S.AddZeroed(16000/2); // 0.5s silence @16kHz mono
	WriteWav(S,16000,1,OutWav);
}

void UGeminiVoiceAssistantSubsystem::StopAndSend()
{
	TArray<uint8> Wav; BuildSilentWav(Wav);
	SendToGemini(Wav);
}

void UGeminiVoiceAssistantSubsystem::SendToGemini(const TArray<uint8>& Wav)
{
	FString ApiKey = FPlatformMisc::GetEnvironmentVariable(TEXT("GEMINI_API_KEY"));
	if (ApiKey.IsEmpty()) { OnGeminiResponse.Broadcast(TEXT("GEMINI_API_KEY not set")); return; }
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Req = FHttpModule::Get().CreateRequest();
	Req->SetURL(TEXT("https://generativelanguage.googleapis.com/v1beta/models/gemini-2.0-flash-exp:generateContent?key=") + ApiKey);
	Req->SetVerb(TEXT("POST"));
	Req->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	TSharedPtr<FJsonObject> Root = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> Contents;
	{
		TSharedPtr<FJsonObject> Part = MakeShared<FJsonObject>();
		TSharedPtr<FJsonObject> Inline = MakeShared<FJsonObject>();
		Inline->SetStringField(TEXT("mimeType"), TEXT("audio/wav"));
		Inline->SetStringField(TEXT("data"), FBase64::Encode(Wav));
		Part->SetObjectField(TEXT("inlineData"), Inline);
		TArray<TSharedPtr<FJsonValue>> Parts; Parts.Add(MakeShared<FJsonValueObject>(Part));
		TSharedPtr<FJsonObject> Content = MakeShared<FJsonObject>(); Content->SetArrayField(TEXT("parts"), Parts);
		Contents.Add(MakeShared<FJsonValueObject>(Content));
	}
	Root->SetArrayField(TEXT("contents"), Contents);
	FString Payload; const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Payload); FJsonSerializer::Serialize(Root.ToSharedRef(), Writer);
	Req->SetContentAsString(Payload);
	Req->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr R, FHttpResponsePtr Resp, bool bOk)
	{
		FString Text;
		if (bOk && Resp.IsValid())
		{
			TSharedPtr<FJsonObject> Obj; const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Resp->GetContentAsString());
			if (FJsonSerializer::Deserialize(Reader, Obj) && Obj.IsValid())
			{
				const TArray<TSharedPtr<FJsonValue>>* Cands;
				if (Obj->TryGetArrayField(TEXT("candidates"), Cands) && Cands->Num()>0)
				{
					const TSharedPtr<FJsonObject> Cand = (*Cands)[0]->AsObject();
					const TSharedPtr<FJsonObject>* Content;
					if (Cand.IsValid() && Cand->TryGetObjectField(TEXT("content"), Content))
					{
						const TArray<TSharedPtr<FJsonValue>>* Parts;
						if ((*Content)->TryGetArrayField(TEXT("parts"), Parts))
						{
							for (const TSharedPtr<FJsonValue>& V : *Parts)
							{
								const TSharedPtr<FJsonObject> P = V->AsObject();
								if (P.IsValid() && P->HasField(TEXT("text"))) { Text += P->GetStringField(TEXT("text")); }
							}
						}
					}
				}
			}
		}
		if (Text.IsEmpty()) { Text = TEXT("(no response)"); }
		OnGeminiResponse.Broadcast(Text);
	});
	Req->ProcessRequest();
}


