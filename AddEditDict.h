//---------------------------------------------------------------------------

#ifndef AddEditDictH
#define AddEditDictH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "JvComponent.hpp"
#include "JvDotNetControls.hpp"
#include "JvEdit.hpp"
#include "JvExControls.hpp"
#include "JvExStdCtrls.hpp"
#include "JvXPButtons.hpp"
#include "JvXPCore.hpp"
#include "SmrtPanel.h"
#include <ExtCtrls.hpp>
#include "Placemnt.hpp"
#include "JvExMask.hpp"
#include "JvSpin.hpp"
#include <Mask.hpp>
#include "CurrEdit.hpp"
#include "ToolEdit.hpp"
//---------------------------------------------------------------------------
class TAddEditDictForm : public TForm
{
__published:	// IDE-managed Components
  TSmrtPanel *buttonsPanel;
  TJvXPButton *JXPBCancel;
  TJvXPButton *JXPBOK;
  TPanel *workPanel;
  TLabel *pubLabel;
  TJvDotNetEdit *P_NameEdit;
  TFormStorage *FormStorage;
  TJvXPButton *pButton;
  TLabel *dnumberLabel;
  TPanel *dPanel;
  TJvDotNetEdit *D_NumberEdit;
  TLabel *pnumberLabel;
  TLabel *periodLabel;
  TLabel *baseLabel;
  TJvDotNetEdit *baseEdit;
  TLabel *summaLabel;
        TStaticText *periodText;
        TRxCalcEdit *summaEdit;
  void __fastcall JXPBCancelClick(TObject *Sender);
  void __fastcall pButtonClick(TObject *Sender);
private:	// User declarations
  AnsiString iniFileName;
  class FBDatabase *fbDB;
  AnsiString &D_ID;
  AnsiString P_ID;
public:		// User declarations
  __fastcall TAddEditDictForm(TComponent* Owner,AnsiString iniFN,FBDatabase *fb_DB,AnsiString &id,AnsiString periodStr);
};
//---------------------------------------------------------------------------
//extern PACKAGE TAddEditDictForm *AddEditDictForm;
//---------------------------------------------------------------------------
#endif
