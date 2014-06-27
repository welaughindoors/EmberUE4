

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

    /** Amount of tracers present during sword trace */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ember Sword Variables")
    float tracerAmount;

    /** Skeletal Meshs that the sword can access */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
    TArray<USkeletalMesh *> SwordMeshes;

    /** Changes the skeletal mesh by index (int). Meshs are determined in BluePrint Constructor */
    UFUNCTION(BlueprintCallable, Category = "Ember Sword Functions")
    void SetSkeletalMesh(int32 Index);

    /** Traces sword for 1 tick. */
    UFUNCTION(BlueprintCallable, Category = "Ember Sword Functions")
    void TraceSword();

    /** Resets sword for new trace segment. Must be called before each attack */
    UFUNCTION(BlueprintCallable, Category = "Ember Sword Functions")
    void ResetSword();

    /** Retrieves StartSocket position of sword */
    UFUNCTION(BlueprintCallable, Category = "Ember Sword Functions")
    FVector StartSocket();

    /** Retrieves EndSocket position of sword */
    UFUNCTION(BlueprintCallable, Category = "Ember Sword Functions")
    FVector EndSocket();

    /**
    * Calculates damage based on HitResult and Category of Damage
    * @param HitResult gives actor (if hit) and how to procede
    * @param iCategory determines which category of damage the hitActor will receive
    */
    UFUNCTION(BlueprintCallable, Category = "Ember Sword Functions")
    void CalculateDamage(FHitResult HitResult, int32 iCategory);

public:
    TArray <FVector> oldPoints;
    AController * parent;
};