#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyStatGameMode.generated.h"

class UMyStartMenuWidget;

UCLASS()
class MYACTION_API AMyStatGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UMyStartMenuWidget> StartMenuWidgetClass;

	UPROPERTY(Transient)
	TObjectPtr<UMyStartMenuWidget> StartMenuWidget;
};
