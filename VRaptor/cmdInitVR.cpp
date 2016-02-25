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
	CVRConduitRender vrConduitLeft;
	CVRConduitRender vrConduitRight;

	CVRConduitUpdate vrConduitUpdateLeft;
	CVRConduitUpdate vrConduitUpdateRight;
};

// The one and only CCommandInitVR object
static class CCommandInitVR theInitVRCommand;

CRhinoCommand::result CCommandInitVR::RunCommand( const CRhinoCommandContext& context )
{
	VR().VRLaunchContext = &context; // CHEAT. Or take some time to understand
	// how Rhino contexts work

	////////////////// BEGIN VIEWS INIT
	
	AFX_MANAGE_STATE( ::RhinoApp().RhinoModuleState() ); // dunno, from example, makes shit work.

	ON_wString wStr;
	wStr.Format( L"READY SET\n", EnglishCommandName() );
	RhinoApp().Print( wStr );

	ON_SimpleArray<CRhinoView*> viewList; // don't know what is up with* this*
	ON_SimpleArray<ON_UUID> viewportIds;
	CRhinoView* lView = 0;
	CRhinoView* rView = 0;
	ON_SimpleArray<CRhinoView*> lrViews; // will contain our vr views

	int i = 0; // also use this in loops
	int lr = 0; // use to track 1st and 2nd find

	// builds a list of (current) viewport IDs
	context.m_doc.GetViewList( viewList, true, false );
	for ( i = 0; i < viewList.Count(); i ++)
	{
		CRhinoView* tempView = viewList[i]; // pull view out -> this is redeclared here, in sample, but not in second loop
		if (tempView)
			viewportIds.Append( tempView->ActiveViewportID() );
	}

	viewList.Empty(); // empty bc we are going to re-build later when new views

	context.m_doc.NewView( ON_3dmView() );
	context.m_doc.NewView( ON_3dmView() ); // put two new views into doc

	context.m_doc.GetViewList( viewList, true, false);
	for (i = 0; i < viewList.Count(); i++)
	{
		CRhinoView* tempView = viewList[i];
		if (tempView)
		{
			int rc = viewportIds.Search( tempView->ActiveViewportID() ); // returns index of 1st element which satisfies search. returns -1 when no such item found
			if (rc < 0 ) // if current tempView did not exist prior to this running
			{
				if (lr > 0) // and if lr already found 1
				{
					rView = tempView; // right is 2nd view we find 
					break;
				// so this breaks when we find, and lView is left as the viewList[i] where we found the new viewport, whose ID was not in our list.
				// and we are left with lView being = viewList[i] at new view
				}
				if (lr == 0)
				{
					lView = tempView; // left is 1st view
					lr = 1;
				}
			}

			else
				tempView = 0; // reset lView to null and re-loop
		}
	}

	lrViews.Append(lView);
	lrViews.Append(rView); // bad management...

	if (lView && rView)
	{
		for (int i = 0; i < 2; i++)
		{
			// 			RhinoApp().ActiveView()->
			ON_3dmView onView = lrViews[i]->ActiveViewport().View();

			if(i == 0)
				onView.m_name = L"lView";
				//lrViews[i]->MoveWindow(0,0,VR().resolution.w/2,VR().resolution.h, true);
			if(i == 1)
				onView.m_name = L"rView";
				//lrViews[i]->MoveWindow(960,0,VR().resolution.w/2,VR().resolution.h, true);
			lrViews[i]->ActiveViewport().SetView(onView);
			lrViews[i]->ActiveViewport().m_v.m_vp.ChangeToPerspectiveProjection(50,true,35);
			//lrViews[i]->FloatRhinoView(true);
			lrViews[i]->Redraw();
		}
	}

	VR().lView = lView; // pass views to our plugin object 
	VR().rView = rView;

	//////////////////////////////// END VIEWS INIT

	/////////////////////////// PIPELINE INIT

	lView->Redraw();
	rView->Redraw();

	VR().rView->GetClientRect(VR().vrRect);

	VR().currentDib.CreateDib(VR().vrRect.Width(), VR().vrRect.Height(), 32, true); // setup with proper color depth

	VR().currentDibFile = L"D:/ppfbDib.bmp";

	VR().InitHMD();
	
	vrConduitLeft.Bind( *lView);
	vrConduitRight.Bind( *rView);

	vrConduitUpdateLeft.Bind( *lView);
	vrConduitUpdateRight.Bind( *rView);

	//////////////////////////// FIRE IN THE HOLE (right only for now)
	vrConduitLeft.Enable();
	vrConduitRight.Enable();

	vrConduitUpdateLeft.Enable();
	vrConduitUpdateRight.Enable();

	//////////////////////////// TEMP RENDER LOOP


	RhinoApp().Print(L"all should be init, now call HMDRender via debug\n");

	return CRhinoCommand::success;


}

#pragma endregion

//
// END InitVR command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
