#pragma once

class CRhinoEscapeKey
{
public:
  CRhinoEscapeKey( bool bHookNow = false );
  ~CRhinoEscapeKey();
  bool Start();
  void Stop();
  bool EscapeKeyPressed() const;
  void ClearEscapeKeyPressedFlag();
protected:
  static LRESULT CALLBACK HookProc( int code, WPARAM wParam, LPARAM lParam );
  static HHOOK m_KeyboardHookProc;
  static bool m_escape_pressed;
};

#pragma endregion