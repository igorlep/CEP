//---------------------------------------------------------------------------

#ifndef AddEditTemplateH
#define AddEditTemplateH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "CurrEdit.hpp"
#include "JvComponent.hpp"
#include "JvExControls.hpp"
#include "JvXPButtons.hpp"
#include "JvXPCore.hpp"
#include "Placemnt.hpp"
#include "SmrtPanel.h"
#include "ToolEdit.hpp"
#include <ExtCtrls.hpp>
#include <Mask.hpp>
#include "DBClasses.h"
//---------------------------------------------------------------------------
class TAddEditTemplateForm : public TForm
{
__published:	// IDE-managed Components
  TSmrtPanel *SmrtPanel2;
  TJvXPButton *JXPBCancel;
  TJvXPButton *JXPBOK;
  TPanel *workPanel;
  TLabel *Label1;
  TLabel *Label2;
  TLabel *Label3;
  TStaticText *NMPText;
  TComboBox *SD_Box;
  TRxCalcEdit *TemplateQEdit;
        TFormStorage *FormStorage;
  void __fastcall FormDestroy(TObject *Sender);
  void __fastcall JXPBCancelClick(TObject *Sender);
  void __fastcall JXPBOKClick(TObject *Sender);
private:	// User declarations
  AnsiString iniFileName;
  FBDatabase *fbDB;
  AnsiString &T_ID;
  AnsiString NMP_ID;
  AnsiString SD_ID;
  TDateTime tempDate;
  TStringList *SD_ID_List;
public:		// User declarations
  __fastcall TAddEditTemplateForm(TComponent* Owner,AnsiString iniFN,FBDatabase *fb_DB,AnsiString &t_id,AnsiString nmp_id,AnsiString sd_id,TDateTime tDate);
};
//---------------------------------------------------------------------------
//extern PACKAGE TAddEditTemplateForm *AddEditTemplateForm;
//---------------------------------------------------------------------------
#endif
