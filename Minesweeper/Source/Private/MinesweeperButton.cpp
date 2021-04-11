//MinesweeperButton designed to act as a button to the minesweeper game
//Author - Lukas Kundelis
#include "MinesweeperButton.h"

#define LOCTEXT_NAMESPACE "SMinesweeperButton"

//Function to construct first instance of the button
//InArgs - passes construct arguments to the values
void SMinesweeperButton::Construct(const FArguments& InArgs)
{
	HasBomb = InArgs._HasBomb;
	OnClicked = InArgs._OnClicked;
		ChildSlot
			[
				SNew(SButton)
				.OnClicked(OnClicked)
				.Text(FText::FromString(TEXT("   ")))
			];
}

//Function that returns true if this button instance has a bomb
bool SMinesweeperButton::IsBomb()
{
	return HasBomb.Get();
}

//Function that returns true if button has bombs around it
bool SMinesweeperButton::HasBombsAround()
{
	return BombsAroundValue > 0;
}

//Function to set the value of how many bombs are surrounding the button
//BombsAround - integer value of bombs surounding the button
void SMinesweeperButton::BombsInProximity(int32 BombsAround)
{
	BombsAroundValue = BombsAround;
}

//Reveal of the button information, either can contain a bomb, be a number of how many bombs are around
//or could contain 0 meaning no bombs are around.
void SMinesweeperButton::Reveal()
{
	if (IsBomb())
	{
		ChildSlot
			[
				SNew(SButton)
				.Text(FText::FromString(TEXT("*")))
				.IsEnabled(false)
			];
		IsCellRevealed = true;
	}
	else
	{
		ChildSlot
			[
				SNew(SButton)
				.Text(FText::FromString(FString::FromInt(BombsAroundValue)))
				.IsEnabled(false)
			];
		IsCellRevealed = true;
	}
}

//Returns true if button has already been revealed
bool SMinesweeperButton::IsRevealed()
{
	return IsCellRevealed;
}

#undef LOCTEXT_NAMESPACE


