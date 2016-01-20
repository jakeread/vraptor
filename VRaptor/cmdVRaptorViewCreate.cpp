/////////////////////////////////////////////////////////////////////////////
// cmdVRaptorViews.cpp : command file
//

#include "StdAfx.h"
#include "VRaptorPlugIn.h"
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN VRaptorViews command
//

#pragma region VRaptorViews command

static void VRaptorViewsUpdate(CRhinoView* left)
{
	


class CCommandVRaptorViews : public CRhinoCommand
{
public:
	CCommandVRaptorViews() {}
	~CCommandVRaptorViews() {}
	UUID CommandUUID()
	{
		// {21D6C0C9-D0E7-4905-8A63-FDF28B7DB555}
		static const GUID VRaptorViewsCommand_UUID =
		{ 0x21D6C0C9, 0xD0E7, 0x4905, { 0x8A, 0x63, 0xFD, 0xF2, 0x8B, 0x7D, 0xB5, 0x55 } };
		return VRaptorViewsCommand_UUID;
	}
	const wchar_t* EnglishCommandName() { return L"VRaptorViews"; }
	const wchar_t* LocalCommandName() const { return L"VRaptorViews"; }
	CRhinoCommand::result RunCommand( const CRhinoCommandContext& );
};

// The one and only CCommandVRaptorViews object
static class CCommandVRaptorViews theVRaptorViewsCommand;

CRhinoCommand::result CCommandVRaptorViews::RunCommand( const CRhinoCommandContext& context )
{
	ON_wString wStr;
	wStr.Format( L"Ready Set Update Views\n", EnglishCommandName() );
	RhinoApp().Print( wStr );
	return CRhinoCommand::success;
}

#pragma endregion

//
// END VRaptorViews command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
