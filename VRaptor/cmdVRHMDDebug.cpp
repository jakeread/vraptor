////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN VRHMDDebug command
//

#include "stdafx.h"
#include "VRaptorPlugIn.h"

#pragma region VRHMDDebug command

class CCommandVRHMDDebug : public CRhinoCommand
{
public:
	CCommandVRHMDDebug() {}
	~CCommandVRHMDDebug() {}
	UUID CommandUUID()
	{
		// {286CE14C-C3C5-4FD6-A15E-463C4515AA0D}
		static const GUID VRHMDDebugCommand_UUID =
		{ 0x286CE14C, 0xC3C5, 0x4FD6, { 0xA1, 0x5E, 0x46, 0x3C, 0x45, 0x15, 0xAA, 0x0D } };
		return VRHMDDebugCommand_UUID;
	}
	const wchar_t* EnglishCommandName() { return L"VRHMDDebug"; }
	const wchar_t* LocalCommandName() const { return L"VRHMDDebug"; }
	CRhinoCommand::result RunCommand( const CRhinoCommandContext& );
};

// The one and only CCommandVRHMDDebug object
static class CCommandVRHMDDebug theVRHMDDebugCommand;

CRhinoCommand::result CCommandVRHMDDebug::RunCommand( const CRhinoCommandContext& context )
{
	RhinoApp().Print(  L"HMD DEBUG: THROWING 100 UPDATES 2 U\n" );
	
	VR().idleCount = 0;

	
	if(!theIdleWatcher.IsRegistered())
	{
		theIdleWatcher.Register();
	}
	else
	{
		theIdleWatcher.Enable(theIdleWatcher.IsEnabled() ? false : true);
	}

	/*
	
	for(int i=0; i<100; i++) {

		//RhinoApp().OnIdle();

		//RhinoApp().PumpMessage(); // returns a bool; I think this is to check if messages are 'pumping' ?

		VR().HMDViewsUpdate();
		VR().tfAfterRedrawCall =  ovr_GetTimeInSeconds() - VR().tfBegin;
		RhinoApp().Wait(1); // wait pauses plugin but keeps 'windows message pump' alive. views redraw.
		VR().tfAfterRedrawWait =  ovr_GetTimeInSeconds() - VR().tfBegin;
		VR().HMDRender();
		VR().StoreTimingVars();
		//Sleep(500);
	}
	*/

	RhinoApp().Print(L"HMD DEBUG: REPORTING TIMING VARS\n");

	VR().PrintTimingVars();
	

	return CRhinoCommand::success;
}

#pragma endregion

//
// END VRHMDDebug command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
