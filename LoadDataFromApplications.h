//---------------------------------------------------------------------------

#ifndef loadDataFromApplicationsH
#define loadDataFromApplicationsH
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
#include "JvCheckListBox.hpp"
#include "JvExCheckLst.hpp"
#include "JvExComCtrls.hpp"
#include "JvExMask.hpp"
#include "JvProgressBar.hpp"
#include "JvStatusBar.hpp"
#include "JvToolEdit.hpp"
#include <CheckLst.hpp>
#include <ComCtrls.hpp>
#include <Mask.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TLoadDataFromApplicationsForm : public TForm
{
__published:	// IDE-managed Components
  TPanel *buttonsPanel;
  TJvXPButton *loadButton;
  TJvXPButton *cancelButton;
  TFormStorage *FormStorage;
  TPanel *workPanel;
  TJvStatusBar *statusBar;
  TJvProgressBar *totalBar;
  TJvProgressBar *stringsBar;
  TJvCheckListBox *checkListBox;
  TJvDirectoryEdit *directoryEdit;
  TLabel *Label1;
  TPopupMenu *PopupMenu;
  TMenuItem *checkAllItem;
  TMenuItem *uncheckAllItem;
  void __fastcall cancelButtonClick(TObject *Sender);
  void __fastcall FormShow(TObject *Sender);
  void __fastcall checkListBoxClick(TObject *Sender);
  void __fastcall directoryEditAfterDialog(TObject *Sender,AnsiString &Name,bool &Action);
  void __fastcall checkAllItemClick(TObject *Sender);
  void __fastcall uncheckAllItemClick(TObject *Sender);
  void __fastcall loadButtonClick(TObject *Sender);
private:	// User declarations
  AnsiString iniFileName;
  class FBDatabase *fbDB;
  AnsiString workDir;
  AnsiString fileMask;
  AnsiString workPath;
  void __fastcall fillCheckListBox(AnsiString fm);
public:		// User declarations
  __fastcall TLoadDataFromApplicationsForm(TComponent* Owner,AnsiString iniFN,class FBDatabase *fb_db);
};
//---------------------------------------------------------------------------
//extern PACKAGE TLoadDataFromApplicationsForm *LoadDataFromApplicationsForm;
//---------------------------------------------------------------------------
#endif
