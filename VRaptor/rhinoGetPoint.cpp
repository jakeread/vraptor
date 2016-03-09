#include "stdafx.h"
#include "VRaptorPlugIn.h"



class CGetPoint : public CRhinoGetPoint
{
public:
	CGetPoint();
	virtual void DynamicDraw( HDC hdc, CRhinoViewport& vp, const ON_3dPoint& pt);
	int GetPoints();
	// we are here: looking at https://github.com/mcneel/Rhino5Samples_CPP/blob/6dcb38493089ae63276565e8c61655a1745fa499/SampleCommands/cmdSamplePolyline.cpp
	// in order to implement UI in VR.
};

/*
  void OnMouseMove( CRhinoViewport&, const ON_3dPoint&, const CPoint* ) {}
  void DynamicDraw( HDC, CRhinoViewport&, const ON_3dPoint& ) {}
*/
