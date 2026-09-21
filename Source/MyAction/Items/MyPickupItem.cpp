#include "Items/MyPickupItem.h"
#include "Components/StaticMeshComponent.h"
#include "Equipments/MyEquipment.h"
#include "Common/MyDefines.h"

AMyPickupItem::AMyPickupItem()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetCollisionObjectType(MY_COLLISION_OBJECT_INTERACTION);
	MeshComponent->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	MeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void AMyPickupItem::BeginPlay()
{
	Super::BeginPlay();
}

void AMyPickupItem::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (TargetItemClass)
	{
		if (AMyEquipment* CDO = TargetItemClass->GetDefaultObject<AMyEquipment>())
		{
			MeshComponent->SetStaticMesh(CDO->GetMeshAsset());
			// MeshComponent->SetSimulatePhysics(true);
		}
	}
}

void AMyPickupItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMyPickupItem::Interact(AActor* InTargetActor)
{
	if (UWorld* World = GetWorld())
	{
		if (InTargetActor && TargetItemClass)
		{
			FActorSpawnParameters ActorSpawnParam;
			ActorSpawnParam.Owner = InTargetActor;

			AMyEquipment* SpawnedItem = World->SpawnActor<AMyEquipment>(TargetItemClass, GetActorTransform(), ActorSpawnParam);
			if (SpawnedItem)
			{
				SpawnedItem->Equip();
				Destroy();
			}
		}
	}
}