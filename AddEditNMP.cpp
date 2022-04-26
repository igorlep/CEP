//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "DBClasses.h"
#include "DifferentFunctions.h"
#include "AddEditNMP.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "JvComponent"
#pragma link "JvDotNetControls"
#pragma link "JvEdit"
#pragma link "JvExControls"
#pragma link "JvExStdCtrls"
#pragma link "JvXPButtons"
#pragma link "JvXPCore"
#pragma link "Placemnt"
#pragma link "SmrtPanel"
#pragma link "JvBaseEdits"
#pragma link "JvExMask"
#pragma link "JvToolEdit"
#pragma link "ToolEdit"
#pragma resource "*.dfm"
//TAddEditNMPForm *AddEditNMPForm;
//---------------------------------------------------------------------------
//**********
//* public *
//**********
__fastcall TAddEditNMPForm::TAddEditNMPForm(TComponent* Owner,AnsiString iniFN,FBDatabase *fb_DB,AnsiString &id,AnsiString p_id)
        : TForm(Owner),iniFileName(iniFN),fbDB(fb_DB),NMP_ID(id),P_ID(p_id)
{
FBQuery *fbRQ=fbDB->getFBQ_T1();
AnsiString sql;

  FormStorage->IniFileName=iniFileName;
  FormStorage->IniSection="AddEditNMPForm";

  sql="select P_NAME from PUBLISHERS_TABLE where P_ID="+P_ID;
  if(fbRQ->execQuery("TAddEditNMPForm::TAddEditNMPForm",sql)==false) return;
  P_NameEdit->Text=fbRQ->getQuery()->FieldByName("P_NAME")->AsString;

  if(!NMP_ID.IsEmpty()) //редактрование
  { sql="select * from NMP_TABLE where NMP_ID="+NMP_ID;
    if(fbRQ->execQuery("TAddEditNMPForm::TAddEditNMPForm",sql)==false) return;
    TpFIBQuery *TQ=fbRQ->getQuery();
    NMP_NameEdit->Text=TQ->FieldByName("NMP_NAME")->AsString;
    NMP_PriceEdit->Text=TQ->FieldByName("NMP_PRICE")->AsString;
    int i;
    for(i=0; i<3; ++i) if(Stavka_NDSBox->Items->Strings[i]==TQ->FieldByName("STAVKA_NDS")->AsString) break;
    if(i < 3) Stavka_NDSBox->ItemIndex=i;
    else Stavka_NDSBox->ItemIndex=-1;
    if(TQ->FieldByName("NMP_TYPE")->AsString=="газета") NMP_TypeBox->ItemIndex=0;
    else if(TQ->FieldByName("NMP_TYPE")->AsString=="журнал") NMP_TypeBox->ItemIndex=1;
    else NMP_TypeBox->ItemIndex=-1;
    if(!TQ->FieldByName("SALE_TIME")->IsNull) SaleTimeEdit->Text=TQ->FieldByName("SALE_TIME")->AsString;
    else SaleTimeEdit->Text="";
    if(TQ->FieldByName("INVOICE_TYPE")->AsString=="н") ITBox->ItemIndex=0;
    else if(TQ->FieldByName("INVOICE_TYPE")->AsString=="м") ITBox->ItemIndex=1;
    else if(TQ->FieldByName("INVOICE_TYPE")->AsString=="а") ITBox->ItemIndex=2;
    Caption="Редактирование издания";
  }
  else //создание нового
  {
    Caption="Новое издание";
  }
}
//---------------------------------------------------------------------------

//*************
//* published *
//*************
void __fastcall TAddEditNMPForm::FormShow(TObject *Sender)
{
  NMP_PriceEdit->Height=20;
  NMP_NameEdit->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TAddEditNMPForm::JXPBCancelClick(TObject *Sender)
{
  Close();
  ModalResult=mrCancel;
}
//---------------------------------------------------------------------------
//Сохранение результатов
void __fastcall TAddEditNMPForm::JXPBOKClick(TObject *Sender)
{
FBQuery *fbRQ=fbDB->getFBQ_T1();
FBQuery *fbWQ=fbDB->getFBQ_T2();
TpFIBTransaction *WT=fbWQ->getTransaction();
AnsiString sql;

  if(NMP_NameEdit->Text.IsEmpty())
  { MessageDlg("Поле \"Наименование издания\" должно быть заполнено!",mtWarning,TMsgDlgButtons() << mbOK,0);
    return;
  }
  sql="select NMP_ID from NMP_TABLE where NMP_NAME='"+NMP_NameEdit->Text+"'";
  if(fbRQ->execQuery("TAddEditNMPForm::JXPBOKClick",sql)==false) return;
  if(fbRQ->getQuery()->RecordCount > 0 && NMP_ID != fbRQ->getQuery()->FieldByName("NMP_ID")->AsInteger)
  { if(MessageDlg("В таблице изданий уже есть издание с таким наименование.\nСохранить изменения?",mtWarning,TMsgDlgButtons() << mbOK << mbCancel,0)==mrCancel)
      return;
  }

  WT->StartTransaction();
  if(!NMP_ID.IsEmpty())  //обновляем существующую запись
  { sql="update NMP_TABLE set";
    sql+=" NMP_NAME='"+NMP_NameEdit->Text+"'";
    sql+=",NMP_PRICE="+NMP_PriceEdit->Text;
    sql+=",STAVKA_NDS="+Stavka_NDSBox->Text;
    sql+=",NMP_TYPE='"+NMP_TypeBox->Text+"'";
    if(!SaleTimeEdit->Text.IsEmpty()) sql+=",SALE_TIME="+SaleTimeEdit->Text;
    else sql+=",SALE_TIME=0";
    if(ITBox->ItemIndex==0) sql+=",INVOICE_TYPE='н'";
    else if(ITBox->ItemIndex==1) sql+=",INVOICE_TYPE='м'";
    else if(ITBox->ItemIndex==2) sql+=",INVOICE_TYPE='а'";
    sql+=" where NMP_ID="+NMP_ID;
    if(fbWQ->execQuery("TAddEditNMPForm::JXPBOKClick",sql)==false)
    { WT->Rollback();
      return;
    }
  }
  else  //создаем новую запись
  { AnsiString nmp_id=getNewKeyValue(fbWQ,"NMP_TABLE");
    if(nmp_id.IsEmpty())
    { WT->Rollback();
      return;
    }
    sql="insert into NMP_TABLE(NMP_ID,P_ID,NMP_NAME,NMP_PRICE,STAVKA_NDS,\
NMP_TYPE,SALE_TIME,INVOICE_TYPE,COMPRISE_OF_TEMPLATE) values(";
    sql+=nmp_id+",";
    sql+=P_ID+",";
    sql+="'"+NMP_NameEdit->Text+"',";
    sql+=NMP_PriceEdit->Text+",";
    sql+=Stavka_NDSBox->Text+",";
    sql+="'"+NMP_TypeBox->Text+"',";
    if(SaleTimeEdit->Text.IsEmpty()) sql+="0,";
    else sql+=SaleTimeEdit->Text+",";
    if(ITBox->ItemIndex==0) sql+="'н',";
    else if(ITBox->ItemIndex==1) sql+="'м',";
    else if(ITBox->ItemIndex==2) sql+="'а',";
    sql+="1)";
    if(fbWQ->execQuery("TAddEditNMPForm::JXPBOKClick",sql)==false)
    { WT->Rollback();
      return;
    }
    NMP_ID=nmp_id;
  }
  WT->Commit();
  Close();
  ModalResult=mrOk;
}
//---------------------------------------------------------------------------

