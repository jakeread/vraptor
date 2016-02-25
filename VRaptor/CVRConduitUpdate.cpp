#include "stdafx.h"
#include "VRaptorPlugIn.h"

CVRConduitUpdate::CVRConduitUpdate()
	: CRhinoDisplayConduit( CSupportChannels::SC_INITFRAMEBUFFER ) // set notifying channel?
{
	RhinoApp().Print(L"Conduits: \t CVRConduitUpdate Constructor \n");
	// do init on conduit
}

void CVRConduitUpdate::NotifyConduit(EConduitNotifiers Notify, CRhinoDisplayPipeline& dp) // called more often than exec... so probably more than once a frame.
{
	// do shit when conduit is notified: with incoming Notify Tag
	//RhinoApp().Print(L"NotifyConduit\n");
	switch( Notify )
	{
		case CN_PROJECTIONCHANGED:
			{
				//ON_wString name;
				//name.Format( this->m_pView->ActiveViewport().Name() );
				//RhinoApp().Print(L"Conduits: \t NotifyConduit case CN_PORJECTIONCHANGED at ");
				//RhinoApp().Print( name  );
				//RhinoApp().Print(L"\n"); 

				//RhinoApp().Print(L"notifyConduit: \tCN_PROJECTIONCHANGED\n");

				// VR().HMDViewsUpdate();
			}
	}

}

bool CVRConduitUpdate::ExecConduit(CRhinoDisplayPipeline& dp, UINT nChannel, bool& bTerminate)
{
	//RhinoApp().Print(L"ExecConduit top\n");
	// do shit when conduit it executed?

	/*
	HWND rhinoHWND = VR().rView->GetSafeHwnd();
	HDC rhinoHDC = GetDC(rhinoHWND);
	*/

  switch( nChannel )
  {
  case CSupportChannels::SC_INITFRAMEBUFFER:
		RhinoApp().Print(L"execConduit: \tSC_INITFRAMEBUFFER\n");

		VR().RHCamsUpdate();

		break;
  }
  return true;
}
