// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/SmashCharacterStateFall.h"

#include "Characters/SmashCharacter.h"
#include "Characters/SmashCharacterSettings.h"
#include "Characters/SmashCharacterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"

ESmashCharacterStateID USmashCharacterStateFall::GetStateID()
{
	return ESmashCharacterStateID::Fall;
}

bool USmashCharacterStateFall::CanFall() const
{
	return false;
}

void USmashCharacterStateFall::OnInputJump()
{
	if (Character->CanDoJump()) StateMachine->ChangeState(ESmashCharacterStateID::Jump);
}

void USmashCharacterStateFall::OnFastFallInput()
{
	StateMachine->ChangeState(ESmashCharacterStateID::Fall);
}

void USmashCharacterStateFall::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);
	Character->GetCharacterMovement()->AirControl = FallAirControl;
	Character->GetCharacterMovement()->GravityScale = Character->ShouldFastFall() ? FastFallGravityScale : FallGravityScale;
	if(PreviousStateID != ESmashCharacterStateID::Jump) Character->ConsumeJump();

	Character->InputJumpEvent.AddDynamic(this, &USmashCharacterStateFall::OnInputJump);
	Character->InputFastFallEvent.AddDynamic(this, &USmashCharacterStateFall::OnFastFallInput);
}

void USmashCharacterStateFall::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);
	Character->GetCharacterMovement()->GravityScale = 1;
	
	Character->InputJumpEvent.RemoveDynamic(this, &USmashCharacterStateFall::OnInputJump);
	Character->InputFastFallEvent.RemoveDynamic(this, &USmashCharacterStateFall::OnFastFallInput);
}

void USmashCharacterStateFall::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	if (FMath::Abs(Character->GetInputMoveX()) > SMSettings->XAxisThreshold)
	{
		Character->SetOrientX(Character->GetInputMoveX());
		Character->AddMovementInput(FVector::ForwardVector, Character->GetOrientX());
	}
	
	if (Character->GetCharacterMovement()->IsMovingOnGround())
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Idle);
	}
}
