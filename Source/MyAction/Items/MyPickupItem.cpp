#include "Items/MyPickupItem.h"
#include "Components/StaticMeshComponent.h"

AMyPickupItem::AMyPickupItem()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
}

void AMyPickupItem::BeginPlay()
{
	Super::BeginPlay();
}

void AMyPickupItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMyPickupItem::Interact(AActor* InTargetActor)
{
	UE_LOG(LogTemp, Log, TEXT("AMyPickupItem::Interact(AActor* InTargetActor)"));
}