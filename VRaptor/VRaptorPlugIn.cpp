/////////////////////////////////////////////////////////////////////////////
// VRaptorPlugIn.cpp : defines the initialization routines for the plug-in.
//

#include "StdAfx.h"
#include "VRaptorPlugIn.h"

// The plug-in object must be constructed before any plug-in classes
// derived from CRhinoCommand. The #pragma init_seg(lib) ensures that
// this happens.

#pragma warning( push )
#pragma warning( disable : 4073 )
#pragma init_seg( lib )
#pragma warning( pop )

// Rhino plug-in declaration
RHINO_PLUG_IN_DECLARE

// Rhino plug-in name
// Provide a short, friendly name for this plug-in.
RHINO_PLUG_IN_NAME( L"VRaptor" );

// Rhino plug-in id
// Provide a unique uuid for this plug-in
RHINO_PLUG_IN_ID( L"295D51BF-1B63-455A-9331-CD3E593494AB" );

// Rhino plug-in version
// Provide a version number string for this plug-in
RHINO_PLUG_IN_VERSION( __DATE__"  "__TIME__ )

// Rhino plug-in developer declarations
// TODO: fill in the following developer declarations with
// your company information. Note, all of these declarations
// must be present or your plug-in will not load.
//
// When completed, delete the following #error directive.
// #error Developer declarations block is incomplete!
RHINO_PLUG_IN_DEVELOPER_ORGANIZATION( L"ekswhyzee" );
RHINO_PLUG_IN_DEVELOPER_ADDRESS( L"Cambridge ON" );
RHINO_PLUG_IN_DEVELOPER_COUNTRY( L"Canada" );
RHINO_PLUG_IN_DEVELOPER_PHONE( L"226-338-6626" );
RHINO_PLUG_IN_DEVELOPER_FAX( L"NA" );
RHINO_PLUG_IN_DEVELOPER_EMAIL( L"jakerobertread@gmail.com" );
RHINO_PLUG_IN_DEVELOPER_WEBSITE( L"http://www.ekswhyzee.com" );
RHINO_PLUG_IN_UPDATE_URL( L"http://www.ekswhyzee.com" );

// The one and only CVRaptorPlugIn object
static CVRaptorPlugIn thePlugIn;

/////////////////////////////////////////////////////////////////////////////
// CVRaptorPlugIn definition

CVRaptorPlugIn& VRaptorPlugIn()
{ 
  // Return a reference to the one and only CVRaptorPlugIn object
  return thePlugIn; 
}


CVRaptorPlugIn::CVRaptorPlugIn()
{
  // Description:
  //   CVRaptorPlugIn constructor. The constructor is called when the
  //   plug-in is loaded and "thePlugIn" is constructed. Once the plug-in
  //   is loaded, CVRaptorPlugIn::OnLoadPlugIn() is called. The 
  //   constructor should be simple and solid. Do anything that might fail in
  //   CVRaptorPlugIn::OnLoadPlugIn().

  // TODO: Add construction code here
  m_plugin_version = RhinoPlugInVersion();
}

CVRaptorPlugIn::~CVRaptorPlugIn()
{
  // Description:
  //   CVRaptorPlugIn destructor. The destructor is called to destroy
  //   "thePlugIn" when the plug-in is unloaded. Immediately before the
  //   DLL is unloaded, CVRaptorPlugIn::OnUnloadPlugin() is called. Do
  //   not do too much here. Be sure to clean up any memory you have allocated
  //   with onmalloc(), onrealloc(), oncalloc(), or onstrdup().

  // TODO: Add destruction code here
}

/////////////////////////////////////////////////////////////////////////////
// US ///

// shorthand
CVRaptorPlugIn& VR()
{
	return thePlugIn;
}

bool CVRaptorPlugIn::HMDInit()
{
	ON_wString wStr;
	wStr.Format( L"Now to init hmd.\n" );
	RhinoApp().Print( wStr );
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

	result = ovr_Create(&hmdSession, &luid);
	if(OVR_FAILURE(result))
	{
		ovr_Shutdown();
		RhinoApp().Print(if2);
		return false;
	}

	desc = ovr_GetHmdDesc(hmdSession);
	resolution = desc.Resolution;
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
	ovr_ConfigureTracking( hmdSession, defaultCaps, requiredCaps);

	const  ovrVector3f hmdToEyeViewOffsetRaptor[] =
	{ // left and right, probably vectors pointing from headPose
	ovr_GetRenderDesc( hmdSession, ovrEye_Left, desc.DefaultEyeFov[0]).HmdToEyeViewOffset, 
	ovr_GetRenderDesc( hmdSession, ovrEye_Right, desc.DefaultEyeFov[1]).HmdToEyeViewOffset
	}; // the above should really call in the constructor. so should (probably) much else...

	return true;
}

void CVRaptorPlugIn::HMDPoseUpdate()
{
	// Query HMD for current tracking state
	ovrTrackingState ts = ovr_GetTrackingState(hmdSession, 0.0, false); 
			// 2nd arg, abstime, defines what absolute system time we want a reading for. 0.0 for most recent reading, where 'predicted pose' and 'sample pose' will be identical.
			// 3rd arg has to do with latency timing for debugging, when true a timer starts from here -> to measure 'app-to-mid-photon' time. 
	if (ts.StatusFlags & (ovrStatus_OrientationTracked | ovrStatus_PositionTracked ))
	{
		RhinoApp().Print( L"passed Tracking if %i \n");
		pose = ts.HeadPose.ThePose;
		RhinoApp().Print( L"ovrPosef.Position = \t \t x %f, y %f, z %f \n", pose.Position.x, pose.Position.y, pose.Position.z);
		RhinoApp().Print( L"ovrPosef.Orientation = \t \t x %f, y %f, z %f, w %f  (quaternion I believe)\n", pose.Orientation.x, pose.Orientation.y, pose.Orientation.z, pose.Orientation.w);
		// RhinoApp().Wait(20); // there's yer problem
		ovr_CalcEyePoses(ts.HeadPose.ThePose, hmdToEyeViewOffsetRaptor, outEyePosesRaptor);
		//ovr_GetEyePoses(hmdSession, 0, false, hmdToEyeViewOffsetRaptor, outEyePosesRaptor, &ts );
		RhinoApp().Print( L"VR().outEyePoses now updated (?) successfully (?) = \t \t eye1 %f \t \t eye2 %f", outEyePosesRaptor[0].Position.y, outEyePosesRaptor[1].Position.y);
	}
	else
	{
		RhinoApp().Print(L"Unable to poll Oculus Tracking: Please open Oculus Configuration Utility and try again\n");
	}
}

void CVRaptorPlugIn::HMDDestroy()
{
	ovr_Destroy(hmdSession);
	ovr_Shutdown();

	ON_wString wStr;
	wStr.Format( L"HMDDestroy.\n" );
	RhinoApp().Print( wStr );
}

void CVRaptorPlugIn::HMDViewUpdate()
{
	VR().HMDPoseUpdate();
	ON_3dPoint location = ON_3dPoint(pose.Position.x, pose.Position.y, pose.Position.z);
	lView->ActiveViewport().m_v.m_vp.SetCameraLocation(location);
	lView->Redraw();
	rView->ActiveViewport().m_v.m_vp.SetCameraLocation(location);
	rView->Redraw();
}


/////////////////////////////////////////////////////////////////////////////
// Required overrides

const wchar_t* CVRaptorPlugIn::PlugInName() const
{
  // Description:
  //   Plug-in name display string.  This name is displayed by Rhino when
  //   loading the plug-in, in the plug-in help menu, and in the Rhino 
  //   interface for managing plug-ins.

  // TODO: Return a short, friendly name for the plug-in.
  return RhinoPlugInName();
}

const wchar_t* CVRaptorPlugIn::PlugInVersion() const
{
  // Description:
  //   Plug-in version display string. This name is displayed by Rhino 
  //   when loading the plug-in and in the Rhino interface for managing
  //   plug-ins.

  // TODO: Return the version number of the plug-in.
  return m_plugin_version;
}

GUID CVRaptorPlugIn::PlugInID() const
{
  // Description:
  //   Plug-in unique identifier. The identifier is used by Rhino to
  //   manage the plug-ins.

  // TODO: Return a unique identifier for the plug-in.
  // {295D51BF-1B63-455A-9331-CD3E593494AB}
  return ON_UuidFromString( RhinoPlugInId() );
}

BOOL CVRaptorPlugIn::OnLoadPlugIn()
{
  // Description:
  //   Called after the plug-in is loaded and the constructor has been
  //   run. This is a good place to perform any significant initialization,
  //   license checking, and so on.  This function must return TRUE for
  //   the plug-in to continue to load.

  // TODO: Add plug-in initialization code here.
  return CRhinoUtilityPlugIn::OnLoadPlugIn();
}

void CVRaptorPlugIn::OnUnloadPlugIn()
{
  // Description:
  //   Called when the plug-in is about to be unloaded.  After
  //   this function is called, the destructor will be called.

  // TODO: Add plug-in cleanup code here.

  CRhinoUtilityPlugIn::OnUnloadPlugIn();
}

/////////////////////////////////////////////////////////////////////////////
// Online help overrides

BOOL CVRaptorPlugIn::AddToPlugInHelpMenu() const
{
  // Description:
  //   Return true to have your plug-in name added to the Rhino help menu.
  //   OnDisplayPlugInHelp will be called when to activate your plug-in help.

  return TRUE;
}

BOOL CVRaptorPlugIn::OnDisplayPlugInHelp( HWND hWnd ) const
{
  // Description:
  //   Called when the user requests help about your plug-in.
  //   It should display a standard Windows Help file (.hlp or .chm).

  // TODO: Add support for online help here.
  return CRhinoUtilityPlugIn::OnDisplayPlugInHelp( hWnd );
}

