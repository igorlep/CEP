//---------------------------------------------------------------------------

#ifndef ContentConfigH
#define ContentConfigH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "DBGridEh.hpp"
#include "JvComponent.hpp"
#include "JvExControls.hpp"
#include "JvNavigationPane.hpp"
#include "JvXPButtons.hpp"
#include "JvXPCore.hpp"
#include "kbmMemTable.hpp"
#include "PLACEMNT.HPP"
#include <DB.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include "Placemnt.hpp"
#include "JvCheckListBox.hpp"
#include "JvDotNetControls.hpp"
#include "JvExCheckLst.hpp"
#include <CheckLst.hpp>
#include "JvxCheckListBox.hpp"
//---------------------------------------------------------------------------
class TContentConfigForm : public TForm
{
__published:	// IDE-managed Components
  TJvNavPanelHeader *header;
  TFormStorage *FormStorage;
  TPanel *buttonsPanel;
  TJvXPButton *SaveButton;
  TJvXPButton *CancelButton;
  TPanel *leftPanel;
  TJvNavPanelHeader *leftHeader;
  TListBox *leftBox;
  TSplitter *Splitter1;
  TPanel *butPanel;
  TJvXPButton *toRightButton;
  TJvXPButton *toLeftButton;
  TJvNavPanelHeader *centerHeader;
  TPanel *rightPanel;
  TJvNavPanelHeader *rightHeader;
  TListBox *rightBox;
  void __fastcall CancelButtonClick(TObject *Sender);
  void __fastcall SaveButtonClick(TObject *Sender);
  void __fastcall toRightButtonClick(TObject *Sender);
  void __fastcall toLeftButtonClick(TObject *Sender);
private:	// User declarations
  AnsiString iniFileName;
  AnsiString iniSectionName;
  AnsiString fieldsInfo;
  AnsiString contentsInfo;
  void __fastcall loadRightBox(void);
  void __fastcall loadLeftBox(void);
public:		// User declarations
  __fastcall TContentConfigForm(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,AnsiString tableTitle,AnsiString FI,AnsiString CI);
  __fastcall ~TContentConfigForm(void);
  AnsiString __fastcall getContentsInfo(void);
};
//---------------------------------------------------------------------------
//extern PACKAGE TContentConfigForm *ContentConfigForm;
//---------------------------------------------------------------------------
#endif
