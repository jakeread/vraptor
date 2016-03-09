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
	void Inputs();
	void StdUpdate();

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
	CRhinoView* hView;

	CRhinoUiDib currentDib[2]; // this gets saved

	LPCTSTR currentDibFile;

	int leftRenderSetTrack;
	int rightRenderSetTrack;
	int renderTrack;
	bool disableConduits;

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
	long long vrFrameIndex;
	double predictedDisplay;
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

// MISC DEBUG SHOULD GO AWAY
	UINT tick;
	UINT tickDelta;
	UINT frameEta;

	void StoreTimingVars();
	void PrintTimingVars();

	int idleCount;

// INTERFACE VARS
	float currentScale;
	POINT cPt;
	ON_3dPoint cPtPt;

	// timing floats. Jesus.
	double tfBegin;
	ON_SimpleArray<double> tfBeginList;
	double tfAtDoTracking;
	ON_SimpleArray<double> tfAtDoTrackingList;
	double tfAtCamsUpdate;
	ON_SimpleArray<double> tfAtCamsUpdateList;

	double tfAtSensorSample;
	ON_SimpleArray<double> tfAtSensorSampleList;

	double tfBeforeRedraw;
	ON_SimpleArray<double> tfBeforeRedrawList;
	double tfAfterRedrawCall;
	ON_SimpleArray<double> tfAfterRedrawCallList;
	double tfAfterRedrawWait;
	ON_SimpleArray<double> tfAfterRedrawWaitList;

	double tfAtRenderTop;
	ON_SimpleArray<double> tfAtRenderTopList;
	double tfBeforeBytesSwap;
	ON_SimpleArray<double> tfBeforeBytesSwapList;

	double tfBeforeTextureSetup;
	ON_SimpleArray<double> tfBeforeTextureSetupList;
	double tfBeforeTextureBlit;
	ON_SimpleArray<double> tfBeforeTextureBlitList;
	double tfAfterTextureWork;
	ON_SimpleArray<double> tfAfterTextureWorkList;

	double tfPredictedDisplay;
	ON_SimpleArray<double> tfPredictedDisplayList;

	double tfBeforeSubmit;
	ON_SimpleArray<double> tfBeforeSubmitList;
	double tfEndRender;
	ON_SimpleArray<double> tfEndRenderList;

	long long tfFrameIndexSubmit;
	ON_SimpleArray<long long> tfFrameIndexSubmitList;
	long long tfFrameIndexTrack;
	ON_SimpleArray<long long> tfFrameIndexTrackList;

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

	ON_3dPoint hCamLoc;
	ON_3dVector hCamDir;
	ON_3dVector hCamUp;

	ON_3dPoint camLoc[2];
	ON_3dVector camDir[2];
	ON_3dVector camUp[2];

private:
  ON_wString m_plugin_version;

  // TODO: Add additional class information here
};

///////////////// IDLE WATCHER

#pragma region CRhinoIdleWatcher class

class CRhinoIdleWatcher : public CRhinoIsIdle
{
public:
  CRhinoIdleWatcher();
  void Notify( const class CRhinoIsIdle::CParameters& params );

private:
  __time64_t m_time0;
};

// The one and only CRhinoIdleWatcher object
static class CRhinoIdleWatcher theIdleWatcher;

#pragma endregion

/////////////// CONDUITS

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


// UI

int EscapeKeyPressed();