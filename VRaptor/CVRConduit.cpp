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

	int texInspectW;
	int texInspectH;

	
	HWND rhinoHWND = VR().rView->GetSafeHwnd();
	HDC rhinoHDC = GetDC(rhinoHWND);


  switch( nChannel )
  {
	case CSupportChannels::SC_POSTPROCESSFRAMEBUFFER:
		RhinoApp().Print(L"execConduit: \tSC_POSTPROCESSFRAMEBUFFER\n");
		VR().rView->GetClientRect(VR().vrRect);
		VR().currentDib.CreateDib(VR().vrRect.Width(), VR().vrRect.Height(), 32, true); // setup with proper color depth
		
		VR().currentDib = VR().rView->DisplayPipeline()->GetFrameBuffer();
		VR().currentDib.SaveBmp(VR().currentDibFile); // WRITE IT. LEAVE IT.
		
		VR().HMDRender(); // will flip to it's OGL context

		// amaze. congrats.

		// maybe we just hit a 'render switch' here... like 'New frame is Ready!'. 
		// ovr OGL is always looking. sets flag back to 0 once it has pushed frame thru.
		// and this is looking for a 'ready to track' flag.. ovr sets that.

		// HGLRC rhinoHGLRC = VR().rView->DisplayPipeline()->GetDrawDC; cannot find

		// wglMakeCurrent(rhinoHDC, NULL); // flip back to rhino ogl context b4 proceeding?
		
		break;
		// need that break, else conduit does not finish, display pipeline becomes upset. then render later.
		// also we keep out DIB / Rhino and our Texture / OVR code in different playpens

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
