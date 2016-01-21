/////////////////////////////////////////////////////////////////////////////
// cmdVRCreateViewSample.cpp : command file
//

#include "StdAfx.h"
#include "VRaptorPlugIn.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN VRCreateViews command
//

#pragma region VRCreateViews command

class CCommandVRCreateViews : public CRhinoCommand
{
public:
	CCommandVRCreateViews() {}
	~CCommandVRCreateViews() {}
	UUID CommandUUID()
	{
		// {DC355E7F-CD52-462F-88E5-796E8E69EDC2}
		static const GUID VRCreateViewsCommand_UUID =
		{ 0xDC355E7F, 0xCD52, 0x462F, { 0x88, 0xE5, 0x79, 0x6E, 0x8E, 0x69, 0xED, 0xC2 } };
		return VRCreateViewsCommand_UUID;
	}
	const wchar_t* EnglishCommandName() { return L"VRCreateViews"; }
	const wchar_t* LocalCommandName() const { return L"VRCreateViews"; }
	CRhinoCommand::result RunCommand( const CRhinoCommandContext& );
};

// The one and only CCommandVRCreateViews object
static class CCommandVRCreateViews theVRCreateViewsCommand;

CRhinoCommand::result CCommandVRCreateViews::RunCommand( const CRhinoCommandContext& context )
{
	ON_wString wStr;
	wStr.Format( L"READY SET\n", EnglishCommandName() );
	RhinoApp().Print( wStr );

	context.m_doc.NewView( ON_3dmView() );
	context.m_doc.NewView( ON_3dmView() ); // make 2 new views

	ON_SimpleArray<CRhinoView*> viewList; // don't know what is up with* this*

	context.m_doc.GetViewList(viewList, true, true);

	CRhinoView* LeftVRView = viewList[viewList.Count()-2];
	CRhinoView* RightVRView = viewList[viewList.Count()-1];

	// TO NEW METHOD

	ON_3dPoint locationL = ON_3dPoint(0.0,100.1,0.1);
	ON_3dPoint locationR = ON_3dPoint(0.0,165.1,0.2);

	LeftVRView->ActiveViewport().SetCameraLocation(locationL);
	RightVRView->ActiveViewport().SetCameraLocation(locationR);

	// proper: should check if 2 viewports with these stats already exist, but that's for later...
	
	// but can't seem to change the name. throws windows assertion error. wtf.

	//ON_wString left;
	//left.Format(L"LeftVRView", EnglishCommandName() );
	//RhinoApp().Print( left );
	//LeftVRView->ActiveViewport().SetName( left );

	//ON_wString right;
	//left.Format(L"RightVRView", EnglishCommandName() );
	//RhinoApp().Print( right );
	// LeftVRView->ActiveViewport().SetName( right ); // bool ??!?!

	// now re-name update positions outside of loop: continuously
	// bring in OVR Tracking and assign to VR Viewports
	// then, orbit?

	return CRhinoCommand::success;
}

#pragma endregion

//
// END VRCreateViews command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
