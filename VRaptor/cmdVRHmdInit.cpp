////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN VRHmdInit command
//

#include "StdAfx.h"
#include "VRaptorPlugIn.h"


// do HMDInit in it's own class; we want global access to it. include session, tracking session, updates etc...


#pragma region VRHmdInit command

class CCommandVRHMDInit : public CRhinoCommand
{
public:
	CCommandVRHMDInit() {}
	~CCommandVRHMDInit() {}
	UUID CommandUUID()
	{
		// {CD2317C9-7C38-4AD4-802F-6409B7D20303}
		static const GUID VRHMDInitCommand_UUID =
		{ 0xCD2317C9, 0x7C38, 0x4AD4, { 0x80, 0x2F, 0x64, 0x09, 0xB7, 0xD2, 0x03, 0x03 } };
		return VRHMDInitCommand_UUID;
	}
	const wchar_t* EnglishCommandName() { return L"VRHMDInit"; }
	const wchar_t* LocalCommandName() const { return L"VRHMDInit"; }
	CRhinoCommand::result RunCommand( const CRhinoCommandContext& );

public:
	CVRConduit vrConduitLeft;
	CVRConduit vrConduitRight;
};

// The one and only CCommandVRHMDInit object
static class CCommandVRHMDInit theVRHMDInitCommand;

CRhinoCommand::result CCommandVRHMDInit::RunCommand( const CRhinoCommandContext& context )
{
	
	////////////////// BEGIN VIEWS INIT
	
	AFX_MANAGE_STATE( ::RhinoApp().RhinoModuleState() ); // dunno, from example

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
	lrViews.Append(rView);

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

	//////////// HMD / OVR INIT

	bool hmdSetupSuccess = VR().HMDInit();

	/////////////////////////// PIPELINE INIT

	//lView->Redraw();
	rView->Redraw();

	//rView->RecreateHWND(); check this out when trying to do pixels

	//vrConduitLeft.Bind( *lView);
	vrConduitRight.Bind( *rView);

	///////////////////////////// HMD RENDER INIT
	//VR().HMDRenderInit();

	//////////////////////////// FIRE IN THE HOLE (right only for now)
	//vrConduitLeft.Enable();
	vrConduitRight.Enable();






	if(hmdSetupSuccess)
	{
		ON_wString prnt1;
		prnt1.Format("Successful end VRHMDInit\n");
		RhinoApp().Print(prnt1);
		return CRhinoCommand::success;
	}
	else
	{
		ON_wString prnt1;
		prnt1.Format("Unsuccessful fin to VRHMDInit\n");
		RhinoApp().Print(prnt1);
		return CRhinoCommand::failure;
	}


}

#pragma endregion

//
// END VRHMDInit command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
