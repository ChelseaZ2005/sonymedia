#include "Modules/ModuleManager.h"

class FAlienDocGeminiVAModule : public IModuleInterface
{
public:
	virtual void StartupModule() override {}
	virtual void ShutdownModule() override {}
};

IMPLEMENT_MODULE(FAlienDocGeminiVAModule, AlienDocGeminiVA)


