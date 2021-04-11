#pragma once

#include "Minesweeper.h"

class SMinesweeperButton : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SMinesweeperButton){}

	//Attribute that sets if button has a bomb
	SLATE_ATTRIBUTE(bool, HasBomb)

	//Event that fires when the button is clicked
	SLATE_EVENT(FOnClicked, OnClicked)

	SLATE_END_ARGS()

public:
	//Funtion declarations
	void Construct(const FArguments& InArgs);
	bool IsBomb();
	bool HasBombsAround();
	void BombsInProximity(int32 BombsAround);
	void Reveal();
	bool IsRevealed();
private:
	//Variables to be used for the MinesweeperButton
	TAttribute<bool> HasBomb;
	FOnClicked OnClicked;
	int32 BombsAroundValue;
	bool IsCellRevealed = false;
};