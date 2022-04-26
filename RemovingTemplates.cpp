//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "DifferentFunctions.h"
#include "RemovingTemplates.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "JvComponent"
#pragma link "JvExControls"
#pragma link "JvXPButtons"
#pragma link "JvXPCore"
#pragma link "SmrtPanel"
#pragma link "Placemnt"
#pragma resource "*.dfm"
//TRemovingTemplatesForm *RemovingTemplatesForm;
//---------------------------------------------------------------------------
//**********
//* public *
//**********
__fastcall TRemovingTemplatesForm::TRemovingTemplatesForm(TComponent* Owner,AnsiString iniFN,FBDatabase *fb_db)
        : TForm(Owner),iniFileName(iniFN), fbDB(fb_db)
{
  FormStorage->IniFileName=iniFileName;
  FormStorage->IniSection="RemovingTemplatesForm";
  FillPeriodBox();
}
//---------------------------------------------------------------------------
//***********
//* private *
//***********
void __fastcall TRemovingTemplatesForm::FillPeriodBox(void)
{
FBDataSet *fbDS=fbDB->getFBDS_T1_T2();
TpFIBDataSet *DS=fbDS->getDataSet();
AnsiString sql;

  periodBox->Clear();
  sql="select distinct TEMP_DATE from TEMPLATE_TABLE";
  if(fbDS->execSelectSQL("TRemovingTemplatesForm::FillPeriodBox",sql)==false) return;
  DS->First();
  while(!DS->Eof)
  { periodBox->Items->Add(getMonthYearString(DS->FieldByName("TEMP_DATE")->AsDateTime));
    DS->Next();
  }
}
//---------------------------------------------------------------------------
//*************
//* published *
//*************
void __fastcall TRemovingTemplatesForm::closeButtonClick(TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------
void __fastcall TRemovingTemplatesForm::deleteButtonClick(TObject *Sender)
{
FBQuery *fbWQ=fbDB->getFBQ_T2();
TpFIBTransaction *WT=fbWQ->getTransaction();
AnsiString sql;

  int checked=0;
  for(int i=0; i<periodBox->Count; ++i) if(periodBox->Checked[i]==true) checked++;
  if(checked==0) return;

  if(MessageDlg("¬ы уверены, что хотите удалить записи из таблицы шаблонов за отмеченные периода?",mtConfirmation,TMsgDlgButtons() << mbOK << mbCancel,0)==mrCancel) return;
  for(int i=0; i<periodBox->Count; ++i)
  { if(periodBox->Checked[i]==true)
    { TDateTime td=getDateFromMonthYearString(periodBox->Items->Strings[i]);
      sql="delete from TEMPLATE_TABLE where TEMP_DATE='"+td.DateString()+"'";
      WT->StartTransaction();
      if(fbWQ->execQuery("",sql)==false)
      { WT->Rollback();
        return;
      }
      WT->Commit();
    }
  }
  FillPeriodBox();
}
//---------------------------------------------------------------------------

