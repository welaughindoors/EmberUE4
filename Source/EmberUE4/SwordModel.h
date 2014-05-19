

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
    TSubobjectPtr<USkeletalMeshComponent> Mesh;

    UPROPERTY(VisibleAnywhere, Category = Mesh)
    USkeletalMesh * SwordMesh1;

    UPROPERTY(VisibleAnywhere, Category = Mesh)
    USkeletalMesh * SwordMesh2;

    UPROPERTY(VisibleAnywhere, Category = Mesh)
    USkeletalMesh * SwordMesh3;
    /** called when something overlaps the sphere component */
    //  UFUNCTION(BlueprintNativeEvent, Category="Switch Functions")
    //  void OnOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    /** the desired intensity for the light */
    //  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Switch Variables")
    // float DesiredIntensity;
public:

    void SetSkeletalMesh(int Index);
    void TraceSword();

    int tracerAmount;

	FVector StartSocket();
	FVector EndSocket();
    TArray <FVector> oldPoints;
};