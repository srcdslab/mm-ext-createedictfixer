#ifdef _INCLUDE_METAMOD_SOURCE_CEF
#endinput
#endif

#define _INCLUDE_METAMOD_SOURCE_CEF

#include <ISmmPlugin.h>
#include "engine_wrappers.h"

edict_t * Hook_CreateEdict(int iIndex);

class CEF : 
	public ISmmPlugin,
    public IConCommandBaseAccessor
{
public:
    bool Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late);
    bool Unload(char *error, size_t maxlen);
    bool Pause(char *error, size_t maxlen);
    bool Unpause(char *error, size_t maxlen);
    void AllPluginsLoaded();
public:
    const char *GetAuthor();
    const char *GetName();
    const char *GetDescription();
    const char *GetURL();
    const char *GetLicense();
    const char *GetVersion();
    const char *GetDate();
    const char *GetLogTag();
public:    //IConCommandBaseAccessor
    bool RegisterConCommandBase(ConCommandBase *pVar);

};

PLUGIN_GLOBALVARS();
