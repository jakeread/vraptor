/////////////////////////////////////////////////////////////////////////////
// cmdVRaptor.cpp : command file
//

#include "StdAfx.h"
#include "VRaptorPlugIn.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN VRaptor command
//

#pragma region VRaptor command


// Do NOT put the definition of class CCommandVRaptor in a header
// file.  There is only ONE instance of a CCommandVRaptor class
// and that instance is the static theVRaptorCommand that appears
// immediately below the class definition.

class CCommandVRaptor : public CRhinoCommand
{
public:
  // The one and only instance of CCommandVRaptor is created below.
  // No copy constructor or operator= is required.  Values of
  // member variables persist for the duration of the application.

  // CCommandVRaptor::CCommandVRaptor()
  // is called exactly once when static theVRaptorCommand is created.
	CCommandVRaptor() {}

  // CCommandVRaptor::~CCommandVRaptor()
  // is called exactly once when static theVRaptorCommand is
  // destroyed.  The destructor should not make any calls to
  // the Rhino SDK.  If your command has persistent settings,
  // then override CRhinoCommand::SaveProfile and CRhinoCommand::LoadProfile.
  ~CCommandVRaptor() {}

  // Returns a unique UUID for this command.
  // If you try to use an id that is already being used, then
  // your command will not work.  Use GUIDGEN.EXE to make unique UUID.
	UUID CommandUUID()
	{
		// {FF3DD782-890F-4B30-9087-46D4EFB70052}
    static const GUID VRaptorCommand_UUID =
    { 0xFF3DD782, 0x890F, 0x4B30, { 0x90, 0x87, 0x46, 0xD4, 0xEF, 0xB7, 0x0, 0x52 } };
    return VRaptorCommand_UUID;
	}

  // Returns the English command name.
	const wchar_t* EnglishCommandName() { return L"VRaptor"; }

  // Returns the localized command name.
	const wchar_t* LocalCommandName() const { return L"VRaptor"; }

  // Rhino calls RunCommand to run the command.
	CRhinoCommand::result RunCommand( const CRhinoCommandContext& );
};

// The one and only CCommandVRaptor object.  
// Do NOT create any other instance of a CCommandVRaptor class.
static class CCommandVRaptor theVRaptorCommand;

CRhinoCommand::result CCommandVRaptor::RunCommand( const CRhinoCommandContext& context )
{
  // CCommandVRaptor::RunCommand() is called when the user runs the "VRaptor"
  // command or the "VRaptor" command is run by a history operation.

  // TODO: Add command code here.

  // Rhino command that display a dialog box interface should also support
  // a command-line, or scriptable interface.

  ON_wString wStr;
  wStr.Format( L"The raptor is sleeping. Git update from Laptop.\n", EnglishCommandName() );
  if( context.IsInteractive() )
    RhinoMessageBox( wStr, PlugIn()->PlugInName(), MB_OK );
  else
	  RhinoApp().Print( wStr );

  // TODO: Return one of the following values:
  //   CRhinoCommand::success:  The command worked.
  //   CRhinoCommand::failure:  The command failed because of invalid input, inability
  //                            to compute the desired result, or some other reason
  //                            computation reason.
  //   CRhinoCommand::cancel:   The user interactively canceled the command 
  //                            (by pressing ESCAPE, clicking a CANCEL button, etc.)
  //                            in a Get operation, dialog, time consuming computation, etc.

  return CRhinoCommand::success;
}

#pragma endregion

//
// END VRaptor command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
