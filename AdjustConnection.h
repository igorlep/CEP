//---------------------------------------------------------------------------

#ifndef AdjustConnectionH
#define AdjustConnectionH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "PLACEMNT.HPP"
#include "Placemnt.hpp"
#include <ExtCtrls.hpp>
#include <Dialogs.hpp>
#include <Buttons.hpp>
//#include "SmrtPanel.h"
#include "JvComponent.hpp"
#include "JvExControls.hpp"
#include "JvXPButtons.hpp"
#include "JvXPCore.hpp"
#include "DBClasses.h"
//---------------------------------------------------------------------------
class TAdjustConnectionForm : public TForm
{
__published:	// IDE-managed Components
  TFormStorage *FormStorage;
  TPanel *buttonsPanel;
  TOpenDialog *OpenDialog;
  TPanel *topPanel;
  TLabel *Label5;
  TEdit *aliasEdit;
  TJvXPButton *SaveButton;
  TJvXPButton *CancelButton;
  TPanel *workPanel;
  TGroupBox *GroupBox1;
  TLabel *Label2;
  TLabel *Label4;
  TRadioButton *localRadioButton;
  TRadioButton *remoteRadioButton;
  TComboBox *serverNameBox;
  TComboBox *protocolBox;
  TLabel *Label3;
  TComboBox *dbFullNameBox;
  TJvXPButton *selectDBButton;
  TLabel *Label6;
  TEdit *userNameEdit;
  TLabel *Label7;
  TEdit *userPasswordEdit;
  void __fastcall FormClose(TObject *Sender,TCloseAction &Action);
  void __fastcall FormShow(TObject *Sender);
  void __fastcall CancelButtonClick(TObject *Sender);
  void __fastcall SaveButtonClick(TObject *Sender);
  void __fastcall selectDBButtonClick(TObject *Sender);
  void __fastcall userPasswordEditKeyPress(TObject *Sender,char &Key);
private:	// User declarations
  AnsiString iniFileName;
  AnsiString workDir;
  AnsiString initString;
  AnsiString userPas;
  SecretWord *sw;
  void __fastcall analysesInitString(void);
public:		// User declarations
  __fastcall TAdjustConnectionForm(TComponent* Owner,AnsiString iniFN,AnsiString workDir,AnsiString initStr);
  __fastcall ~TAdjustConnectionForm(void);

};
//---------------------------------------------------------------------------
//extern PACKAGE TAdjustConnectionForm *AdjustConnectionForm;
//---------------------------------------------------------------------------
#endif
