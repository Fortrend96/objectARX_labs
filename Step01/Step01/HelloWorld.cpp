//#include "StdAfx.h"
#include "tchar.h"
#include "aced.h"
#include "rxregsvc.h"

void initApp();
void unloadApp();

void helloWorld();

void initApp()
{
	/*
	register a command with the AutoCAD command mechanism
	*/

	acedRegCmds->addCommand(L"HELLOWORLD_COMMANDS",
		L"Hello",
		L"Bonjour",
		ACRX_CMD_TRANSPARENT,
		helloWorld);
}

void unloadApp()
{
	acedRegCmds->removeGroup(L"HELLOWORLD_COMMANDS");
}

void helloWorld()
{
	acutPrintf(L"\nHello World!");
}

extern "C" AcRx::AppRetCode
acrxEntryPoint(AcRx::AppMsgCode msg, void* pkt)
{
	switch (msg)
	{
	case AcRx::kInitAppMsg:
		acrxDynamicLinker->unlockApplication(pkt);
		acrxRegisterAppMDIAware(pkt);
		initApp();
		break;
	case AcRx::kUnloadAppMsg:
		unloadApp();
		break;
	default:
		break;
	}

	return AcRx::kRetOK;
}


