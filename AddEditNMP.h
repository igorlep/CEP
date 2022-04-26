//---------------------------------------------------------------------------

#ifndef AddEditNMPH
#define AddEditNMPH
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
#include "Placemnt.hpp"
#include "SmrtPanel.h"
#include <ExtCtrls.hpp>
#include "JvBaseEdits.hpp"
#include "JvExMask.hpp"
#include "JvToolEdit.hpp"
#include <Mask.hpp>
#include "ToolEdit.hpp"
//---------------------------------------------------------------------------
class TAddEditNMPForm : public TForm
{
__published:	// IDE-managed Components
  TFormStorage *FormStorage;
  TPanel *workPanel;
  TLabel *Label1;
  TLabel *Label2;
  TLabel *Label3;
  TLabel *Label5;
  TLabel *Label6;
  TLabel *Label7;
  TJvDotNetEdit *P_NameEdit;
  TJvDotNetEdit *NMP_NameEdit;
  TJvCalcEdit *NMP_PriceEdit;
  TComboBox *Stavka_NDSBox;
  TComboBox *NMP_TypeBox;
  TJvDotNetEdit *SaleTimeEdit;
  TSmrtPanel *SmrtPanel2;
  TJvXPButton *JXPBCancel;
  TJvXPButton *JXPBOK;
  TLabel *Label4;
  TComboBox *ITBox;
  void __fastcall FormShow(TObject *Sender);
  void __fastcall JXPBCancelClick(TObject *Sender);
  void __fastcall JXPBOKClick(TObject *Sender);
private:	// User declarations
  AnsiString iniFileName;
  FBDatabase *fbDB;
  AnsiString &NMP_ID;
  AnsiString P_ID;
public:		// User declarations
  __fastcall TAddEditNMPForm(TComponent* Owner,AnsiString iniFN,FBDatabase *fb_DB,AnsiString &id,AnsiString p_id);
};
//---------------------------------------------------------------------------
//extern PACKAGE TAddEditNMPForm *AddEditNMPForm;
//---------------------------------------------------------------------------
#endif
