// Copyright Yuuki Ogino. All Rights Reserved


#include "VoicevoxQueryFactory.h"
#include "VoicevoxQuery.h"

UVoicevoxQueryFactory::UVoicevoxQueryFactory():Super()
{
	bCreateNew = true;
	SupportedClass = UVoicevoxQuery::StaticClass();
}

UObject* UVoicevoxQueryFactory::FactoryCreateNew(UClass* InClass, UObject* InParent,
                                                    FName InName,  EObjectFlags Flags,
											 UObject* Context, FFeedbackContext* Warn )
{
	
	return NewObject<UVoicevoxQuery>(InParent, InClass, InName, Flags, Context);
}


