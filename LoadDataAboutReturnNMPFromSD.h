//---------------------------------------------------------------------------

#ifndef LoadDataAboutReturnNMPFromSDH
#define LoadDataAboutReturnNMPFromSDH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "JvComponent.hpp"
#include "JvExControls.hpp"
#include "JvXPButtons.hpp"
#include "JvXPCore.hpp"
#include <ExtCtrls.hpp>
#include "Placemnt.hpp"
#include "JvExMask.hpp"
#include "JvToolEdit.hpp"
#include <Mask.hpp>
#include "JvCheckListBox.hpp"
#include "JvExCheckLst.hpp"
#include <CheckLst.hpp>
#include "JvExComCtrls.hpp"
#include "JvStatusBar.hpp"
#include <ComCtrls.hpp>
#include <Menus.hpp>
#include "JvProgressBar.hpp"
//---------------------------------------------------------------------------
class TLoadDataAboutReturnNMPFromSDForm : public TForm
{
__published:	// IDE-managed Components
  TFormStorage *FormStorage;
  TPanel *buttonsPanel;
  TJvXPButton *loadButton;
  TJvXPButton *cancelButton;
  TPanel *workPanel;
  TLabel *Label1;
  TJvCheckListBox *checkListBox;
  TJvStatusBar *statusBar;
  TJvDirectoryEdit *directoryEdit;
  TPopupMenu *PopupMenu;
  TMenuItem *checkAllItem;
  TMenuItem *uncheckAllItem;
  TJvProgressBar *totalBar;
  TJvProgressBar *stringsBar;
  void __fastcall FormShow(TObject *Sender);
  void __fastcall cancelButtonClick(TObject *Sender);
  void __fastcall directoryEditAfterDialog(TObject *Sender,AnsiString &Name, bool &Action);
  void __fastcall checkListBoxClick(TObject *Sender);
  void __fastcall checkAllItemClick(TObject *Sender);
  void __fastcall uncheckAllItemClick(TObject *Sender);
  void __fastcall loadButtonClick(TObject *Sender);
private:	// User declarations
  AnsiString iniFileName;
  AnsiString workDir;
  AnsiString fileMask;
  AnsiString workPath;
  class FBDatabase *fbDB;
  void __fastcall fillCheckListBox(AnsiString fm);
public:		// User declarations
  __fastcall TLoadDataAboutReturnNMPFromSDForm(TComponent* Owner,AnsiString iniFN,FBDatabase *fbdb);
};
//---------------------------------------------------------------------------
//extern PACKAGE TLoadDataAboutReturnNMPFromSDForm *LoadDataAboutReturnNMPFromSDForm;
//---------------------------------------------------------------------------
#endif
