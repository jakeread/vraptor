/////////////////////////////////////////////////////////////////////////////
// rhinoEscapeKey.cpp : looks through messages for an escape, used to cancel long processes...
//

#include "StdAfx.h"
#include "VRaptorPlugIn.h"

bool CRhinoEscapeKey::m_escape_pressed = false;
HHOOK CRhinoEscapeKey::m_KeyboardHookProc = NULL;

CRhinoEscapeKey::CRhinoEscapeKey( bool bStartNow )
{
  if( bStartNow )
    Start();
}

CRhinoEscapeKey::~CRhinoEscapeKey()
{
  Stop();
}

bool CRhinoEscapeKey::Start()
{
  if( NULL == m_KeyboardHookProc )
    m_KeyboardHookProc = ::SetWindowsHookEx(
                              WH_KEYBOARD,
                              CRhinoEscapeKey::HookProc,
                              RhinoApp().RhinoInstanceHandle(),
                              ::AfxGetThread()->m_nThreadID
                              );
  ClearEscapeKeyPressedFlag();
  return( NULL != m_KeyboardHookProc );
}

void CRhinoEscapeKey::Stop()
{
  if( m_KeyboardHookProc )
    UnhookWindowsHookEx( m_KeyboardHookProc );
  m_KeyboardHookProc = NULL;
}

bool CRhinoEscapeKey::EscapeKeyPressed() const
{
  RhinoApp().Wait(0);
  return m_escape_pressed;
}

void CRhinoEscapeKey::ClearEscapeKeyPressedFlag()
{
  m_escape_pressed = false;
}

LRESULT CALLBACK CRhinoEscapeKey::HookProc( int code, WPARAM wParam, LPARAM lParam )
{
  // On escape key down....
  if( code == HC_ACTION && wParam == VK_ESCAPE && !(lParam & 0x80000000) )
  {
    m_escape_pressed = true;
    UnhookWindowsHookEx( m_KeyboardHookProc );
    m_KeyboardHookProc = NULL;
    return 0; // Eat the escape key
  }
  // call next hook proc including standard windows proc.
  return CallNextHookEx( m_KeyboardHookProc, code, wParam, lParam );
}