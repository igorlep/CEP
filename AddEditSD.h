//---------------------------------------------------------------------------

#ifndef AddEditSDH
#define AddEditSDH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "JvComponent.hpp"
#include "JvExControls.hpp"
#include "JvXPButtons.hpp"
#include "JvXPCore.hpp"
#include "DBClasses.h"
#include "PLACEMNT.HPP"
#include "Placemnt.hpp"
#include <Buttons.hpp>
#include "JvDotNetControls.hpp"
#include "JvEdit.hpp"
#include "JvExStdCtrls.hpp"
#include "JvBaseEdits.hpp"
#include "JvExMask.hpp"
#include "JvToolEdit.hpp"
#include <Mask.hpp>
#include "JvXPCheckCtrls.hpp"
//---------------------------------------------------------------------------
class TAddEditSDForm : public TForm
{
__published:	// IDE-managed Components
  TPanel *buttonsPanel;
  TPanel *workPanel;
  TFormStorage *FormStorage;
  TLabel *Label1;
  TLabel *Label2;
  TLabel *Label3;
  TLabel *Label4;
  TLabel *Label5;
  TLabel *Label6;
  TLabel *Label7;
  TLabel *Label8;
  TJvCalcEdit *OrderNumberEdit;
  TComboBox *SD_TypeBox;
  TEdit *postNameEdit;
  TEdit *dirNameEdit;
  TEdit *SD_PlaceEdit;
  TEdit *SD_AddressEdit;
  TEdit *SD_EMailEdit;
  TEdit *SD_NameEdit;
  TJvXPButton *postButton;
  TJvXPButton *postClearButton;
  TJvXPButton *directButton;
  TJvXPButton *dirClearButton;
  TJvXPButton *cancelButton;
  TJvXPButton *saveButton;
  TJvXPCheckbox *progFLGCheckbox;
  void __fastcall cancelButtonClick(TObject *Sender);
  void __fastcall FormShow(TObject *Sender);
  void __fastcall postButtonClick(TObject *Sender);
  void __fastcall postClearButtonClick(TObject *Sender);
  void __fastcall directButtonClick(TObject *Sender);
  void __fastcall dirClearButtonClick(TObject *Sender);
  void __fastcall saveButtonClick(TObject *Sender);
private:	// User declarations
  AnsiString iniFileName;
  FBDatabase *fbDB;
  AnsiString &SD_ID;
  AnsiString D_ID,PO_ID;
public:		// User declarations
  __fastcall TAddEditSDForm(TComponent* Owner,AnsiString iniFileName,FBDatabase *fbdb,AnsiString &sd_id);
};
//---------------------------------------------------------------------------
//extern PACKAGE TAddEditSDForm *AddEditSDForm;
//---------------------------------------------------------------------------
#endif
