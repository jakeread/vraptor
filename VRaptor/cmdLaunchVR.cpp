////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN LaunchVR command
//

#include "stdafx.h"
#include "VRaptorPlugIn.h"

#pragma region LaunchVR command

class CCommandLaunchVR : public CRhinoCommand
{
public:
	CCommandLaunchVR() {}
	~CCommandLaunchVR() {}
	UUID CommandUUID()
	{
		// {23820956-87B8-4A90-B966-CD089FA759A0}
		static const GUID LaunchVRCommand_UUID =
		{ 0x23820956, 0x87B8, 0x4A90, { 0xB9, 0x66, 0xCD, 0x08, 0x9F, 0xA7, 0x59, 0xA0 } };
		return LaunchVRCommand_UUID;
	}
	const wchar_t* EnglishCommandName() { return L"LaunchVR"; }
	const wchar_t* LocalCommandName() const { return L"LaunchVR"; }
	CRhinoCommand::result RunCommand( const CRhinoCommandContext& );

	CRhinoEscapeKey escape;
	POINT cPt;
};

// The one and only CCommandLaunchVR object
static class CCommandLaunchVR theLaunchVRCommand;

CRhinoCommand::result CCommandLaunchVR::RunCommand( const CRhinoCommandContext& context )
{
	escape.Start();

	int i = 0;
	while(true)
	{
		if ( escape.EscapeKeyPressed() )
		{
			escape.Stop(); // stops the escape look loop
			RhinoApp().Print(L"BAIL VR\n");
			// should also throw an 'escaped' screen onto the hmd
			break;
		}

		VR().Inputs();
		VR().StdUpdate();
	}
	
	return CRhinoCommand::success;
}

#pragma endregion

//
// END LaunchVR command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
