//---------------------------------------------------------------------------


#ifndef RestoreDBFH
#define RestoreDBFH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "JvComponent.hpp"
#include "JvExControls.hpp"
#include "JvNavigationPane.hpp"
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TRestoreDBFrame : public TFrame
{
__published:	// IDE-managed Components
  TJvNavPanelHeader *header;
  TPanel *workPanel;
  TSplitter *vSplitter;
private:	// User declarations
  AnsiString iniFileName;
  AnsiString iniSectionName;
  class FBDatabase *fbDB;

  class TRestoreBaseFrame *restoreBaseF;

  void __fastcall loadProperties(void);
  void __fastcall saveProperties(void);
public:		// User declarations
  __fastcall TRestoreDBFrame(TComponent* Owner,AnsiString iniFN,class FBDatabase *fbfb);
  __fastcall ~TRestoreDBFrame(void);
  void __fastcall initFrame(void);
};
//---------------------------------------------------------------------------
//extern PACKAGE TRestoreDBFrame *RestoreDBFrame;
//---------------------------------------------------------------------------
#endif
