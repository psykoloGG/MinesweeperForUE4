#pragma once

#include "Minesweeper.h"
#include "MinesweeperButton.h"

class SMinesweeperWidget : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SMinesweeperWidget) {}

	SLATE_END_ARGS()

public:

	void Construct(const FArguments& InArgs);

private:
	//Text for the labels of UI
	const FText HeightInputBoxLabel = FText::FromString(TEXT("Height:"));
	const FText WidthInputBoxLabel = FText::FromString(TEXT("Width:"));
	const FText BombAmountLabel = FText::FromString(TEXT("Bomb amount:"));
	const FText GenerateButtonLabel = FText::FromString(TEXT("Generate new minefield"));
	
	//Storage for input field values as a text
	FString HeightValue;
	FString WidthValue;
	FString BombAmountValue;

	//Storage for information about the game field and individual buttons
	int32 Height;
	int32 Width;
	int32 BombAmount;
	int32 SizeOfField;
	int32 RandomNumber;
	int32 ButtonIndex;
	int32 RevealedTiles;

	//Array for predetermined bomb spots
	TArray<int32> BombSpots;
	
	//Structure that enables TArray to be two dimensional array, allowing to be used as a grid
	//with x and y axes
	struct Coordinates {
	public:
		TArray<TSharedRef<SMinesweeperButton>> ColumnCoordinate;
		
		TSharedRef<SMinesweeperButton> operator[] (int32 i)
		{
			return ColumnCoordinate[i];
		}

		void Add(TSharedRef<SMinesweeperButton> MinefieldButton)
		{
			ColumnCoordinate.Add(MinefieldButton);
		}
	};

	TArray<Coordinates> RowCoordinate;
private:
	//Functions declarations
	void BombProximity();
	void BombPlacement();
	void HandleOnHeightChange(const FText& InText);
	void HandleOnWidthChange(const FText& InText);
	void HandleOnBombAmountChange(const FText& InText);
	bool IsInputValid();
	void GenerateMinefield();
	void RevealField();
	void IsWon();
	FReply HandleOnGenerateButtonClicked();
	FReply HandleMinefieldButtonClicked(int32 i, int32 j);
};