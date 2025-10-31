// Copyright Red Lotus Games, All Rights Reserved.


#include "SignetPlayerInfoWidget.h"

#include "Components/TextBlock.h"
#include "SignetGame/Abilities/SignetAbilitySystemComponent.h"
#include "SignetGame/Abilities/Attributes/SignetPrimaryAttributeSet.h"
#include "SignetGame/Player/SignetPlayerState.h"
#include "SignetGame/Save/SignetSaveTypes.h"

void USignetPlayerInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (const auto Pawn = GetOwningPlayerPawn())
	{
		InitWithActorInfo(Pawn);
	}

	
}

void USignetPlayerInfoWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!bInitialized)
	{
		InitWithActorInfo(GetOwningPlayerPawn());
	}

	if (!bFoundPlayerState)
	{
		if (const auto P = GetOwningPlayer())
		{
			if (const auto PS = P->GetPlayerState<ASignetPlayerState>())
			{
				SetPlayerName();
				PS->OnPlayerUpdated.AddDynamic(this, &USignetPlayerInfoWidget::OnPlayerUpdated);
				bFoundPlayerState = true;
			}
		}
	}
}

void USignetPlayerInfoWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (PlayerAsc.IsValid())
	{
		for (auto& Handle : BoundAttributeHandles)
		{
			PlayerAsc->GetGameplayAttributeValueChangeDelegate(Handle.Key)
				.Remove(Handle.Value);
		}	
	}

	BoundAttributeHandles.Empty();
}

void USignetPlayerInfoWidget::InitWithActorInfo(AActor* InActor)
{
	if (const auto Asc = InActor->GetComponentByClass<USignetAbilitySystemComponent>())
	{
		PlayerAsc = Asc;
		
		BindAttributeChange(USignetPrimaryAttributeSet::GetJobAttribute());
		BindAttributeChange(USignetPrimaryAttributeSet::GetJobLevelAttribute());
		BindAttributeChange(USignetPrimaryAttributeSet::GetSubJobAttribute());
		BindAttributeChange(USignetPrimaryAttributeSet::GetSubJobLevelAttribute());
		BindAttributeChange(USignetPrimaryAttributeSet::GetHPAttribute());
		BindAttributeChange(USignetPrimaryAttributeSet::GetMaxHPAttribute());
		BindAttributeChange(USignetPrimaryAttributeSet::GetMPAttribute());
		BindAttributeChange(USignetPrimaryAttributeSet::GetMaxMPAttribute());
		BindAttributeChange(USignetPrimaryAttributeSet::GetSTRAttribute());
		BindAttributeChange(USignetPrimaryAttributeSet::GetDEXAttribute());
		BindAttributeChange(USignetPrimaryAttributeSet::GetVITAttribute());
		BindAttributeChange(USignetPrimaryAttributeSet::GetAGIAttribute());
		BindAttributeChange(USignetPrimaryAttributeSet::GetINTAttribute());
		BindAttributeChange(USignetPrimaryAttributeSet::GetMNDAttribute());
		BindAttributeChange(USignetPrimaryAttributeSet::GetCHRAttribute());
		Update();
		bInitialized = true;
	}
}

void USignetPlayerInfoWidget::BindAttributeChange(const FGameplayAttribute& Attribute)
{
	if (!PlayerAsc.IsValid()) return;

	const auto Handle = PlayerAsc->GetGameplayAttributeValueChangeDelegate(Attribute)
		.AddUObject(this, &ThisClass::OnAttributeChanged);

	BoundAttributeHandles.Add(Attribute, Handle);
}

void USignetPlayerInfoWidget::OnAttributeChanged(const FOnAttributeChangeData& Data)
{
	const auto Attr = Data.Attribute;
	if (Attr == USignetPrimaryAttributeSet::GetJobAttribute()) {SetJobText();}
	if (Attr == USignetPrimaryAttributeSet::GetJobLevelAttribute()) {SetJobText();}
	if (Attr == USignetPrimaryAttributeSet::GetSubJobAttribute()) {SetSubJobText();}
	if (Attr == USignetPrimaryAttributeSet::GetSubJobLevelAttribute()) {SetSubJobText();}
	if (Attr == USignetPrimaryAttributeSet::GetHPAttribute()) {SetHPText();}
	if (Attr == USignetPrimaryAttributeSet::GetMaxHPAttribute()) {SetHPText();}
	if (Attr == USignetPrimaryAttributeSet::GetMPAttribute()) {SetMPText();}
	if (Attr == USignetPrimaryAttributeSet::GetMaxMPAttribute()) {SetMPText();}
	if (Attr == USignetPrimaryAttributeSet::GetSTRAttribute()) {SetSTRText();}
	if (Attr == USignetPrimaryAttributeSet::GetDEXAttribute()) {SetDEXText();}
	if (Attr == USignetPrimaryAttributeSet::GetVITAttribute()) {SetVITText();}
	if (Attr == USignetPrimaryAttributeSet::GetAGIAttribute()) {SetAGIText();}
	if (Attr == USignetPrimaryAttributeSet::GetINTAttribute()) {SetINTText();}
	if (Attr == USignetPrimaryAttributeSet::GetMNDAttribute()) {SetMNDText();}
	if (Attr == USignetPrimaryAttributeSet::GetCHRAttribute()) {SetCHRText();}
}

void USignetPlayerInfoWidget::OnPlayerUpdated(ASignetPlayerState* Player)
{
	SetPlayerName();
}

void USignetPlayerInfoWidget::SetPlayerName()
{
	if (const auto P = GetOwningPlayer())
	{
		if (const auto PS = P->GetPlayerState<ASignetPlayerState>())
		{
			const auto Name = PS->PlayerName;
			PlayerNameTextBlock->SetText(FText::FromString(Name));
		}
	}
}

void USignetPlayerInfoWidget::Update()
{
	SetPlayerName();
	SetJobText();
	SetSubJobText();
	SetHPText();
	SetMPText();
	SetSTRText();
	SetDEXText();
	SetVITText();
	SetAGIText();
	SetINTText();
	SetMNDText();
	SetCHRText();
}

void USignetPlayerInfoWidget::SetJobText()
{
	if (!PlayerAsc.IsValid()) return;

	bool bFound = false;
	const auto JobValue = PlayerAsc->GetGameplayAttributeValue(USignetPrimaryAttributeSet::GetJobAttribute(), bFound);
	const auto Job = static_cast<EJob>(JobValue);
	const auto JobAbbr = GetJobName(Job);
	const int JobLevel = static_cast<int>(PlayerAsc->GetGameplayAttributeValue(USignetPrimaryAttributeSet::GetJobLevelAttribute(), bFound));
	const auto Str = FString::Printf(TEXT("Lv%i %s"), JobLevel, *JobAbbr);
	JobTextBlock->SetText(FText::FromString(Str));
}

void USignetPlayerInfoWidget::SetSubJobText()
{
	if (!PlayerAsc.IsValid()) return;
	bool bFound = false;
	const EJob SubJob = static_cast<EJob>(PlayerAsc->GetGameplayAttributeValue(USignetPrimaryAttributeSet::GetSubJobAttribute(), bFound));
	const auto SubJobAbbr = GetJobName(SubJob);
	const int SubJobLevel = static_cast<int>(PlayerAsc->GetGameplayAttributeValue(USignetPrimaryAttributeSet::GetSubJobLevelAttribute(), bFound));

	if (SubJob == EJob::None)
	{
		SubJobTextBlock->SetText(FText::FromString(TEXT("None")));
	}
	else
	{
		const auto Str = FString::Printf(TEXT("Lv%i %s"), SubJobLevel, *SubJobAbbr);
		SubJobTextBlock->SetText(FText::FromString(Str));	
	}
	
}

void USignetPlayerInfoWidget::SetHPText()
{
	if (!PlayerAsc.IsValid()) return;
	
	bool bFound = false;
	const auto HP = static_cast<int>(PlayerAsc->GetGameplayAttributeValue(USignetPrimaryAttributeSet::GetHPAttribute(), bFound));
	const auto MaxHP = static_cast<int>(PlayerAsc->GetGameplayAttributeValue(USignetPrimaryAttributeSet::GetMaxHPAttribute(), bFound));
	const auto Str = FString::Printf(TEXT("%i/%i"), HP, MaxHP);
	HPTextBlock->SetText(FText::FromString(Str));
}

void USignetPlayerInfoWidget::SetMPText()
{
	if (!PlayerAsc.IsValid()) return;
	
	bool bFound = false;
	const auto MP = static_cast<int>(PlayerAsc->GetGameplayAttributeValue(USignetPrimaryAttributeSet::GetMPAttribute(), bFound));
	const auto MaxMP = static_cast<int>(PlayerAsc->GetGameplayAttributeValue(USignetPrimaryAttributeSet::GetMaxMPAttribute(), bFound));
	const auto Str = FString::Printf(TEXT("%i/%i"), MP, MaxMP);
	MPTextBlock->SetText(FText::FromString(Str));
}

void USignetPlayerInfoWidget::SetSTRText()
{
	SetAttrText(STRTextBlock, USignetPrimaryAttributeSet::GetSTRAttribute());
}

void USignetPlayerInfoWidget::SetDEXText()
{
	SetAttrText(DEXTextBlock, USignetPrimaryAttributeSet::GetDEXAttribute());
}

void USignetPlayerInfoWidget::SetVITText()
{
	SetAttrText(VITTextBlock, USignetPrimaryAttributeSet::GetVITAttribute());
}

void USignetPlayerInfoWidget::SetAGIText()
{
	SetAttrText(AGITextBlock, USignetPrimaryAttributeSet::GetAGIAttribute());
}

void USignetPlayerInfoWidget::SetINTText()
{
	SetAttrText(INTTextBlock, USignetPrimaryAttributeSet::GetINTAttribute());
}

void USignetPlayerInfoWidget::SetMNDText()
{
	SetAttrText(MNDTextBlock, USignetPrimaryAttributeSet::GetMNDAttribute());
}

void USignetPlayerInfoWidget::SetCHRText()
{
	SetAttrText(CHRTextBlock, USignetPrimaryAttributeSet::GetCHRAttribute());
}

void USignetPlayerInfoWidget::SetAttrText(UTextBlock* InText, const FGameplayAttribute& Attribute)
{
	if (!PlayerAsc.IsValid()) return;
	if (!InText) return;
	
	bool bFound = false;
	const auto Value = static_cast<int>(PlayerAsc->GetGameplayAttributeValue(Attribute, bFound));
	InText->SetText(FText::FromString(FString::Printf(TEXT("%i"), Value)));
}
