//---------------------------------------------------------------------------

#ifndef SelectBaseH
#define SelectBaseH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//#include "SmrtPanel.h"
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include "Placemnt.hpp"
#include "AdjustConnection.h"
#include "FIBDatabase.hpp"
#include "pFIBDatabase.hpp"
#include "JvAnimTitle.hpp"
#include "JvComponent.hpp"
#include "FIBQuery.hpp"
#include "pFIBQuery.hpp"
#include "JvExControls.hpp"
#include "JvXPButtons.hpp"
#include "JvXPCore.hpp"
#include <ComCtrls.hpp>
#include "DBClasses.h"
//---------------------------------------------------------------------------
class TSelectBaseForm : public TForm
{
__published:	// IDE-managed Components
  TPanel *TopPanel;
  TFormStorage *FormStorage;
  TJvAnimTitle *JvAnimTitle1;
  TPanel *Panel;
  TLabel *Label4;
  TListBox *DBNamesBox;
  TJvXPButton *ConnectButton;
  TJvXPButton *AddButton;
  TJvXPButton *EditButton;
  TJvXPButton *DelButton;
  TStatusBar *StatusBar;
  TLabel *programNameLabel;
  TPanel *TPanel;
  TSplitter *Splitter1;
  void __fastcall CancelButtonClick(TObject *Sender);
  void __fastcall FormShow(TObject *Sender);
  void __fastcall DBNamesBoxClick(TObject *Sender);
  void __fastcall EditButtonClick(TObject *Sender);
  void __fastcall AddButtonClick(TObject *Sender);
  void __fastcall DelButtonClick(TObject *Sender);
  void __fastcall FormDestroy(TObject *Sender);
  void __fastcall ConnectButtonClick(TObject *Sender);
private:	// User declarations
  AnsiString iniFileName;
  AnsiString workDir;
  int DBIndex;
  bool EditFLG;
  TAdjustConnectionForm *ACF;
  SecretWord *sw;
  void __fastcall setStatusBar(void);
  void __fastcall LoadDBNamesList(void);
  bool __fastcall SaveDBNameToList(bool AddFLG);
  TStringList *DBNames;
public:		// User declarations
  __fastcall TSelectBaseForm(TComponent* Owner,AnsiString iniFN,AnsiString wd,AnsiString programName);
  AnsiString __fastcall getInitString(void);
};
//---------------------------------------------------------------------------
//extern PACKAGE TSelectBaseForm *SBF;
//---------------------------------------------------------------------------
#endif
