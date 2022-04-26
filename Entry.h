//---------------------------------------------------------------------------

#ifndef EntryH
#define EntryH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//#include "SmrtPanel.h"
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include "JvAnimTitle.hpp"
#include "JvComponent.hpp"
#include "JvExControls.hpp"
#include "JvXPButtons.hpp"
#include "JvXPCore.hpp"
#include "JvDotNetControls.hpp"
#include "JvEdit.hpp"
#include "JvExStdCtrls.hpp"
#include "UserClass.h"
#include "PLACEMNT.HPP"
#include "Placemnt.hpp"
#include "JvXPCheckCtrls.hpp"
//---------------------------------------------------------------------------
class TEntryForm : public TForm
{
__published:	// IDE-managed Components
  TPanel *battonsPanel;
  TJvAnimTitle *AnimTitle;
  TJvXPButton *EntryButton;
  TPanel *workPanel;
  TLabel *UserLabel;
  TComboBox *UsersBox;
  TLabel *PasswordLabel;
  TEdit *PasswordEdit;
  TLabel *programNameLabel;
  TFormStorage *FormStorage;
        TJvXPCheckbox *loadingFLG;
  void __fastcall FormShow(TObject *Sender);
  void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
  void __fastcall UsersBoxChange(TObject *Sender);
  void __fastcall PasswordEditKeyPress(TObject *Sender, char &Key);
  void __fastcall FormDestroy(TObject *Sender);
  void __fastcall EntryButtonClick(TObject *Sender);
        void __fastcall loadingFLGClick(TObject *Sender);
private:	// User declarations
  AnsiString iniFileName;
  class FBDatabase *fbDB;
  FBDataSet *fbUDS;
  FBQuery *RQ, *WQ;  
  class SecretWord *sw;
  PCUser **PCU;
  int userIndex;
public:		// User declarations
  __fastcall TEntryForm(TComponent* Owner,AnsiString iniFN,AnsiString initDBString,AnsiString pName);
  PCUser * __fastcall getSelectedUser(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TEntryForm *EntryForm;
//---------------------------------------------------------------------------
#endif
