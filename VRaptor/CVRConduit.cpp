#include "stdafx.h"
#include "VRaptorPlugIn.h"

CVRConduit::CVRConduit()
	: CRhinoDisplayConduit( CSupportChannels::SC_POSTPROCESSFRAMEBUFFER ) // set notifying channel?
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
	CRhinoUiDib ppfbDib;
	CDisplayPipelineAttributes * cdpa = &CDisplayPipelineAttributes();
	LPCTSTR ppfbDibFile = L"D:/ppfbDib.bmp";

  switch( nChannel )
  {

	case CSupportChannels::SC_POSTPROCESSFRAMEBUFFER:
		RhinoApp().Print(L"execConduit: \tSC_POSTPROCESSFRAMEBUFFER\n");
		ppfbDib = VR().rView->DisplayPipeline()->GetFrameBuffer();
		ppfbDib.SaveBmp(ppfbDibFile);
		
		// CALL send to HMD
		// how to make class independent of view?

		break;

	case CSupportChannels::SC_CALCBOUNDINGBOX:
		RhinoApp().Print(L"execConduit: \tSC_CALCBOUNDINGBOX\n");

		// FIGURINZ

		// three UINT
		VR().tick = VR().rView->DisplayPipeline()->GetFrameTick(); // abs clock time -> clock cycles since Rhino Launched.
		VR().tickDelta = VR().rView->DisplayPipeline()->GetFrameTickDelta(); // clock cycles since current frame drawing began
		VR().frameEta = VR().rView->DisplayPipeline()->GetFrameETA(); // estimated time of next frame arrival. I think also clock cycles.

		RhinoApp().Print(L"GetFrameTick() \t \t %i\n", VR().tick);
		RhinoApp().Print(L"GetFrameTickDelta() \t %i\n", VR().tickDelta); // not behaving as expected
		RhinoApp().Print(L"GetFrameEta() \t \t %i\n\n", VR().frameEta);

		//RhinoApp().Wait(16);

		// these two are disabled for now while we init & debug HMD Rendering
		//VR().HMDViewsUpdate();
		
		// how to add wait here ??

		//VR().HMDViewsRender();

		break;

	case CSupportChannels::SC_INITFRAMEBUFFER: // ah: incoming nChannel is one of these SC_FLAGS and we watch for INITFRAMEBUFFER
		RhinoApp().Print(L"execConduit: \tSC_INITFRAMEBUFFER\n");

		break;

  }
  return true;
}
