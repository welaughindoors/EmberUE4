

#pragma once

#include "GameFramework/Actor.h"
#include "SwordModel.generated.h"

/**
 * 
 */
UCLASS()
class ASwordModel : public AActor
{
	GENERATED_UCLASS_BODY()
    /** sphere component */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Switch Components")
    TSubobjectPtr<USkeletalMeshComponent> Sphere1;

    /** called when something overlaps the sphere component */
  //  UFUNCTION(BlueprintNativeEvent, Category="Switch Functions")
  //  void OnOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    /** the desired intensity for the light */
  //  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Switch Variables")
   // float DesiredIntensity;

};
