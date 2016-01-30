////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN VRHmdInit command
//

#include "StdAfx.h"
#include "VRaptorPlugIn.h"

#include <OVR_CAPI_GL.h>

bool HMDInit()
{
	ON_wString if1;
	if1.Format( L"initFailed1\n" );
	ON_wString if2;
	if2.Format( L"initFailed2\n");
	ON_wString finInit;
	finInit.Format( L"HMDInit()fin\n" );


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
	RhinoApp().Print( L"resolution.h = %i \n", resolution.h); // is int
	RhinoApp().Print( L"resolution.w = %i \n", resolution.w);

	float refresh = desc.DisplayRefreshRate;
	RhinoApp().Print( L"DisplayRefreshRate = %f \n", refresh);

	unsigned int availableCaps = desc.AvailableTrackingCaps; // Capability bits described by ovrTrackingCaps which the HMD currently supports.
	unsigned int defaultCaps = desc.DefaultTrackingCaps; // Default capability bits described by ovrTrackingCaps for the current HMD.

	RhinoApp().Print( L"availableCaps, unsigned int = %i \n", availableCaps);
	RhinoApp().Print( L"defaultCaps, unsigned int = %i \n", defaultCaps);

	RhinoApp().Print( L"setting up Tracking \n");

	unsigned int requiredCaps = 35; // if hmd reports less than this tracking will fail. 

	// setup tracking for this session, ask for 'caps' and set lower 'caps' limit (capability, as per LibOVR)
	ovr_ConfigureTracking( session, defaultCaps, requiredCaps);

	// tiny loop to watch for updates;
	for (int i = 0; i<2000; i++)
	{
	// Query HMD for current tracking state
	ovrTrackingState ts = ovr_GetTrackingState(session, 0.0, false); 
			// 2nd arg, abstime, defines what absolute system time we want a reading for. 0.0 for most recent reading, where 'predicted pose' and 'sample pose' will be identical.
			// 3rd arg has to do with latency timing for debugging, when true a timer starts from here -> to measure 'app-to-mid-photon' time. 
	if (ts.StatusFlags & (ovrStatus_OrientationTracked | ovrStatus_PositionTracked ))
	{
		RhinoApp().Print( L"passed Tracking if %i \n", i);
		ovrPosef pose = ts.HeadPose.ThePose;
		//ON_3dPoint posePosition = ON_3dPoint(pose.Position.x,pose.Position.y,pose.Position.z);
		RhinoApp().Print( L"ovrPosef.Position = \t \t x %f, y %f, z %f \n", pose.Position.x, pose.Position.y, pose.Position.z);
		RhinoApp().Print( L"ovrPosef.Orientation = \t \t x %f, y %f, z %f, w %f  (quaternion I believe)\n", pose.Orientation.x, pose.Orientation.y, pose.Orientation.z, pose.Orientation.w);
		RhinoApp().Wait(20);
	}
	}

	// run tracking in loop... on call? what? why no updates? then bring into rhino geometry.


	ovr_Destroy(session);
	ovr_Shutdown();

	RhinoApp().Print(finInit);
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

	bool setupSuccess = HMDInit();
	if (setupSuccess)
	{
		ON_wString yas;
		yas.Format(L"HMDInitSuccess");
		RhinoApp().Print(yas);
		return CRhinoCommand::success;
	}
	else
	{
		ON_wString nah;
		nah.Format(L"HMDInitFailure");
		RhinoApp().Print(nah);
		return CRhinoCommand::failure;
	}
}

#pragma endregion

//
// END VRHmdInit command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
