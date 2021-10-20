// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Console/Cartridge.h"
#include "BullCowCartridge.generated.h"

struct FBullCowCount
{
	int32 Bulls = 0;
	int32 Cows = 0;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BULLCOWGAME_API UBullCowCartridge : public UCartridge
{
	GENERATED_BODY()

	public:
	virtual void BeginPlay() override;
	virtual void OnInput(const FString& Input) override;

	// Your declarations go below!
	private:
	FString HiddenWord;
	uint8 Lives;
	bool bGameOver;
	TArray<FString> Words;
	FBullCowCount Count;

	void SetUpGame();
	void EndGame();
	void ProcessGuess(const FString& Guess);
	static bool IsIsogram(const FString& Word);
	FBullCowCount GetBullsCows(const FString& Guess) const;
};
