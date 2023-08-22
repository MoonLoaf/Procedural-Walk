#include "LegComponentManager.h"

#include "LegMovementComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ULegComponentManager::ULegComponentManager()
{
	PrimaryComponentTick.bCanEverTick = true;
}

TArray<ULegMovementComponent*> ULegComponentManager::GetAllLegs()
{
	return LegArray;
}

void ULegComponentManager::BeginPlay()
{
	Super::BeginPlay();
		
	GetLegReferences();
}

bool ULegComponentManager::CheckCanLegUnplant(ULegMovementComponent* LegComponent)
{
	// Find the other leg in the array
	ULegMovementComponent* OtherLeg = nullptr;
	for (ULegMovementComponent* Leg : LegArray)
	{
		if (Leg != LegComponent)
		{
			OtherLeg = Leg;
			break;
		}
	}

	// If the other leg is unplanted, cannot unplant
	if (OtherLeg && !OtherLeg->bIsPlanted)
	{
		return false;
	}

	// If the other leg is planted or doesn't exist, can unplant
	return true;
}

void ULegComponentManager::GetLegReferences()
{
	PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (PlayerCharacter)
	{
		TArray<UActorComponent*> Components;
		PlayerCharacter->GetComponents(Components);

		for (UActorComponent* Component : Components)
		{
			ULegMovementComponent* LegMovementComponent = Cast<ULegMovementComponent>(Component);
			if (LegMovementComponent)
			{
				LegArray.Add(LegMovementComponent);
			}
		}
	}
}

void ULegComponentManager::WalkingUpdate(float DeltaTime)
{
	for (ULegMovementComponent* Component : LegArray)
	{
		Component->UpdateLegWalk(DeltaTime);
	}
}

void ULegComponentManager::JumpingUpdate()
{
	for (ULegMovementComponent* Component : LegArray)
	{
		Component->UpdateLegJump();
	}
}


void ULegComponentManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	PlayerVelocity = PlayerCharacter->GetMovementComponent()->Velocity;

	if(PlayerCharacter->GetMovementComponent()->IsMovingOnGround())
	{
		WalkingUpdate(DeltaTime);
	}
	else
	{
		JumpingUpdate();
	}
}

