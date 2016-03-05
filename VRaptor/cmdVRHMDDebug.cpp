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
	ON_wString wStr;
	wStr.Format( L"HMD DEBUG: THROWING 2000 UPDATES 2 U\n");
	RhinoApp().Print( wStr );

	if (false) { //VR().renderTrack == 0
		VR().InitHMD();
	}
	
	for(int i=0; i<2000; i++) {
		VR().HMDViewsUpdate();
	}

	//VR().HMDDisplayAnything();


	return CRhinoCommand::success;
}

#pragma endregion

//
// END VRHMDDebug command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
