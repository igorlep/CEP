//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "DifferentFunctions.h"
#include "CopyTemplates.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Placemnt"
#pragma link "JvComponent"
#pragma link "JvEdit"
#pragma link "JvExControls"
#pragma link "JvExStdCtrls"
#pragma link "JvSpin"
#pragma link "JvStaticText"
#pragma link "SmrtPanel"
#pragma link "CSPIN"
#pragma link "JvXPButtons"
#pragma link "JvXPCore"
#pragma link "JvExComCtrls"
#pragma link "JvProgressBar"
#pragma link "JvNavigationPane"
#pragma resource "*.dfm"
//TCopyTemplatesForm *CopyTemplatesForm;
//---------------------------------------------------------------------------
//**********
//* public *
//**********
__fastcall TCopyTemplatesForm::TCopyTemplatesForm(TComponent* Owner,AnsiString iniFN,FBDatabase *fb_db,unsigned short year,unsigned short monthIndex)
        : TForm(Owner),iniFileName(iniFN),fbDB(fb_db)
{
FBDataSet *fbDS=fbDB->getFBDS_T1_T2();
TpFIBDataSet *DS=fbDS->getDataSet();
AnsiString sql;

  FormStorage->IniFileName=iniFileName;
  FormStorage->IniSection="CopyTemplatesForm";

  toPeriodEdit->Height=21;
  toPeriodEdit->Text="";
  sql="select distinct TEMP_DATE from TEMPLATE_TABLE";
  if(fbDS->execSelectSQL("TCopyTemplatesForm::TCopyTemplatesForm",sql)==false) return;
  fromPeriodBox->Clear();
  DS->First();
  while(!DS->Eof)
  { fromPeriodBox->Items->Add(getMonthYearString(DS->FieldByName("TEMP_DATE")->AsDateTime));
    DS->Next();
  }
  if(DS->RecordCount > 0) fromPeriodBox->ItemIndex=DS->RecordCount-1;
  toPeriodEdit->Text=getMonthYearString(TDateTime(year,monthIndex,1));
  DS->Close();
}
//---------------------------------------------------------------------------
//***********
//* private *
//***********
//*************
//* published *
//*************
void __fastcall TCopyTemplatesForm::spinButtonBottomClick(
      TObject *Sender)
{
unsigned short y,m,d;

  getDateFromMonthYearString(toPeriodEdit->Text).DecodeDate(&y,&m,&d);
  m--; if(m==0){ m=12; y--;}
  toPeriodEdit->Text=getMonthYearString(TDateTime(y,m,d));
}
//---------------------------------------------------------------------------
void __fastcall TCopyTemplatesForm::spinButtonTopClick(TObject *Sender)
{
unsigned short y,m,d;

  getDateFromMonthYearString(toPeriodEdit->Text).DecodeDate(&y,&m,&d);
  m++; if(m>12){ m=1; y++;}
  toPeriodEdit->Text=getMonthYearString(TDateTime(y,m,d));
}
//---------------------------------------------------------------------------
void __fastcall TCopyTemplatesForm::cancelButtonClick(TObject *Sender)
{
  Close();
  ModalResult=mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TCopyTemplatesForm::copyButtonClick(TObject *Sender)
{
FBQuery *fbWQ=fbDB->getFBQ_T2();
TpFIBTransaction *WT=fbWQ->getTransaction();
AnsiString sql;
TDateTime fromDT=getDateFromMonthYearString(fromPeriodBox->Text),
          toDT=getDateFromMonthYearString(toPeriodEdit->Text);

  WT->StartTransaction();
  sql="select count(T_ID) as COUNT_RECORDS from TEMPLATE_TABLE where TEMP_DATE='"+toDT.DateString()+"'";
  if(fbWQ->execQuery("TCopyTemplatesForm::copyButtonClick",sql)==false)
  { WT->Rollback();
    return;
  }
  if(fbWQ->getQuery()->FieldByName("COUNT_RECORDS")->AsInteger > 0)
  { if(MessageDlg("В таблице шаблонов имеются записи за "+toPeriodEdit->Text+" года.\nДанные будут перезаписаны. Продолжить?",mtWarning,TMsgDlgButtons() << mbOK << mbCancel,0)==mrCancel)
    { fbWQ->getQuery()->Close();
      WT->Rollback();
      fbWQ->getQuery()->Close();
      return;
    }
  }
  //Удаляем старые данные
  if(fbWQ->getQuery()->FieldByName("COUNT_RECORDS")->AsInteger > 0)
  { sql="delete from TEMPLATE_TABLE where TEMP_DATE='"+toDT.DateString()+"'";
    if(fbWQ->execQuery("TCopyTemplatesForm::copyButtonClick",sql)==false)
    { WT->Rollback();
      return;
    }
  }
  //Заносим новые данные
  FBDataSet *fbDS=fbDB->getFBDS_T1_T2();
  TpFIBDataSet *DS=fbDS->getDataSet();
  sql="select t.* from TEMPLATE_TABLE t where exists";
  sql+=" (select n.nmp_id from nmp_table n where n.nmp_id=t.nmp_id and (n.del_flg is null or n.del_flg=0))";
  sql+=" and t.temp_date='"+fromDT.DateString()+"'";
  if(fbDS->execSelectSQL("TCopyTemplatesForm::copyButtonClick",sql)==false)
  { WT->Rollback();
    return;
  }
  DS->Last();
  progressBar->Max=DS->RecordCount;
  DS->First();
  progressBar->Position=0;
  GetAsyncKeyState(VK_ESCAPE);
  while(!DS->Eof)
  { if(DS->FieldByName("TEMP_Q")->AsInteger > 0)
    { sql="insert into TEMPLATE_TABLE(T_ID,NMP_ID,SD_ID,TEMP_Q,TEMP_DATE) values(";
      sql+="GEN_ID(GEN_TEMPLATE_TABLE_ID,1),";
      sql+=DS->FieldByName("NMP_ID")->AsString+",";
      sql+=DS->FieldByName("SD_ID")->AsString+",";
      sql+=DS->FieldByName("TEMP_Q")->AsString+",";
      sql+="'"+toDT.DateString()+"')";
      if(fbWQ->execQuery("",sql)==false)
      { DS->Close();
        WT->Rollback();
        return;
      }
    }
    DS->Next();
    progressBar->StepIt();
    if((GetAsyncKeyState(VK_ESCAPE) << 1)!=0)
    { MessageDlg("Прекратить обработку файла?",mtConfirmation,TMsgDlgButtons() << mbYes << mbNo,0);
      if(MessageDlg("Прекратить обработку файла?",mtConfirmation,TMsgDlgButtons() << mbYes << mbNo,0) == mrYes)
      { DS->Close();
        WT->Rollback();
        return;
      }
    }
  }
  WT->Commit();
  DS->Close();
  progressBar->Position=0;
  MessageDlg("Операция завершена!",mtInformation,TMsgDlgButtons() << mbOK,0);
  Close();
  ModalResult=mrOk;
}
//---------------------------------------------------------------------------

