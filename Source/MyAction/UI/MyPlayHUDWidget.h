#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyPlayHUDWidget.generated.h"

enum class EMyAttributeType : uint8;
class UMyStatBarWidget;
struct FMyAttributeChangeParam;


UCLASS()
class MYACTION_API UMyPlayHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	void OnAttributeChanged(EMyAttributeType InAttributeType, const FMyAttributeChangeParam& InChangeParam);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMyStatBarWidget> StaminaBarWidget;

private:
	FDelegateHandle OnAttributeChangedHandle;
};
