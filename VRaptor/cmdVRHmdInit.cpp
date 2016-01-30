////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN VRHmdInit command
//

#include "StdAfx.h"
#include "VRaptorPlugIn.h"


bool HMDInit()
{
	ON_wString if1;
	if1.Format( L"initFailed1\n" );
	ON_wString if2;
	if2.Format( L"initFailed2\n");
	ON_wString outtahere;
	outtahere.Format( L"outPCE\n" );


	ovrResult result = ovr_Initialize(nullptr);
	if(OVR_FAILURE(result))
	{
		RhinoApp().Print(if1);
		return false;
	}

	ovrSession session;
	ovrGraphicsLuid luid;
	result = ovr_Create(&session, &luid);
	if(OVR_FAILURE(result))
	{
		ovr_Shutdown();
		RhinoApp().Print(if2);
		return false;
	}

	ovrHmdDesc desc = ovr_GetHmdDesc(session);
	ovrSizei resolution = desc.Resolution;

	ovr_Destroy(session);
	ovr_Shutdown();

	RhinoApp().Print(outtahere);
	return true;
}

#pragma region VRHmdInit command

class CCommandVRHmdInit : public CRhinoCommand
{
public:
	CCommandVRHmdInit() {}
	~CCommandVRHmdInit() {}
	UUID CommandUUID()
	{
		// {CD2317C9-7C38-4AD4-802F-6409B7D20303}
		static const GUID VRHmdInitCommand_UUID =
		{ 0xCD2317C9, 0x7C38, 0x4AD4, { 0x80, 0x2F, 0x64, 0x09, 0xB7, 0xD2, 0x03, 0x03 } };
		return VRHmdInitCommand_UUID;
	}
	const wchar_t* EnglishCommandName() { return L"VRHmdInit"; }
	const wchar_t* LocalCommandName() const { return L"VRHmdInit"; }
	CRhinoCommand::result RunCommand( const CRhinoCommandContext& );
};

// The one and only CCommandVRHmdInit object
static class CCommandVRHmdInit theVRHmdInitCommand;

CRhinoCommand::result CCommandVRHmdInit::RunCommand( const CRhinoCommandContext& context )
{
	ON_wString wStr;
	wStr.Format( L"The \"%s\" now to init hmd.\n", EnglishCommandName() );
	RhinoApp().Print( wStr );
	return CRhinoCommand::success;
}

#pragma endregion

//
// END VRHmdInit command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
