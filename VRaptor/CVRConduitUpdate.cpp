#include "stdafx.h"
#include "VRaptorPlugIn.h"

CVRConduitUpdate::CVRConduitUpdate()
	: CRhinoDisplayConduit( CSupportChannels::SC_POSTPROCESSFRAMEBUFFER ) // set notifying channel?
{
	RhinoApp().Print(L"Conduits: \t CVRConduitUpdate Constructor \n");
}

void CVRConduitUpdate::NotifyConduit(EConduitNotifiers Notify, CRhinoDisplayPipeline& dp) // called more often than exec... so probably more than once a frame.
{
	switch( Notify )
	{
		case CN_PROJECTIONCHANGED:
			{
				// notified.
			}
	}

}

bool CVRConduitUpdate::ExecConduit(CRhinoDisplayPipeline& dp, UINT nChannel, bool& bTerminate)
{
	if (VR().disableConduits == true)
	{
		this->Disable();
	}

  switch( nChannel )
  {
  case CSupportChannels::SC_POSTPROCESSFRAMEBUFFER:
		//RhinoApp().Print(L"execConduit: \tSC_INITFRAMEBUFFER\n");
		
		if(VR().renderTrack < 50)
		{
			RhinoApp().Print(L"execConduit: Update: hView: \tSC_INITFRAMEBUFFER\n");
		}
		else
		{
			//VR().StdUpdate();
		}

		break;
  }
  return true;
}
