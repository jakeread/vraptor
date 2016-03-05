#include "stdafx.h"
#include "VRaptorPlugIn.h"

CVRConduitRender::CVRConduitRender()
	: CRhinoDisplayConduit( CSupportChannels::SC_POSTPROCESSFRAMEBUFFER ) // set notifying channel?
{
	RhinoApp().Print(L"Conduits: \t CVRConduitRender Constructor \n");
	// do init on conduit
}

void CVRConduitRender::NotifyConduit(EConduitNotifiers Notify, CRhinoDisplayPipeline& dp) // called more often than exec... so probably more than once a frame.
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

bool CVRConduitRender::ExecConduit(CRhinoDisplayPipeline& dp, UINT nChannel, bool& bTerminate)
{
	//RhinoApp().Print(L"ExecConduit top\n");
	// do shit when conduit it executed?

	/*
	HWND rhinoHWND = VR().rView->GetSafeHwnd();
	HDC rhinoHDC = GetDC(rhinoHWND);
	*/

	if (VR().disableConduits == true)
	{
		this->Disable();
	}

  switch( nChannel )
  {
	case CSupportChannels::SC_POSTPROCESSFRAMEBUFFER: 

		RhinoApp().Print(L"firingContuit at PostProcess at Render\n");
		/*

		// going to try a dumb update loop ...

		// then will try to restructure these conduits.
		can we have just one and update the eye render dibs manually, as below?

		// and need to do lview->vp->drawToDib()  I think. push lview & rviews 
		through on a single
		conduit / view which is *basically* your mirror buffer. lview & rview become 'dummies'
		and mouse / ux / interaction happens in a single fullscreen view for mouse...

		you've gotta build this differently. 
		class should know which (lview or rview) it is, the check is not robust enough.
		and there is much else restructuring to do.
		missed this bet but OK there are bigger fish! Keep it in your head.

		right now all timing is basically off. the big question is how do we know when we have two
		fresh DIBS? How do we push a render through in a healthy way?

		then we need to do the position update, probably on some timer.. another bit to figure.

		we are getting errors for 'illegal camera position change' so look / learn from some
		rhino examples re: cam changes
		
		watch for & avoid FPU errors - or check this FPUdirty thing.

		home stretch for the rendering core though... this might work!

		*/

		//RhinoApp().Print(L"execConduit: \tSC_POSTPROCESSFRAMEBUFFER\n");

		GUID theConduitVPID = dp.GetRhinoVP().ViewportId();
		GUID rightViewVPID = VR().rView->MainViewport().ViewportId();
		GUID leftViewVPID = VR().lView->MainViewport().ViewportId();

		if ( theConduitVPID == leftViewVPID)
		{
			int rl = 0;
			VR().currentDib[rl] = VR().lView->DisplayPipeline()->GetFrameBuffer();
			VR().leftRenderSetTrack = 1;
		}

		if ( theConduitVPID == rightViewVPID )
		{
			int rl = 1;
			VR().currentDib[rl] = VR().rView->DisplayPipeline()->GetFrameBuffer();
			VR().rightRenderSetTrack = 1;
		}

		if ( !(theConduitVPID == leftViewVPID) && !(theConduitVPID == rightViewVPID) )
		{
			RhinoApp().Print(L"couldn't find a Left or Right VR View on POSTPROCESS at Render Conduit\n");

			RhinoApp().Print(L"theConduitVPID \t");
			VR().rhinoPrintGuid(theConduitVPID);

			RhinoApp().Print(L"theRightVPID \t");
			VR().rhinoPrintGuid(rightViewVPID);

			RhinoApp().Print(L"theLeftVPID \t");
			VR().rhinoPrintGuid(leftViewVPID);

			VR().disableConduits = true;
		}

		if (true) // if no render, do it regardless. endless loop not initiated
		{
			VR().leftRenderSetTrack = 1;
			VR().rightRenderSetTrack = 1;
		}

		if (VR().leftRenderSetTrack == 1 && VR().rightRenderSetTrack == 1) // do we have both dibs?
		{
			//RhinoApp().Print(L"THROWING RENDER\n");
			VR().HMDRender(); // will flip to it's OGL context, and render 2 dibs...
			// VR().OVRDoTracking(); // do it 2 next
			VR().leftRenderSetTrack = 0;
			VR().rightRenderSetTrack = 0;
		}

		// HGLRC rhinoHGLRC = VR().rView->DisplayPipeline()->GetDrawDC; cannot find

		// wglMakeCurrent(rhinoHDC, NULL); // flip back to rhino ogl context b4 proceeding?
		
		break;
		// need that break, else conduit does not finish, display pipeline becomes upset. then render later.
		// also we keep out DIB / Rhino and our Texture / OVR code in different playpens

  }
  return true;
}
