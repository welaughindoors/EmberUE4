
#include "EmberUE4.h"
#include "SwordModel.h"
#include "EmberUtilities.h"


ASwordModel::ASwordModel(const class FPostConstructInitializeProperties& PCIP)
    : Super(PCIP)
{
    Mesh = PCIP.CreateAbstractDefaultSubobject<USkeletalMeshComponent>(this, TEXT("Block"));
	
    static ConstructorHelpers::FObjectFinder <UMaterial>Material_Blue(TEXT("Material'/Game/Materials/M_Ground_Grass.M_Ground_Grass'"));

    static ConstructorHelpers::FObjectFinder <USkeletalMesh>SkeletalMesh(TEXT("SkeletalMesh'/Game/ArtAnimation/EP_Weapons/ember_weapon_heavy2.ember_weapon_heavy2'"));		
	SwordMeshes.Add(SkeletalMesh.Object);
	
    static ConstructorHelpers::FObjectFinder <USkeletalMesh>SkeletalMesh2(TEXT("SkeletalMesh'/Game/ArtAnimation/EP_Weapons/ember_weapon_katana.ember_weapon_katana'"));
	SwordMeshes.Add(SkeletalMesh2.Object);

    static ConstructorHelpers::FObjectFinder <USkeletalMesh>SkeletalMesh3(TEXT("SkeletalMesh'/Game/ArtAnimation/EP_Weapons/ember_weapon_heavy.ember_weapon_heavy'"));
	SwordMeshes.Add(SkeletalMesh3.Object);

    Mesh->SetSkeletalMesh(SwordMeshes[1]);
    Mesh->SetMaterial(0, Material_Blue.Object);

    RootComponent = Mesh;
    //Sphere1->OnComponentBeginOverlap.AddDynamic(this, &ASwordModel::OnOverlap);        // set up a notification for when this component overlaps something
    //Sphere1->OnComponentEndOverlap.AddDynamic(this, &ASwordModel::OnOverlap);      // set up a notification for when this component overlaps something
}

void ASwordModel::SetSkeletalMesh(int Index)
{
	if(Index > SwordMeshes.Num()) return;

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
void ASwordModel::TraceSword()
{
            TArray <FVector> nArray;
            TArray <FHitResult> HitResultArray;
            FVector normaled;
            FVector vEnd, vStart;
            float distance;
            FHitResult testHitResult(ForceInit);

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
            for(int i = 0; i < tracerAmount; i++)
                nArray.Add(vStart + (normaled * (distance * i)));
                nArray.Add(vEnd);

            // If this is the first attack, make oldpoints = new points (to avoid zero loc traces)
            if(nArray.Num() != oldPoints.Num()) oldPoints = nArray;

            // Do the trace for each point (debug only atm)
            for (int i = 0; i < nArray.Num(); ++i)
                {
                    if(i < 5)
                        DrawDebugLine(GetWorld(), oldPoints[i],  nArray[i],  FColor(255,0,255),  false, 2, 0,  1 );

                    else if (i >= 5 && i < 10)
                        DrawDebugLine(GetWorld(), oldPoints[i],  nArray[i],  FColor(0,0,255),  false, 2, 0,  1 );

                    else if (i >= 10 && i < 15)
                        DrawDebugLine(GetWorld(), oldPoints[i],  nArray[i],  FColor(34, 139, 34),  false, 2, 0,  1 );

                    if (GetWorld()->LineTraceSingle(testHitResult, nArray[i], oldPoints[i], ECC_WorldStatic, TraceParams))
                        HitResultArray.Add(testHitResult);
                }
			if(HitResultArray.Num() > 0)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, HitResultArray[0].Component->GetName());
				HitResultArray.Empty();
			}
            // Save the current points as old points, to use for next trace
            oldPoints = nArray;
}