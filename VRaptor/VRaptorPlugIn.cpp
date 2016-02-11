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

///////////
// UTILITY

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

	hmdToEyeViewOffsetRaptor[0] = ovr_GetRenderDesc( hmdSession, ovrEye_Left, desc.DefaultEyeFov[0]).HmdToEyeViewOffset;
	hmdToEyeViewOffsetRaptor[1] = ovr_GetRenderDesc( hmdSession, ovrEye_Right, desc.DefaultEyeFov[1]).HmdToEyeViewOffset;
	 // the above should really call in the constructor. so should (probably) much else...

	RhinoApp().Print( L"desc.DefaultEyeFov[0].UpTan %f \t desc.DefaulEyeFov[1].UpTan %f \n", desc.DefaultEyeFov[0].UpTan, desc.DefaultEyeFov[1].UpTan);
	RhinoApp().Print( L"hmdToEyeViewoffsetRaptor[0] %f %f %f\n", hmdToEyeViewOffsetRaptor[0].x, hmdToEyeViewOffsetRaptor[0].y, hmdToEyeViewOffsetRaptor[0].z);
	RhinoApp().Print( L"hmdToEyeViewoffsetRaptor[1] %f %f %f\n", hmdToEyeViewOffsetRaptor[1].x, hmdToEyeViewOffsetRaptor[1].y, hmdToEyeViewOffsetRaptor[1].z);

	scaleMult = 100; // scaling all HMD Positions

	dirBase = ON_3dVector(0.0, 1.0, 0.0);
	upBase = ON_3dVector(0.0, 0.0, 1.0);

	return true;
}

bool CVRaptorPlugIn::HMDRenderInit() // setup for render: making textures, buffers etc..
{
	// build texture sizes according to HMD Description
	OVR::Sizei recommendedTex0Size = ovr_GetFovTextureSize(hmdSession, ovrEye_Left, desc.DefaultEyeFov[0], 1.0f);
	OVR::Sizei recommendedTex1Size = ovr_GetFovTextureSize(hmdSession, ovrEye_Left, desc.DefaultEyeFov[1], 1.0f);

	OVR::Sizei bufferSize;

	bufferSize.w = recommendedTex0Size.w + recommendedTex1Size.w;
	bufferSize.h = max(recommendedTex0Size.h, recommendedTex1Size.h);


	ovrGLTexture * mirrorTexture = nullptr;

	ovrSwapTextureSet * pTextureSet = 0;
	//ovr_CreateSwapTextureSetGL(hmdSession, GL_SRGB8_ALPHA8, bufferSize.w, bufferSize.h, &pTextureSet); // causes break. I think bc we do not have GL dependency
	ovr_CreateMirrorTextureGL(hmdSession, 0x8C43, bufferSize.w, bufferSize.h, reinterpret_cast<ovrTexture**>(&mirrorTexture)); 
	// access violation at 00000000000000000000000 is trying to read a nullpointer. setup is bunz. read note in stdafx.h

	return true;
}


void CVRaptorPlugIn::HMDRender() //copies current lView and rView buffers to ovr TextureSet and submits frame
{
	// PLACEHOLDER. NO IDEA WHAT IS GOING TO GO HERE
	VR().lView->DisplayPipeline()->GetFrameBuffer();
	VR().rView->DisplayPipeline()->GetFrameBuffer();

	ovrResult       result = ovr_SubmitFrame(hmdSession, 0, nullptr, 0, 0);
}

void CVRaptorPlugIn::HMDPrintUpdate()
{
	// code here to print all relevant data from class, for debugging
}

void CVRaptorPlugIn::HMDDestroy()
{
	ovr_Destroy(hmdSession);
	ovr_Shutdown();

	ON_wString wStr;
	wStr.Format( L"HMDDestroy.\n" );
	RhinoApp().Print( wStr );
}

///////////
// Runtime

ovrTrackingState CVRaptorPlugIn::OVRDoTracking()	// in the future we should set this up to take an absTime function for 
										// ovr_GetTrackingState to pull
{
		// Query HMD for current tracking state
	ts = ovr_GetTrackingState(hmdSession, 0.0, false); 
			// 2nd arg, abstime, defines what absolute system time we want a reading for. 0.0 for most recent reading, where 'predicted pose' and 'sample pose' will be identical.
			// 3rd arg has to do with latency timing for debugging, when true a timer starts from here -> to measure 'app-to-mid-photon' time. 
	if (ts.StatusFlags & (ovrStatus_OrientationTracked | ovrStatus_PositionTracked ))
	{
		// straight to eye poses, big TY to ovr for this offsets maths.
		ovr_CalcEyePoses(ts.HeadPose.ThePose, hmdToEyeViewOffsetRaptor, tsEyePoses); // yall got 0's in your offset vector

		// setup all relevant values to do next moves
		// already have all things in ts and tsEyePoses
		eyePoseQuats[0] = tsEyePoses[0].Orientation; // quaternion class for rotation angle, used later
		eyePoseQuats[1] = tsEyePoses[1].Orientation;

	}
	else
	{
		RhinoApp().Print(L"Unable to poll Oculus Tracking: Please open Oculus Configuration Utility and try again\n");
	}

	return ts;
}

void CVRaptorPlugIn::OVRtoRHCams(ovrPosef pose[2]) // there pose[2] is tsEyePoses 
{
	for (int i = 0; i< 2; i++)
	{
		// ok, take pose and decomp into location
		// camLocation[]
		camLoc[i] = scaleMult * ON_3dPoint(pose[i].Position.x, pose[i].Position.y, pose[i].Position.z);

		rotationVector = eyePoseQuats[i].ToRotationVector();

		//RhinoApp().Print( L"OVRtoRHCams rotationVector = \t %f \t %f \t %f \n", rotationVector.x, rotationVector.y, rotationVector.x);

		dirBase = ON_3dVector(0.0, 1.0, 0.0);
		upBase = ON_3dVector(0.0, 0.0, 1.0);

		dirBase.Rotate( rotationVector.Length() , ON_3dVector( rotationVector.x, rotationVector.y, rotationVector.z) );
		upBase.Rotate( rotationVector.Length() , ON_3dVector( rotationVector.x, rotationVector.y, rotationVector.z) );

		camDir[i] = dirBase;
		camUp[i] = upBase; // there's a chance dir and up can be called only once; given parallel eyes

		//RhinoApp().Print( L"camDir[i] = \t\t\t %f \t %f \t %f \n", camDir[i].x, camDir[i].y, camDir[i].z );
		//RhinoApp().Print( L"camup[i] = \t\t\t %f \t %f \t %f \n", camUp[i].x, camUp[i].y, camUp[i].z );

		// then setcams (2nd for, so simulteneous)
		// then redraw
	}

	lView->ActiveViewport().m_v.m_vp.SetCameraLocation(camLoc[0]);
	lView->ActiveViewport().m_v.m_vp.SetCameraDirection(camDir[0]);
	lView->ActiveViewport().m_v.m_vp.SetCameraUp(camUp[0]);

	rView->ActiveViewport().m_v.m_vp.SetCameraLocation(camLoc[1]);
	rView->ActiveViewport().m_v.m_vp.SetCameraDirection(camDir[1]);
	rView->ActiveViewport().m_v.m_vp.SetCameraUp(camUp[1]);
	
	lView->Redraw(); // so we will be re-calling immediately -> maybe no bueno
	rView->Redraw();
}

void CVRaptorPlugIn::HMDViewsUpdate()
{
	OVRDoTracking(); // to update all vals
	OVRtoRHCams(tsEyePoses);
	//RhinoApp().Wait(16); // so approximately 60fps; that's not a lot of milliseconds! 
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

