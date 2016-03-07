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
	void InitView(CRhinoView *newView, int num);
	void InitRHVars();

// RUNTIME
	void HMDViewsUpdate();
	bool HMDRender();
	void OVRDoTracking();
	void RHCamsUpdate();

// Utilities & Destroyers & Debug
	void rhinoPrintGuid(GUID guid);
	void ManualDibDraw();
	void ManualDibSave();
	void HMDDestroy();
	void makeMortyTex();
	void bailOnDibInvalid();

// GRAPHICS RHINOSIDE

	CRhinoView* lView;
	CRhinoView* rView;

	CRhinoUiDib currentDib[2]; // this gets saved

	LPCTSTR currentDibFile;

	int doubleCount;
	int leftRenderSetTrack;
	int rightRenderSetTrack;

	GLsizei rhDibW[2];
	GLsizei rhDibH[2];

	CRect vrLeftRect;
	CRect vrRightRect;

// OVR NATIVES
	ovrSession HMD;
	ovrHmdDesc hmdDesc;
	ovrSizei resolution;
	ovrSizei idealTextureSize;

// TRACKING LOCATIONS OVR & RHINO
	// OVR TRACKING
	ovrTrackingState ts;
	double sensorSampleTime; // passed during submitFrame, updated when tracking
	ovrPosef tsEyePoses[2]; // left and right.. final
	ovrVector3f viewOffsets[2]; // is - const - in OVR example so don't fuck with
	ovrEyeRenderDesc eyeRenderDescs[2];

// GL / OVR TEX & BUFFERS
	// OVR GL
	TextureBuffer *eyeRenderTexture[2];
	DepthBuffer *eyeDepthBuffer[2];
	ovrGLTexture *mirrorTexture;

	// GL
	GLuint mirrorFBO;

	GLuint rhinoTexSet[2];
	GLuint mortyTex;

	GLuint readBuffer; // should eliminate sided-ness
	GLuint drawBuffer;

	GLint theTexW[2]; // could be [][] of [rl][w] and [rl][h] but hey!
	GLint theTexH[2];

	GLint mortyTexW;
	GLint mortyTexH;

	int renderTrack;

	bool disableConduits;

// MISC DEBUG SHOULD GO AWAY
	UINT tick;
	UINT tickDelta;
	UINT frameEta;

// INTERFACE VARS
	float currentScale;

private:

	ovrGraphicsLuid luid;
	// GL Context things
	HGLRC ovrHGLRC;
	HWND ovrHWND;
	HDC ovrHDC;

	// largely belong to HMDRender()
	LPBYTE theBytes[2];
	ovrGLTexture* tex;
	ovrViewScaleDesc viewScaleDesc;
	ovrLayerEyeFov ld;
	ovrLayerHeader* layers;
	ovrResult resultSubmit;

// Graphics

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

	void SetID(int ID);
	int internalID;

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


