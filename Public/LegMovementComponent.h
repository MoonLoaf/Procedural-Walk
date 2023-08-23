#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "LegMovementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAM15_GP3_API ULegMovementComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	
	ULegMovementComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	//Primary Movement Functions
	
	UFUNCTION(BlueprintCallable, Category= "Legs Movement")
	void UpdateLegWalk(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category= "Legs Movement")
	void UpdateLegJump();

	UPROPERTY(BlueprintReadOnly, Category="Legs Movement")
	FVector FootPosition = FVector(0.,0.,0.);
	
	bool bIsPlanted;
	
protected:
	
	virtual void BeginPlay() override;

private:

	class ACharacter* PlayerCharacter;

	class ULegComponentManager* LegManager;
	
	//Calculation Functions
	UFUNCTION(BlueprintCallable, Category="LineTrace")
	FHitResult Linetrace(FVector Start, FVector End);
	
	UFUNCTION()
	void TakeFootStep(float DeltaTime);
	
private:

	//Movement vars
	UPROPERTY(EditAnywhere,Category="Movement Variables")
	UCurveFloat* StepSpeedCurve;
	
	UPROPERTY(EditAnywhere,Category="Movement Variables")
	UCurveFloat* StepHeightCurve;

	UPROPERTY(EditAnywhere,Category="Movement Variables")
	float LineTraceDistance;
	
	UPROPERTY(EditAnywhere,Category="Movement Variables")
	float StepDistance;
	
	UPROPERTY(EditAnywhere,Category="Movement Variables")
	float StepTime;
	
	UPROPERTY(EditAnywhere,Category="Movement Variables")
	float StepHeight;

	UPROPERTY(EditAnywhere, Category = "Movement Variables")
	float VelocityDivider;

	FVector PlantStartPosition;
	FVector TargetPosition;

	bool bHasStartedStep = false;

	float TimeSinceUnplanted = 0.;
	float PercentageReplanted = 0.;

	FVector RayStart;
	FVector RayEnd;

	FVector LineTracePos;
};