//---------------------------------------------------------------------------

#ifndef RemovingTemplatesH
#define RemovingTemplatesH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "JvComponent.hpp"
#include "JvExControls.hpp"
#include "JvXPButtons.hpp"
#include "JvXPCore.hpp"
#include "SmrtPanel.h"
#include <ExtCtrls.hpp>
#include "Placemnt.hpp"
#include <CheckLst.hpp>
#include "DBClasses.h"
//---------------------------------------------------------------------------
class TRemovingTemplatesForm : public TForm
{
__published:	// IDE-managed Components
  TFormStorage *FormStorage;
  TPanel *workPanel;
  TCheckListBox *periodBox;
  TMemo *memo;
  TPanel *buttonsPanel;
  TJvXPButton *deleteButton;
  TJvXPButton *closeButton;
  void __fastcall closeButtonClick(TObject *Sender);
  void __fastcall deleteButtonClick(TObject *Sender);
private:	// User declarations
  AnsiString iniFileName;
  FBDatabase *fbDB;
  void __fastcall FillPeriodBox(void);
public:		// User declarations
  __fastcall TRemovingTemplatesForm(TComponent* Owner,AnsiString iniFN,FBDatabase *fb_db);
};
//---------------------------------------------------------------------------
//extern PACKAGE TRemovingTemplatesForm *RemovingTemplatesForm;
//---------------------------------------------------------------------------
#endif
