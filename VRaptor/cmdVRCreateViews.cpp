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

static void SyncVR( CRhinoView* left, CRhinoView* right) // not calling continuously. much else to add; re:conduits ... pull from example. werk till it werks. then OVRintegration
{
	ON_wString inLoopVR;
	inLoopVR.Format(L"inLoopVR\n");
	RhinoApp().Print( inLoopVR );

	if (left && right) 
	{ // first we will make it sync the two views as in example
		ON_wString inIf;
		inIf.Format(L"inIf\n");
		RhinoApp().Print( inIf );

		const ON_3dmView& v0 = left->Viewport().View(); // get Open Nurbs view from 1st CRhinoView* object
		const ON_Viewport& vp0 = left->Viewport().VP(); // get ON viewport from 1st CRhinoView* object 

		ON_3dmView& v1 = right->Viewport().m_v; // non-const; ? this important? otherwise same structure as above
		ON_Viewport& vp1 = right->Viewport().m_v.m_vp; // m_v is settings, m_vp is projection information
		
		vp1.SetProjection( vp0.Projection() ); // so using Viewport().View() to pull from and Viewport().m_v to write to. cannot write to function, yah, duh, all these Things() are functions which return values. not things themselves.
		vp1.SetCameraLocation( vp0.CameraLocation() );
		vp1.SetCameraDirection( vp0.CameraDirection() );
		vp1.SetCameraUp( vp0.CameraUp() ); // breakpoint triggered here
		v1.SetTargetPoint( v0.TargetPoint() ); // ok so all these assignments make sense

		double fl, fr, ft, fb, fn, ff, fa; // ok dealing with frustrums now: 'Bounding Box' for rendering images. Apparently needs to be kept in sync as well
		vp1.GetFrustumAspect( fa ); // vp1 may be an error -> vp0 . this from example
		vp0.GetFrustum( &fl, &fr, &fb, &ft, &fn, &ff ); // & denotes pointer, I think, or 'disembodied' reference. so the double can be written to?
		vp1.SetFrustum( fl, fr, fb, ft, fn, ff );
		vp1.SetFrustumAspect( fa );

		left->Viewport().SetTarget( right->Viewport().Target() ); // this is confusing, though... we have been setting left to right otherwise? ? AND it works both ways!
		
		ON_wString endIf;
		endIf.Format(L"endIf\n");
		RhinoApp().Print( endIf );
	}
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

	ON_3dPoint locationL = ON_3dPoint(100.0,100.0,100.0);
	ON_3dPoint locationR = ON_3dPoint(100.0,165.1,100.0);
	ON_3dPoint targetSetup = ON_3dPoint(0,0,0);

	// ok maybe try pulling m_vp out of the default "perspective" viewport, or current, and setting it here.
	// but we're going to have to learn how to control viewport settings (all) outright regardless
	// so probably actually just try to do it from the syncviews example.

	LeftVRView->Viewport().m_v.m_vp.ChangeToPerspectiveProjection(50,true,35); // target distance, symmetricFrustrum?,lenslength
	RightVRView->Viewport().m_v.m_vp.ChangeToPerspectiveProjection(50,true,35);

	LeftVRView->Viewport().m_v.m_vp.SetCameraLocation(locationL);
	RightVRView->Viewport().m_v.m_vp.SetCameraLocation(locationR);
	
	LeftVRView->Viewport().m_v.m_vp.SetTargetPoint(targetSetup);
	RightVRView->Viewport().m_v.m_vp.SetTargetPoint(targetSetup);

	// proper: should check if 2 viewports with these stats already exist, but that's for later...
	
	// but can't seem to change the name. throws windows assertion error. wtf.

	ON_wString left;
	left.Format(L"LeftVRView\n", EnglishCommandName() );
	RhinoApp().Print( left );
	LeftVRView->Viewport().m_v.m_name = left;

	ON_wString right;
	right.Format(L"RightVRView\n", EnglishCommandName() );
	RhinoApp().Print( right );
	RightVRView->Viewport().m_v.m_name = right; 

	ON_wString SYNC;
	SYNC.Format(L"SYNCVRBEGIN\n", EnglishCommandName() );
	RhinoApp().Print( SYNC );

	SyncVR(LeftVRView, RightVRView); // ok it runs once. we should also set them up perspective & looking at 0,0

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
