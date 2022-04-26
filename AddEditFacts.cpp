//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "DifferentFunctions.h"
#include "SomeFunctions.h"
#include "AddEditFacts.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "JvComponent"
#pragma link "JvExControls"
#pragma link "JvXPButtons"
#pragma link "JvXPCore"
#pragma link "Placemnt"
#pragma link "SmrtPanel"
#pragma link "CurrEdit"
#pragma link "ToolEdit"
#pragma link "JvExMask"
#pragma link "JvToolEdit"
#pragma resource "*.dfm"
//TAddEditFactsForm *AddEditFactsForm;
//---------------------------------------------------------------------------
//**********
//* public *
//**********
__fastcall TAddEditFactsForm::TAddEditFactsForm(TComponent* Owner,AnsiString iniFN,FBDatabase *fbdb,AnsiString &f_id,AnsiString e_id,AnsiString nmp_id)
        : TForm(Owner),iniFileName(iniFN),fbDB(fbdb),F_ID(f_id),E_ID(e_id),NMP_ID(nmp_id)
{
AnsiString sql;
FBQuery *fbRQ=fbDB->getFBQ_T1();
TpFIBQuery *Q=fbRQ->getQuery();
FBDataSet *fbDS=fbDB->getFBDS_T1_T2();
TDataSet *DS=fbDS->getDataSet();

  FormStorage->IniFileName=iniFileName;
  FormStorage->IniSection="AddEditFactsForm";

  SD_Box->Text="";
  FactQEdit->Value=0;

  sql="select e.E_NUMBER,e.E_DATE, n.NMP_NAME from EXITS_TABLE e";
  sql+=" left outer join NMP_TABLE n on n.NMP_ID=e.NMP_ID";
  sql+=" where e.E_ID="+E_ID;
  if(fbRQ->execQuery("TAddEditFactsForm::TAddEditFactsForm",sql)==false) return;
  if(Q->RecordCount==0)
  { MessageDlg("Не найдена запись с кодом "+E_ID+" в таблице EXITS_TABLE",mtError,TMsgDlgButtons() << mbOK,0);
    return;
  }
  NMPText->Caption=Q->FieldByName("NMP_NAME")->AsString;
  e_date=Q->FieldByName("E_DATE")->AsDateTime;
  EText->Caption=Q->FieldByName("E_NUMBER")->AsString+" от "+e_date.DateString();
  Q->Close();

  SD_ID_List=NULL;

  sql="select s.SD_ID,s.SD_NAME from subdivisions_table s where not exists";
  sql+="(select SD_ID from facts_table f where E_ID="+E_ID;
  sql+=" and f.sd_id=s.sd_id) and (s.DEL_FLG is NULL or s.DEL_FLG=0)";
  sql+=" order by s.SD_NAME";
  if(fbDS->execSelectSQL("TAddEditFactsForm::TAddEditFactsForm",sql)==false) return;
  SD_ID_List=new TStringList();
  DS->First();
  while(!DS->Eof)
  { SD_Box->Items->Add(DS->FieldByName("SD_NAME")->AsString);
    SD_ID_List->Add(DS->FieldByName("SD_ID")->AsString);
    DS->Next();
  }
  DS->Close();

  if(F_ID.IsEmpty())
  {
    Caption="Новая разнарядка";
  }
  else
  { sql="select f.FACT_Q,f.SD_ID,s.SD_NAME from FACTS_TABLE f";
    sql+=" left outer join SUBDIVISIONS_TABLE s on s.SD_ID=f.SD_ID";
    sql+=" where f.FACT_ID="+F_ID;
    if(fbRQ->execQuery("TAddEditFactsForm::TAddEditFactsForm",sql)==false) return;
    if(Q->RecordCount > 0)
    { SD_Box->ItemIndex=SD_Box->Items->Add(Q->FieldByName("SD_NAME")->AsString);
      SD_ID_List->Add(Q->FieldByName("SD_ID")->AsString);
      FactQEdit->Value=Q->FieldByName("FACT_Q")->AsInteger;
    }
    Q->Close();
    Caption="Редактирование разнарядки";
  }
}
//---------------------------------------------------------------------------
//***********
//* private *
//***********
//*************
//* published *
//*************
void __fastcall TAddEditFactsForm::FormShow(TObject *Sender)
{
  FactQEdit->Height=20;
  if(F_ID.IsEmpty()) SD_Box->SetFocus();
  else FactQEdit->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TAddEditFactsForm::FormDestroy(TObject *Sender)
{
  if(SD_ID_List!=NULL) delete SD_ID_List;
}
//---------------------------------------------------------------------------
void __fastcall TAddEditFactsForm::JXPBCancelClick(TObject *Sender)
{
  Close();
  ModalResult=mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TAddEditFactsForm::JXPBOKClick(TObject *Sender)
{
  TDateTime lockDate=getConstValue(this,fbDB,"ORDERING_LOCK_DATE",0);
  if(lockDate >= e_date)
  { if(lockDate > e_date)
      MessageDlg("Дата разнарядки меньше даты запрета редактирования. Сохранение невозможно!",mtWarning,TMsgDlgButtons() << mbOK,0);
    else
      MessageDlg("Дата разнарядки равна дате запрета редактирования. Сохранение невозможно!",mtWarning,TMsgDlgButtons() << mbOK,0);
    return;
  }

  if(SD_Box->Text.IsEmpty())
  { MessageDlg("Не заполнено поле \"Подразделение\"",mtWarning,TMsgDlgButtons() << mbOK,0);
    return;
  }
  if(SD_Box->ItemIndex==-1)
  { MessageDlg("Подразделение необходимо выбирать из выпадающего списка",mtWarning,TMsgDlgButtons() << mbOK,0);
    return;
  }

  if(int(FactQEdit->Value)==0)
  { MessageDlg("\"Количество\" не может быть равно нулю",mtWarning,TMsgDlgButtons() << mbOK,0);
    return;
  }

  FBQuery *fbWQ=fbDB->getFBQ_T2();
  TpFIBTransaction *WT=fbWQ->getTransaction();
  AnsiString sql;

  WT->StartTransaction();
  //Дата списания издания
//  TDateTime ret_date=getWriteOffDate(this,fbDB,f_date,E_ID);

  if(F_ID.IsEmpty())  //создаем новую запись
  { AnsiString f_id=getNewKeyValue(fbWQ,"FACTS_TABLE");
    if(f_id.IsEmpty())
    { WT->Rollback();
      return;
    }
    sql="insert into FACTS_TABLE(FACT_ID,E_ID,NMP_ID,SD_ID,FACT_Q,LOCK_DATA) values(";
    sql+=f_id+",";
    sql+=E_ID+",";
    sql+=NMP_ID+",";
    sql+=SD_ID_List->Strings[SD_Box->ItemIndex]+",";
    sql+=FactQEdit->Text+",";
    sql+="0)";
    if(fbWQ->execQuery("TAddEditFactsForm::JXPBOKClick",sql)==false)
    { WT->Rollback();
      return;
    }
    F_ID=f_id;
  }
  else    //обновляем существующую запись
  { sql="update FACTS_TABLE set";
    sql+=" SD_ID="+SD_ID_List->Strings[SD_Box->ItemIndex];
    sql+=",FACT_Q="+FactQEdit->Text;
    sql+=",LOCK_DATA=0";
    sql+=" where FACT_ID="+F_ID;
    if(fbWQ->execQuery("TAddEditFactsForm::JXPBOKClick",sql)==false)
    { WT->Rollback();
      return;
    }
  }
  //Если подразделение - СКЛАД УФПС, сразу списываем ГЖП
  if(SD_Box->Text == "СКЛАД УФПС")
  { sql="update FACTS_TABLE set RETURN_Q="+FactQEdit->Text;
    sql+=" where FACT_ID="+F_ID;
    if(fbWQ->execQuery("TAddEditFactsForm::JXPBOKClick",sql)==false)
    { WT->Rollback();
      return;
    }
  }
  WT->Commit();
  Close();
  ModalResult=mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TAddEditFactsForm::SD_BoxKeyPress(TObject *Sender,
      char &Key)
{
  if(Key==VK_RETURN){ FactQEdit->SetFocus(); Key=0;}
}
//---------------------------------------------------------------------------

