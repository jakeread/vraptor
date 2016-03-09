#include "stdafx.h"
#include "VRaptorPlugIn.h"

CVRConduitRender::CVRConduitRender()
	: CRhinoDisplayConduit( CSupportChannels::SC_POSTPROCESSFRAMEBUFFER ) // set notifying channel?
{
	RhinoApp().Print(L"Conduits: \t CVRConduitRender Constructor \n");
	// do init on conduit. add notes on which view it is...
}

void CVRConduitRender::SetID(int ID)
{
	internalID = ID;
}

void CVRConduitRender::NotifyConduit(EConduitNotifiers Notify, CRhinoDisplayPipeline& dp) // called more often than exec... so probably more than once a frame.
{
	// do shit when conduit is notified: with incoming Notify Tag
	//RhinoApp().Print(L"NotifyConduit\n");
	switch( Notify )
	{
		case CN_PROJECTIONCHANGED:
			{
				//RhinoApp().Print(L"Conduits: \t NotifyConduit case CN_PORJECTIONCHANGED at ");
			}
	}

}

bool CVRConduitRender::ExecConduit(CRhinoDisplayPipeline& dp, UINT nChannel, bool& bTerminate)
{
	//RhinoApp().Print(L"ExecConduit top\n");

	if (VR().disableConduits == true)
	{
		this->Disable();
	}

  switch( nChannel )
  {
	case CSupportChannels::SC_POSTPROCESSFRAMEBUFFER: 

		// RhinoApp().Print(L"execConduit: \tSC_POSTPROCESSFRAMEBUFFER\n");

		if (internalID == 0)
		{
			VR().currentDib[0] = dp.GetFrameBuffer();
			VR().leftRenderSetTrack = true;
			//RhinoApp().Print(L"LEFTSET\n");
		}

		if (internalID == 1)
		{
			VR().currentDib[1] = dp.GetFrameBuffer();
			VR().rightRenderSetTrack = true;
			//RhinoApp().Print(L"RIGHTSET\n");
		}

		if (VR().leftRenderSetTrack && VR().rightRenderSetTrack)
		{
			VR().leftRenderSetTrack = false;
			VR().rightRenderSetTrack = false;
			//RhinoApp().Print(L"FIRE\n");
			//VR().HMDRender();  // -> this method is causing left/right jitter. to debug later
		}

		break;
  }
  return true;
}
