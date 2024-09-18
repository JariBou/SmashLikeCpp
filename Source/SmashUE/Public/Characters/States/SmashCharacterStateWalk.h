// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/SmashCharacterState.h"
#include "UObject/Object.h"
#include "SmashCharacterStateWalk.generated.h"

class UCharacterMovementComponent;
/**
 * 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SMASHUE_API USmashCharacterStateWalk : public USmashCharacterState
{
	GENERATED_BODY()

public:
	virtual ESmashCharacterStateID GetStateID() override;

	virtual void StateInit(USmashCharacterStateMachine* InStateMachine) override;

	virtual void StateEnter(ESmashCharacterStateID PreviousStateID) override;

	virtual void StateExit(ESmashCharacterStateID NextStateID) override;

	virtual void StateTick(float DeltaTime) override;

	UFUNCTION()
	void OnInputMoveXFast(float X);

	UPROPERTY(EditAnywhere)
	float MaxSpeed = 500.f;

	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> MovementComponent;
};
