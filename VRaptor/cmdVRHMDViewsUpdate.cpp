////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN VRHMDViewsUpdate command
//

#include "stdafx.h"
#include "VRaptorPlugIn.h"

#pragma region VRHMDViewsUpdate command

class CCommandVRHMDViewsUpdate : public CRhinoCommand
{
public:
	CCommandVRHMDViewsUpdate() {}
	~CCommandVRHMDViewsUpdate() {}
	UUID CommandUUID()
	{
		// {4AAB5D96-3F4A-4721-95C7-DE197EF35862}
		static const GUID VRHMDViewsUpdateCommand_UUID =
		{ 0x4AAB5D96, 0x3F4A, 0x4721, { 0x95, 0xC7, 0xDE, 0x19, 0x7E, 0xF3, 0x58, 0x62 } };
		return VRHMDViewsUpdateCommand_UUID;
	}
	const wchar_t* EnglishCommandName() { return L"VRHMDViewsUpdate"; }
	const wchar_t* LocalCommandName() const { return L"VRHMDViewsUpdate"; }
	CRhinoCommand::result RunCommand( const CRhinoCommandContext& );
};

// The one and only CCommandVRHMDViewsUpdate object
static class CCommandVRHMDViewsUpdate theVRHMDViewsUpdateCommand;

CRhinoCommand::result CCommandVRHMDViewsUpdate::RunCommand( const CRhinoCommandContext& context )
{
	ON_wString wStr;
	wStr.Format( L"VRHMD Views Update\n", EnglishCommandName() );
	RhinoApp().Print( wStr );

	for (int i = 0; i<200; i++)
	{
		VR().HMDViewsUpdate();
	}

	RhinoApp().Print( wStr );
	return CRhinoCommand::success;
}

#pragma endregion

//
// END VRHMDViewsUpdate command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
