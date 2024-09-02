#include "StdAfx.h"
#include "resource.h"

#define szRDS _RXST("")

class CCustomLabDbxApp : public AcRxDbxApp {

public:
	CCustomLabDbxApp () : AcRxDbxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) 
	{
		AcRx::AppRetCode retCode =AcRxDbxApp::On_kInitAppMsg (pkt) ;
	
		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) 
	{
		AcRx::AppRetCode retCode =AcRxDbxApp::On_kUnloadAppMsg(pkt) ;
		return retCode;
	}

	virtual void RegisterServerComponents () {}	
};
//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CCustomLabDbxApp)

