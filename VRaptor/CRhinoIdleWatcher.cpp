#include "stdafx.h"
#include "VRaptorPlugIn.h"

CRhinoIdleWatcher::CRhinoIdleWatcher()
: CRhinoIsIdle( RhinoApp().Rhino5_UUID() )
, m_time0(0)
{
}

void CRhinoIdleWatcher::Notify( const class CRhinoIsIdle::CParameters& params )
{
	VR().idleCount++;
	
	VR().HMDViewsUpdate();
	VR().tfAfterRedrawCall =  ovr_GetTimeInSeconds() - VR().tfBegin;
	RhinoApp().Wait(1); // wait pauses plugin but keeps 'windows message pump' alive. views redraw.
	VR().tfAfterRedrawWait =  ovr_GetTimeInSeconds() - VR().tfBegin;
	VR().HMDRender();
	VR().StoreTimingVars();
	//RhinoApp().Print(L"CRhinoIdleWatcher: %i\n", VR().idleCount);
}

//void CRhinoIdleWatcher::CRhinoEventWatcherEx