//MinesweeperWidget - a custom widget used for creation and logic for the entire Minesweeper Plugin
//Author - Lukas Kundelis
#include "MinesweeperWidget.h"

#define LOCTEXT_NAMESPACE "SMinesweeperWidget"

//Initial construction of the Minesweeper widget
void SMinesweeperWidget::Construct(const FArguments& InArgs)
{
	ChildSlot
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SGridPanel)
				.FillColumn(1, 1.f)
				.FillColumn(3, 1.f)
				.FillColumn(5, 1.f)
				+ SGridPanel::Slot(0, 0)
				[
					SNew(STextBlock)
					.Text(HeightInputBoxLabel)
				]
				+ SGridPanel::Slot(1, 0)
				[
					SNew(SEditableTextBox)
					.OnTextChanged(FOnTextChanged::CreateRaw(this, &SMinesweeperWidget::HandleOnHeightChange))
				]
				+ SGridPanel::Slot(2, 0)
				[
					SNew(STextBlock)
					.Text(WidthInputBoxLabel)
				]
				+ SGridPanel::Slot(3, 0)
				[
					SNew(SEditableTextBox)
					.OnTextChanged(FOnTextChanged::CreateRaw(this, &SMinesweeperWidget::HandleOnWidthChange))
				]
				+ SGridPanel::Slot(4, 0)
				[
					SNew(STextBlock)
					.Text(BombAmountLabel)
				]
				+ SGridPanel::Slot(5, 0)
				[
					SNew(SEditableTextBox)
					.OnTextChanged(FOnTextChanged::CreateRaw(this, &SMinesweeperWidget::HandleOnBombAmountChange))
				]
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SButton)
				.Text(GenerateButtonLabel)
				.HAlign(HAlign_Center)
				.OnClicked(FOnClicked::CreateRaw(this, &SMinesweeperWidget::HandleOnGenerateButtonClicked))
			]
		];
}

//Function calculates and assigns the number of bombs surrounding a button in the grid
void SMinesweeperWidget::BombProximity()
{
	for (int i = 0; i < Height; i++)
	{
		for (int j = 0; j < Width; j++)
		{
			if (!RowCoordinate[i].ColumnCoordinate[j].Get().IsBomb())
			{
				int32 BombsAround = 0;
				if (i + 1 < Height)
				{
					if (RowCoordinate[i + 1].ColumnCoordinate[j].Get().IsBomb())
					{
						BombsAround++;
					}
					if (j + 1 < Width && RowCoordinate[i + 1].ColumnCoordinate[j + 1].Get().IsBomb())
					{
						BombsAround++;
					}
					if (j - 1 >= 0 && RowCoordinate[i + 1].ColumnCoordinate[j - 1].Get().IsBomb())
					{
						BombsAround++;
					}
				}
				if (i - 1 >= 0)
				{
					if (RowCoordinate[i - 1].ColumnCoordinate[j].Get().IsBomb())
					{
						BombsAround++;
					}
					if (j + 1 < Width && RowCoordinate[i - 1].ColumnCoordinate[j + 1].Get().IsBomb())
					{
						BombsAround++;
					}
					if (j - 1 >= 0 && RowCoordinate[i - 1].ColumnCoordinate[j - 1].Get().IsBomb())
					{
						BombsAround++;
					}
				}
				if (j - 1 >= 0 && RowCoordinate[i].ColumnCoordinate[j - 1].Get().IsBomb())
				{
					BombsAround++;
				}
				if (j + 1 < Width && RowCoordinate[i].ColumnCoordinate[j + 1].Get().IsBomb())
				{
					BombsAround++;
				}
				RowCoordinate[i].ColumnCoordinate[j].Get().BombsInProximity(BombsAround);
			}
		}
	}
}

//Function for predetermining the places of the bombs
void SMinesweeperWidget::BombPlacement()
{
	BombSpots.Empty();
	SizeOfField = Width * Height;
	for (int i = 0; i < BombAmount; i++)
	{
		RandomNumber = rand() % SizeOfField + 1;
		while (BombSpots.Contains(RandomNumber))
		{
			RandomNumber = rand() % SizeOfField + 1;
		}
		BombSpots.Add(RandomNumber);
	}
}

//Function to take the text of Height input field and save it
//InText - text passed from the input field to the function
void SMinesweeperWidget::HandleOnHeightChange(const FText& InText)
{
	HeightValue = InText.ToString();
}

//Function to take the text of Width input field and save it
//InText - text passed from the input field to the function
void SMinesweeperWidget::HandleOnWidthChange(const FText& InText)
{
	WidthValue = InText.ToString();
}

//Function to take the text of Bomb Amount input field and save it
//InText - text passed from the input field to the function
void SMinesweeperWidget::HandleOnBombAmountChange(const FText& InText)
{
	BombAmountValue = InText.ToString();
}

//Function responsible for generating the field for the game
void SMinesweeperWidget::GenerateMinefield()
{
	//Initializing new values so that every time the new field is generated
	//It will be random, no matter if all the information given is the same or not
	RowCoordinate.Empty();
	RevealedTiles = 0;
	TSharedRef<SGridPanel> MinefieldBox = SNew(SGridPanel);
	ButtonIndex = 1;

	//Generation of the grid
	if (IsInputValid())
	{
		BombPlacement();
		for (int32 i = 0; i < Height; i++)
		{
			RowCoordinate.Add(Coordinates());
			for (int32 j = 0; j < Width; j++)
			{
				if (BombSpots.Contains(ButtonIndex))
				{
					RowCoordinate[i].Add(SNew(SMinesweeperButton)
						.HasBomb(true)
						.OnClicked(FOnClicked::CreateRaw(this, &SMinesweeperWidget::HandleMinefieldButtonClicked, i, j)));
					MinefieldBox->AddSlot(j, i)
						[
							RowCoordinate[i].ColumnCoordinate[j]
						];
				}
				else
				{
					RowCoordinate[i].Add(SNew(SMinesweeperButton)
						.OnClicked(FOnClicked::CreateRaw(this, &SMinesweeperWidget::HandleMinefieldButtonClicked, i, j)));
					MinefieldBox->AddSlot(j, i)
						[
							RowCoordinate[i].ColumnCoordinate[j]
						];
				}
				ButtonIndex++;
			}
		}

		//Setting the number of surrounding bombs for the buttons
		BombProximity();
		
		//Placing the playing field inside the window of UI
		ChildSlot
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SGridPanel)
				.FillColumn(1, 1.f)
				.FillColumn(3, 1.f)
				.FillColumn(5, 1.f)
				+ SGridPanel::Slot(0, 0)
				[
					SNew(STextBlock)
					.Text(HeightInputBoxLabel)
				]
				+ SGridPanel::Slot(1, 0)
				[
					SNew(SEditableTextBox)
					.Text(FText::FromString(HeightValue))
					.OnTextChanged(FOnTextChanged::CreateRaw(this, &SMinesweeperWidget::HandleOnHeightChange))
				]
				+ SGridPanel::Slot(2, 0)
				[
					SNew(STextBlock)
					.Text(WidthInputBoxLabel)
				]
				+ SGridPanel::Slot(3, 0)
				[
					SNew(SEditableTextBox)
					.Text(FText::FromString(WidthValue))
					.OnTextChanged(FOnTextChanged::CreateRaw(this, &SMinesweeperWidget::HandleOnWidthChange))
				]
				+ SGridPanel::Slot(4, 0)
				[
					SNew(STextBlock)
					.Text(BombAmountLabel)
				]
				+ SGridPanel::Slot(5, 0)
				[
					SNew(SEditableTextBox)
					.Text(FText::FromString(BombAmountValue))
					.OnTextChanged(FOnTextChanged::CreateRaw(this, &SMinesweeperWidget::HandleOnBombAmountChange))
				]
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SButton)
				.Text(GenerateButtonLabel)
				.HAlign(HAlign_Center)
				.OnClicked(FOnClicked::CreateRaw(this, &SMinesweeperWidget::HandleOnGenerateButtonClicked))
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
					MinefieldBox
			]
		];
	}
	
}

//Function that revelas all the buttons in the field
void SMinesweeperWidget::RevealField()
{
	for (int32 i = 0; i < Height; i++)
	{
		for (int32 j = 0; j < Width; j++)
		{
			RowCoordinate[i].ColumnCoordinate[j].Get().Reveal();
		}
	}
}

//Function that checks if a player has won the game
void SMinesweeperWidget::IsWon()
{
	if (RevealedTiles == SizeOfField - BombAmount)
	{
		RevealField();
		FMessageDialog::Open(EAppMsgType::Type::Ok, FText::FromString(TEXT("Congratulations! You have won!")));
	}
}

//Function for the "Generate new minefield" button, initializes minefield generation
FReply SMinesweeperWidget::HandleOnGenerateButtonClicked()
{
	GenerateMinefield();
	return FReply::Handled();
}

//Function responsible for behaviour of the minefield buttons
//i - Y coordinate of the grid
//j - X coordinate of the grid
FReply SMinesweeperWidget::HandleMinefieldButtonClicked(int32 i, int32 j)
{
	if (i >= Height || i < 0 || j >= Width || j < 0)
	{
		return FReply::Handled();
	}
	else
	{
		if (!RowCoordinate[i].ColumnCoordinate[j].Get().IsRevealed())
		{
			if (RowCoordinate[i].ColumnCoordinate[j].Get().IsBomb())
			{
				RevealField();
				FMessageDialog::Open(EAppMsgType::Type::Ok, FText::FromString(TEXT("You lost and exploded :(")));
				return FReply::Handled();
			}
			else if (RowCoordinate[i].ColumnCoordinate[j].Get().HasBombsAround())
			{
				RowCoordinate[i].ColumnCoordinate[j].Get().Reveal();
				RevealedTiles++;
				IsWon();
				return FReply::Handled();
			}
			else
			{
				RowCoordinate[i].ColumnCoordinate[j].Get().Reveal();
				RevealedTiles++;
				IsWon();
				HandleMinefieldButtonClicked(i + 1, j);
				HandleMinefieldButtonClicked(i - 1, j);
				HandleMinefieldButtonClicked(i, j - 1);
				HandleMinefieldButtonClicked(i, j + 1);
				HandleMinefieldButtonClicked(i + 1, j + 1);
				HandleMinefieldButtonClicked(i + 1, j - 1);
				HandleMinefieldButtonClicked(i - 1, j + 1);
				HandleMinefieldButtonClicked(i - 1, j - 1);
				return FReply::Handled();
			}
		}
		return FReply::Handled();
	}
}

//Function to check if the inputs in the input fields are valid
//If they are, they are also assigned to integers to be used for logic of the game
bool SMinesweeperWidget::IsInputValid()
{
	if (HeightValue.IsEmpty() || WidthValue.IsEmpty() || BombAmountValue.IsEmpty())
	{
		FMessageDialog::Open(EAppMsgType::Type::Ok, FText::FromString(TEXT("Entry boxes cannot be empty!")));
		return false;
	}
	else if (!HeightValue.IsNumeric() || !WidthValue.IsNumeric() || !BombAmountValue.IsNumeric())
	{
		FMessageDialog::Open(EAppMsgType::Type::Ok, FText::FromString(TEXT("Please enter integers only!")));
		return false;
	}
	else if (FCString::Atof(*HeightValue) - FCString::Atoi(*HeightValue) != 0 || FCString::Atof(*WidthValue) - FCString::Atoi(*WidthValue) != 0 || FCString::Atof(*BombAmountValue) - FCString::Atoi(*BombAmountValue) != 0)
	{
		FMessageDialog::Open(EAppMsgType::Type::Ok, FText::FromString(TEXT("Please enter integers only!")));
		return false;
	}
	else
	{
		Height = FCString::Atoi(*HeightValue);
		Width = FCString::Atoi(*WidthValue);
		BombAmount = FCString::Atoi(*BombAmountValue);

		if (Width < 1 || Height < 1 || BombAmount < 1)
		{
			FMessageDialog::Open(EAppMsgType::Type::Ok, FText::FromString(TEXT("All value should be more than 0!")));
			return false;
		}
		else if (Height * Width - 1 < BombAmount)
		{
			FString MaximumAmountBombError = "Maximum amount of bombs are: ";
			MaximumAmountBombError.AppendInt(Height * Width - 1);
			MaximumAmountBombError.Append(" for the given width and height.");
			FMessageDialog::Open(EAppMsgType::Type::Ok, FText::FromString(MaximumAmountBombError));
			return false;
		}
		else
		{
			return true;
		}
	}
}

#undef LOCTEXT_NAMESPACE