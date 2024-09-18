// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/SmashCharacterStateJump.h"

#include "Characters/SmashCharacter.h"
#include "Characters/SmashCharacterSettings.h"
#include "Characters/SmashCharacterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"

ESmashCharacterStateID USmashCharacterStateJump::GetStateID()
{
	return ESmashCharacterStateID::Jump;
}

bool USmashCharacterStateJump::CanFall() const
{
	return Character->GetVelocity().Z < 0;
}

void USmashCharacterStateJump::OnInputJump()
{
	if (Character->CanDoJump()) StateMachine->ChangeState(ESmashCharacterStateID::Jump);
}

void USmashCharacterStateJump::OnFastFallInput()
{
	StateMachine->ChangeState(ESmashCharacterStateID::Fall);
}

void USmashCharacterStateJump::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);
	
	Character->GetCharacterMovement()->AirControl = JumpAirControl;
	Character->GetCharacterMovement()->JumpZVelocity = 2 * JumpMaxHeight / JumpDuration;
	Character->GetCharacterMovement()->GravityScale = (-2 * JumpMaxHeight / (JumpDuration*JumpDuration)) / BaseGravityValue;
	Character->DoJump();
	Character->InputJumpEvent.AddDynamic(this, &USmashCharacterStateJump::OnInputJump);
	Character->InputFastFallEvent.AddDynamic(this, &USmashCharacterStateJump::OnFastFallInput);
}

void USmashCharacterStateJump::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);
	Character->GetCharacterMovement()->GravityScale = 1;
	Character->InputJumpEvent.RemoveDynamic(this, &USmashCharacterStateJump::OnInputJump);
	Character->InputFastFallEvent.RemoveDynamic(this, &USmashCharacterStateJump::OnFastFallInput);
}

void USmashCharacterStateJump::StateTick(float DeltaTime)
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
