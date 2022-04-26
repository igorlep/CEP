//---------------------------------------------------------------------------

#ifndef ViewNMPBySDH
#define ViewNMPBySDH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "JvComponent.hpp"
#include "JvExControls.hpp"
#include "JvNavigationPane.hpp"
#include "Placemnt.hpp"
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TViewNMPBySDForm : public TForm
{
__published:	// IDE-managed Components
  TFormStorage *FormStorage;
  TPanel *workPanel;
  TJvNavPanelHeader *header;
  TStatusBar *statusBar;
private:	// User declarations
  AnsiString iniFileName;
  AnsiString workDir;
  AnsiString iniSectionName;
  class FBDatabase *fbDB;
  int typeForm;
  AnsiString ID;
  TDateTime date1,date2;

  class TGridFrame *gf;
  
public:		// User declarations
  __fastcall TViewNMPBySDForm(TComponent* Owner,AnsiString iniFN,FBDatabase *fbdb,int type,AnsiString id,TDateTime dt1,TDateTime dt2,AnsiString nmpName);
  __fastcall ~TViewNMPBySDForm(void);
};
//---------------------------------------------------------------------------
//extern PACKAGE TViewNMPBySDForm *ViewNMPBySDForm;
//---------------------------------------------------------------------------
#endif
