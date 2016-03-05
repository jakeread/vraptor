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
	GLuint rhinoTexSet[2];
	GLuint mortyTex;
	GLuint theTex;

	GLint theTexW[2];
	GLint theTexH[2];

	GLint mortyTexW;
	GLint mortyTexH;

	int renderTrack;

	GLuint readBufferTexLeft; // should eliminate sided-ness
	GLuint drawBufferTexLeft;

	void HMDDisplayAnything(); // depreciated
	void HMDDisplayWithDocCode();

	void HMDDestroy();
	void HMDViewsUpdate();
	void HMDRender();

	void ManualDibDraw();

	void OVRDoTracking();
	ovrTrackingState ts;
	ovrPosef tsEyePoses[2]; // left and right.. final

	void RHCamsUpdate();

	void rhinoPrintGuid(GUID guid);

	bool disableConduits;

	
	/*
	CVRConduitRender* conduitUpdatePointers[2];
	CVRConduitUpdate* conduitRenderPointers[2];
	*/

// OVR THINGS
	ovrSession HMD;
	ovrResult resultSubmit;
	ovrHmdDesc hmdDesc;
	ovrSizei resolution;
	ovrSizei idealTextureSize;
	ovrGLTexture * mirrorTexture;

// MAGIC: THE VIEWS;
	CRhinoView* lView;
	CRhinoView* rView;
	int doubleCount;

// GRAPHICS RHINOSIDE
	CRhinoUiDib currentDib[2]; // this gets saved

	LPCTSTR currentDibFile;

	GLsizei rhDibW[2];
	GLsizei rhDibH[2];

	CRect vrLeftRect;
	CRect vrRightRect;

	int leftRenderSetTrack;
	int rightRenderSetTrack;


// TRACKING
	ovrVector3f ViewOffset[2]; // is - const - in OVR example so don't fuck with

// MISC DEBUG SHOULD GO AWAY
	UINT tick;
	UINT tickDelta;
	UINT frameEta;

//// OTHERS
	float scaleMult;

private:

	ovrGraphicsLuid luid;

// Graphics
	TextureBuffer * eyeRenderTexture[2];
	DepthBuffer * eyeDepthBuffer[2];
	GLuint mirrorFBO;

	// call these together to update once and have
	// access to later. so ts = OVRDoTracking(); and then use ts. following

// vals req'd for cam resets

	OVR::Quat<float> eyePoseQuats[2];
	ON_3dVector upBase;
	ON_3dVector dirBase;
	ON_3dPoint locBase; // use for location prior to ovr tracking / positioning
	OVR::Vector3<float> rotationVector;
 
/// cams 

	ON_3dPoint camLoc[2];
	ON_3dVector camDir[2];
	ON_3dVector camUp[2];

private:
  ON_wString m_plugin_version;

  // TODO: Add additional class information here
};

class CVRConduitRender: public CRhinoDisplayConduit // calls HMDRender currently
{
public:
	CVRConduitRender();

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

class CVRConduitUpdate: public CRhinoDisplayConduit // does updating 2 views
{
public:
	CVRConduitUpdate();

	bool ExecConduit(
		CRhinoDisplayPipeline&,
		UINT,
		bool&
		);

	void NotifyConduit(
		EConduitNotifiers,
		CRhinoDisplayPipeline&
		);

public:

};

CVRaptorPlugIn& VRaptorPlugIn();

// this is us, for shorthand 
CVRaptorPlugIn& VR();


