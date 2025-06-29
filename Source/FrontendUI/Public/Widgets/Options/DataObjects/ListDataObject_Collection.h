// Dmytro Chesniuk All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/DataObjects/ListDataObject_Base.h"
#include "ListDataObject_Collection.generated.h"

UCLASS()
class FRONTENDUI_API UListDataObject_Collection : public UListDataObject_Base
{
	GENERATED_BODY()

public:
	void AddChildListData(UListDataObject_Base* InChildListData);
	
	virtual TArray<UListDataObject_Base*> GetAllChildListData() const override;
	virtual bool HasAnyChildListData() const override;
	
private:
	UPROPERTY(Transient)
	TArray<UListDataObject_Base*> ChildListDataArray;
	
};
