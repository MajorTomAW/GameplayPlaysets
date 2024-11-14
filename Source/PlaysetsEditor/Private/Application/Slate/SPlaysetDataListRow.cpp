// Copyright © 2024 MajorT. All rights reserved.


#include "SPlaysetDataListRow.h"

#include "SlateOptMacros.h"
#include "SPlaysetDataList.h"
#include "StateTreeEditorStyle.h"
#include "Style/PlaysetStyle.h"
#include "Styling/SlateIconFinder.h"

namespace UE::Playset::Editor
{
	FLinearColor LerpColorSRGB(const FLinearColor ColorA, FLinearColor ColorB, float T)
	{
		const FColor A = ColorA.ToFColorSRGB();
		const FColor B = ColorB.ToFColorSRGB();
		return FLinearColor(FColor(
			static_cast<uint8>(FMath::RoundToInt(static_cast<float>(A.R) * (1.f - T) + static_cast<float>(B.R) * T)),
			static_cast<uint8>(FMath::RoundToInt(static_cast<float>(A.G) * (1.f - T) + static_cast<float>(B.G) * T)),
			static_cast<uint8>(FMath::RoundToInt(static_cast<float>(A.B) * (1.f - T) + static_cast<float>(B.B) * T)),
			static_cast<uint8>(FMath::RoundToInt(static_cast<float>(A.A) * (1.f - T) + static_cast<float>(B.A) * T))));
	}
}

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SPlaysetDataListRow::Construct(
	const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView, TSharedRef<FPlaysetDataObjectEntry> InDataItem, const TSharedPtr<SScrollBox>& ViewBox)
{
	ConstructInternal(STableRow::FArguments()
		.Padding(5.f)
		.Style(&FStateTreeEditorStyle::Get().GetWidgetStyle<FTableRowStyle>("StateTree.Selection"))
		, InOwnerTableView
		);
	
	this->ChildSlot
	.HAlign(HAlign_Fill)
	.VAlign(VAlign_Fill)
	[
		SNew(SBox)
		.MinDesiredWidth_Lambda([WeakOwnerViewBox = ViewBox.ToWeakPtr()]()
		{
			if (const TSharedPtr<SScrollBox> OwnerViewBox = WeakOwnerViewBox.Pin())
			{
				return OwnerViewBox->GetTickSpaceGeometry().GetLocalSize().X;
			}
			
			return 0.f;
		})
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.VAlign(VAlign_Fill)
			.HAlign(HAlign_Left)
			.AutoWidth()
			[
				SNew(SExpanderArrow, SharedThis(this))
				.ShouldDrawWires(true)
				.IndentAmount(true)
				.BaseIndentLevel(0)
			]

			+ SHorizontalBox::Slot()
			.VAlign(VAlign_Fill)
			.HAlign(HAlign_Left)
			.Padding(FMargin(0.f, 4.f))
			.AutoWidth()
			[
				SNew(SBox)
				.HeightOverride(28.f)
				.VAlign(VAlign_Fill)
				[
					SNew(SBorder)
					.BorderImage(FStateTreeEditorStyle::Get().GetBrush("StateTree.State.Border"))
					.BorderBackgroundColor_Lambda([this]()
					{
						return bIsSelected ? FStyleColors::Select : FStyleColors::Transparent;
					})
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.VAlign(VAlign_Center)
						.AutoWidth()
						[
							SNew(SBox)
							.HeightOverride(38.f)
							//.Padding(4.f, 0.f)
							.VAlign(VAlign_Fill)
							[
								SNew(SBorder)
								.BorderImage(FAppStyle::GetBrush("WhiteBrush"))
								.BorderBackgroundColor(FSlateColor(UE::Playset::Editor::LerpColorSRGB(FStyleColors::Secondary.GetSpecifiedColor(), FColor::White, 0.1f)))
								.Padding(FMargin(8.f, 0.f, 12.f, 0.f))
								.VAlign(VAlign_Fill)
								[
									SNew(SOverlay)
									+ SOverlay::Slot()
									[
										SNew(SHorizontalBox)
										
										// Icon
										+ SHorizontalBox::Slot()
										.VAlign(VAlign_Center)
										.AutoWidth()
										[
											SNew(SBox)
											.Padding(FMargin(0.f, 0.f, 4.f, 0.f))
											[
												SNew(SImage)
												.Image_Lambda([InDataItem]()
												{
													const FSlateBrush* IconBrush = FSlateIconFinder::FindIconBrushForClass(InDataItem->DataItem->GetClass());
													return IconBrush;
												})
											]
										]

										// Name
										+ SHorizontalBox::Slot()
										.VAlign(VAlign_Center)
										.AutoWidth()
										[
											SNew(STextBlock)
											.Text(FText::FromString(InDataItem.Get().GetDisplayName()))
											.TextStyle(FPlaysetStyle::Get(), "PlaysetsEditor.Text.Name")
											//.TransformPolicy(ETextTransformPolicy::ToUpper)
										]
									]
								]
							]
						]
					]
				]
			]

			// Description
			+ SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			.AutoWidth()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				.Padding(FMargin(8.f, 0.f, 0.f, 0.f))
				.AutoWidth()
				[
					SNew(STextBlock)
					.Text(FText::FromString(InDataItem.Get().GetDescription()))
					.TextStyle(FPlaysetStyle::Get(), "PlaysetsEditor.Text.Details")
				]
			]
		]
	];
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION


