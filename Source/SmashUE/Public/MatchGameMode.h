﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MatchGameMode.generated.h"

class AArenaPlayerStart;
class ASmashCharacter;

/**
 * 
 */
UCLASS()
class SMASHUE_API AMatchGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere)
	TArray<ASmashCharacter*> CharactersInsideArena;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<ASmashCharacter> SmashCharacterP0;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<ASmashCharacter> SmashCharacterP1;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<ASmashCharacter> SmashCharacterP2;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<ASmashCharacter> SmashCharacterP3;
	
	
private:
	void FindPlayerStartActorsInArena(TArray<AArenaPlayerStart*>& OutPlayerStartActors) const;

	void SpawnCharacters(const TArray<AArenaPlayerStart*>& SpawnPoints);

	TSubclassOf<ASmashCharacter> GetSmashCharacterClassFromInputType(EAutoReceiveInput::Type InputType) const;
};
