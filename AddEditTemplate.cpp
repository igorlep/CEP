//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "DBClasses.h"
#include "DifferentFunctions.h"
#include "AddEditTemplate.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CurrEdit"
#pragma link "JvComponent"
#pragma link "JvExControls"
#pragma link "JvXPButtons"
#pragma link "JvXPCore"
#pragma link "Placemnt"
#pragma link "SmrtPanel"
#pragma link "ToolEdit"
#pragma resource "*.dfm"
//TAddEditTemplateForm *AddEditTemplateForm;
//---------------------------------------------------------------------------
//**********
//* public *
//**********
__fastcall TAddEditTemplateForm::TAddEditTemplateForm(TComponent* Owner,AnsiString iniFN,FBDatabase *fb_DB,AnsiString &t_id,AnsiString nmp_id,AnsiString sd_id,TDateTime tDate)
        : TForm(Owner),iniFileName(iniFN),fbDB(fb_DB),T_ID(t_id),NMP_ID(nmp_id),SD_ID(sd_id),tempDate(tDate)
{
AnsiString sql;
FBQuery *fbRQ=fbDB->getFBQ_T1();

  FormStorage->IniFileName=iniFileName;
  FormStorage->IniSection="AddEditTemplateForm";

  SD_Box->Text="";
  TemplateQEdit->Value=0;

  sql="select NMP_NAME from NMP_TABLE";
  sql+=" where NMP_ID="+NMP_ID;
  if(fbRQ->execQuery("TAddEditTemplateForm::TAddEditTemplateForm",sql)==false) return;
  if(fbRQ->getQuery()->RecordCount==0)
  { MessageDlg("Не найдена запись с кодом "+NMP_ID+" в таблице NMP_TABLE",mtError,TMsgDlgButtons() << mbOK,0);
    return;
  }
  NMPText->Caption=fbRQ->getQuery()->FieldByName("NMP_NAME")->AsString;

  try
  {
    SD_ID_List=new TStringList();
  }
  catch (Exception &exc)
  { MessageDlg("Ошибка создания объекта SD_ID_List\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    SD_ID_List=NULL;
    return;
  }

  FBDataSet *fbDS=fbDB->getFBDS_T1_T2();
  sql="select * from subdivisions_table s where not exists";
  sql+="(select SD_ID from template_table t where NMP_ID="+NMP_ID;
  sql+=" and t.TEMP_DATE='"+tempDate.DateString()+"'";
  sql+=" and t.sd_id=s.sd_id) and (s.DEL_FLG is NULL or s.DEL_FLG=0)";
  sql+=" order by s.SD_NAME";
  if(fbDS->execSelectSQL("TAddEditTemplateForm::TAddEditTemplateForm",sql)==false) return;
  TDataSet *ds=fbDS->getDataSet();
  ds->First();
  while(!ds->Eof)
  { SD_Box->Items->Add(ds->FieldByName("SD_NAME")->AsString);
    SD_ID_List->Add(ds->FieldByName("SD_ID")->AsString);
    ds->Next();
  }
  ds->Close();

  if(T_ID.IsEmpty())
  {
    Caption="Новый шаблон";
  }
  else
  { sql="select t.SD_ID,t.TEMP_Q,s.SD_NAME from TEMPLATE_TABLE t";
    sql+=" left outer join SUBDIVISIONS_TABLE s on s.SD_ID=t.SD_ID";
    sql+=" where t.T_ID="+T_ID;
    if(fbRQ->execQuery("TAddEditTemplateForm::TAddEditTemplateForm",sql)==false) return;
    if(fbRQ->getQuery()->RecordCount > 0)
    { SD_Box->ItemIndex=SD_Box->Items->Add(fbRQ->getQuery()->FieldByName("SD_NAME")->AsString);
      SD_ID_List->Add(fbRQ->getQuery()->FieldByName("SD_ID")->AsString);
      TemplateQEdit->Value=fbRQ->getQuery()->FieldByName("TEMP_Q")->AsInteger;
    }
    Caption="Редактирование шаблона";
  }
  fbRQ->getQuery()->Close();
}
//---------------------------------------------------------------------------

//*************
//* published *
//*************
void __fastcall TAddEditTemplateForm::FormDestroy(TObject *Sender)
{
  if(SD_ID_List!=NULL) delete SD_ID_List;
}
//---------------------------------------------------------------------------
void __fastcall TAddEditTemplateForm::JXPBCancelClick(TObject *Sender)
{
  Close();
  ModalResult=mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TAddEditTemplateForm::JXPBOKClick(TObject *Sender)
{
FBQuery *fbWQ=fbDB->getFBQ_T2();
AnsiString sql;

  if(SD_Box->Text.IsEmpty())
  { MessageDlg("Не заполнено поле \"Подразделение\"",mtWarning,TMsgDlgButtons() << mbOK,0);
    return;
  }

  TpFIBTransaction *Transaction=fbWQ->getTransaction();
  Transaction->StartTransaction();
  if(T_ID.IsEmpty())  //создаем новую запись
  { AnsiString t_id=getNewKeyValue(fbDB,"TEMPLATE_TABLE");
    if(t_id.IsEmpty())
    { Transaction->Rollback();
      return;
    }
    sql="insert into TEMPLATE_TABLE(T_ID,NMP_ID,SD_ID,TEMP_Q,TEMP_DATE) values(";
    sql+=t_id+",";
    sql+=NMP_ID+",";
    sql+=SD_ID_List->Strings[SD_Box->ItemIndex]+",";
    sql+=TemplateQEdit->Text+",";
    sql+="'"+tempDate.DateString()+"')";
    if(fbWQ->execQuery("TAddEditTemplateForm::JXPBOKClick",sql)==false)
    { Transaction->Rollback();
      return;
    }
    T_ID=t_id;
  }
  else    //обновляем существующую запись
  { sql="update TEMPLATE_TABLE set";
    sql+=" SD_ID="+SD_ID_List->Strings[SD_Box->ItemIndex];
    sql+=",TEMP_Q="+TemplateQEdit->Text;
    sql+=" where T_ID="+T_ID;
    if(fbWQ->execQuery("TAddEditTemplateForm::JXPBOKClick",sql)==false)
    { Transaction->Rollback();
      return;
    }
  }
  Transaction->Commit();
  fbWQ->getQuery()->Close();
  Close();
  ModalResult=mrOk;
}
//---------------------------------------------------------------------------
