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
					lView = tempView;

					/*
					CFrameWnd *lFrameWnd;
					lFrameWnd = lView->FloatRhinoView(true);
					HWND lHWnd = lFrameWnd->GetSafeHwnd();
					// can update one position, but when both it fails?
					
					bool setWnd = SetWindowPos(lHWnd, HWND_TOP, 0, 960/2, 960/2, 1080/2, SWP_NOACTIVATE);  // set up a window placement 
					if (setWnd == 0)
					{
						RhinoApp().Print(L"windowSet failed at lView\n");
					}
					*/

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
		for (int rl = 0; rl < 2; rl++)
		{
			
			// more properly we need a popup for our interface window & then to dumb render into
			// https://msdn.microsoft.com/en-us/library/windows/desktop/ms633545%28v=vs.85%29.aspx

			if (rl == 1) // if we do this to both the conduits break. restructuring them anyways, so onward:
			{
				CFrameWnd *rFrameWnd;
				rFrameWnd = lrViews[rl]->FloatRhinoView(true); // returns pointer to the view's parent container
				HWND rHWnd = rFrameWnd->GetSafeHwnd();

				const CWnd *cwndptr = &CWnd::wndNoTopMost;
			
				bool setWndThru = rFrameWnd->SetWindowPos(cwndptr, 960/(rl+1), 960/2, 960/2, 1080/2, SWP_NOACTIVATE);

				//bool setWndThru = lrViews[rl]->SetWindowPos(cwndptr, 960/(rl+1), 960/2, 960/2, 1080/2, SWP_SHOWWINDOW);
				if (setWndThru == 0)
				{
					RhinoApp().Print(L"windowSet failed at rView\n");
				} 
				/*
				bool setWnd = SetWindowPos(rHWnd, HWND_TOP, 960/(rl+1), 960/2, 960/2, 1080/2, SWP_NOACTIVATE);  // set up a window placement 
				if (setWnd == 0)
				{
					RhinoApp().Print(L"windowSet failed at rView\n");
				} 
				*/
			}


			ON_3dmView onView = lrViews[rl]->ActiveViewport().View();

			if(rl == 0)
				onView.m_name = L"lView";
				//lrViews[i]->MoveWindow(0,0,VR().resolution.w/2,VR().resolution.h, true);
			if(rl == 1)
				onView.m_name = L"rView";
				//lrViews[i]->MoveWindow(960,0,VR().resolution.w/2,VR().resolution.h, true);
			lrViews[rl]->ActiveViewport().SetView(onView);
			lrViews[rl]->ActiveViewport().m_v.m_vp.ChangeToPerspectiveProjection(50,true,24);
			lrViews[rl]->Redraw();
		}
	}

	VR().lView = lView; // pass views to our plugin object 
	VR().rView = rView;
	
	//////////////////////////////// END VIEWS INIT

	/////////////////////////// PIPELINE INIT

	VR().lView->Redraw();
	VR().rView->Redraw();

	VR().currentDibFile = L"D:/ppfbDib.bmp";

	VR().disableConduits = false;

	VR().InitHMD();
	
	vrConduitRenderLeft.Bind( *VR().lView);
	vrConduitRenderRight.Bind( *VR().rView); // was working without this. odd.

	//vrConduitUpdateLeft.Bind( *VR().lView);
	//vrConduitUpdateRight.Bind( *VR().rView);

	//////////////////////////// FIRE IN THE HOLE (right only for now)
	vrConduitRenderLeft.Enable();
	vrConduitRenderRight.Enable();

	//vrConduitUpdateLeft.Enable();
	//vrConduitUpdateRight.Enable();

	//////////////////////////// TEMP RENDER LOOP

	/*
	VR().conduitUpdatePointers = { &vrConduitUpdateLeft, &vrConduitUpdateRight };
	VR().conduitRenderPointers = { &vrConduitLeft, &vrConduitRight };
	*/

	RhinoApp().Print(L"all should be init, now call HMDRender via debug\n");

	VR().scaleMult = 50;

	return CRhinoCommand::success;


}

#pragma endregion

//
// END InitVR command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
