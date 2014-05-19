
#include "EmberUE4.h"
#include "SwordModel.h"
#include "EmberUtilities.h"


ASwordModel::ASwordModel(const class FPostConstructInitializeProperties& PCIP)
    : Super(PCIP)
{
    Mesh = PCIP.CreateAbstractDefaultSubobject<USkeletalMeshComponent>(this, TEXT("Block"));


    static ConstructorHelpers::FObjectFinder <UMaterial>Material_Blue(TEXT("Material'/Game/Materials/M_Ground_Grass.M_Ground_Grass'"));

    //Sphere1 = PCIP.CreateDefaultSubobject<USphereComponent>(this, TEXT("Sphere1"));
    static ConstructorHelpers::FObjectFinder <USkeletalMesh>SkeletalMesh1(TEXT("SkeletalMesh'/Game/ArtAnimation/EP_Weapons/ember_weapon_heavy2.ember_weapon_heavy2'"));
    SwordMesh1 = SkeletalMesh1.Object;
    static ConstructorHelpers::FObjectFinder <USkeletalMesh>SkeletalMesh2(TEXT("SkeletalMesh'/Game/ArtAnimation/EP_Weapons/ember_weapon_katana.ember_weapon_katana'"));
    SwordMesh2 = SkeletalMesh2.Object;
    static ConstructorHelpers::FObjectFinder <USkeletalMesh>SkeletalMesh3(TEXT("SkeletalMesh'/Game/ArtAnimation/EP_Weapons/ember_weapon_heavy.ember_weapon_heavy'"));
    SwordMesh3 = SkeletalMesh3.Object;

    Mesh->SetSkeletalMesh(SwordMesh2);
    Mesh->SetMaterial(0, Material_Blue.Object);

    RootComponent = Mesh;
    //Sphere1->OnComponentBeginOverlap.AddDynamic(this, &ASwordModel::OnOverlap);        // set up a notification for when this component overlaps something
    //Sphere1->OnComponentEndOverlap.AddDynamic(this, &ASwordModel::OnOverlap);      // set up a notification for when this component overlaps something
	//tracerAmount = 15;
}

void ASwordModel::SetSkeletalMesh(int Index)
{
    switch(Index)
        {
        case 0:
            Mesh->SetSkeletalMesh(SwordMesh1);
            break;
        case 1:
            Mesh->SetSkeletalMesh(SwordMesh2);
            break;
        case 2:
            Mesh->SetSkeletalMesh(SwordMesh3);
            break;
        }
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
            FVector normaled;
            FVector vEnd, vStart;
            float distance;

            vEnd = EndSocket();
            vStart = StartSocket();

            // Get normal vector along sword, and length of sword
            normaled = EmberUtilities::Normal(vEnd - vStart);
            distance = EmberUtilities::VSize(vEnd - vStart);

			distance /= tracerAmount - 1;

            // Create points along sword to trace from
				nArray.Add(vStart);
            for(int i = 1; i < tracerAmount; i++)
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
                }
            // Save the current points as old points, to use for next trace
            oldPoints = nArray;
}