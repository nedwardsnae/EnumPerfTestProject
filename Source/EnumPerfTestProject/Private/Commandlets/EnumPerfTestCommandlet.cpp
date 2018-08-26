///////////////////////////////////////////////////////////////////////////
// Filename: EnumPerfTestCommandlet.cpp
///////////////////////////////////////////////////////////////////////////

// EnumPerfTestProject
#include "EnumPerfTestCommandlet.h"
#include "Types/TestEnum.h"

// Engine
#include "Class.h"
#include "Package.h"
#include "ProfilingDebugging/ScopedTimers.h"

///////////////////////////////////////////////////////////////////////////
// Forward Declarations

enum class EOtherTestEnum : uint8;

///////////////////////////////////////////////////////////////////////////

constexpr int32 GetAccumulatedTestEnumValue()
{
	int32 AccumulatedValue = 0;
	for (int32 Index = 0; Index < (int32)EMyTestEnum::MAX; ++Index)
		AccumulatedValue += Index;
	return AccumulatedValue;
}

///////////////////////////////////////////////////////////////////////////

#define NAMIFY_ENUM(EnumEntry) TEXT(#EnumEntry),

///////////////////////////////////////////////////////////////////////////

static const FName TestEnumNames[(int32)EMyTestEnum::MAX] =
{
	FOREACH_ENUM_EMYTESTENUM(NAMIFY_ENUM)
};

///////////////////////////////////////////////////////////////////////////

namespace EnumStringHelpers_Private
{
	FORCEINLINE FString GetTrimmedEnumEntryName(const TCHAR* InEnumEntryName)
	{
		// We strip off the enum prefix and just return the entry name by itself.
		check(InEnumEntryName);
		const TCHAR* Result = FCString::Strrstr(InEnumEntryName, TEXT("::"));
		checkf(Result && Result[2] != (TCHAR)'0', TEXT("'%s' does not look like an enum entry"), InEnumEntryName);
		return FString(Result + 2);
	}
}

///////////////////////////////////////////////////////////////////////////

#define STRINGIFY_ENUM(EnumEntry) EnumStringHelpers_Private::GetTrimmedEnumEntryName(TEXT(#EnumEntry)),

///////////////////////////////////////////////////////////////////////////

static const FString TestEnumStrings[(int32)EMyTestEnum::MAX] =
{
	FOREACH_ENUM_EMYTESTENUM(STRINGIFY_ENUM)
};

///////////////////////////////////////////////////////////////////////////

int32 GetAccumulatedTestEnumStringLengths()
{
	int32 AccumulatedValue = 0;
	for (int32 Index = 0; Index < (int32)EMyTestEnum::MAX; ++Index)
		AccumulatedValue += TestEnumStrings[Index].Len();
	return AccumulatedValue;
}

///////////////////////////////////////////////////////////////////////////

int32 UEnumPerfTestCommandlet::Main(const FString& Params)
{
	constexpr int32 AccumulatedTestEnumValue = GetAccumulatedTestEnumValue();
	constexpr int32 TestIterations           = 10000;

	double FindObjectDuration = 0.0;
	for (int32 Iteration = 0; Iteration < TestIterations; ++Iteration)
	{
		FScopedDurationTimer ScopedDurationTimer(FindObjectDuration);
		int32 AccumulatedValue = 0;
		UEnum* EnumClass = FindObject<UEnum>(ANY_PACKAGE, TEXT("EMyTestEnum"));
		for (int32 Index = 0; Index < (int32)EMyTestEnum::MAX; ++Index)
			AccumulatedValue += EnumClass->GetValueByIndex(Index);

		check(AccumulatedValue == AccumulatedTestEnumValue);
	}

	double FindObjectExactDuration = 0.0;
	for (int32 Iteration = 0; Iteration < TestIterations; ++Iteration)
	{
		FScopedDurationTimer ScopedDurationTimer(FindObjectExactDuration);
		int32 AccumulatedValue = 0;
		UEnum* EnumClass = FindObject<UEnum>(ANY_PACKAGE, TEXT("/Script/EnumPerfTestProject.EMyTestEnum"), true);
		for (int32 Index = 0; Index < (int32)EMyTestEnum::MAX; ++Index)
			AccumulatedValue += EnumClass->GetValueByIndex(Index);

		check(AccumulatedValue == AccumulatedTestEnumValue);
	}

	double StaticEnumDuration = 0.0;
	for (int32 Iteration = 0; Iteration < TestIterations; ++Iteration)
	{
		FScopedDurationTimer ScopedDurationTimer(StaticEnumDuration);
		int32 AccumulatedValue = 0;
		UEnum* EnumClass = StaticEnum<EMyTestEnum>();
		for (int32 Index = 0; Index < (int32)EMyTestEnum::MAX; ++Index)
			AccumulatedValue += EnumClass->GetValueByIndex(Index);

		check(AccumulatedValue == AccumulatedTestEnumValue);
	}

	double StaticEnumNaiveDuration = 0.0;
	for (int32 Iteration = 0; Iteration < TestIterations; ++Iteration)
	{
		FScopedDurationTimer ScopedDurationTimer(StaticEnumNaiveDuration);
		int32 AccumulatedValue = 0;
		for (int32 Index = 0; Index < (int32)EMyTestEnum::MAX; ++Index)
		{
			UEnum* EnumClass = StaticEnum<EMyTestEnum>();
			AccumulatedValue += EnumClass->GetValueByIndex(Index);
		}

		check(AccumulatedValue == AccumulatedTestEnumValue);
	}

	UE_LOG(LogTemp, Display, TEXT("[FindObject]      Total: %.10f, Avg: %.10f"), FindObjectDuration,      FindObjectDuration      / (double)TestIterations);
	UE_LOG(LogTemp, Display, TEXT("[FindObjectExact] Total: %.10f, Avg: %.10f"), FindObjectExactDuration, FindObjectExactDuration / (double)TestIterations);
	UE_LOG(LogTemp, Display, TEXT("[StaticEnum]      Total: %.10f, Avg: %.10f"), StaticEnumDuration,      StaticEnumDuration      / (double)TestIterations);
	UE_LOG(LogTemp, Display, TEXT("[StaticEnumNaive] Total: %.10f, Avg: %.10f"), StaticEnumNaiveDuration, StaticEnumNaiveDuration / (double)TestIterations);

	const int32 AccumulatedTestEnumStringLengths = GetAccumulatedTestEnumStringLengths();

	double RawStringRetrievalDuration = 0.0;
	for (int32 Iteration = 0; Iteration < TestIterations; ++Iteration)
	{
		FScopedDurationTimer ScopedDurationTimer(RawStringRetrievalDuration);
		int32 AccumulatedValue = 0;
		for (int32 Index = 0; Index < (int32)EMyTestEnum::MAX; ++Index)
		{
			AccumulatedValue += TestEnumStrings[Index].Len();
		}

		check(AccumulatedValue == AccumulatedTestEnumStringLengths);
	}

	double OldMethodStringRetrievalDuration = 0.0;
	for (int32 Iteration = 0; Iteration < TestIterations; ++Iteration)
	{
		FScopedDurationTimer ScopedDurationTimer(OldMethodStringRetrievalDuration);
		int32 AccumulatedValue = 0;
		UEnum* EnumClass = FindObject<UEnum>(ANY_PACKAGE, TEXT("EMyTestEnum"));
		for (int32 Index = 0; Index < (int32)EMyTestEnum::MAX; ++Index)
		{
			AccumulatedValue += EnumClass->GetNameStringByIndex(Index).Len();
		}

		check(AccumulatedValue == AccumulatedTestEnumStringLengths);
	}

	double NewMethodStringRetrievalDuration = 0.0;
	for (int32 Iteration = 0; Iteration < TestIterations; ++Iteration)
	{
		FScopedDurationTimer ScopedDurationTimer(NewMethodStringRetrievalDuration);
		int32 AccumulatedValue = 0;
		UEnum* EnumClass = StaticEnum<EMyTestEnum>();
		for (int32 Index = 0; Index < (int32)EMyTestEnum::MAX; ++Index)
		{
			AccumulatedValue += EnumClass->GetNameStringByIndex(Index).Len();
		}

		check(AccumulatedValue == AccumulatedTestEnumStringLengths);
	}

	double NewNaiveMethodStringRetrievalDuration = 0.0;
	for (int32 Iteration = 0; Iteration < TestIterations; ++Iteration)
	{
		FScopedDurationTimer ScopedDurationTimer(NewNaiveMethodStringRetrievalDuration);
		int32 AccumulatedValue = 0;
		for (int32 Index = 0; Index < (int32)EMyTestEnum::MAX; ++Index)
		{
			UEnum* EnumClass = StaticEnum<EMyTestEnum>();
			AccumulatedValue += EnumClass->GetNameStringByIndex(Index).Len();
		}

		check(AccumulatedValue == AccumulatedTestEnumStringLengths);
	}

	UE_LOG(LogTemp, Display, TEXT("[Raw]      Total: %.10f, Avg: %.10f"), RawStringRetrievalDuration,            RawStringRetrievalDuration            / (double)TestIterations);
	UE_LOG(LogTemp, Display, TEXT("[Old]      Total: %.10f, Avg: %.10f"), OldMethodStringRetrievalDuration,      OldMethodStringRetrievalDuration      / (double)TestIterations);
	UE_LOG(LogTemp, Display, TEXT("[New]      Total: %.10f, Avg: %.10f"), NewMethodStringRetrievalDuration,      NewMethodStringRetrievalDuration      / (double)TestIterations);
	UE_LOG(LogTemp, Display, TEXT("[NewNaive] Total: %.10f, Avg: %.10f"), NewNaiveMethodStringRetrievalDuration, NewNaiveMethodStringRetrievalDuration / (double)TestIterations);

	UEnum* ForwardDeclaredEnum = StaticEnum<EOtherTestEnum>();
	
	for (int32 Index = 0; Index < ForwardDeclaredEnum->NumEnums(); ++Index)
	{
		UE_LOG(LogTemp, Display, TEXT("%d: %s"), Index, *ForwardDeclaredEnum->GetNameStringByIndex(Index));
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////