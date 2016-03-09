////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN InitVR command
//

#include "StdAfx.h"
#include "VRaptorPlugIn.h"


// REALLY this is viewport init... on re-structure make that clear.

#pragma region InitVR command 

class CCommandInitVR : public CRhinoCommand
{
public:
	CCommandInitVR() {}
	~CCommandInitVR() {}
	UUID CommandUUID()
	{
		// {CD2317C9-7C38-4AD4-802F-6409B7D20303}
		static const GUID InitVRCommand_UUID =
		{ 0xCD2317C9, 0x7C38, 0x4AD4, { 0x80, 0x2F, 0x64, 0x09, 0xB7, 0xD2, 0x03, 0x03 } };
		return InitVRCommand_UUID;
	}
	const wchar_t* EnglishCommandName() { return L"InitVR"; }
	const wchar_t* LocalCommandName() const { return L"InitVR"; }
	CRhinoCommand::result RunCommand( const CRhinoCommandContext& );

public:

	CVRConduitRender vrConduitRenderLeft;
	CVRConduitRender vrConduitRenderRight;

	CVRConduitUpdate vrConduitUpdateH;

};

// The one and only CCommandInitVR object
static class CCommandInitVR theInitVRCommand;

CRhinoCommand::result CCommandInitVR::RunCommand( const CRhinoCommandContext& context )
{

	////////////////// BEGIN VIEWS INIT
	
	AFX_MANAGE_STATE( ::RhinoApp().RhinoModuleState() ); 
	// dunno, from example, makes shit work.
	// docs say to be careful with this one though... so, learning...

	RhinoApp().Print(L"LAUNCHING VR\n");
	
	VR().InitHMD(); 

	///////////////// BUILD VIEWS

	ON_SimpleArray<CRhinoView*> viewList;
	ON_SimpleArray<ON_UUID> viewportIds;

	VR().lView = 0; // initialize object views
	VR().rView = 0;
	VR().hView = 0;
	
	// builds a list of (current) viewport IDs
	context.m_doc.GetViewList( viewList, true, false );
	for ( int i = 0; i < viewList.Count(); i ++)
	{
		CRhinoView* tempView = viewList[i]; // pull view out -> this is redeclared here, in sample, but not in second loop
		if (tempView)
			viewportIds.Append( tempView->ActiveViewportID() );
	}

	viewList.Empty(); // empty bc we are going to re-build later when new views

	context.m_doc.NewView( ON_3dmView() );
	context.m_doc.NewView( ON_3dmView() ); // put three new views into doc
	context.m_doc.NewView( ON_3dmView() ); 

	context.m_doc.GetViewList( viewList, true, false);

	int viewFindCount = 0;
	for (int i = 0; i < viewList.Count(); i++)
	{
		CRhinoView* tempView = viewList[i];
		if (tempView)
		{
			int rc = viewportIds.Search( tempView->ActiveViewportID() ); // returns index of 1st element which satisfies search. returns -1 when no such item found
			if (rc < 0 ) // if current tempView did not exist prior to this running
			{
				if (viewFindCount > 1)
				{
					VR().hView = tempView;
					break;
				}
				if (viewFindCount > 0) // and if lr already found 1
				{
					VR().rView = tempView; // right is 2nd view we find
					viewFindCount = 2;
					// so this breaks when we find, and lView is left as the viewList[i] where we found the new viewport, whose ID was not in our list.
					// and we are left with lView being = viewList[i] at new view
				}
				if (viewFindCount == 0)
				{
					VR().lView = tempView;
					viewFindCount = 1;
				}
			}

			else
				tempView = 0; // reset lView to null and re-loop
		}
	}

	///////////// END BUILD VIEWS

	if (VR().lView && VR().rView) // if exist, are not 0
	{
		VR().InitView(VR().lView, 0); // uses tag (2nd arg) to set view specific: names, locations
		VR().InitView(VR().rView, 1);
		VR().InitView(VR().hView, 2);
	}
	
	//////////////////////////////// END VIEWS INIT

	// RHINO DIB INIT
	VR().lView->GetClientRect(VR().vrLeftRect);
	VR().currentDib[0].CreateDib(VR().vrLeftRect.Width(), VR().vrLeftRect.Height(), 32, true);

	VR().rView->GetClientRect(VR().vrRightRect);
	VR().currentDib[1].CreateDib(VR().vrRightRect.Width(), VR().vrRightRect.Height(), 32, true); // setup with proper color depth

	// RENDERING CONDUITS (Write to DIBS)

	vrConduitRenderLeft.Bind( *VR().lView); 
	vrConduitRenderRight.Bind( *VR().rView); 
	 
	vrConduitRenderLeft.SetID(0);
	vrConduitRenderRight.SetID(1);

	vrConduitRenderLeft.Enable();
	vrConduitRenderRight.Enable();

	VR().InitRHVars(); 

	/*
	// to set to shaded...
	const CDisplayPipelineAttributes* pShddAttrbs = CRhinoDisplayAttrsMgr::StdShadedAttrs();

	VR().hView->DisplayPipeline()->SetDisplayMode(pShddAttrbs->Id());

	*/

	/* // disabled currently in favour of manual-escape 4ever loop
	// IDLE WATCHER (FIRES UPDATE LOOP)
	VR().idleCount = 0;
	
	if(!theIdleWatcher.IsRegistered()) 
	{
		theIdleWatcher.Register();
	}
	else
	{
		theIdleWatcher.Enable(theIdleWatcher.IsEnabled() ? false : true); // flip 2 tru
	}
	*/
	
	/*
	// UPDATE WATCHER (from Master View)
	vrConduitUpdateH.Bind( *VR().hView);
	vrConduitUpdateH.Enable();
	*/

	RhinoApp().Print(L"ALL the VRs are now Init\n");

	return CRhinoCommand::success;
}

#pragma endregion

//
// END InitVR command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
