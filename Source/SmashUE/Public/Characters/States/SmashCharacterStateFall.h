// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/SmashCharacterState.h"
#include "UObject/Object.h"
#include "SmashCharacterStateFall.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SMASHUE_API USmashCharacterStateFall : public USmashCharacterState
{
	GENERATED_BODY()
	
	virtual ESmashCharacterStateID GetStateID() override;

	virtual bool CanFall() const override;

	UFUNCTION()
	void OnInputJump();

	UFUNCTION()
	void OnFastFallInput();
	
	virtual void StateEnter(ESmashCharacterStateID PreviousStateID) override;
	
	virtual void StateExit(ESmashCharacterStateID NextStateID) override;

	virtual void StateTick(float DeltaTime) override;

private:	
	UPROPERTY(EditAnywhere)
	float FallAirControl = 1;
	
	UPROPERTY(EditAnywhere)
	float FallGravityScale = 3;

	UPROPERTY(EditAnywhere)
	float FastFallGravityScale = 10;
};
