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
  void HMDPoseUpdate();

  ovrSession hmdSession;
  ovrGraphicsLuid luid;
  ovrHmdDesc desc;
  ovrSizei resolution;

  ovrPosef pose;

private:
  ON_wString m_plugin_version;

  // TODO: Add additional class information here
};

// there can only be one true RHOVR // where do we define functiones?


CVRaptorPlugIn& VRaptorPlugIn();

// this is us, for shorthand 
CVRaptorPlugIn& VR();



