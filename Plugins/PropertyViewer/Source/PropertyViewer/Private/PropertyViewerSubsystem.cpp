#include "PropertyViewerSubsystem.h"

#include "DrawDebugHelpers.h"
#include "PropertyViewerHelper.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(PropertyViewer, Log, All)

void StringLog(AActor* Actor, FString LogData)
{
	UE_LOG(PropertyViewer, Log, TEXT("%s:%s"), *Actor->GetName(), *LogData);
}

void UPropertyViewerSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FlushDebugStrings(GetWorld());

	//bool IsServer = GetWorld()->GetNetMode() == ENetMode::NM_DedicatedServer;
	auto Tokens = UPropertyViewerHelper::GetTokensList();
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), Actors);
	FPropertyViewerReturnData RetData;
	for (auto Actor : Actors)
	{
		for (auto Token : Tokens)
		{
			bool RetValue = UPropertyViewerHelper::GetValueByTokens(Actor, Token.Key, RetData);

			if (!RetValue) continue;

			auto Config = Token.Value;
			if (Config.LogType == EPropertyViewerLogType::StringLog)
			{
				if (RetData.Type == EPropertyViewerDataType::Vector)
				{
					if (Config.PrintToLog)
					{
						StringLog(Actor, RetData.Val_Vector.ToString());
					}
					DrawDebugString(Actor->GetWorld(),
					                Actor->GetActorLocation() + Config.Offset,
					                RetData.Val_Vector.ToString(),
					                nullptr,
					                Config.Color.ToFColor(true));
				}
				else if (RetData.Type == EPropertyViewerDataType::Rotator)
				{
					if (Config.PrintToLog)
					{
						StringLog(Actor, RetData.Val_Rotator.ToString());
					}
					DrawDebugString(Actor->GetWorld(),
					                Actor->GetActorLocation() + Config.Offset,
					                RetData.Val_Rotator.ToString(),
					                nullptr,
					                Config.Color.ToFColor(true));
				}
				else if (RetData.Type == EPropertyViewerDataType::Int)
				{
					if (Config.PrintToLog)
					{
						StringLog(Actor, FString::Printf(TEXT("%d"), RetData.Val_Int));
					}
					DrawDebugString(Actor->GetWorld(),
					                Actor->GetActorLocation() + Config.Offset,
					                FString::Printf(TEXT("%d"), RetData.Val_Int),
					                nullptr,
					                Config.Color.ToFColor(true));
				}
				else if (RetData.Type == EPropertyViewerDataType::Bool)
				{
					FString DrawData = RetData.Val_Bool ? "True" : "False";
					if (Config.PrintToLog)
					{
						StringLog(Actor, FString::Printf(TEXT("%s"), *DrawData));
					}
					DrawDebugString(Actor->GetWorld(),
					                Actor->GetActorLocation() + Config.Offset,
					                FString::Printf(TEXT("%s"), *DrawData),
					                nullptr,
					                Config.Color.ToFColor(true));
				}
				else if (RetData.Type == EPropertyViewerDataType::String)
				{
					if (Config.PrintToLog)
					{
						StringLog(Actor, FString::Printf(TEXT("%s"), *RetData.Val_String));
					}
					DrawDebugString(Actor->GetWorld(),
					                Actor->GetActorLocation() + Config.Offset,
					                FString::Printf(TEXT("%s"), *RetData.Val_String),
					                nullptr,
					                Config.Color.ToFColor(true));
				}
				else if (RetData.Type == EPropertyViewerDataType::Float)
				{
					if (Config.PrintToLog)
					{
						StringLog(Actor, FString::Printf(TEXT("%f"), RetData.Val_Float));
					}
					DrawDebugString(Actor->GetWorld(),
					                Actor->GetActorLocation() + Config.Offset,
					                FString::Printf(TEXT("%f"), RetData.Val_Float),
					                nullptr,
					                Config.Color.ToFColor(true));
				}
				else if (RetData.Type == EPropertyViewerDataType::Enum)
				{
					if (Config.PrintToLog)
					{
						StringLog(Actor, FString::Printf(TEXT("%s"), *RetData.Val_Byte));
					}
					DrawDebugString(Actor->GetWorld(),
					                Actor->GetActorLocation() + Config.Offset,
					                FString::Printf(TEXT("%s"), *RetData.Val_Byte),
					                nullptr,
					                Config.Color.ToFColor(true));
				}
				else if (RetData.Type == EPropertyViewerDataType::Struct)
				{
					if (Config.PrintToLog)
					{
						StringLog(Actor, FString::Printf(TEXT("%s"), *RetData.Val_Struct));
					}
					DrawDebugString(Actor->GetWorld(),
					                Actor->GetActorLocation() + Config.Offset,
					                FString::Printf(TEXT("%s"), *RetData.Val_Struct),
					                nullptr,
					                Config.Color.ToFColor(true));
				}
			}
			else if (Config.LogType == EPropertyViewerLogType::VectorLog)
			{
				if (RetData.Type == EPropertyViewerDataType::Vector)
				{
					FVector DrawData = Config.NormalizeVector
						                   ? RetData.Val_Vector.GetSafeNormal() * Config.Radius
						                   : RetData.Val_Vector;
					DrawDebugDirectionalArrow(Actor->GetWorld(),
					                          Actor->GetActorLocation() + Config.Offset,
					                          Actor->GetActorLocation() + Config.Offset + DrawData,
					                          10,
					                          Config.Color.ToFColor(true),
					                          false,
					                          -1,
					                          0,
					                          Config.Thickness
					);
				}
				else if (RetData.Type == EPropertyViewerDataType::Rotator)
				{
					FVector DrawData = Config.NormalizeVector
						                   ? RetData.Val_Rotator.Vector().GetSafeNormal() * Config.Radius
						                   : RetData.Val_Rotator.Vector();
					DrawDebugDirectionalArrow(Actor->GetWorld(),
					                          Actor->GetActorLocation() + Config.Offset,
					                          Actor->GetActorLocation() + Config.Offset + DrawData,
					                          10,
					                          Config.Color.ToFColor(true),
					                          false,
					                          -1,
					                          0,
					                          Config.Thickness
					);
				}
				else if (RetData.Type == EPropertyViewerDataType::Int)
				{
					DrawDebugString(Actor->GetWorld(),
					                Config.Offset,
					                FString::Printf(TEXT("%d"), RetData.Val_Int),
					                Actor,
					                Config.Color.ToFColor(true)
					);
				}
				else if (RetData.Type == EPropertyViewerDataType::Bool)
				{
					FString DrawData = RetData.Val_Bool ? "True" : "False";
					DrawDebugString(Actor->GetWorld(),
					                Config.Offset,
					                FString::Printf(TEXT("%s"), *DrawData),
					                Actor,
					                Config.Color.ToFColor(true)
					);
				}
				else if (RetData.Type == EPropertyViewerDataType::String)
				{
					DrawDebugString(Actor->GetWorld(),
					                Config.Offset,
					                FString::Printf(TEXT("%s"), *RetData.Val_String),
					                Actor,
					                Config.Color.ToFColor(true)
					);
				}
				else if (RetData.Type == EPropertyViewerDataType::Float)
				{
					DrawDebugString(Actor->GetWorld(),
					                Config.Offset,
					                FString::Printf(TEXT("%f"), RetData.Val_Float),
					                Actor,
					                Config.Color.ToFColor(true)
					);
				}
				else if (RetData.Type == EPropertyViewerDataType::Enum)
				{
					DrawDebugString(Actor->GetWorld(),
					                Config.Offset,
					                FString::Printf(TEXT("%s"), *RetData.Val_Byte),
					                Actor,
					                Config.Color.ToFColor(true)
					);
				}
				else if (RetData.Type == EPropertyViewerDataType::Struct)
				{
					DrawDebugString(Actor->GetWorld(),
					                Config.Offset,
					                FString::Printf(TEXT("%s"), *RetData.Val_Struct),
					                Actor,
					                Config.Color.ToFColor(true)
					);
				}
			}
			else if (Config.LogType == EPropertyViewerLogType::VisualLog)
			{
				if (RetData.Type == EPropertyViewerDataType::Vector)
				{
					FVector DrawData = Config.NormalizeVector
						                   ? RetData.Val_Vector.GetSafeNormal() * Config.Radius
						                   : RetData.Val_Vector;
					FVisualLogger::ArrowLogf(Actor,
					                         FName(TEXT("PropertyViewer")),
					                         ELogVerbosity::Type::Log,
					                         Actor->GetActorLocation() + Config.Offset,
					                         Actor->GetActorLocation() + Config.Offset + DrawData,
					                         Config.Color.ToFColor(true),
					                         TEXT("%s"),
					                         *DrawData.ToString()
					);
				}
				else if (RetData.Type == EPropertyViewerDataType::Rotator)
				{
					FVector DrawData = Config.NormalizeVector
						                   ? RetData.Val_Rotator.Vector().GetSafeNormal() * Config.Radius
						                   : RetData.Val_Rotator.Vector();
					FVisualLogger::ArrowLogf(Actor,
					                         FName(TEXT("PropertyViewer")),
					                         ELogVerbosity::Type::Log,
					                         Actor->GetActorLocation() + Config.Offset,
					                         Actor->GetActorLocation() + Config.Offset + DrawData,
					                         Config.Color.ToFColor(true),
					                         TEXT("%s"),
					                         *RetData.Val_Rotator.ToString()
					);
				}
				else if (RetData.Type == EPropertyViewerDataType::Int)
				{
					FVisualLogger::GeometryShapeLogf(Actor,
					                                 FName(TEXT("PropertyViewer")),
					                                 ELogVerbosity::Type::Log,
					                                 Actor->GetActorLocation() + Config.Offset,
					                                 10,
					                                 Config.Color.ToFColor(true),
					                                 TEXT("%s"),
					                                 *FString::Printf(TEXT("%d"), RetData.Val_Int)
					);
				}
				else if (RetData.Type == EPropertyViewerDataType::Bool)
				{
					FString DrawData = RetData.Val_Bool ? "True" : "False";
					FVisualLogger::GeometryShapeLogf(Actor,
					                                 FName(TEXT("PropertyViewer")),
					                                 ELogVerbosity::Type::Log,
					                                 Actor->GetActorLocation() + Config.Offset,
					                                 10,
					                                 Config.Color.ToFColor(true),
					                                 TEXT("%s"),
					                                 *FString::Printf(TEXT("%s"), *DrawData)
					);
				}
				else if (RetData.Type == EPropertyViewerDataType::String)
				{
					FVisualLogger::GeometryShapeLogf(Actor,
					                                 FName(TEXT("PropertyViewer")),
					                                 ELogVerbosity::Type::Log,
					                                 Actor->GetActorLocation() + Config.Offset,
					                                 10,
					                                 Config.Color.ToFColor(true),
					                                 TEXT("%s"),
					                                 *FString::Printf(TEXT("%s"), *RetData.Val_String)
					);
				}
				else if (RetData.Type == EPropertyViewerDataType::Float)
				{
					FVisualLogger::GeometryShapeLogf(Actor,
					                                 FName(TEXT("PropertyViewer")),
					                                 ELogVerbosity::Type::Log,
					                                 Actor->GetActorLocation() + Config.Offset,
					                                 10,
					                                 Config.Color.ToFColor(true),
					                                 TEXT("%s"),
					                                 *FString::Printf(TEXT("%f"), RetData.Val_Float)
					);
				}
				else if (RetData.Type == EPropertyViewerDataType::Enum)
				{
					FVisualLogger::GeometryShapeLogf(Actor,
					                                 FName(TEXT("PropertyViewer")),
					                                 ELogVerbosity::Type::Log,
					                                 Actor->GetActorLocation() + Config.Offset,
					                                 10,
					                                 Config.Color.ToFColor(true),
					                                 TEXT("%s"),
					                                 *FString::Printf(TEXT("%s"), *RetData.Val_Byte)
					);
				}
				else if (RetData.Type == EPropertyViewerDataType::Struct)
				{
					FVisualLogger::GeometryShapeLogf(Actor,
					                                 FName(TEXT("PropertyViewer")),
					                                 ELogVerbosity::Type::Log,
					                                 Actor->GetActorLocation() + Config.Offset,
					                                 10,
					                                 Config.Color.ToFColor(true),
					                                 TEXT("%s"),
					                                 *FString::Printf(TEXT("%s"), *RetData.Val_Struct)
					);
				}
			}
		}
	}
}

bool UPropertyViewerSubsystem::IsTickableInEditor() const
{
	return false;
}

TStatId UPropertyViewerSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UPropertyViewerSubsystem, STATGROUP_Tickables);
}
