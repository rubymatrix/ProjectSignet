#include "SignetDebugHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "SignetGame/Save/SignetSaveTypes.h"
#include "SignetGame/Save/SignetSaveSubsystem.h"

const FName ASignetDebugHUD::CategoryName(TEXT("SignetProfile"));

ASignetDebugHUD::ASignetDebugHUD()
{
	PrimaryActorTick.bCanEverTick = true;
}

USignetSaveSubsystem* ASignetDebugHUD::GetSaveSubsystem() const
{
	if (const UGameInstance* GI = GetGameInstance())
	{
		if (auto Sub = GI->GetSubsystem<USignetSaveSubsystem>())
		{
			return Sub;
		}
	}
	return nullptr;
}

void ASignetDebugHUD::ShowDebugInfo(float& YL, float& YPos)
{
	Super::ShowDebugInfo(YL, YPos);

	if (!Canvas) { return; }
	
	if (DebugDisplay.Contains(CategoryName))
	{
		// Bridge: build an array to reuse DrawSignetProfile
		DrawSignetProfile(Canvas, PlayerOwner);
	}
}

float ASignetDebugHUD::DrawHeader(UCanvas* InCanvas, float X, float& Y, const FString& Title) const
{
	UFont* Font = GEngine->GetMediumFont();
	FCanvasTextItem TI(FVector2D(X, Y), FText::FromString(Title), Font, FLinearColor::Yellow);
	TI.EnableShadow(FLinearColor::Black);
	InCanvas->DrawItem(TI);

	float UnusedW, UsedH;
	InCanvas->StrLen(Font, TEXT("X"), UnusedW, UsedH);
	LineH = FMath::Max(LineH, UsedH);
	Y += UsedH + 6.f;
	return UsedH;
}

float ASignetDebugHUD::DrawKeyValue(UCanvas* InCanvas, float X, float& Y, const FString& Key, const FString& Value) const
{
	UFont* Font = GEngine->GetSmallFont();
	FCanvasTextItem KI(FVector2D(X, Y), FText::FromString(Key), Font, FLinearColor::White);
	KI.EnableShadow(FLinearColor::Black);
	InCanvas->DrawItem(KI);

	const float ColonPad = 220.f; // left column width
	FCanvasTextItem VI(FVector2D(X + ColonPad, Y), FText::FromString(Value), Font, FLinearColor::White);
	VI.EnableShadow(FLinearColor::Black);
	InCanvas->DrawItem(VI);

	float UnusedW, UsedH;
	InCanvas->StrLen(Font, TEXT("X"), UnusedW, UsedH);
	LineH = FMath::Max(LineH, UsedH);
	Y += UsedH + 2.f;
	return UsedH;
}

float ASignetDebugHUD::DrawSeparator(UCanvas* InCanvas, float X, float& Y) const
{
	const float W = InCanvas->ClipX * 0.45f;
	const float H = 1.0f;
	FCanvasTileItem Line(FVector2D(X, Y), FVector2D(W, H), FLinearColor(1,1,1,0.25f));
	Line.BlendMode = SE_BLEND_Translucent;
	InCanvas->DrawItem(Line);
	Y += 6.f;
	return H;
}

static FString SecondsToHMS(int64 Total)
{
	const int64 H = Total / 3600;
	const int64 M = (Total % 3600) / 60;
	const int64 S = Total % 60;
	return FString::Printf(TEXT("%lldh %lldm %llds"), H, M, S);
}

void ASignetDebugHUD::DrawSignetProfile(UCanvas* InCanvas, APlayerController* PC) const
{
	float X = LeftX;
	float Y = TopY;
	LineH = 0.f;

	USignetSaveSubsystem* Sub = GetSaveSubsystem();
	if (!Sub)
	{
		DrawHeader(InCanvas, X, Y, TEXT("SignetProfile (Save subsystem missing)"));
		DrawKeyValue(InCanvas, X, Y, TEXT("Status"), TEXT("<null> USignetSaveSubsystem"));
		return;
	}

	const FSignetPlayerProfile D = Sub->GetSave()->Profile;
	const FSignetJobProgress* CurrentJob = Sub->GetOrAddJob(D.CurrentJob);
	const FSignetCurrencyWallet Wallet = Sub->GetSave()->Wallet;

	DrawHeader(InCanvas, X, Y, TEXT("SignetProfile"));
	DrawKeyValue(InCanvas, X, Y, TEXT("Display Name"), D.DisplayName);
	DrawSeparator(InCanvas, X, Y);

	DrawKeyValue(InCanvas, X, Y, TEXT("Home Nation"), StaticEnum<ENation>()->GetNameStringByValue(static_cast<int64>(D.HomeNation)));
	DrawKeyValue(InCanvas, X, Y, TEXT("Character"),
		FString::Printf(TEXT("%s (%s)"), *StaticEnum<ERace>()->GetNameStringByValue(static_cast<int64>(D.SelectedRace)), *StaticEnum<EFace>()->GetNameStringByValue(static_cast<int64>(D.SelectedFace)))
	);
	DrawKeyValue(InCanvas, X, Y, TEXT("Job"), FString::Printf(TEXT("Lv. %i %s"), CurrentJob->Level, *StaticEnum<EJob>()->GetNameStringByValue(static_cast<int64>(D.CurrentJob))));

	// Currencies
	DrawHeader(InCanvas, X, Y, TEXT("Currencies"));
	DrawKeyValue(InCanvas, X, Y, TEXT("Experience"), FString::Printf(TEXT("%lld"), Wallet.Experience));
	DrawKeyValue(InCanvas, X, Y, TEXT("Ability Points"), FString::Printf(TEXT("%lld"), Wallet.AbilityPoints));
	DrawKeyValue(InCanvas, X, Y, TEXT("Skill Points"), FString::Printf(TEXT("%lld"), Wallet.SkillPoints));
	DrawKeyValue(InCanvas, X, Y, TEXT("Beastmen Seals"), FString::Printf(TEXT("%lld"), Wallet.BeastmenSeals));
	DrawKeyValue(InCanvas, X, Y, TEXT("Gil"), FString::Printf(TEXT("%lld"), Wallet.Gil));
	
	DrawSeparator(InCanvas, X, Y);

	// Jobs
	DrawHeader(InCanvas, X, Y, TEXT("Jobs"));

	// Loop over all EJob

	for (const auto& JobPair : Sub->GetSave()->Jobs)
	{
		const EJob JobType = JobPair.Key;
		const FSignetJobProgress& Job = JobPair.Value;
		DrawKeyValue(InCanvas, X, Y, StaticEnum<EJob>()->GetNameStringByValue(static_cast<int64>(JobType)), FString::Printf(TEXT("Level %d  |  Exp %lld"), Job.Level, Job.CurrentExp));
	}

	// Tip footer
	Y += 8.f;
	DrawSeparator(InCanvas, X, Y);
	// DrawKeyValue(InCanvas, X, Y, TEXT("Tip"), TEXT("Use: ShowDebug SignetProfile  |  HideDebug  |  ShowDebugToggle"));
}
