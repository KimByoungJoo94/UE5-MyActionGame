#include "MyEquipment.h"
#include "Components/StaticMeshComponent.h"
#include "Character/MyCharacter.h"


AMyEquipment::AMyEquipment()
{	
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(MeshComponent);
}

void AMyEquipment::BeginPlay()
{
	Super::BeginPlay();
}

void AMyEquipment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMyEquipment::AttachToOwner(const FName InSocketName)
{
	if (AMyCharacter* OwnerCharacter = Cast<AMyCharacter>(GetOwner()))
	{
		if (USkeletalMeshComponent* OwnerCharacterMesh = OwnerCharacter->GetMesh())
		{
			AttachToComponent(OwnerCharacterMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), InSocketName);
		}
	}
}


