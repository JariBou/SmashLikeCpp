// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/SmashCharacterStateRun.h"

#include "Characters/SmashCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

ESmashCharacterStateID USmashCharacterStateRun::GetStateID()
{
	return ESmashCharacterStateID::Run;
}

void USmashCharacterStateRun::StateInit(USmashCharacterStateMachine* InStateMachine)
{
	Super::StateInit(InStateMachine);
	MovementComponent = Character->FindComponentByClass<UCharacterMovementComponent>();
}

void USmashCharacterStateRun::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);
	
	GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Cyan,
		TEXT("Enter StateRun"));
}

void USmashCharacterStateRun::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);

	GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Red,
		TEXT("Exit StateRun"));
}

void USmashCharacterStateRun::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	
	FVector MovementVector = FVector(Speed * Character->GetOrientX() * -1, 0, 0);
	MovementComponent->ApplyWorldOffset(MovementVector, false);

	GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Green,
		TEXT("Tick StateRun"));
}
