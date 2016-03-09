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
	
	VR().StdUpdate();
	//RhinoApp().Print(L"CRhinoIdleWatcher: %i\n", VR().idleCount);
}

//void CRhinoIdleWatcher::CRhinoEventWatcherEx