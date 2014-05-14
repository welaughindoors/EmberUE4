
#include "EmberUE4.h"
#include "SwordModel.h"


ASwordModel::ASwordModel(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
static ConstructorHelpers::FObjectFinder <USkeletalMesh>SkeletalMesh(TEXT("SkeletalMesh'/Game/ArtAnimation/EP_Weapons/ember_weapon_katana.ember_weapon_katana'"));
 
static ConstructorHelpers::FObjectFinder <UMaterial>Material_Blue(TEXT("Material'/Game/Materials/M_Ground_Grass.M_Ground_Grass'"));
 
    //Sphere1 = PCIP.CreateDefaultSubobject<USphereComponent>(this, TEXT("Sphere1"));
	Sphere1 = PCIP.CreateAbstractDefaultSubobject<USkeletalMeshComponent>(this, TEXT("Block")); 
	
    Sphere1->SetSkeletalMesh(SkeletalMesh.Object);
	Sphere1->SetMaterial(0, Material_Blue.Object);
    RootComponent = Sphere1;

    //Sphere1->OnComponentBeginOverlap.AddDynamic(this, &ASwordModel::OnOverlap);        // set up a notification for when this component overlaps something
    //Sphere1->OnComponentEndOverlap.AddDynamic(this, &ASwordModel::OnOverlap);      // set up a notification for when this component overlaps something
}
