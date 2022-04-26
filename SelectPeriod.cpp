//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "SelectPeriod.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "JvComponent"
#pragma link "JvExControls"
#pragma link "JvExMask"
#pragma link "JvNavigationPane"
#pragma link "JvSpin"
#pragma link "JvXPButtons"
#pragma link "JvXPCore"
#pragma link "Placemnt"
#pragma link "Placemnt"
#pragma resource "*.dfm"
//TSelectPeriodForm *SelectPeriodForm;
//---------------------------------------------------------------------------
__fastcall TSelectPeriodForm::TSelectPeriodForm(TComponent* Owner,AnsiString iniFN,AnsiString caption,int year,int monthIndex)
        : TForm(Owner),iniFileName(iniFN)
{
  FormStorage->IniFileName=iniFileName;
  FormStorage->IniSection="SelectPeriodForm";

  header->Caption=caption;
  yearEdit->Value=year;
  monthBox->ItemIndex=monthIndex;
}
//---------------------------------------------------------------------------
void __fastcall TSelectPeriodForm::JXPBCancelClick(TObject *Sender)
{
  Close();
  ModalResult=mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TSelectPeriodForm::JXPBOKClick(TObject *Sender)
{
  Close();
  ModalResult=mrOk;
}
//---------------------------------------------------------------------------

