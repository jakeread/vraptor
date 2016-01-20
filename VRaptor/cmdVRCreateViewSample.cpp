/////////////////////////////////////////////////////////////////////////////
// cmdVRCreateViewSample.cpp : command file
//

#include "StdAfx.h"
#include "VRaptorPlugIn.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN VRCreateViewSample command
//


#pragma region VRCreateViewSample command

class CCommandVRCreateViewSample : public CRhinoCommand
{
public:
	CCommandVRCreateViewSample() {}
	~CCommandVRCreateViewSample() {}
	UUID CommandUUID()
	{
		// {F6359510-4232-4EAE-8056-4D7A6A5EE65B}
		static const GUID VRCreateViewSampleCommand_UUID =
		{ 0xF6359510, 0x4232, 0x4EAE, { 0x80, 0x56, 0x4D, 0x7A, 0x6A, 0x5E, 0xE6, 0x5B } };
		return VRCreateViewSampleCommand_UUID;
	}
	const wchar_t* EnglishCommandName() { return L"VRCreateViewSample"; }
	const wchar_t* LocalCommandName() const { return L"VRCreateViewSample"; }
	CRhinoCommand::result RunCommand( const CRhinoCommandContext& );
};

// The one and only CCommandVRCreateViewSample object
static class CCommandVRCreateViewSample theVRCreateViewSampleCommand;

CRhinoCommand::result CCommandVRCreateViewSample::RunCommand( const CRhinoCommandContext& context )
{
	ON_wString wStr;
	wStr.Format( L"The \"%s\" set; new command.\n", EnglishCommandName() );
	RhinoApp().Print( wStr );

		// begin sample from http://wiki.mcneel.com/developer/sdksamples/newview

	AFX_MANAGE_STATE( ::RhinoApp().RhinoModuleState() );
 
	ON_SimpleArray<ON_UUID> viewport_ids;
	ON_SimpleArray<CRhinoView*> view_list;
	CRhinoView* view = 0; // initializes with nothing
	int i = 0;
 
	// Build a list of (current) viewport ids
	context.m_doc.GetViewList( view_list, true, false ); // an array of CRhinoView*('s) currently open. name is view_list
	for( i = 0; i < view_list.Count(); i++ ) // for each view in list,
	{
	  CRhinoView* view = view_list[i]; // object view, get from list position i
	  if( view ) // not sure why, but check again that view exists ? or what ?
	     viewport_ids.Append( view->ActiveViewportID() ); // for each view, appends ViewportID to ON_SimpleArray named viewport_ids
	 }
	 view_list.Empty(); // decrements count by one ... ? b/c taking out '0' initializing view
 
	 // Create a new view
	 context.m_doc.NewView( ON_3dmView() ); // new view. can we do this str8 ? will want to initialize view with our own info anyways
 
	 // Find the viewport (id) that was just created
	 context.m_doc.GetViewList( view_list, true, false );
	 for( i = 0; i < view_list.Count(); i++ )
	 {
	  view = view_list[i];
	  if( view )
	   {
	    int rc = viewport_ids.Search( view->ActiveViewportID() );
	     if( rc < 0 )
	       break;
	     else
	       view = 0;
	   }
	  }
 
	 // Make the necessary updates.
	 if( view )
	 {
	   ON_3dmView v = view->ActiveViewport().View();
	  v.m_name = L"viewview";
	  view->ActiveViewport().SetView( v );
	  view->Redraw();
	 }

	return CRhinoCommand::success;
}

#pragma endregion

//
// END VRCreateViewSample command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
