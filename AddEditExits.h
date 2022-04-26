//---------------------------------------------------------------------------

#ifndef AddEditExitsH
#define AddEditExitsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "JvComponent.hpp"
#include "JvExControls.hpp"
#include "JvXPButtons.hpp"
#include "JvXPCore.hpp"
#include "Placemnt.hpp"
#include "SmrtPanel.h"
#include <ExtCtrls.hpp>
#include "ToolEdit.hpp"
#include <Mask.hpp>
#include "CurrEdit.hpp"
#include "JvBaseEdits.hpp"
#include "JvExMask.hpp"
#include "JvToolEdit.hpp"
#include <Buttons.hpp>
#include "JvEdit.hpp"
#include "JvExStdCtrls.hpp"
#include "DBClasses.h"
//---------------------------------------------------------------------------
class TAddEditExitsForm : public TForm
{
__published:	// IDE-managed Components
  TFormStorage *FormStorage;
  TPanel *workPanel;
  TLabel *Label1;
  TLabel *Label2;
  TLabel *Label3;
  TLabel *Label4;
  TLabel *Label5;
  TLabel *Label6;
  TStaticText *NMPText;
  TJvCalcEdit *E_PriceEdit;
  TJvEdit *ExitNumberEdit;
  TJvDateEdit *ExitDateEdit;
  TStaticText *E_FormatBox;
  TStaticText *E_List_QEdit;
  TSpeedButton *SelectSPButton;
  TPanel *buttonsPanel;
  TJvXPButton *JXPBOK;
  TJvXPButton *JXPBCancel;
  TLabel *Label7;
  TJvDateEdit *ReturnDateEdit;
  TLabel *Label8;
  TComboBox *NDS_StavkaEdit;
        TLabel *Label9;
        TJvDateEdit *DateReturnEdit;
  void __fastcall FormShow(TObject *Sender);
  void __fastcall JXPBCancelClick(TObject *Sender);
  void __fastcall JXPBOKClick(TObject *Sender);
  void __fastcall E_PriceEditKeyPress(TObject *Sender, char &Key);
  void __fastcall ExitNumberEditKeyPress(TObject *Sender, char &Key);
  void __fastcall SelectSPButtonClick(TObject *Sender);
  void __fastcall ExitDateEditChange(TObject *Sender);
private:	// User declarations
  AnsiString iniFileName;
  FBDatabase *fbDB;
  AnsiString &E_ID;
  AnsiString NMP_ID;
  AnsiString SP_ID;
  AnsiString nmpType;
  double nmpWeight;
public:		// User declarations
  __fastcall TAddEditExitsForm(TComponent* Owner,AnsiString iniFN,FBDatabase *fbdb,AnsiString &e_if,AnsiString nmp_id);
};
//---------------------------------------------------------------------------
//extern PACKAGE TAddEditExitsForm *AddEditExitsForm;
//---------------------------------------------------------------------------
#endif
