#pragma once
#include "Characters/SmashCharacterState.h"

USTRUCT(BlueprintType)
struct FSmashCharacterStateSettings
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<USmashCharacterState> State;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> Montage;

	FSmashCharacterStateSettings()
	{
		State = nullptr;
		Montage = nullptr;
	}

};