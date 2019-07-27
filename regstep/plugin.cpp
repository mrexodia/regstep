#include "plugin.h"

enum
{
	MENU_ENABLED,
};

static bool regstepEnabled = true;

PLUG_EXPORT void CBMENUENTRY(CBTYPE, PLUG_CB_MENUENTRY* info)
{
	switch(info->hEntry)
	{
	case MENU_ENABLED:
	{
		regstepEnabled = !regstepEnabled;
		BridgeSettingSetUint("regstep", "Enabled", regstepEnabled);
	}
	break;
	}
}

PLUG_EXPORT void CBSTEPPED(CBTYPE, PLUG_CB_STEPPED* info)
{
	if(!regstepEnabled)
		return;

	REGDUMP regdump;
	DbgGetRegDump(&regdump);

	auto& r = regdump.regcontext;
#ifdef _WIN64
	_plugin_logprintf(R"(rax=%p rbx=%p rcx=%p
rdx=%p rsi=%p rdi=%p
rip=%p rsp=%p rbp=%p
 r8=%p  r9=%p r10=%p
r11=%p r12=%p r13=%p
r14=%p r15=%p
)",
	r.cax, r.cbx, r.ccx,
	r.cdx, r.csi, r.cdi,
	r.cip, r.csp, r.cbp,
	r.r8, r.r9, r.r10,
	r.r11, r.r12, r.r13,
	r.r14, r.r15);
#else
	_plugin_logprintf(R"(eax=%p ebx=%p ecx=%p
edx=%p esi=%p edi=%p
eip=%p esp=%p ebp=%p
)",
r.cax, r.cbx, r.ccx,
r.cdx, r.csi, r.cdi,
r.cip, r.csp, r.cbp);
#endif //_WIN64
}

//Initialize your plugin data here.
bool pluginInit(PLUG_INITSTRUCT* initStruct)
{
	duint setting = regstepEnabled;
	BridgeSettingGetUint("regstep", "Enabled", &setting);
	regstepEnabled = !!setting;
    return true; //Return false to cancel loading the plugin.
}

//Deinitialize your plugin data here.
void pluginStop()
{
}

//Do GUI/Menu related things here.
void pluginSetup()
{
	_plugin_menuaddentry(hMenu, MENU_ENABLED, "Enabled");
	_plugin_menuentrysetchecked(pluginHandle, MENU_ENABLED, regstepEnabled);
}
