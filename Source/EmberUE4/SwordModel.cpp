
#include "EmberUE4.h"
#include "SwordModel.h"


ASwordModel::ASwordModel(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	Sphere1 = PCIP.CreateAbstractDefaultSubobject<USkeletalMeshComponent>(this, TEXT("Block")); 

 
static ConstructorHelpers::FObjectFinder <UMaterial>Material_Blue(TEXT("Material'/Game/Materials/M_Ground_Grass.M_Ground_Grass'"));
 
    //Sphere1 = PCIP.CreateDefaultSubobject<USphereComponent>(this, TEXT("Sphere1"));
	static ConstructorHelpers::FObjectFinder <USkeletalMesh>SkeletalMesh1(TEXT("SkeletalMesh'/Game/ArtAnimation/EP_Weapons/ember_weapon_heavy2.ember_weapon_heavy2'"));
		SwordMesh1 = SkeletalMesh1.Object;
	static ConstructorHelpers::FObjectFinder <USkeletalMesh>SkeletalMesh(TEXT("SkeletalMesh'/Game/ArtAnimation/EP_Weapons/ember_weapon_katana.ember_weapon_katana'"));	
		SwordMesh2 = SkeletalMesh.Object;
	static ConstructorHelpers::FObjectFinder <USkeletalMesh>SkeletalMesh2(TEXT("SkeletalMesh'/Game/ArtAnimation/EP_Weapons/ember_weapon_heavy.ember_weapon_heavy'"));
		SwordMesh3 = SkeletalMesh2.Object;

    Sphere1->SetSkeletalMesh(SwordMesh2);
	Sphere1->SetMaterial(0, Material_Blue.Object);

    RootComponent = Sphere1;
    //Sphere1->OnComponentBeginOverlap.AddDynamic(this, &ASwordModel::OnOverlap);        // set up a notification for when this component overlaps something
    //Sphere1->OnComponentEndOverlap.AddDynamic(this, &ASwordModel::OnOverlap);      // set up a notification for when this component overlaps something
}

void ASwordModel::SetSkeletalMesh(int Index)
{
	switch(Index)
	{
	case 0:
		Sphere1->SetSkeletalMesh(SwordMesh1);
		break;
	case 1:
		Sphere1->SetSkeletalMesh(SwordMesh2);
		break;
	case 2:
		Sphere1->SetSkeletalMesh(SwordMesh3);
		break;
	}
//static ConstructorHelpers::FObjectFinder <USkeletalMesh>SkeletalMesh(TEXT("SkeletalMesh'/Game/ArtAnimation/EP_Weapons/ember_weapon_heavy.ember_weapon_heavy'"));
//SwordMesh = SkeletalMesh.Object;
    //Sphere1->SetSkeletalMesh(SwordMesh);

}