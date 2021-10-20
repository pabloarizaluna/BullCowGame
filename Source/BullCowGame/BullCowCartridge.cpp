// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
	const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordLists/Words.txt");
	FFileHelper::LoadFileToStringArray(Words, *WordListPath);
	this->SetUpGame();
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
	if (this->bGameOver)
	{
		ClearScreen();
		this->SetUpGame();
		return;
	}
	ProcessGuess(Input);
}

void UBullCowCartridge::SetUpGame()
{
	this->HiddenWord = Words[FMath::RandRange(0, Words.Num()-1)];
	this->Lives = this->HiddenWord.Len();
	this->bGameOver = false;

	PrintLine(TEXT("Welcome to Bull Cow game!"));
	PrintLine(FString::Printf(TEXT("HiddenWord is %s"), *this->HiddenWord));
	PrintLine(FString::Printf(TEXT("Guess the %u letters word!"), this->HiddenWord.Len()));
	PrintLine(FString::Printf(TEXT("You have %u lives"), this->HiddenWord.Len()));
	PrintLine(TEXT("Please press Enter to continue..."));
}

void UBullCowCartridge::EndGame()
{
	this->bGameOver = true;

	PrintLine(FString::Printf(TEXT("The hidden word was %s"), *HiddenWord));
	PrintLine(TEXT("Press enter to play again"));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
	if (this->HiddenWord == Guess)
	{
		PrintLine(TEXT("You have won"));
		this->EndGame();
		return;
	}

	if (this->HiddenWord.Len() != Guess.Len())
	{
		PrintLine(FString::Printf(TEXT("The Hidden word is %u characters long"), this->HiddenWord.Len()));
		return;
	}

	if (!this->IsIsogram(Guess))
	{
		PrintLine(TEXT("No repeating letter, guess again"));
		return;
	}

	--this->Lives;

	if (this->Lives < 1)
	{
		PrintLine(TEXT("You have lost"));
		this->EndGame();
		return;
	}

	PrintLine(TEXT("You have lost one live"));

	const FBullCowCount Score = GetBullsCows(Guess);

	PrintLine(FString::Printf(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows));
	PrintLine(FString::Printf(TEXT("Guess again, you have %u lives left"), this->Lives));
}

bool UBullCowCartridge::IsIsogram(const FString& Word)
{
	for (auto LetterIt = Word.CreateConstIterator(); LetterIt; ++LetterIt)
	{
		for (auto SearchIt = LetterIt+1; SearchIt; ++SearchIt)
		{
			if (*LetterIt == *SearchIt)
				return false;
		}
	}
	return true;
}

FBullCowCount UBullCowCartridge::GetBullsCows(const FString& Guess) const
{
	int32 BullsCount = 0;
	int32 CowsCount = 0;

	for (auto LetterIt = Guess.CreateConstIterator(); LetterIt; ++LetterIt)
	{
		int32 Index;
		this->HiddenWord.FindChar(*LetterIt, Index);
		if (Index != INDEX_NONE && Index == LetterIt.GetIndex())
			++BullsCount;
		else if (Index != INDEX_NONE && Index != LetterIt.GetIndex())
			++CowsCount;
	}

	--BullsCount; // Remove the '\0' count

	return FBullCowCount{ BullsCount, CowsCount };
}

