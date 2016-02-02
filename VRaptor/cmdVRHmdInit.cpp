////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN VRHmdInit command
//

#include "StdAfx.h"
#include "VRaptorPlugIn.h"
// OVR Include is in StdAfx.h


// do HMDInit in it's own class; we want global access to it. include session, tracking session, updates etc...


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
	
	bool setupSuccess = VR().HMDInit();
	for (int i=0; i<100; i++){
		VR().HMDPoseUpdate();
	}
	VR().HMDDestroy();
	if(setupSuccess)
	{
		ON_wString prnt1;
		prnt1.Format("Successful end VRHmdInit\n");
		RhinoApp().Print(prnt1);
		return CRhinoCommand::success;
	}
	else
	{
		ON_wString prnt1;
		prnt1.Format("Unsuccessful fin to VRHmdInit\n");
		RhinoApp().Print(prnt1);
		return CRhinoCommand::failure;
	}


}

#pragma endregion

//
// END VRHmdInit command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
