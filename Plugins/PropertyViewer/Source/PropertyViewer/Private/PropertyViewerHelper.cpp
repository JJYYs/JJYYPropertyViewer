// Fill out your copyright notice in the Description page of Project Settings.


#include "PropertyViewerHelper.h"

void FindCacheData(const TArray<UClass*>& AllActorCls,UClass*& CacheCls,UScriptStruct*& CacheStruct,UObject*& OwnerObject,void*& OwnerStruct,TArray<FString>& Tokens)
{
	
	CacheCls = nullptr;
	CacheStruct = nullptr;
	
	FString CurrentToken = Tokens.Pop();
	for (auto Cls:AllActorCls)
	{
		if (Cls->GetName() == CurrentToken)
		{
			CacheCls = Cls;
			break;
		}
	}

	while (Tokens.Num()>0)
	{
		CurrentToken = Tokens.Pop();
		FProperty* FindProperty = nullptr;

		if (CacheCls)
		{
			for (TFieldIterator<FProperty> i(CacheCls); i; ++i)
			{
				FProperty* Prop=*i;
				if (CurrentToken == Prop->GetName())
				{
					FindProperty = Prop;
					if (OwnerObject)
					{
						if (auto ObjectProperty = CastField<FObjectProperty>(FindProperty) )
						{
							OwnerObject = ObjectProperty->GetObjectPropertyValue(ObjectProperty->ContainerPtrToValuePtr<UObject>(OwnerObject));
							OwnerStruct = nullptr;
						}
						else if (auto StructProperty = CastField<FStructProperty>(FindProperty))
						{
							OwnerStruct = StructProperty->ContainerPtrToValuePtr<void>(OwnerObject);
							OwnerObject = nullptr;
						}
					}
					else if (OwnerStruct)
					{
						if (auto ObjectProperty = CastField<FObjectProperty>(FindProperty) )
						{
							OwnerObject = ObjectProperty->GetObjectPropertyValue(ObjectProperty->ContainerPtrToValuePtr<UObject>(OwnerStruct));
							OwnerStruct = nullptr;
						}
						else if (auto StructProperty = CastField<FStructProperty>(FindProperty))
						{
							OwnerStruct = StructProperty->ContainerPtrToValuePtr<void>(OwnerStruct);
							OwnerObject = nullptr;
						}
					}
					break;
				}
			}

			for (TFieldIterator<UFunction> i(CacheCls); i; ++i)
			{
				UFunction* func=*i;
				if (CurrentToken == func->GetName())
				{
					if (func->GetReturnProperty())
					{
						FindProperty = func->GetReturnProperty();
					}
					break;
				}
			}
				
		}

		if (CacheStruct)
		{
			for (TFieldIterator<FProperty> i(CacheStruct); i; ++i)
			{
				FProperty* Prop=*i;
				if (CurrentToken == Prop->GetName())
				{
					FindProperty = Prop;
					break;
				}
			}
		}
			
		CacheCls = nullptr;
		CacheStruct = nullptr;

		if (auto ObjectProperty = CastField<FObjectProperty>(FindProperty) )
		{
			CacheCls = ObjectProperty->PropertyClass;
		}

		if (auto StructProperty = CastField<FStructProperty>(FindProperty) )
		{
			CacheStruct = StructProperty->Struct;
		}
	}
}

void AddPropertyNameAndType(TMap<FString, FString>& OutNames,FProperty* Prop)
{
	if (auto ObjectProperty = CastField<FObjectProperty>(Prop) )
	{
		OutNames.Add(Prop->GetName(),ObjectProperty->PropertyClass->GetName());
	}
	else if (auto StructProperty = CastField<FStructProperty>(Prop) )
	{
		OutNames.Add(Prop->GetName(),StructProperty->Struct->GetName());
	}
	else if (auto BoolProperty = CastField<FBoolProperty>(Prop) )
	{
		OutNames.Add(Prop->GetName(),"Bool");
	}
	else if (auto IntProperty = CastField<FIntProperty>(Prop) )
	{
		OutNames.Add(Prop->GetName(),"Int");
	}
	else if (auto FloatProperty = CastField<FFloatProperty>(Prop) )
	{
		OutNames.Add(Prop->GetName(),"Float");
	}
	else if (auto ByteProperty = CastField<FByteProperty>(Prop) )
	{
		OutNames.Add(Prop->GetName(),"Enum");
	}
	else if (auto EnumProperty = CastField<FEnumProperty>(Prop) )
	{
		OutNames.Add(Prop->GetName(),"Enum");
	}
	else if (auto StrProperty = CastField<FStrProperty>(Prop) )
	{
		OutNames.Add(Prop->GetName(),"String");
	}
	else
	{
		OutNames.Add(Prop->GetName(),Prop->GetClass()->GetName());
	}
}

void GetPropertyType(FString& RetType,FProperty* Prop)
{
	if (auto ObjectProperty = CastField<FObjectProperty>(Prop) )
	{
		RetType = ObjectProperty->PropertyClass->GetName();
	}
	else if (auto StructProperty = CastField<FStructProperty>(Prop) )
	{
		RetType = StructProperty->Struct->GetName();
	}
	else if (auto BoolProperty = CastField<FBoolProperty>(Prop) )
	{
		RetType = "Bool";
	}
	else if (auto IntProperty = CastField<FIntProperty>(Prop) )
	{
		RetType = "Int";
	}
	else if (auto FloatProperty = CastField<FFloatProperty>(Prop) )
	{
		RetType = "Float";
	}
	else if (auto ByteProperty = CastField<FByteProperty>(Prop) )
	{
		RetType = "Enum";
	}
	else if (auto EnumProperty = CastField<FEnumProperty>(Prop) )
	{
		RetType = "Enum";
	}
	else if (auto StrProperty = CastField<FStrProperty>(Prop) )
	{
		RetType = "String";
	}
	else
	{
		RetType = Prop->GetClass()->GetName();
	}
}

template<class T>
bool GetPropertyValue(FProperty* Prop,T* Owner,FPropertyViewerReturnData& RetData)
{
	if (auto StructProperty = CastField<FStructProperty>(Prop) )
	{
		if (StructProperty->Struct->GetName() == "Vector")
		{
			auto Data = StructProperty->ContainerPtrToValuePtr<FVector>(Owner);
			if (Data)
			{
				RetData.Type = EPropertyViewerDataType::Vector;
				RetData.Val_Vector = *Data;
				return true;
			}
		}
		else if (StructProperty->Struct->GetName() == "Rotator")
		{
			auto Data = StructProperty->ContainerPtrToValuePtr<FRotator>(Owner);
			if (Data)
			{
				RetData.Type = EPropertyViewerDataType::Rotator;
				RetData.Val_Rotator = *Data;
				return true;
			}
		}
		else
		{
			FString KeyString;
			auto Data = Prop->ContainerPtrToValuePtr<void>(Owner);
			StructProperty->ExportTextItem(KeyString, Data, Data, nullptr, PPF_PropertyWindow | PPF_BlueprintDebugView, nullptr);

			if (Data)
			{
				RetData.Type = EPropertyViewerDataType::Struct;
				RetData.Val_Struct = KeyString;
				return true;
			}
		}
	}
	else if (auto BoolProperty = CastField<FBoolProperty>(Prop) )
	{
		auto Data = BoolProperty->ContainerPtrToValuePtr<bool>(Owner);
		if (Data)
		{
			RetData.Type = EPropertyViewerDataType::Bool;
			RetData.Val_Bool = *Data;
			return true;
		}
	}
	else if (auto IntProperty = CastField<FIntProperty>(Prop) )
	{
		auto Data = Prop->ContainerPtrToValuePtr<int32>(Owner);
		if (Data)
		{
			RetData.Type = EPropertyViewerDataType::Int;
			RetData.Val_Int = *Data;
			return true;
		}
	}
	else if (auto FloatProperty = CastField<FFloatProperty>(Prop) )
	{
		auto Data = Prop->ContainerPtrToValuePtr<float>(Owner);
		if (Data)
		{
			RetData.Type = EPropertyViewerDataType::Float;
			RetData.Val_Float = *Data;
			return true;
		}
	}
	else if (auto ByteProperty = CastField<FByteProperty>(Prop) )
	{
		auto Data = Prop->ContainerPtrToValuePtr<void>(Owner);
		if (Data)
		{
			RetData.Type = EPropertyViewerDataType::Enum;
			uint8 index = *(uint8*)Data;
			if (ByteProperty->IsEnum())
			{
				RetData.Val_Byte = ByteProperty->Enum->GetNameStringByIndex(index);
			}
			else
			{
				RetData.Val_Byte =  FString::Printf(TEXT("%d"),index);
			}
							
			return true;
		}
	}
	else if (auto EnumProperty = CastField<FEnumProperty>(Prop) )
	{
		auto Data = Prop->ContainerPtrToValuePtr<void>(Owner);
		if (Data)
		{
			RetData.Type = EPropertyViewerDataType::Enum;
			uint8 index = *(uint8*)Data;
			RetData.Val_Byte = EnumProperty->GetEnum()->GetNameStringByIndex(index);
							
			return true;
		}
	}

	return false;
}

void UPropertyViewerHelper::GetTipsArray(FString InTokens, TMap<FString, FString>& OutNames, FString& OutPrevToken,
                                         FString& OutLastToken)
{
	OutPrevToken = "";
	OutLastToken = "";
	OutNames.Empty();
	
	TArray<UClass*> AllActorCls;
	GetDerivedClasses(AActor::StaticClass(), AllActorCls);
	
	TArray<FString> Tokens;
	FString CacheToken = InTokens;
	while (CacheToken.Contains("."))
	{
		FString Left,right;
		CacheToken.Split(".",&Left,&right);
		Tokens.Insert(Left,0);
		OutPrevToken.Append(Left);
		OutPrevToken.Append(".");
		CacheToken = right;
	}
	OutLastToken = CacheToken;
	
	
	if (Tokens.Num()>0)
	{
		UClass* CacheCls = nullptr;
		UScriptStruct* CacheStruct = nullptr;
		UObject* OwnerObject = nullptr;
		void* OwnerStruct = nullptr;
		FindCacheData(AllActorCls,CacheCls,CacheStruct,OwnerObject,OwnerStruct,Tokens);
		
		if (CacheCls)
		{
			//查属性
			for (TFieldIterator<FProperty> i(CacheCls); i; ++i)
			{
				FProperty* Prop=*i;
				AddPropertyNameAndType(OutNames,Prop);
			}
			//查函数
			for (TFieldIterator<UFunction> i(CacheCls); i; ++i)
			{
				UFunction* func=*i; 
				//遍历函数的参数
				int32 ParamNum =0;
				for (TFieldIterator<FProperty> j(func); j; ++j)
				{
					ParamNum++;
				}
				if (auto Prop = func->GetReturnProperty())
				{
					if (ParamNum > 1) continue;
					FString RetType = "void";
					GetPropertyType(RetType,Prop);
					FString ReturnValueName = FString::Printf(TEXT("( )->%s"),*RetType);
					OutNames.Add(func->GetName(),ReturnValueName);
				}
			}
		}

		if (CacheStruct)
		{
			//查属性
			for (TFieldIterator<FProperty> i(CacheStruct); i; ++i)
			{
				FProperty* Prop=*i;
				AddPropertyNameAndType(OutNames,Prop);
			}
		} 
		
	}
	else
	{
		for (auto Cls:AllActorCls)
		{
			FString Name = Cls->GetName();
			if (Name.StartsWith("REINST_")) continue;
			if (Name.StartsWith("SKEL_")) continue;

			OutNames.Add(Name,Cls->GetClass()->GetName());
		}
	}
}

bool UPropertyViewerHelper::CheckIsValidToken(FString InTokens, FString& RetType)
{

	TArray<UClass*> AllActorCls;
	GetDerivedClasses(AActor::StaticClass(), AllActorCls);
	
	TArray<FString> Tokens;
	FString CacheToken = InTokens;
	while (CacheToken.Contains("."))
	{
		FString Left,right;
		CacheToken.Split(".",&Left,&right);
		Tokens.Insert(Left,0);
		CacheToken = right;
	}
	FString LastToken = CacheToken;
	
	if (Tokens.Num()>0)
	{
		UClass* CacheCls = nullptr;
		UScriptStruct* CacheStruct = nullptr;
		UObject* OwnerObject = nullptr;
		void* OwnerStruct = nullptr;
		FindCacheData(AllActorCls,CacheCls,CacheStruct,OwnerObject,OwnerStruct,Tokens);

		if (CacheCls)
		{
			//查属性
			for (TFieldIterator<FProperty> i(CacheCls); i; ++i)
			{
				
				FProperty* Prop=*i;
				if (Prop->GetName() == LastToken)
				{
					GetPropertyType(RetType,Prop);
					return  true;
				}
			}
			//查函数
			for (TFieldIterator<UFunction> i(CacheCls); i; ++i)
			{
				UFunction* func=*i;
				if (func->GetName() != LastToken)
				{
					continue;
				}
				//遍历函数的参数
				int32 ParamNum =0;
				for (TFieldIterator<FProperty> j(func); j; ++j)
				{
					ParamNum++;
				}
				
				RetType = "void";
				if (auto Prop = func->GetReturnProperty())
				{
					if (ParamNum > 1) continue;
					GetPropertyType(RetType,Prop);
					RetType = FString::Printf(TEXT("( )->%s"),*RetType);
					return true;
				}
			}
		}

		if (CacheStruct)
		{
			//查属性
			for (TFieldIterator<FProperty> i(CacheStruct); i; ++i)
			{
				FProperty* Prop=*i;
				if (Prop->GetName() == LastToken)
				{
					GetPropertyType(RetType,Prop);
					return true;
				}
			}
		} 
		
	}
	
	return false;
}

bool UPropertyViewerHelper::GetValueByTokens(AActor* InTarget, FString InTokens,FPropertyViewerReturnData& RetData)
{
	TArray<UClass*> AllActorCls;
	GetDerivedClasses(AActor::StaticClass(), AllActorCls);
	
	TArray<FString> Tokens;
	FString CacheToken = InTokens;
	while (CacheToken.Contains("."))
	{
		FString Left,right;
		CacheToken.Split(".",&Left,&right);
		Tokens.Insert(Left,0);
		CacheToken = right;
	}
	FString LastToken = CacheToken;


	if (Tokens.Num()>0)
	{
		UObject* OwnerObject = InTarget;
		if (Tokens.Top() != InTarget->GetClass()->GetName()) return false;
		
		UClass* CacheCls = nullptr;
		UScriptStruct* CacheStruct = nullptr;
		void* OwnerStruct = nullptr;
		FindCacheData(AllActorCls,CacheCls,CacheStruct,OwnerObject,OwnerStruct,Tokens);
		

		if (CacheCls)
		{
			//查属性
			for (TFieldIterator<FProperty> i(CacheCls); i; ++i)
			{
				FProperty* Prop=*i;
				if (Prop->GetName() == LastToken)
				{
					return GetPropertyValue<UObject>(Prop,OwnerObject,RetData);
				}
			}
			//查函数
			for (TFieldIterator<UFunction> i(CacheCls); i; ++i)
			{
				UFunction* func=*i;
				if (func->GetName() != LastToken)
				{
					continue;
				}
				//遍历函数的参数
				int32 ParamNum =0;
				for (TFieldIterator<FProperty> j(func); j; ++j)
				{
					ParamNum++;
				}

				if (auto Prop = func->GetReturnProperty())
				{
					if (auto ObjectProperty = CastField<FObjectProperty>(Prop) )
					{
					}
					else if (auto StructProperty = CastField<FStructProperty>(Prop) )
					{
						if (StructProperty->Struct->GetName() == "Vector")
						{
							struct Func_Params  
							{
								FVector ReturnValue;
							};
							Func_Params Param;
							OwnerObject->ProcessEvent(func,&Param);
							auto Data = Param.ReturnValue;
							
							RetData.Type = EPropertyViewerDataType::Vector;
							RetData.Val_Vector = Data;
							return true;
						}
						else if (StructProperty->Struct->GetName() == "Rotator")
						{
							struct Func_Params  
							{
								FRotator ReturnValue;
							};
							Func_Params Param;
							OwnerObject->ProcessEvent(func,&Param);
							auto Data = Param.ReturnValue;
							
							RetData.Type = EPropertyViewerDataType::Rotator;
							RetData.Val_Rotator = Data;
							return true;
						}
						else
						{
							void* Param = FMemory_Alloca(func->ParmsSize);
							FMemory::Memzero(Param, func->ParmsSize);
							FEditorScriptExecutionGuard ScriptGuard;
							
							OwnerObject->ProcessEvent(func,Param);

							FString KeyString;
							auto Data = Prop->ContainerPtrToValuePtr<void>(Param);
							StructProperty->ExportTextItem(KeyString, Data, Data, nullptr, PPF_PropertyWindow | PPF_BlueprintDebugView, nullptr);

							if (Data)
							{
								RetData.Type = EPropertyViewerDataType::Struct;
								RetData.Val_Struct = KeyString;
								return true;
							}
							return false;
						}
					}
					else if (auto BoolProperty = CastField<FBoolProperty>(Prop) )
					{
						struct Func_Params  
						{
							bool ReturnValue;
						};
						Func_Params Param;
						OwnerObject->ProcessEvent(func,&Param);
						auto Data = Param.ReturnValue;
												
						RetData.Type = EPropertyViewerDataType::Bool;
						RetData.Val_Bool = Data;
						return true;
					}
					else if (auto IntProperty = CastField<FIntProperty>(Prop) )
					{
						struct Func_Params  
						{
							int32 ReturnValue;
						};
						Func_Params Param;
						OwnerObject->ProcessEvent(func,&Param);
						auto Data = Param.ReturnValue;
							
						RetData.Type = EPropertyViewerDataType::Int;
						RetData.Val_Int = Data;
						return true;
					}
					else if (auto FloatProperty = CastField<FFloatProperty>(Prop) )
					{
						struct Func_Params  
						{
							float ReturnValue;
						};
						Func_Params Param;
						OwnerObject->ProcessEvent(func,&Param);
						auto Data = Param.ReturnValue;
							
						RetData.Type = EPropertyViewerDataType::Float;
						RetData.Val_Float = Data;
						return true;
					}
					else
					{
					}
				}
				return false;
			}
		}

		if (CacheStruct)
		{
			//查属性
			for (TFieldIterator<FProperty> i(CacheStruct); i; ++i)
			{
				FProperty* Prop=*i;
				if (Prop->GetName() == LastToken)
				{
					//Type = Prop->GetClass()->GetName();
					GetPropertyValue<void>(Prop,OwnerStruct,RetData);
					return false;
				}
			}
		} 
	}
	
	return false;
}

TArray<FPropertyViewerUIConfigData> UPropertyViewerHelper::GetUIConfig()
{
	return  GetMutableDefault<UPropertyViewerSettings>()->UIConfig;
}

void UPropertyViewerHelper::SetUIConfig(TArray<FPropertyViewerUIConfigData> UIConfig)
{
	GetMutableDefault<UPropertyViewerSettings>()->UIConfig = UIConfig;
	GetMutableDefault<UPropertyViewerSettings>()->SaveConfig();
}

TMap<FString,FPropertyViewerDrawConfigData> UPropertyViewerHelper::GetTokensList()
{
	return  GetMutableDefault<UPropertyViewerSettings>()->DrawTokensConfig;
}

void UPropertyViewerHelper::SetTokensList(TMap<FString,FPropertyViewerDrawConfigData> TokensList)
{
	GetMutableDefault<UPropertyViewerSettings>()->DrawTokensConfig = TokensList;
	GetMutableDefault<UPropertyViewerSettings>()->SaveConfig();
}

UPropertyViewerSettings* UPropertyViewerHelper::GetDrawTokensConfigObject()
{
	return GetMutableDefault<UPropertyViewerSettings>();
}

void UPropertyViewerHelper::SaveDrawTokensConfig()
{
	GetMutableDefault<UPropertyViewerSettings>()->SaveConfig();
}

