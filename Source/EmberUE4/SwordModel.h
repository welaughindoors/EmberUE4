

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
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sword Components")
    TSubobjectPtr<USkeletalMeshComponent> Mesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sword Variables")
    float tracerAmount;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
    TArray<USkeletalMesh *> SwordMeshes;


    UFUNCTION(BlueprintCallable, Category = "Sword Functions")
    void SetSkeletalMesh(int32 Index);

    UFUNCTION(BlueprintCallable, Category = "Sword Functions")
    void TraceSword();

    UFUNCTION(BlueprintCallable, Category = "Sword Functions")
    FVector StartSocket();

    UFUNCTION(BlueprintCallable, Category = "Sword Functions")
    FVector EndSocket();
    /** the desired intensity for the light */
    //  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Switch Variables")
    // float DesiredIntensity;
public:
    TArray <FVector> oldPoints;
};