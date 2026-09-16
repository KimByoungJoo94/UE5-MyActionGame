#include "Components/MyCombatComponent.h"
#include "Equipments/MyWeapon.h"

UMyCombatComponent::UMyCombatComponent()
{	
	PrimaryComponentTick.bCanEverTick = true;
}

void UMyCombatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UMyCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UMyCombatComponent::SetWeapon(AMyWeapon* InWeapon)
{
	MainWeapon = InWeapon;
}

