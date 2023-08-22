#include "LegMovementComponent.h"
#include "LegComponentManager.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ULegMovementComponent::ULegMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void ULegMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<ACharacter>(GetOwner());

	LegManager = GetOwner()->GetComponentByClass<ULegComponentManager>();

	if(!LegManager)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, "Could not find LegManager");
	}
}

void ULegMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

FHitResult ULegMovementComponent::Linetrace(FVector Start, FVector End)
{
	FHitResult Hit;
	FCollisionQueryParams Params;

	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(GetOwner());
	Params.AddIgnoredActors(IgnoredActors);

	if (UKismetSystemLibrary::LineTraceSingle(GetWorld(), Start, End, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, IgnoredActors, EDrawDebugTrace::None, Hit, true, FLinearColor::Green, FLinearColor::Red, 1.0f))
	{
		return Hit;
	}
	if (UKismetSystemLibrary::LineTraceSingle(GetWorld(), End, End - FVector(LineTraceDistance * 1.5f, 0.f,0.f), UEngineTypes::ConvertToTraceType(ECC_Visibility), false, IgnoredActors, EDrawDebugTrace::None, Hit, true, FLinearColor::Green, FLinearColor::Red, 1.0f))
	{
		return Hit;
	}
	return Hit;
}

void ULegMovementComponent::UpdateLegWalk(float DeltaTime)
{
	GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, "Walking");
	RayStart = GetComponentLocation();
	RayEnd = GetComponentTransform().GetRotation().GetForwardVector() * LineTraceDistance + RayStart;

	LineTracePos = Linetrace(RayStart, RayEnd).Location;

	TargetPosition = PlayerCharacter->GetMovementComponent()->Velocity / 14.f + LineTracePos;

	if((FootPosition - TargetPosition).Length() > StepDistance && bIsPlanted && LegManager->CheckCanLegUnplant(this))
	{
		if(!bHasStartedStep)
		{
			PlantStartPosition = FootPosition;
			TimeSinceUnplanted = 0.f;
			PercentageReplanted = 0.f;
		}
		bIsPlanted = false; 
		bHasStartedStep = true;
	}
	if(!bIsPlanted)
	{
		TakeFootStep(DeltaTime);
	}
}

void ULegMovementComponent::TakeFootStep(float DeltaTime)
{
	PercentageReplanted = TimeSinceUnplanted / StepTime;

	float t = StepSpeedCurve->GetFloatValue(PercentageReplanted);
	float FootHeight = StepHeightCurve->GetFloatValue(PercentageReplanted) * StepHeight;

	FootPosNoHeight = FMath::Lerp(PlantStartPosition, TargetPosition, t);

	FootPosition = FVector(FootPosNoHeight.X, FootPosNoHeight.Y, FootPosNoHeight.Z + FootHeight);
	

	TimeSinceUnplanted = TimeSinceUnplanted + DeltaTime;

	if(TimeSinceUnplanted >= StepTime)
	{
		bIsPlanted = true;
		bHasStartedStep = false;
	}
}

void ULegMovementComponent::UpdateLegJump()
{
	GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, "Jumping");
	FootPosition = GetComponentLocation() + FVector(0.f,0.f,50.f);
}