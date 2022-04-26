//---------------------------------------------------------------------------

#ifndef SelectPeriodH
#define SelectPeriodH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "JvComponent.hpp"
#include "JvExControls.hpp"
#include "JvExMask.hpp"
#include "JvNavigationPane.hpp"
#include "JvSpin.hpp"
#include "JvXPButtons.hpp"
#include "JvXPCore.hpp"
#include "PLACEMNT.HPP"
#include <ExtCtrls.hpp>
#include <Mask.hpp>
#include "Placemnt.hpp"
//---------------------------------------------------------------------------
class TSelectPeriodForm : public TForm
{
__published:	// IDE-managed Components
  TFormStorage *FormStorage;
  TPanel *workPanel;
  TJvNavPanelHeader *header;
  TLabel *periodLabel;
  TComboBox *monthBox;
  TJvSpinEdit *yearEdit;
  TLabel *Label1;
  TPanel *buttonsPanel;
  TJvXPButton *JXPBOK;
  TJvXPButton *JXPBCancel;
  void __fastcall JXPBCancelClick(TObject *Sender);
  void __fastcall JXPBOKClick(TObject *Sender);
private:	// User declarations
  AnsiString iniFileName;
public:		// User declarations
  __fastcall TSelectPeriodForm(TComponent* Owner,AnsiString iniFN,AnsiString caption,int year,int monthIndex);
  int __fastcall getMonthIndex(void){ return monthBox->ItemIndex; }
  int __fastcall getYear(void){ return yearEdit->Value; }
};
//---------------------------------------------------------------------------
//extern PACKAGE TSelectPeriodForm *SelectPeriodForm;
//---------------------------------------------------------------------------
#endif
