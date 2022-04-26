//---------------------------------------------------------------------------

#ifndef AddEditFactsH
#define AddEditFactsH
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
#include "CurrEdit.hpp"
#include "ToolEdit.hpp"
#include <Mask.hpp>
#include "DBClasses.h"
#include "JvExMask.hpp"
#include "JvToolEdit.hpp"
//---------------------------------------------------------------------------
class TAddEditFactsForm : public TForm
{
__published:	// IDE-managed Components
  TPanel *workPanel;
  TLabel *Label1;
  TLabel *Label3;
  TLabel *Label2;
  TLabel *Label4;
  TStaticText *NMPText;
  TStaticText *EText;
  TComboBox *SD_Box;
  TRxCalcEdit *FactQEdit;
  TPanel *buttonsPanel;
  TJvXPButton *JXPBOK;
  TJvXPButton *JXPBCancel;
  TFormStorage *FormStorage;
  void __fastcall FormShow(TObject *Sender);
  void __fastcall FormDestroy(TObject *Sender);
  void __fastcall JXPBCancelClick(TObject *Sender);
  void __fastcall JXPBOKClick(TObject *Sender);
  void __fastcall SD_BoxKeyPress(TObject *Sender, char &Key);
private:	// User declarations
  AnsiString iniFileName;
  FBDatabase *fbDB;
  AnsiString &F_ID;
  AnsiString E_ID;
  AnsiString NMP_ID;
  TStringList *SD_ID_List;
  TDateTime e_date;
public:		// User declarations
  __fastcall TAddEditFactsForm(TComponent* Owner,AnsiString iniFN,FBDatabase *fbdb,AnsiString &f_id,AnsiString e_id,AnsiString nmp_id);
};
//---------------------------------------------------------------------------
//extern PACKAGE TAddEditFactsForm *AddEditFactsForm;
//---------------------------------------------------------------------------
#endif
