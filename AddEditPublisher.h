//---------------------------------------------------------------------------

#ifndef AddEditPublisherH
#define AddEditPublisherH
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
#include <ExtCtrls.hpp>
#include "Placemnt.hpp"
#include "JvBevel.hpp"
#include "JvExExtCtrls.hpp"
#include "SmrtPanel.h"
//#include "SmrtPanel.h"
//---------------------------------------------------------------------------
class TAddEditPublisherForm : public TForm
{
__published:	// IDE-managed Components
  TFormStorage *FormStorage;
  TPanel *workPanel;
  TLabel *Label1;
  TLabel *Label2;
  TLabel *Label3;
  TLabel *Label4;
  TLabel *Label5;
  TLabel *Label7;
  TLabel *Label8;
  TLabel *Label9;
  TLabel *Label10;
  TJvBevel *JvBevel1;
  TJvDotNetEdit *P_NameEdit;
  TJvDotNetEdit *P_OfficialNameEdit;
  TJvDotNetEdit *P_AddressEdit;
  TJvDotNetEdit *P_INN_KPPEdit;
  TJvDotNetEdit *P_BankEdit;
  TJvDotNetEdit *P_PhoneEdit;
  TJvDotNetEdit *ReasonEdit;
  TJvDotNetEdit *P_EMailEdit;
  TSmrtPanel *SmrtPanel2;
  TJvXPButton *JXPBOK;
  TJvXPButton *JXPBCancel;
  void __fastcall FormShow(TObject *Sender);
  void __fastcall JXPBCancelClick(TObject *Sender);
  void __fastcall JXPBOKClick(TObject *Sender);
private:	// User declarations
  AnsiString iniFileName;
  FBDatabase *fbDB;
  AnsiString &P_ID;
public:		// User declarations
  __fastcall TAddEditPublisherForm(TComponent* Owner,AnsiString iniFN,FBDatabase *fbDB,AnsiString &id);
};
//---------------------------------------------------------------------------
//extern PACKAGE TAddEditPublisherForm *AddEditPublisherForm;
//---------------------------------------------------------------------------
#endif
