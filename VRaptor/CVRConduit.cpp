#include "stdafx.h"
#include "VRaptorPlugIn.h"

CVRConduit::CVRConduit()
	: CRhinoDisplayConduit( CSupportChannels::SC_CALCBOUNDINGBOX ) // set notifying channel?
{
	RhinoApp().Print(L"Conduits: \t CVRConduit Constructor \n");
	// do init on conduit
}

void CVRConduit::NotifyConduit(EConduitNotifiers Notify, CRhinoDisplayPipeline& dp) // called more often than exec... so probably more than once a frame.
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

bool CVRConduit::ExecConduit(CRhinoDisplayPipeline& dp, UINT nChannel, bool& bTerminate)
{
	RhinoApp().Print(L"ExecConduit top\n");
	// do shit when conduit it executed?
  switch( nChannel )
  {
	  int tick;
	  int tickDelta;
	  int frameEta;

		case CSupportChannels::SC_CALCBOUNDINGBOX:
			RhinoApp().Print(L"execConduit: \tSC_CALCBOUNDINGBOX\n");

			// FIGURINZ

			// three UINT
			tick = VR().rView->DisplayPipeline()->GetFrameTick(); // abs clock time -> clock cycles since Rhino Launched.
			tickDelta = VR().rView->DisplayPipeline()->GetFrameTickDelta(); // clock cycles since current frame drawing began
			frameEta = VR().rView->DisplayPipeline()->GetFrameETA(); // estimated time of next frame arrival. I think also clock cycles.

			RhinoApp().Print(L"GetFrameTick() \t \t %i\n", tick);
			RhinoApp().Print(L"GetFrameTickDelta() \t %i\n", tickDelta); // not behaving as expected
			RhinoApp().Print(L"GetFrameEta() \t \t %i\n\n", frameEta);
			//RhinoApp().Wait(16);

			// these two are disabled for now while we init & debug HMD Rendering
			//VR().HMDViewsUpdate();

			//VR().HMDViewsRender();

			break;

		case CSupportChannels::SC_INITFRAMEBUFFER: // ah: incoming nChannel is one of these SC_FLAGS and we watch for INITFRAMEBUFFER
			RhinoApp().Print(L"execConduit: \tSC_INITFRAMEBUFFER\n");

			break;

  }
  return true;
}
