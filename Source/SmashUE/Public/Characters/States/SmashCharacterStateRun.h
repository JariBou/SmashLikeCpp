﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/SmashCharacterState.h"
#include "UObject/Object.h"
#include "SmashCharacterStateRun.generated.h"

class UCharacterMovementComponent;
/**
 * 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SMASHUE_API USmashCharacterStateRun : public USmashCharacterState
{
	GENERATED_BODY()

public:
	virtual ESmashCharacterStateID GetStateID() override;

	virtual void StateInit(USmashCharacterStateMachine* InStateMachine) override;

	virtual void StateEnter(ESmashCharacterStateID PreviousStateID) override;

	virtual void StateExit(ESmashCharacterStateID NextStateID) override;

	virtual void StateTick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere)
	float Speed = 5.f;

	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> MovementComponent;
};
