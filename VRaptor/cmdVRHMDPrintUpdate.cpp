////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN VRHMDPrintUpdate command
//

#include "stdafx.h"
#include "VRaptorPlugIn.h"

#pragma region VRHMDPrintUpdate command

class CCommandVRHMDPrintUpdate : public CRhinoCommand
{
public:
	CCommandVRHMDPrintUpdate() {}
	~CCommandVRHMDPrintUpdate() {}
	UUID CommandUUID()
	{
		// {BFAF49DA-E464-453B-BBFE-B6D1FAF0D04E}
		static const GUID VRHMDPrintUpdateCommand_UUID =
		{ 0xBFAF49DA, 0xE464, 0x453B, { 0xBB, 0xFE, 0xB6, 0xD1, 0xFA, 0xF0, 0xD0, 0x4E } };
		return VRHMDPrintUpdateCommand_UUID;
	}
	const wchar_t* EnglishCommandName() { return L"VRHMDPrintUpdate"; }
	const wchar_t* LocalCommandName() const { return L"VRHMDPrintUpdate"; }
	CRhinoCommand::result RunCommand( const CRhinoCommandContext& );
};

// The one and only CCommandVRHMDPrintUpdate object
static class CCommandVRHMDPrintUpdate theVRHMDPrintUpdateCommand;

CRhinoCommand::result CCommandVRHMDPrintUpdate::RunCommand( const CRhinoCommandContext& context )
{
	ON_wString wStr;
	wStr.Format( L"VRHMD Print Update\n");
	RhinoApp().Print( wStr );

	for (int i = 0; i<200; i++)
	{
		VR().HMDPrintUpdate();
		RhinoApp().Wait(10);
	}

	RhinoApp().Print( wStr );
	return CRhinoCommand::success;
}

#pragma endregion

//
// END VRHMDPrintUpdate command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
