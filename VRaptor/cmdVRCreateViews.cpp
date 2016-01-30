/////////////////////////////////////////////////////////////////////////////
// cmdVRCreateViewSample.cpp : command file
//

#include "StdAfx.h"
#include "VRaptorPlugIn.h"
#include <iostream>


////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN VRCreateViews command
//

static void SyncVR( CRhinoView* lView, CRhinoView* rView) // not calling continuously. much else to add; re:conduits ... pull from example. werk till it werks. then OVRintegration
{

	if (lView && rView) 
	{ // first we will make it sync the two views as in example

		ON_wString inIf;
		inIf.Format(L"inIf\n");
		RhinoApp().Print( inIf );

		const ON_3dmView& v0 = lView->Viewport().View(); // get Open Nurbs view from 1st CRhinoView* object
		const ON_Viewport& vp0 = lView->Viewport().VP(); // get ON viewport from 1st CRhinoView* object 

		ON_3dmView& v1 = rView->Viewport().m_v; // non-const; ? this important? otherwise same structure as above
		ON_Viewport& vp1 = rView->Viewport().m_v.m_vp; // m_v is settings, m_vp is projection information
		
		vp1.SetProjection( vp0.Projection() ); // so using Viewport().View() to pull from and Viewport().m_v to write to. cannot write to function, yah, duh, all these Things() are functions which return values. not things themselves.
		vp1.SetCameraLocation( vp0.CameraLocation() );
		vp1.SetCameraDirection( vp0.CameraDirection() );
		vp1.SetCameraUp( vp0.CameraUp() ); // breakpoint triggered here
		// v1.SetTargetPoint( v0.TargetPoint() ); // ok so all these assignments make sense

		double fl, fr, ft, fb, fn, ff, fa; // ok dealing with frustrums now: 'Bounding Box' for rendering images. Apparently needs to be kept in sync as well
		vp1.GetFrustumAspect( fa ); // vp1 may be an error -> vp0 . this from example
		vp0.GetFrustum( &fl, &fr, &fb, &ft, &fn, &ff ); // & denotes pointer, I think, or 'disembodied' reference. so the double can be written to?
		vp1.SetFrustum( fl, fr, fb, ft, fn, ff );
		vp1.SetFrustumAspect( fa );

		lView->Viewport().SetTarget( rView->Viewport().Target() ); // this is confusing, though... we have been setting left to right otherwise? ? AND it works both ways!
		
	}
} // need to setup so this calls everytime at beginning of pipeline

class VRSyncViewsConduit: public CRhinoDisplayConduit // class def ?
{
public:
	VRSyncViewsConduit();

	bool ExecConduit(
		CRhinoDisplayPipeline&, // pipeline executing this conduit
		UINT,					// current channel in pipeline
		bool&					// channel termination flag
		);						// this all from cmbSampleSyncViews.cpp in rhino sdk / examples c++ github

	void NotifyConduit(
		EConduitNotifiers,		// event reported by the display pipeline
		CRhinoDisplayPipeline&	// display pipeling calling this conduit
		);

public:
	CRhinoView* m_pView1; // truly lost; idk what / why is declared here
	CRhinoView* m_pView2;
	HWND m_hWnd1;
	HWND m_hWnd2;
	bool m_bDirty1;
	bool m_bDirty2;
};

VRSyncViewsConduit::VRSyncViewsConduit() // class constructor ?
	: CRhinoDisplayConduit(	CSupportChannels::SC_INITFRAMEBUFFER )
{
	// init all public vars as in class def
	m_pView1 = 0;
	m_pView2 = 0;
	m_hWnd1 = 0;
	m_hWnd2 = 0;
	m_bDirty1 = false;
	m_bDirty2 = false;
}

void VRSyncViewsConduit::NotifyConduit( EConduitNotifiers Notify, CRhinoDisplayPipeline& dp)
{

	if ( (m_pView1 && m_pView1->DisplayPipeline() == 0) ||  // if views empty then bail
		(m_pView2 && m_pView2->DisplayPipeline() == 0 ) )
	{
		m_pView1 = 0;
		m_pView2 = 0;
		Disable();
		return;
	}

	switch( Notify )
	{

		case CN_PROJECTIONCHANGED:
		{
			CRhinoView* pActiveView = ::RhinoApp().ActiveView(); // pick up currently active view
			if (pActiveView) // check again if exists lol wut
			{
				if ( (pActiveView == m_pView1) && (m_pView == m_pView1) ) // if active is the one we are syncing with if m_pView ? where u declare eh?
				{
					m_hWnd1 = m_pView1->m_hWnd; // look up m_hWnd, m_pView, m_bDirty. Rhino native types.
					SyncVR( m_pView1, m_pView2); // or l to r
					m_bDirty1 = true;			// m_pView "The view that the conduit is working with at the time of ExecConduit"
					m_bDirty2 = false;			// m_hWnd is a mystery.
				}								// so is m_bDirty
				else if ( (pActiveView == m_pView2) && (m_pView == m_pView2) )
				{
					m_hWnd2 = m_pView2->m_hWnd;
					SyncVR( m_pView2, m_pView1); // or r to l
					m_bDirty2 = true;
					m_bDirty1 = false;
				}
			}
			break;
		}

		case CN_PIPELINECLOSED:
		{
			if (m_bDirty1 && !m_bDirty2) // if true & false
			{
				m_bDirty1 = false;
				CClientDC dc( m_pView2 );
				if (m_pView2->DisplayPipeline()->DrawFrameBuffer(m_pView2->DisplayAttributes()) )
					m_pView2->DisplayPipeline()->ShowFrameBuffer( &dc ); // truly idk
			}
			else if ( !m_bDirty1 && m_bDirty2 )
			{
				m_bDirty2 = false;
		        CClientDC dc( m_pView1 );
			    if( m_pView1->DisplayPipeline()->DrawFrameBuffer(m_pView1->DisplayAttributes()) ) // breakpoint here on launch command
					m_pView1->DisplayPipeline()->ShowFrameBuffer( &dc );
			}
			else
			{
				m_bDirty1 = m_bDirty2 = false;
			}
			break;
		}
	}
}

bool VRSyncViewsConduit::ExecConduit( CRhinoDisplayPipeline& dp, UINT nChannel, bool& bTerminate)
{
	switch( nChannel )
	{
	case CSupportChannels::SC_INITFRAMEBUFFER:
		break;
	}
	return true;
}

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

public:
	VRSyncViewsConduit vrConduit;
};

// The one and only CCommandVRCreateViews object
static class CCommandVRCreateViews theVRCreateViewsCommand;

CRhinoCommand::result CCommandVRCreateViews::RunCommand( const CRhinoCommandContext& context )
{
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
	context.m_doc.NewView( ON_3dmView() ); // we will build two

	// find viewport UUID just created
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

	// init points
	ON_3dPoint locationL = ON_3dPoint(100.0,100.0,100.0);
	ON_3dPoint locationR = ON_3dPoint(100.0,165.1,100.0);
	ON_3dPoint targetSetup = ON_3dPoint(0,0,0);

	if (lView && rView)
	{
		for (int i = 0; i < 2; i++)
		{
			ON_3dmView onView = lrViews[i]->ActiveViewport().View();
			if(i == 0)
				onView.m_name = L"lView";
			if(i == 1)
				onView.m_name = L"rView";
			lrViews[i]->ActiveViewport().SetView(onView);
			lrViews[i]->ActiveViewport().m_v.m_vp.ChangeToPerspectiveProjection(50,true,35);
			lrViews[i]->ActiveViewport().m_v.m_vp.SetCameraLocation(locationL);
			// lrViews[i]->ActiveViewport().m_v.m_vp.SetTargetPoint(targetSetup); // obvs something is up with setting the target. throws math errors. dir needed perhaps
			lrViews[i]->Redraw();
		}
	}


	ON_wString SYNC;
	SYNC.Format(L"SYNCVRBEGIN\n", EnglishCommandName() );
	RhinoApp().Print( SYNC );

	if (vrConduit.IsEnabled()
	&& ::IsWindow( vrConduit.m_hWnd1 )
	&& ::IsWindow( vrConduit.m_hWnd2 ) ) // if is already enabled ?
	{
		vrConduit.m_pView1 = 0;
		vrConduit.m_pView2 = 0;
		vrConduit.Disable();
	}
	else
	{
		vrConduit.m_pView1 = lView;
		vrConduit.m_pView2 = rView;
		vrConduit.m_hWnd1 = vrConduit.m_pView1->m_hWnd;
		vrConduit.m_hWnd2 = vrConduit.m_pView2->m_hWnd;

		SyncVR(lView, rView); // ok it runs once. we should also set them up perspective & looking at 0,0

		vrConduit.Bind( *lView );
		vrConduit.Bind( *rView );

		lView->Redraw();
		rView->Redraw();
		
		vrConduit.Enable();
	}

	// but do not update names immediately; have to refresh somehow

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
