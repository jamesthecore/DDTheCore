#include "Libraries/DD_NetLibrary.h"

bool UDD_NetLibrary::IsServer(const UObject* WorldContextObject)
{
	const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	return World && World->GetNetMode() < ENetMode::NM_Client;
}

bool UDD_NetLibrary::IsClient(const UObject* WorldContextObject)
{
	const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	return World && World->GetNetMode() != ENetMode::NM_DedicatedServer;
}
