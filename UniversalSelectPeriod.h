//---------------------------------------------------------------------------

#ifndef UniversalSelectPeriodH
#define UniversalSelectPeriodH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "PLACEMNT.HPP"
#include "JvComponent.hpp"
#include "JvExControls.hpp"
#include "JvXPButtons.hpp"
#include "JvXPCore.hpp"
#include "Placemnt.hpp"
#include <ExtCtrls.hpp>
#include "JvNavigationPane.hpp"
#include "CSPIN.h"
#include "JvXPCheckCtrls.hpp"
#include "TOOLEDIT.HPP"
#include <Mask.hpp>
#include "ToolEdit.hpp"
//---------------------------------------------------------------------------
class TUniversalSelectPeriodForm : public TForm
{
__published:	// IDE-managed Components
  TFormStorage *FormStorage;
  TPanel *buttonsPanel;
  TJvXPButton *JXPBOK;
  TJvXPButton *JXPBCancel;
  TPanel *workPanel;
  TJvNavPanelHeader *header;
  TRadioButton *MonthButton;
  TEdit *Month;
  TCSpinButton *MonthUpDown;
  TJvXPCheckbox *YearCheckBox;
  TRadioButton *DayButton;
  TDateEdit *Day;
  TJvXPCheckbox *MonthCheckBox;
  TRadioButton *IntButton;
  TLabel *Label1;
  TDateEdit *IntDate1;
  TLabel *Label2;
  TDateEdit *IntDate2;
  void __fastcall JXPBCancelClick(TObject *Sender);
  void __fastcall JXPBOKClick(TObject *Sender);
  void __fastcall MonthButtonClick(TObject *Sender);
  void __fastcall MonthUpDownDownClick(TObject *Sender);
  void __fastcall MonthUpDownUpClick(TObject *Sender);
  void __fastcall DayButtonClick(TObject *Sender);
  void __fastcall IntButtonClick(TObject *Sender);
  void __fastcall FormDestroy(TObject *Sender);
private:	// User declarations
  AnsiString iniFileName;
  AnsiString iniSectionName;
  TDateTime &date1,&date2;
  int MonthPosition;
public:		// User declarations
  __fastcall TUniversalSelectPeriodForm(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,AnsiString caption,TDateTime &dt1,TDateTime &dt2);
};
//---------------------------------------------------------------------------
//extern PACKAGE TUniversalSelectPeriodForm *UniversalSelectPeriodForm;
//---------------------------------------------------------------------------
#endif
