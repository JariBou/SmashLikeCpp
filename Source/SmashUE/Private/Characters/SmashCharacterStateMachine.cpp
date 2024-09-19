// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SmashCharacterStateMachine.h"

#include "SmashCharacterState.h"
#include "SmashCharacterStateSettings.h"
#include "Characters/SmashCharacter.h"
#include "Characters/SmashCharacterSettings.h"
#include "Logging/StructuredLog.h"

void USmashCharacterStateMachine::Init(ASmashCharacter* InCharacter)
{
	Character = InCharacter;
	FindStates();
	InitStates();

	ChangeState(ESmashCharacterStateID::Idle);
}

void USmashCharacterStateMachine::Tick(float DeltaTime)
{
	if (CurrentState == nullptr) return;
	CurrentState->StateTick(DeltaTime);
}

void USmashCharacterStateMachine::ChangeState(ESmashCharacterStateID NextStateID)
{
	USmashCharacterState* NextState = GetState(NextStateID);

	if (NextState == nullptr) return;

	if (CurrentState != nullptr) CurrentState->StateExit(NextStateID);

	ESmashCharacterStateID PreviousStateID = CurrentStateID;
	CurrentState = NextState;
	CurrentStateID = NextStateID;

	if (CurrentState != nullptr) CurrentState->StateEnter(PreviousStateID);
}

USmashCharacterState* USmashCharacterStateMachine::GetState(ESmashCharacterStateID StateID)
{
	for (USmashCharacterState* State : AllStates)
	{
		if (StateID == State->GetStateID()) return State;
	}

	return nullptr;
}

ASmashCharacter* USmashCharacterStateMachine::GetCharacter() const
{
	return Character;
}

void USmashCharacterStateMachine::FindStates()
{
	// TArray<UActorComponent*> FoundComponents = Character->K2_GetComponentsByClass(USmashCharacterState::StaticClass());
	
	TArray<ESmashCharacterStateID> PossibleStates;
	const USmashCharacterSettings* SMSettings = GetDefault<USmashCharacterSettings>();
	SMSettings->StateMap.GetKeys(PossibleStates);

	if (PossibleStates.IsEmpty()) return;
	
	for (const ESmashCharacterStateID State : PossibleStates)
	{
		if (State == ESmashCharacterStateID::None) continue;
		const FSmashCharacterStateSettings* StateSettings;
		if (Character->OverrideStates.Contains(State))
		{
			StateSettings = Character->OverrideStates.Find(State);
			// USmashCharacterState* NewState = NewObject<USmashCharacterState>(this, *SubclassOf);
			// NewState->SetMontage(nullptr);
			// AllStates.Add(NewState);
			
			// Soooo.... we need a *.... why??
			// AllStates.Add(Cast<USmashCharacterState>(SubclassOf));
		} else
		{
			StateSettings = SMSettings->StateMap.Find(State);
			// USmashCharacterState* NewState = NewObject<USmashCharacterState>(this, *SubclassOf);
			// NewState->SetMontage(nullptr);
			// AllStates.Add(NewState);
		}

		USmashCharacterState* NewState = NewObject<USmashCharacterState>(this, *StateSettings->State);
		NewState->SetMontage(StateSettings->Montage);
		AllStates.Add(NewState);
	}
	
	// for (UActorComponent* StateComponent : FoundComponents)
	// {
	// 	USmashCharacterState* State = Cast<USmashCharacterState>(StateComponent);
	// 	if (State == nullptr) continue;
	// 	if (State->GetStateID() == ESmashCharacterStateID::None) continue;
	//
	// 	AllStates.Add(State);
	// }
}

void USmashCharacterStateMachine::InitStates()
{
	for (USmashCharacterState* State : AllStates)
	{
		State->StateInit(this);
	}
}
