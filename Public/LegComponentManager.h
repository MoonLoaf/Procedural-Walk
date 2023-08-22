#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LegComponentManager.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAM15_GP3_API ULegComponentManager : public UActorComponent
{
	GENERATED_BODY()

public:
	
	ULegComponentManager();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category="Legs Movements")
	bool CheckCanLegUnplant(class ULegMovementComponent* LegComponent);

	UFUNCTION(BlueprintCallable, Category="Legs")
	TArray<ULegMovementComponent*> GetAllLegs();

	UPROPERTY(BlueprintReadOnly, Category="Leg")
	FVector PlayerVelocity;

protected:
	
	virtual void BeginPlay() override;

private:
	
	UPROPERTY(VisibleAnywhere, Category="Legs")
	TArray<class ULegMovementComponent*> LegArray;

	void GetLegReferences();
	
	void WalkingUpdate(float DeltaTime);

	void JumpingUpdate();

	class ACharacter* PlayerCharacter;
		
};