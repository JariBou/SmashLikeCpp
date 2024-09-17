// Fill out your copyright notice in the Description page of Project Settings.


#include "MatchGameMode.h"

#include "Arena/ArenaPlayerStart.h"
#include "Kismet/GameplayStatics.h"

void AMatchGameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<AArenaPlayerStart*> PlayerStartPoints;
	FindPlayerStartActorsInArena(PlayerStartPoints);

	for (AArenaPlayerStart* PlayerStartPoint : PlayerStartPoints)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, PlayerStartPoint->GetFName().ToString());
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
