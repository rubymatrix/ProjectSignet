#include "SignetGameEditor.h"
#include "Modules/ModuleManager.h"

DEFINE_LOG_CATEGORY(LogSignetGameEditor);

class FSignetGameEditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override
	{
	}

	virtual void ShutdownModule() override
	{
	}
};

IMPLEMENT_MODULE(FSignetGameEditorModule, SignetGameEditor)