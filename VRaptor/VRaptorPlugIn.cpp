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

  // setup dumb vars
  VR().currentDibFile = L"D:/ppfbDib.bmp"; // setup for debug
  VR().disableConduits = false; // tag which we can set to destroy conduits...

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

// Just a shorthand
CVRaptorPlugIn& VR()
{
	return thePlugIn;
}

///////////
// INITS

void CVRaptorPlugIn::InitView(CRhinoView *newView, int num)
{
	// more properly we need a popup for our interface window & then to dumb render into
	// https://msdn.microsoft.com/en-us/library/windows/desktop/ms633545%28v=vs.85%29.aspx

	CFrameWnd *rFrameWnd;
	rFrameWnd = newView->FloatRhinoView(true); // returns pointer to the view's parent container
	HWND rHWnd = rFrameWnd->GetSafeHwnd();

	const CWnd *cwndptr = &CWnd::wndNoTopMost;
			
	bool setWndThru = rFrameWnd->SetWindowPos(cwndptr, 960/(2-num), 960/2, idealTextureSize.w, idealTextureSize.h, SWP_NOACTIVATE);

	if (setWndThru == 0)
	{
		RhinoApp().Print(L"windowSet failed at rView\n");
	} 

	// set names
	if(num == 0)
		newView->ActiveViewport().SetName(L"lView");
	if(num == 1)
		newView->ActiveViewport().SetName(L"rView");
	
	newView->ActiveViewport().m_v.m_vp.ChangeToPerspectiveProjection(50,true,24);
	//newView->Redraw();
}

void CVRaptorPlugIn::InitOvrWinWomb()
{
		// BEGIN what was WINAPI WinMain
	OVR::System::Init(); // we're probably going to have to de-construct this page & rebuild in the plugin. best of luck.

    // Initializes LibOVR, and the Rift
    ovrResult result001 = ovr_Initialize(nullptr);
	if (!OVR_SUCCESS(result001))
		RhinoApp().Print(L"Failed at\t ovr_Initialize()\n");

	HINSTANCE hinst = GetModuleHandle(nullptr); // thanks b to stackoverflow. gets current HINSTANCE of whatever exe is running...
	// this one from OGL. need to open window, need HINSTANCE from WinMain...
	VALIDATE(Platform.InitWindow(hinst, L"VRAPTOR"), "Failed to open window.");
}

void CVRaptorPlugIn::InitHMD()
{
	VR().InitOvrWinWomb();
	using namespace OVR;

	ovrResult result002 = ovr_Create(&VR().HMD, &luid);
    if (!OVR_SUCCESS(result002))
		RhinoApp().Print(L"Failed at\t ovr_Create\n");

	VR().hmdDesc = ovr_GetHmdDesc(VR().HMD);

	for(int i = 0; i<2; i++)
	{
		VR().eyeRenderTexture[i] = nullptr;
	}
	VR().mirrorTexture = nullptr;
	VR().mirrorFBO = 0;

	// make sure we have an HMD
	if(VR().hmdDesc.Type==ovrHmd_None)
		RhinoApp().Print(L"OVR::hmdDesc.Type = ovrHmd_None\n");

	// inits for mirror framebuffer
	ovrSizei windowSize = { VR().hmdDesc.Resolution.w / 2, VR().hmdDesc.Resolution.h / 2 }; 

	// init platform device
    if (!Platform.InitDevice(windowSize.w, windowSize.h, reinterpret_cast<LUID*>(&luid))) 
		RhinoApp().Print(L"Failed at\t Platform.InitDevice\n");

	// Make eye render buffers
    for (int eye = 0; eye < 2; ++eye)
    {  // try next to do idealTextureSize before window creation. have to call hmdInit before, then
		idealTextureSize = ovr_GetFovTextureSize(VR().HMD, ovrEyeType(eye), VR().hmdDesc.DefaultEyeFov[eye], 0.35);
        VR().eyeRenderTexture[eye] = new TextureBuffer(VR().HMD, true, true, idealTextureSize, 1, NULL, 1);
        if (!VR().eyeRenderTexture[eye]->TextureSet)
        {
            RhinoApp().Print(L"Failed at\t createEyRenderTexture TextureSet\n");
        }
    }

	// Create mirror texture and an FBO used to copy mirror texture to back buffer
    ovrResult resultOVRCREATE = ovr_CreateMirrorTextureGL(VR().HMD, GL_SRGB8_ALPHA8, windowSize.w, windowSize.h, 
		reinterpret_cast<ovrTexture**>(&VR().mirrorTexture));
    if (!OVR_SUCCESS(resultOVRCREATE))
    {
        RhinoApp().Print(L"Failed at\t ovr_CrseateMirrorTextureGL()\n");
    }

	// Configure the MIRROR read buffer
    glGenFramebuffers(1, &mirrorFBO);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, mirrorFBO);
    glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mirrorTexture->OGL.TexId, 0);
    glFramebufferRenderbuffer(GL_READ_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

    eyeRenderDescs[0] = ovr_GetRenderDesc(HMD, ovrEye_Left, hmdDesc.DefaultEyeFov[0]);
    eyeRenderDescs[1] = ovr_GetRenderDesc(HMD, ovrEye_Right, hmdDesc.DefaultEyeFov[1]);

	// Get eye poses, feeding in correct IPD offset
    viewOffsets[0] = eyeRenderDescs[0].HmdToEyeViewOffset;
	viewOffsets[1] = eyeRenderDescs[1].HmdToEyeViewOffset;

    // Turn off vsync to let the compositor do its magic
    wglSwapIntervalEXT(0);

	for (int rl = 0; rl < 2; rl++)
	{
		glGenTextures(1, &VR().rhinoTexSet[rl]); 

		glBindTexture(GL_TEXTURE_2D, VR().rhinoTexSet[rl]); // 

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	
	// also make swap framebuffers
	glGenFramebuffers(1, &VR().readBuffer);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, VR().readBuffer); 
	glGenFramebuffers(1, &VR().drawBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, VR().drawBuffer);

	renderTrack = 0;
	vrFrameIndex = 0;

	// gotta do this wglMakeCurrent(hDC, hGLRC);
	// IMPROVE: this can probably call / update without constructing...
	ovrHGLRC = Platform.WglContext; // now how do we get these really?
	ovrHWND = Platform.Window;
	ovrHDC = GetDC(ovrHWND);
}

void CVRaptorPlugIn::InitRHVars() // runs at the end of InitVR command.
{
	VR().currentScale = 50;

	////// SET DIB SETTINGS, BYTES
	for (int rl = 0; rl<2; rl++)
	{
		VR().rhDibW[rl] = VR().currentDib[rl].Width(); // rhfb rhinoframebuffer as in above
		VR().rhDibH[rl] = VR().currentDib[rl].Height(); // OK just need to get the DIB to actually contain a viewport...
	}

}

///////////
// RUNTIME

bool CVRaptorPlugIn::HMDRender() //copies current lView and rView buffers to ovr TextureSet and submits frame
{
	// Swap to OVR GL Context

	tfAtRenderTop =  ovr_GetTimeInSeconds() - tfBegin;

	if(!wglMakeCurrent(ovrHDC, ovrHGLRC))
	{
		RhinoApp().Print(L"failed to make OVR OGL Context Current");
	}

	wglSwapIntervalEXT(0);

	renderTrack++;
	if (renderTrack % 100 == 0)
		RhinoApp().Print(L"renderTrack() %i\n", renderTrack);

	using namespace OVR;

	tfBeforeBytesSwap = ovr_GetTimeInSeconds() - tfBegin;

	theBytes[0] =  VR().currentDib[0].FindDIBBits(); // y'all is constant. DIB maybe isn't writing into 32 bit dib.
	theBytes[1] =  VR().currentDib[1].FindDIBBits(); 

	/*
	LPCTSTR theDibFile = L"D:/currentDibAtHMDRender.bmp";  // it's a dib alright
	VR().currentDib[0].SaveBmp(theDibFile);
	*/

	if (theBytes[0] == nullptr || theBytes[1] == nullptr)
	{
		bailOnDibInvalid();
		return false;
	}

	tfBeforeTextureSetup =  ovr_GetTimeInSeconds() - tfBegin;

	//////// SET TEXTURES UP
	for(int rl = 0; rl<2; rl++)
	{
		glBindTexture(GL_TEXTURE_2D, VR().rhinoTexSet[rl]); // 
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, VR().rhDibW[rl], VR().rhDibH[rl], 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, theBytes[rl]);
		// breaks here when gl is unhappy with incoming bits
		// sometimes this only happens for 1 or two frames: we should update to check for nullness...
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &theTexW[rl]);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &theTexH[rl]);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	tfBeforeTextureBlit =  ovr_GetTimeInSeconds() - tfBegin;

	///////// OVR RENDERING
	for (int eye = 0; eye < 2; ++eye) // flip eye name to rl
	{
		VR().eyeRenderTexture[eye]->TextureSet->CurrentIndex = (eyeRenderTexture[eye]->TextureSet->CurrentIndex + 1) % eyeRenderTexture[eye]->TextureSet->TextureCount;

		// pull current Texture from OVR set
		tex = (ovrGLTexture*)&eyeRenderTexture[eye]->TextureSet->Textures[eyeRenderTexture[eye]->TextureSet->CurrentIndex];
		glBindTexture(GL_TEXTURE_2D, tex->OGL.TexId);	// bind OVR tex

		// get OVR tex width / height / format
		GLint ovrTexW, ovrTexH, ovrTexFormat;
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &ovrTexW); 
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &ovrTexH);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &ovrTexFormat); // shant not b emtpy. check.

		// Set one Framebuffer as read, the other as write. can these be any framebuffers? 
		glBindFramebuffer(GL_READ_FRAMEBUFFER, readBuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, drawBuffer);

		// tex->OGL.TexID is the Oculus texture from the texture set.
		glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, VR().rhinoTexSet[eye], 0); // stays down here
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex->OGL.TexId, 0);
		// Do the copy.
		glBlitFramebuffer(0, 0, theTexW[eye], theTexH[eye], 0, 0, ovrTexW, ovrTexH, GL_COLOR_BUFFER_BIT, GL_NEAREST); // BLIT IT HOME MORTY
	}

	tfAfterTextureWork =  ovr_GetTimeInSeconds() - tfBegin;

	// Do distortion rendering, Present and flush/sync

	if (renderTrack < 6) 
		VR().OVRDoTracking();	// updates eye pose, etc -> should only call without using tracking data on init
								// so that the first frame actually shows up

	// Set up positional data.
	viewScaleDesc.HmdSpaceToWorldScaleInMeters = 1.0f;
	viewScaleDesc.HmdToEyeViewOffset[0] = viewOffsets[0];
	viewScaleDesc.HmdToEyeViewOffset[1] = viewOffsets[1];

	ld.Header.Type  = ovrLayerType_EyeFov;
	ld.Header.Flags = ovrLayerFlag_TextureOriginAtBottomLeft;   // Because OpenGL.

	for (int eye = 0; eye < 2; ++eye)
	{
		// do textureSet debug here ?
		ld.ColorTexture[eye] = eyeRenderTexture[eye]->TextureSet; // DO IT LIKE THIS MORTY
		ld.Viewport[eye]     = Recti(eyeRenderTexture[eye]->GetSize());
		ld.Fov[eye]          = hmdDesc.DefaultEyeFov[eye];
		ld.RenderPose[eye]   = tsEyePoses[eye];					// updated on tracking
		ld.SensorSampleTime  = sensorSampleTime;				// updated on tracking
	}

	layers = &ld.Header;

	tfBeforeSubmit =  ovr_GetTimeInSeconds() - tfBegin;
	tfFrameIndexSubmit = vrFrameIndex;
	resultSubmit = ovr_SubmitFrame(VR().HMD, vrFrameIndex, &viewScaleDesc, &layers, 1);

	// exit the rendering loop if submit returns an error, will retry on ovrError_DisplayLost
	if (!OVR_SUCCESS(resultSubmit))
		RhinoApp().Print(L"Failure on ovr_SubmitFrame\n");

	// Blit mirror texture to back buffer
	glBindFramebuffer(GL_READ_FRAMEBUFFER, mirrorFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	GLint w = mirrorTexture->OGL.Header.TextureSize.w;
	GLint h = mirrorTexture->OGL.Header.TextureSize.h;
	glBlitFramebuffer(0, h, w, 0,
		0, 0, w, h,
		GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

	SwapBuffers(Platform.hDC);

	vrFrameIndex ++;
	tfEndRender =  ovr_GetTimeInSeconds() - tfBegin;
}

void CVRaptorPlugIn::HMDViewsUpdate()
{
	tfBegin = ovr_GetTimeInSeconds();
	OVRDoTracking(); // to update all vals
	tfAtDoTracking = ovr_GetTimeInSeconds() - tfBegin;
	RHCamsUpdate(); // to set cams & redraw, calling OVR in conduit.
	tfAtCamsUpdate = ovr_GetTimeInSeconds() - tfBegin;
}

void CVRaptorPlugIn::OVRDoTracking()	// needs to update tsEyePoses
										// also this should take & hit a pointer, not a var...
										// ovr_GetTrackingState to pull
{
	tfFrameIndexTrack = vrFrameIndex;
	predictedDisplay = ovr_GetPredictedDisplayTime(VR().HMD, vrFrameIndex);
	tfPredictedDisplay = predictedDisplay - tfBegin;
	// need to add ovr_GetPredictedDisplayTime 
	// Query HMD for current tracking state
	ts = ovr_GetTrackingState(VR().HMD, predictedDisplay, true); 
	sensorSampleTime = ovr_GetTimeInSeconds();
	tfAtSensorSample = sensorSampleTime - tfBegin; 
			// 2nd arg, abstime, defines what absolute system time we want a reading for. 0.0 for most recent reading, where 'predicted pose' and 'sample pose' will be identical.
			// 3rd arg has to do with latency timing for debugging, when true a timer starts from here -> to measure 'app-to-mid-photon' time. 
	if (ts.StatusFlags & (ovrStatus_OrientationTracked | ovrStatus_PositionTracked ))
	{
		// straight to eye poses, big TY to ovr for the offsets maths
		ovr_CalcEyePoses(ts.HeadPose.ThePose, viewOffsets, tsEyePoses);

		// setup all relevant values to do next moves
		// already have all things in ts and tsEyePoses
		eyePoseQuats[0] = VR().tsEyePoses[0].Orientation; // quaternion class for rotation angle, used later
		eyePoseQuats[1] = VR().tsEyePoses[1].Orientation;
	}
	else
	{
		RhinoApp().Print(L"Unable to poll Oculus Tracking: Please open Oculus Configuration Utility and try again\n");
	}

	////// & convert to Rhino Cam Data, for reset later...
	for (int rl = 0; rl < 2; rl++)
	{

		/* SET LOCATIONS
		RHINO		OVR
		X		=	X
		Y		=	-Z
		Z		=	Y
		*/
		rotationVector = eyePoseQuats[rl].ToRotationVector();

		dirBase = ON_3dVector(0.0, 1.0, 0.0);
		upBase = ON_3dVector(0.0, 0.0, 1.0); // these can be local, are not
		locBase = ON_3dPoint(-10,-100,10);

		dirBase.Rotate( rotationVector.Length() , ON_3dVector( rotationVector.x, -rotationVector.z, rotationVector.y) );
		upBase.Rotate( rotationVector.Length() , ON_3dVector( rotationVector.x, -rotationVector.z, rotationVector.y) );

		// set 'em all
		camDir[rl] = dirBase;
		camUp[rl] = upBase; // there's a chance dir and up can be called only once; given parallel eyes
		camLoc[rl] = locBase + currentScale * ON_3dPoint(tsEyePoses[rl].Position.x, -tsEyePoses[rl].Position.z, tsEyePoses[rl].Position.y);

		//RhinoApp().Print( L"camDir[i] = \t\t\t %f \t %f \t %f \n", camDir[i].x, camDir[i].y, camDir[i].z );
		//RhinoApp().Print( L"camup[i] = \t\t\t %f \t %f \t %f \n", camUp[i].x, camUp[i].y, camUp[i].z );
	}
}

void CVRaptorPlugIn::RHCamsUpdate() // uses current camLoc[] camDir[] and camUp[] to update lView & rView
{
	// doing dumb update
	// set proper rotations etc with new XYZ data / flipped coordinate systems

	lView->ActiveViewport().m_v.m_vp.SetCameraLocation(camLoc[0]);
	lView->ActiveViewport().m_v.m_vp.SetCameraDirection(camDir[0]);
	lView->ActiveViewport().m_v.m_vp.SetCameraUp(camUp[0]);
	
	rView->ActiveViewport().m_v.m_vp.SetCameraLocation(camLoc[1]);
	rView->ActiveViewport().m_v.m_vp.SetCameraDirection(camDir[1]);
	rView->ActiveViewport().m_v.m_vp.SetCameraUp(camUp[1]);

	tfBeforeRedraw =  ovr_GetTimeInSeconds() - tfBegin;

	lView->Redraw(); // doing this sends dib thru to OVR pretty nicely via conduit. 
	rView->Redraw();
}

//////////
// Utilities & Destroyers & Debug

void CVRaptorPlugIn::StoreTimingVars()
{
	tfBeginList.Append(tfBegin);
	tfAtSensorSampleList.Append(tfAtSensorSample);
	tfBeforeRedrawList.Append(tfBeforeRedraw);
	tfAfterRedrawCallList.Append(tfAfterRedrawCall);
	tfAfterRedrawWaitList.Append(tfAfterRedrawWait);
	tfAtRenderTopList.Append(tfAtRenderTop);
	tfBeforeBytesSwapList.Append(tfBeforeBytesSwap);
	tfBeforeTextureSetupList.Append(tfBeforeTextureSetup);
	tfAfterTextureWorkList.Append(tfAfterTextureWork);
	tfBeforeSubmitList.Append(tfBeforeSubmit);
	tfPredictedDisplayList.Append(tfPredictedDisplay);
	tfEndRenderList.Append(tfEndRender);

	tfFrameIndexSubmitList.Append(tfFrameIndexSubmit);
	tfFrameIndexTrackList.Append(tfFrameIndexTrack);
}

void CVRaptorPlugIn::PrintTimingVars()
{
	for(int i = tfBeginList.Count()/10; i < tfBeginList.Count(); i++)
	{
		//double theVal = *tfBeginList.At(i);
		RhinoApp().Print(L"tfBegin: \t\t\t %f \n", *tfBeginList.At(i)); // it's pointers in here
		RhinoApp().Print(L"tfAtSensorSample: \t %f \n", *tfAtSensorSampleList.At(i));
		RhinoApp().Print(L"tfBeforeRedraw: \t\t %f \n", *tfBeforeRedrawList.At(i));
		RhinoApp().Print(L"tfAfterRedrawCall: \t %f \n", *tfAfterRedrawCallList.At(i));
		RhinoApp().Print(L"tfAfterRedrawWait: \t %f \n", *tfAfterRedrawWaitList.At(i));
		RhinoApp().Print(L"tfAtRenderTop: \t\t %f \n", *tfAtRenderTopList.At(i));
		RhinoApp().Print(L"tfBeforeBytesSwap: \t %f \n", *tfBeforeBytesSwapList.At(i));
		RhinoApp().Print(L"tfBeforeTextureSetup: \t %f \n", *tfBeforeTextureSetupList.At(i));
		RhinoApp().Print(L"tfAfterTextureWork: \t %f \n", *tfAfterTextureWorkList.At(i));
		RhinoApp().Print(L"tfBeforeSubmit: \t\t %f \n", *tfBeforeSubmitList.At(i));
		RhinoApp().Print(L"tfPredictedDisplay: \t %f \n", *tfPredictedDisplayList.At(i));
		RhinoApp().Print(L"tfEndRender: \t\t %f \n", *tfEndRenderList.At(i));
		RhinoApp().Print(L"\n");
		RhinoApp().Print(L"tfFrameIndexSubmit: \t %i \n", *tfFrameIndexSubmitList.At(i));
		RhinoApp().Print(L"tfFrameIndexTrack: \t %i \n", *tfFrameIndexTrackList.At(i));
		RhinoApp().Print(L"\n");
	}
}

void CVRaptorPlugIn::rhinoPrintGuid(GUID guid) {
    RhinoApp().Print(L"{%08lX-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX} \n", 
      guid.Data1, guid.Data2, guid.Data3, 
      guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
      guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);
}

void CVRaptorPlugIn::bailOnDibInvalid()
{
	RhinoApp().Print(L"DIB Bits incoming at HMDRender are Null\n");
	RhinoApp().Print(L"renderTrack() %i\n", renderTrack);
}

void CVRaptorPlugIn::ManualDibDraw()
{
	// this writes a Dib which GL does not like to read from.
	VR().lView->DisplayPipeline()->DrawToDib(VR().currentDib[0], VR().currentDib[0].Width(), VR().currentDib[0].Height(), VR().lView->DisplayAttributes());
	VR().rView->DisplayPipeline()->DrawToDib(VR().currentDib[1], VR().currentDib[1].Width(), VR().currentDib[1].Height(), VR().rView->DisplayAttributes());
	// and perhaps we can setup these dibs to not even belong to a VP... 
	
}

void CVRaptorPlugIn::ManualDibSave()
{
	LPCTSTR dibFiles[2] = {L"D:/vrCurrentDib0.bmp", L"D:/vrCurrentDib1.bmp"};
	for (int rl = 0; rl<2; rl++)
	{
		VR().currentDib[rl].SaveBmp(dibFiles[rl]);
	}
}

void CVRaptorPlugIn::HMDDestroy()
{
	ovr_Destroy(HMD);
	ovr_Shutdown();

	// add delete for textures as well

	ON_wString wStr;
	wStr.Format( L"HMDDestroy.\n" );
	RhinoApp().Print( wStr );

    if (mirrorFBO) glDeleteFramebuffers(1, &mirrorFBO);
    if (mirrorTexture) ovr_DestroyMirrorTexture(HMD, reinterpret_cast<ovrTexture*>(mirrorTexture));
    for (int eye = 0; eye < 2; ++eye)
    {
        delete eyeRenderTexture[eye];
        delete eyeDepthBuffer[eye];
    }
    Platform.ReleaseDevice();
    ovr_Destroy(HMD);

	RhinoApp().Print(L"\n //////////////////////////////////////// FIN DRAWANYTHING\n");

}

void CVRaptorPlugIn::makeMortyTex() // 
{
	int soilW, soilH;
	unsigned char* morty = SOIL_load_image("D:/mortyAmorphy.png", &soilW, &soilH, 0, SOIL_LOAD_RGBA); // loading not werking
	if (morty == NULL)
	{
		RhinoApp().Print(L"MORTY IS NULL MORTY\n");
	}

	// make mortyTex & setup to be identical to ovr swapset textures.

	glGenTextures(1, &VR().mortyTex); 
	glBindTexture(GL_TEXTURE_2D, VR().mortyTex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, soilW, soilH, 0, GL_RGBA, GL_UNSIGNED_BYTE, morty);  // must be rgba

	if (morty == NULL)
	{
		RhinoApp().Print(L"MORTY IS NULL MORTY AT THE SECOND ONE MORTY\n");
	}

	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &mortyTexW);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &mortyTexH);
	GLboolean isTextureVRIs = glIsTexture(VR().mortyTex);

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

