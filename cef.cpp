#include "cef.h"

using namespace SourceHook;

CEF g_CEF;

PLUGIN_EXPOSE(CEF, g_CEF);

IVEngineServer *engine = NULL;
CGlobalVars *gpGlobals = NULL;

SH_DECL_HOOK1(IVEngineServer, CreateEdict, SH_NOATTRIB, 0, edict_t *, int);

#if !defined ORANGEBOX_BUILD
ICvar* g_pCVar = NULL;
#endif

ICvar* GetICVar()
{
#if defined METAMOD_PLAPI_VERSION
#if SOURCE_ENGINE==SE_ORANGEBOX || SOURCE_ENGINE==SE_LEFT4DEAD || SOURCE_ENGINE==SE_LEFT4DEAD2 || SOURCE_ENGINE==SE_TF2 || SOURCE_ENGINE==SE_DODS || SOURCE_ENGINE==SE_HL2DM || SOURCE_ENGINE==SE_NUCLEARDAWN || \
    SOURCE_ENGINE==SE_ALIENSWARM || SOURCE_ENGINE==SE_BLOODYGOODTIME || SOURCE_ENGINE==SE_CSGO || SOURCE_ENGINE==SE_CSS || SOURCE_ENGINE==SE_INSURGENCY || SOURCE_ENGINE==SE_SDK2013 || SOURCE_ENGINE== SE_BMS
    return (ICvar *)((g_SMAPI->GetEngineFactory())(CVAR_INTERFACE_VERSION, NULL));
#else
    return (ICvar *)((g_SMAPI->GetEngineFactory())(VENGINE_CVAR_INTERFACE_VERSION, NULL));
#endif
#else
    return (ICvar *)((g_SMAPI->engineFactory())(VENGINE_CVAR_INTERFACE_VERSION, NULL));
#endif
}

ConVar cvar_cef_log("cef_log", "0", FCVAR_NONE, "Log edict indexes");

bool CEF::Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	PLUGIN_SAVEVARS();
	GET_V_IFACE_CURRENT(GetEngineFactory, engine, IVEngineServer, INTERFACEVERSION_VENGINESERVER);
	SH_ADD_HOOK(IVEngineServer, CreateEdict, engine, SH_STATIC(Hook_CreateEdict), false);
	gpGlobals = ismm->GetCGlobals();

	META_LOG(g_PLAPI, "Starting plugin.");

#if SOURCE_ENGINE==SE_ORANGEBOX || SOURCE_ENGINE==SE_LEFT4DEAD || SOURCE_ENGINE==SE_LEFT4DEAD2 || SOURCE_ENGINE==SE_TF2 || SOURCE_ENGINE==SE_DODS || SOURCE_ENGINE==SE_HL2DM || SOURCE_ENGINE==SE_NUCLEARDAWN || \
    SOURCE_ENGINE==SE_ALIENSWARM || SOURCE_ENGINE==SE_BLOODYGOODTIME || SOURCE_ENGINE==SE_CSGO || SOURCE_ENGINE==SE_CSS || SOURCE_ENGINE==SE_INSURGENCY || SOURCE_ENGINE==SE_SDK2013 || SOURCE_ENGINE== SE_BMS
    g_pCVar = GetICVar();
    ConVar_Register(0, this);
#else
    ConCommandBaseMgr::OneTimeInit(this);
#endif

	return true;
}

bool CEF::Unload(char *error, size_t maxlen)
{
	SH_REMOVE_HOOK(IVEngineServer, CreateEdict, engine, SH_STATIC(Hook_CreateEdict), false);
	return true;
}

edict_t * Hook_CreateEdict(int iIndex)
{
	if (iIndex > 0)
	{
		RETURN_META_VALUE(MRES_IGNORED, 0);
	}

	int i = 0;
	while (PEntityOfEntIndex(i) != NULL)
	{
		i++;
	}

	if (i >= 2048) /* Maybe we should do something about 2047? */
	{
		RETURN_META_VALUE(MRES_IGNORED, 0);
	}

	if (cvar_cef_log.GetBool())
	{
		g_SMAPI->LogMsg(g_PLAPI, "CEF: %d", i);
	}

	edict_t * pEdict = ENGINE_CALL(CreateEdict)(i);

	if (pEdict == NULL)
	{
		RETURN_META_VALUE(MRES_IGNORED, 0);
	}

	RETURN_META_VALUE(MRES_SUPERCEDE, pEdict);
}

bool CEF::RegisterConCommandBase(ConCommandBase *pVar)
{
    return META_REGCVAR(pVar);
}

bool CEF::Pause(char *error, size_t maxlen)
{
    return true;
}

bool CEF::Unpause(char *error, size_t maxlen)
{
    return true;
}

void CEF::AllPluginsLoaded()
{
}

const char *CEF::GetLicense()
{
	return "GPL";
}

const char *CEF::GetVersion()
{
	return "1.1.5";
}

const char *CEF::GetDate()
{
	return __DATE__;
}

const char *CEF::GetLogTag()
{
	return "CEF";
}

const char *CEF::GetAuthor()
{
	return "Kyle Sanderson";
}

const char *CEF::GetDescription()
{
	return "Fixes the currently messed up CreateEdict implementation.";
}

const char *CEF::GetName()
{
	return "Create Edict Fixer";
}

const char *CEF::GetURL()
{
	return "http://www.SourceMod.net/";
}