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

  bool HMDInit();
  void HMDDestroy();
  void HMDPrintUpdate();
  void HMDViewsUpdate();

  ovrSession hmdSession;
  ovrGraphicsLuid luid;
  ovrHmdDesc desc;
  ovrSizei resolution;

  ovrPosef pose;
  ovrPosef headPose;

  ovrVector3f hmdToEyeViewOffsetRaptor[2]; // is - const - in OVR example so don't fuck with
  ovrPosef outEyePosesRaptor[2]; // left and right.. final

  CRhinoView* lView;
  CRhinoView* rView;

  ON_3dPoint lLocation;
  ON_3dPoint rLocation;


private:
  ON_wString m_plugin_version;

  // TODO: Add additional class information here
};

// there can only be one true RHOVR // where do we define functiones?


CVRaptorPlugIn& VRaptorPlugIn();

// this is us, for shorthand 
CVRaptorPlugIn& VR();



