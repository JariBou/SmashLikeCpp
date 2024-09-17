// Fill out your copyright notice in the Description page of Project Settings.


#include "MatchGameMode.h"

#include "SmashCharacter.h"
#include "Arena/ArenaPlayerStart.h"
#include "Kismet/GameplayStatics.h"

void AMatchGameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<AArenaPlayerStart*> PlayerStartPoints;
	FindPlayerStartActorsInArena(PlayerStartPoints);

	for (AArenaPlayerStart* PlayerStartPoint : PlayerStartPoints)
	{
		EAutoReceiveInput::Type InputType = PlayerStartPoint->AutoReceiveInput.GetValue();
		

		TSubclassOf<ASmashCharacter> SmashCharacterClass = GetSmashCharacterClassFromInputType(InputType);
		if (SmashCharacterClass == nullptr) continue;

		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, SmashCharacterClass->GetFName().ToString());
	}
}

void AMatchGameMode::FindPlayerStartActorsInArena(TArray<AArenaPlayerStart*>& OutPlayerStartActors) const
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AArenaPlayerStart::StaticClass(), FoundActors);

	for (int i = 0; i < FoundActors.Num(); ++i)
	{
		AArenaPlayerStart* ArenaStart = Cast<AArenaPlayerStart>(FoundActors[i]);
		if (ArenaStart == nullptr) continue;

		OutPlayerStartActors.Add(ArenaStart);
	}
}

TSubclassOf<ASmashCharacter> AMatchGameMode::GetSmashCharacterClassFromInputType(
	EAutoReceiveInput::Type InputType) const
{
	switch (InputType)
	{
		case EAutoReceiveInput::Player0:
			return SmashCharacterP0;
		
		case EAutoReceiveInput::Player1:
			return SmashCharacterP1;
		
		case EAutoReceiveInput::Player2:
			return SmashCharacterP2;
		
		case EAutoReceiveInput::Player3:
			return SmashCharacterP3;
		
		case EAutoReceiveInput::Player4:
		case EAutoReceiveInput::Player5:
		case EAutoReceiveInput::Player6:
		case EAutoReceiveInput::Player7:
		default:
			return nullptr;
	}
}
