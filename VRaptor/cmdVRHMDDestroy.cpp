////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN VRHMDDestroy command
//

#include "stdafx.h"
#include "VRaptorPlugIn.h"

#pragma region VRHMDDestroy command

class CCommandVRHMDDestroy : public CRhinoCommand
{
public:
	CCommandVRHMDDestroy() {}
	~CCommandVRHMDDestroy() {}
	UUID CommandUUID()
	{
		// {C4F4A46C-218C-4C97-9DE0-D41D43C43E06}
		static const GUID VRHMDDestroyCommand_UUID =
		{ 0xC4F4A46C, 0x218C, 0x4C97, { 0x9D, 0xE0, 0xD4, 0x1D, 0x43, 0xC4, 0x3E, 0x06 } };
		return VRHMDDestroyCommand_UUID;
	}
	const wchar_t* EnglishCommandName() { return L"VRHMDDestroy"; }
	const wchar_t* LocalCommandName() const { return L"VRHMDDestroy"; }
	CRhinoCommand::result RunCommand( const CRhinoCommandContext& );
};

// The one and only CCommandVRHMDDestroy object
static class CCommandVRHMDDestroy theVRHMDDestroyCommand;

CRhinoCommand::result CCommandVRHMDDestroy::RunCommand( const CRhinoCommandContext& context )
{
	VR().HMDDestroy();

	ON_wString wStr;
	wStr.Format( L"Destroying OVR Instance" );
	RhinoApp().Print( wStr );
	return CRhinoCommand::success;
}

#pragma endregion

//
// END VRHMDDestroy command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
