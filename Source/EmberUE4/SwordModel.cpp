
#include "EmberUE4.h"
#include "SwordModel.h"
#include "EmberUtilities.h"


ASwordModel::ASwordModel(const class FPostConstructInitializeProperties &PCIP)
    : Super(PCIP)
{
    Mesh = PCIP.CreateAbstractDefaultSubobject<USkeletalMeshComponent>(this, TEXT("Block"));

    static ConstructorHelpers::FObjectFinder <UMaterial>Material_Blue(TEXT("Material'/Game/Materials/M_Ground_Grass.M_Ground_Grass'"));

    static ConstructorHelpers::FObjectFinder <USkeletalMesh>SkeletalMesh(TEXT("SkeletalMesh'/Game/ArtAnimation/EP_Weapons/ember_weapon_heavy2.ember_weapon_heavy2'"));
    SwordMeshes.Add(SkeletalMesh.Object);

    //static ConstructorHelpers::FObjectFinder <USkeletalMesh>SkeletalMesh2(TEXT("SkeletalMesh'/Game/ArtAnimation/EP_Weapons/ember_weapon_katana.ember_weapon_katana'"));
    //SwordMeshes.Add(SkeletalMesh2.Object);

    //static ConstructorHelpers::FObjectFinder <USkeletalMesh>SkeletalMesh3(TEXT("SkeletalMesh'/Game/ArtAnimation/EP_Weapons/ember_weapon_heavy.ember_weapon_heavy'"));
    //SwordMeshes.Add(SkeletalMesh3.Object);

    Mesh->SetSkeletalMesh(SwordMeshes[0]);
    Mesh->SetMaterial(0, Material_Blue.Object);

    RootComponent = Mesh;
    //Sphere1->OnComponentBeginOverlap.AddDynamic(this, &ASwordModel::OnOverlap);        // set up a notification for when this component overlaps something
    //Sphere1->OnComponentEndOverlap.AddDynamic(this, &ASwordModel::OnOverlap);      // set up a notification for when this component overlaps something
}

void ASwordModel::SetSkeletalMesh(int Index)
{
    if(EmberUtilities::SafeToUseInArray(Index, SwordMeshes.Num()))
        Mesh->SetSkeletalMesh(SwordMeshes[Index]);
}

FVector ASwordModel::StartSocket()
{
    return Mesh->GetSocketLocation("StartControl");
}

FVector ASwordModel::EndSocket()
{
    return Mesh->GetSocketLocation("EndControl");
}

void ASwordModel::ResetSword()
{
    oldPoints.Empty();
}

void ASwordModel::TraceSword()
{
    TArray <FVector> nArray;
    TArray <FHitResult> HitResultArray;
    FVector normaled;
    FVector vEnd, vStart;
    float distance;
    int32 iCategory;
    FHitResult testHitResult(ForceInit);

	iCategory = 0;

    FCollisionQueryParams TraceParams(TEXT("SwordTrace"), true, this);
    TraceParams.bTraceComplex = true;
    TraceParams.bTraceAsyncScene = true;
    TraceParams.bReturnPhysicalMaterial = false;

    vEnd = EndSocket();
    vStart = StartSocket();

    // Get normal vector along sword, and length of sword
    normaled = EmberUtilities::Normal(vEnd - vStart);
    distance = EmberUtilities::VSize(vEnd - vStart);

    distance /= tracerAmount - 1;

    // Create points along sword to trace from
    for (int i = 0; i < tracerAmount; i++)
        nArray.Add(vStart + (normaled * (distance * i)));
    nArray.Add(vEnd);

    // If this is the first attack, make oldpoints = new points (to avoid zero loc traces)
    if (nArray.Num() != oldPoints.Num()) oldPoints = nArray;

    // Do the trace for each point
    for (int i = 0; i < nArray.Num(); ++i)
    {
        if (i < nArray.Num() / 3)
        {
            iCategory = 1;
            DrawDebugLine(GetWorld(), oldPoints[i],  nArray[i],  FColor(255, 0, 255),  false, 2, 0,  1 );
        }

        else if (i >= nArray.Num() / 3 && i < (2 * nArray.Num()) / 3)
        {
            iCategory = 2;
            DrawDebugLine(GetWorld(), oldPoints[i],  nArray[i],  FColor(0, 0, 255),  false, 2, 0,  1 );
        }

        else if (i >= (2 * nArray.Num()) / 3 && i < nArray.Num())
        {
            iCategory = 3;
            DrawDebugLine(GetWorld(), oldPoints[i],  nArray[i],  FColor(34, 139, 34),  false, 2, 0,  1 );
        }

        // Actual real trace
        if (GetWorld()->LineTraceSingle(testHitResult, nArray[i], oldPoints[i], ECC_WorldStatic, TraceParams))
            CalculateDamage(testHitResult, iCategory);
    }
    // Save the current points as old points, to use for next trace
    oldPoints = nArray;
}

/**
* Calculates damage based on HitResult and Category of Damage
* @param HitResult gives actor (if hit) and how to procede
* @param iCategory determines which category of damage the hitActor will receive
*/
void ASwordModel::CalculateDamage(FHitResult HitResult, int32 iCategory)
{
    EmberUtilities::Debug(HitResult.Component->GetName());
}