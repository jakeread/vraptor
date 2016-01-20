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
	wStr.Format( L"READY SET", EnglishCommandName() );
	RhinoApp().Print( wStr );

	context.m_doc.NewView( ON_3dmView() );
	context.m_doc.NewView( ON_3dmView() ); // make 2 new views

	ON_SimpleArray<CRhinoView*> viewList; // don't know what is up with* this*

	context.m_doc.GetViewList(viewList, true, true);

	CRhinoView* LeftVRView = viewList[viewList.Count()-1];
	CRhinoView* RightVRView = viewList[viewList.Count()];

	// now re-name, update positions, update positions outside of loop: continuously
	// then, orbit?

	return CRhinoCommand::success;
}

#pragma endregion

//
// END VRCreateViews command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
