/////////////////////////////////////////////////////////////////////////////
// VRaptorPlugIn.h : main header file for the VRaptor plug-in
//

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CVRaptorPlugIn
// See VRaptorPlugIn.cpp for the implementation of this class
//

class CVRaptorPlugIn : public CRhinoUtilityPlugIn
{
public:
	CVRaptorPlugIn();
	~CVRaptorPlugIn();

	const CRhinoCommandContext *VRLaunchContext; 
	// don't think this is safe...
	// probably it changes every time a command is launched...

// Required overrides
	const wchar_t* PlugInName() const;
	const wchar_t* PlugInVersion() const;
	GUID PlugInID() const;
	BOOL OnLoadPlugIn();
	void OnUnloadPlugIn();

// Online help overrides
	BOOL AddToPlugInHelpMenu() const;
	BOOL OnDisplayPlugInHelp( HWND hWnd ) const;


// INITS
	void InitOvrWinWomb();
	void InitHMD();

// GL UTIL
	//void makeRhinoTex(CRhinoUiDib* incomingDib);
	void makeMortyTex();

	GLuint rhinoTex;
	GLuint rhinoTexSet[20];
	GLuint mortyTex;
	GLuint theTex;

	GLint theTexW;
	GLint theTexH;

	int renderTrack;

	GLuint readBufferTexLeft;

	GLuint drawBufferTexLeft;

	void HMDDisplayAnything(); // depreciated
	void HMDDisplayWithDocCode();

	void HMDDestroy();
	void HMDViewsUpdate();
	void HMDRender();


	void OVRtoRHCams(ovrPosef pose[2]);

// OVR THINGS
	ovrSession HMD;
	ovrResult resultSubmit;
	ovrHmdDesc hmdDesc;
	ovrSizei resolution;
	ovrGLTexture * mirrorTexture;

// MAGIC: THE VIEWS;
	CRhinoView* lView;
	CRhinoView* rView;

// GRAPHICS RHINOSIDE
	CRhinoUiDib currentDib; // this gets saved
	LPCTSTR currentDibFile;
	GLsizei rhDibW;
	GLsizei rhDibH;
	CRect vrRect;


// TRACKING
	ovrVector3f ViewOffset[2]; // is - const - in OVR example so don't fuck with

// MISC DEBUG SHOULD GO AWAY
	UINT tick;
	UINT tickDelta;
	UINT frameEta;

private:

	ovrGraphicsLuid luid;

// Graphics
	TextureBuffer * eyeRenderTexture[2];
	DepthBuffer * eyeDepthBuffer[2];
	GLuint mirrorFBO;



	ovrTrackingState OVRDoTracking();
	ovrTrackingState ts; 
	// call these together to update once and have
	// access to later. so ts = OVRDoTracking(); and then use ts. following

	ovrPosef tsEyePoses[2]; // left and right.. final


// vals req'd for cam resets

	OVR::Quat<float> eyePoseQuats[2];
	ON_3dVector upBase;
	ON_3dVector dirBase;
	OVR::Vector3<float> rotationVector;
 
/// cams 

	ON_3dPoint camLoc[2];
	ON_3dVector camDir[2];
	ON_3dVector camUp[2];

//// OTHERS
	float scaleMult;

private:
  ON_wString m_plugin_version;

  // TODO: Add additional class information here
};

class CVRConduit: public CRhinoDisplayConduit // really this one should be in the header
{
public:
	CVRConduit();

	bool ExecConduit(
		CRhinoDisplayPipeline&,	// pipeline executing this conduit
		UINT,					// current channel in pipeline
		bool&					// channel termination flag
		);

	void NotifyConduit(
		EConduitNotifiers,		// event reported by display pipeline
		CRhinoDisplayPipeline&	// pipeline calling this conduit
		);

public:

};

CVRaptorPlugIn& VRaptorPlugIn();

// this is us, for shorthand 
CVRaptorPlugIn& VR();


