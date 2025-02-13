#pragma once

#include "CoreMinimal.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

/***** Check *****/
#define CheckTure(x) { if(x == true) return; }
#define CheckTrueResult(x, y) { if(x == true) return y; }

#define CheckFalse(x) { if(x == false) return; }
#define CheckFalseResult(x, y) { if(x == false) return y; }

#define CheckNull(x) { if(x == nullptr) return; }
#define CheckNullResult(x, y) { if(x == nullptr) return y; }


class DOOMTEMP_API C_Helpers
{
public:
	/***** Component *****/
	template<typename T>
	static void CreateComponent(AActor* InActor, T** OutComponent, FName InName, USceneComponent* InParent = nullptr)
	{
		*OutComponent = InActor->CreateDefaultSubobject<T>( InName );
		if (!!InParent)
		{
			(*OutComponent)->SetupAttachment(InParent);
			return;
		}
		InActor->SetRootComponent(*OutComponent);
	}


	template<typename T>
	static void CreateActorComponent(AActor* InActor, T** OutComponent, FName InName)
	{
		*OutComponent = InActor->CreateDefaultSubobject<T>( InName );
	}


	/***** Get *****/
	template<typename T>
	static void GetAsset(T** OutObject, FString InPath)
	{
		ConstructorHelpers::FObjectFinder<T> asset(*InPath);
		if(asset.Succeeded())
			*OutObject = asset.Object;
	}

	template<typename T>
	static void GetAssetDynamic(T** OutObject, FString InPath)
	{
		*OutObject = Cast<T>(StaticLoadObject(T::StaticClass(), nullptr, *InPath));
	}

	template<typename T>
	static void GetClass(TSubclassOf<T>* OutClass, FString InPath)
	{
		ConstructorHelpers::FClassFinder<T> asset(*InPath);
		*OutClass = asset.Class;
	}

	template<typename T>
	static T* GetComponent(AActor* InActor)
	{
		return Cast<T>( InActor->GetComponentByClass(T::StaticClass()));
	}

	template<typename T>
	static T* GetComponent(AActor* InActor, const FString& InName)
	{
		TArray<T*> components;
		InActor->GetComponents<T>( components );

		for(T* myComp : components)
			if(myComp->GetName() == InName)
				return myComp;

		return nullptr;
	}


	/***** Attach *****/
	static void AttachTo(AActor* InActor, USceneComponent* InParent, FName InSocketName)
	{
		InActor->AttachToComponent(InParent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), InSocketName);
	}
};
