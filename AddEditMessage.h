//---------------------------------------------------------------------------

#ifndef AddEditMessageH
#define AddEditMessageH
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
#include "DBClasses.h"
#include "JvArrowButton.hpp"
#include "JvBevel.hpp"
#include "JvDialogs.hpp"
#include "JvDotNetControls.hpp"
#include "JvExComCtrls.hpp"
#include "JvExExtCtrls.hpp"
#include "JvExStdCtrls.hpp"
#include "JvListView.hpp"
#include "JvMemo.hpp"
#include "PLACEMNT.HPP"
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
#include <ImgList.hpp>
#include <Menus.hpp>
#include "Placemnt.hpp"
#include "JvXPCheckCtrls.hpp"
#include "Message.h"
//---------------------------------------------------------------------------
class TAddEditMessageForm : public TForm
{
__published:	// IDE-managed Components
  TPanel *buttonsPanel;
  TJvXPButton *cancelButton;
  TJvXPButton *saveButton;
  TPanel *workPanel;
  TFormStorage *FormStorage;
  TLabel *Label1;
  TJvBevel *JvBevel1;
  TLabel *Label2;
  TLabel *Label3;
  TEdit *rNameEdit;
  TEdit *rMailEdit;
  TJvArrowButton *JvArrowButton1;
  TPopupMenu *buttonMenu;
  TMenuItem *subdivisionsItem;
  TMenuItem *publishersItem;
  TLabel *Label4;
  TLabel *Label5;
  TEdit *subjectEdit;
  TJvMemo *contentMemo;
  TLabel *Label6;
  TJvDotNetListView *afLV;
  TPopupMenu *afMenu;
  TImageList *afImages;
  TMenuItem *addAttachedFileItem;
  TMenuItem *delAttachedFileItem;
  TMenuItem *clearAttachedListItem;
  TJvOpenDialog *fileOpenDialog;
  TJvXPCheckbox *delFilesCheckbox;
  TJvXPCheckbox *replyCheckbox;
  void __fastcall cancelButtonClick(TObject *Sender);
  void __fastcall subdivisionsItemClick(TObject *Sender);
  void __fastcall publishersItemClick(TObject *Sender);
  void __fastcall addAttachedFileItemClick(TObject *Sender);
  void __fastcall delAttachedFileItemClick(TObject *Sender);
  void __fastcall clearAttachedListItemClick(TObject *Sender);
  void __fastcall afMenuPopup(TObject *Sender);
  void __fastcall saveButtonClick(TObject *Sender);
private:	// User declarations
  AnsiString iniFileName;
  FBDatabase *fbDB;
  Message *mes;
//  AnsiString folderID;
  AnsiString initialDir;
  TStringList *fileNames;
//  TStringList *restoreFileNames;
  void __fastcall choiceRicipient(int choice_place);
public:		// User declarations
//  __fastcall TAddEditMessageForm(TComponent* Owner,AnsiString iniFN,FBDatabase *fbdb,Message *m);
  __fastcall TAddEditMessageForm(TComponent* Owner,AnsiString iniFN,FBDatabase *fbdb,Message *m);
  __fastcall ~TAddEditMessageForm(void);
};
//---------------------------------------------------------------------------
//extern PACKAGE TAddEditMessageForm *AddEditMessageForm;
//---------------------------------------------------------------------------
#endif
