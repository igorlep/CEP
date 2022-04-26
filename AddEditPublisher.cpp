//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "DBClasses.h"
#include "DifferentFunctions.h"
#include "AddEditPublisher.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "JvComponent"
#pragma link "JvDotNetControls"
#pragma link "JvEdit"
#pragma link "JvExControls"
#pragma link "JvExStdCtrls"
#pragma link "JvXPButtons"
#pragma link "JvXPCore"
#pragma link "SmrtPanel"
#pragma link "Placemnt"
#pragma link "JvBevel"
#pragma link "JvExExtCtrls"
#pragma link "SmrtPanel"
#pragma link "SmrtPanel"
#pragma resource "*.dfm"
//TAddEditPublisherForm *AddEditPublisherForm;
//---------------------------------------------------------------------------
//**********
//* public *
//**********
__fastcall TAddEditPublisherForm::TAddEditPublisherForm(TComponent* Owner,AnsiString iniFN,FBDatabase *fb_DB,AnsiString &id)
        : TForm(Owner),iniFileName(iniFN),fbDB(fb_DB),P_ID(id)
{
  FormStorage->IniFileName=iniFileName;
  FormStorage->IniSection="AddEditPublisherForm";

  if(!P_ID.IsEmpty()) //редактрование
  { FBQuery *fbRQ=fbDB->getFBQ_T1();
    AnsiString sql;
    sql="select * from PUBLISHERS_TABLE where P_ID="+P_ID;
    if(fbRQ->execQuery("TAddEditPublisherForm::TAddEditPublisherForm",sql)==false) return;
    TpFIBQuery *TQ=fbRQ->getQuery();
    P_NameEdit->Text=TQ->FieldByName("P_NAME")->AsString;
    P_OfficialNameEdit->Text=TQ->FieldByName("P_OFFICIAL_NAME")->AsString;
    P_AddressEdit->Text=TQ->FieldByName("P_ADDRESS")->AsString;
    P_INN_KPPEdit->Text=TQ->FieldByName("P_INN_KPP")->AsString;
    P_BankEdit->Text=TQ->FieldByName("P_BANK_REQUISITIONS")->AsString;
    P_PhoneEdit->Text=TQ->FieldByName("P_PHONE")->AsString;
    P_EMailEdit->Text=TQ->FieldByName("P_EMAIL")->AsString;
    ReasonEdit->Text=TQ->FieldByName("REASON")->AsString;
//    if(TQ->FieldByName("INVOICE_TYPE")->AsString=="н") ITBox->ItemIndex=0;
//    else if(TQ->FieldByName("INVOICE_TYPE")->AsString=="м") ITBox->ItemIndex=1;
//    else if(TQ->FieldByName("INVOICE_TYPE")->AsString=="а") ITBox->ItemIndex=2;
    TQ->Close();
    Caption="Редактирование поставщика";
  }
  else //создание нового
  {
    Caption="Новый поставщик";
  }
}
//---------------------------------------------------------------------------

//*************
//* published *
//*************
void __fastcall TAddEditPublisherForm::FormShow(TObject *Sender)
{
  P_NameEdit->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TAddEditPublisherForm::JXPBCancelClick(TObject *Sender)
{
  Close();
  ModalResult=mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TAddEditPublisherForm::JXPBOKClick(TObject *Sender)
{
FBQuery *fbRQ=fbDB->getFBQ_T1();
FBQuery *fbWQ=fbDB->getFBQ_T2();
TpFIBTransaction *WT=fbWQ->getTransaction();
AnsiString sql;

  if(P_NameEdit->Text.IsEmpty())
  { MessageDlg("Поле \"Краткое наименование поставщика\" должно быть заполнено!",mtWarning,TMsgDlgButtons() << mbOK,0);
    return;
  }
  sql="select P_ID from PUBLISHERS_TABLE where P_NAME='"+P_NameEdit->Text+"'";
  if(fbRQ->execQuery("TAddEditPublisherForm::JXPBOKClick",sql)==false) return;
  if(fbRQ->getQuery()->RecordCount > 0 && P_ID != fbRQ->getQuery()->FieldByName("P_ID")->AsInteger)
  { if(MessageDlg("В таблице издательств уже есть издательство с таким наименование.\nСохранить изменения?",mtWarning,TMsgDlgButtons() << mbOK << mbCancel,0)==mrCancel)
      return;
  }

  WT->StartTransaction();
  if(!P_ID.IsEmpty())
  { sql="update PUBLISHERS_TABLE set";
    sql+=" P_NAME='"+P_NameEdit->Text+"'";
    if(!P_OfficialNameEdit->Text.IsEmpty()) sql+=",P_OFFICIAL_NAME='"+P_OfficialNameEdit->Text+"'";
    else sql+=",P_OFFICIAL_NAME=NULL";
    if(!P_AddressEdit->Text.IsEmpty()) sql+=",P_ADDRESS='"+P_AddressEdit->Text+"'";
    else sql+=",P_ADDRESS=NULL";
    if(!P_INN_KPPEdit->Text.IsEmpty()) sql+=",P_INN_KPP='"+P_INN_KPPEdit->Text+"'";
    else sql+=",P_INN_KPP=NULL";
    if(!P_BankEdit->Text.IsEmpty()) sql+=",P_BANK_REQUISITIONS='"+P_BankEdit->Text+"'";
    else sql+=",P_BANK_REQUISITIONS=NULL";
    if(!P_PhoneEdit->Text.IsEmpty()) sql+=",P_PHONE='"+P_PhoneEdit->Text+"'";
    else sql+=",P_PHONE=NULL";
//    if(ITBox->ItemIndex==0) sql+=",INVOICE_TYPE='н'";
//    else if(ITBox->ItemIndex==1) sql+=",INVOICE_TYPE='м'";
//    else if(ITBox->ItemIndex==2) sql+=",INVOICE_TYPE='а'";
    if(!P_EMailEdit->Text.IsEmpty()) sql+=",P_EMAIL='"+P_EMailEdit->Text+"'";
    else sql+=",P_EMAIL=NULL";
    if(!ReasonEdit->Text.IsEmpty()) sql+=",REASON='"+ReasonEdit->Text+"'";
    else sql+=",REASON=NULL";
    sql+=" where P_ID="+P_ID;
    if(fbWQ->execQuery("TAddEditPublisherForm::JXPBOKClick",sql)==false)
    { WT->Rollback();
      return;
    }
  }
  else
  { AnsiString p_id=getNewKeyValue(fbWQ,"PUBLISHERS_TABLE");
    if(p_id.IsEmpty())
    { WT->Rollback();
      return;
    }
    sql="insert into PUBLISHERS_TABLE(P_ID,P_NAME,P_OFFICIAL_NAME,P_ADDRESS,P_INN_KPP,";
    sql+="P_BANK_REQUISITIONS,P_PHONE,P_EMAIL,REASON,DEL_FLG) values(";
    sql+=p_id+",";
    sql+="'"+P_NameEdit->Text+"',";
    if(!P_OfficialNameEdit->Text.IsEmpty()) sql+="'"+P_OfficialNameEdit->Text+"',";
    else sql+="NULL,";
    if(!P_AddressEdit->Text.IsEmpty()) sql+="'"+P_AddressEdit->Text+"',";
    else sql+="NULL,";
    if(!P_INN_KPPEdit->Text.IsEmpty()) sql+="'"+P_INN_KPPEdit->Text+"',";
    else sql+="NULL,";
    if(!P_BankEdit->Text.IsEmpty()) sql+="'"+P_BankEdit->Text+"',";
    else sql+="NULL,";
    if(!P_PhoneEdit->Text.IsEmpty()) sql+="'"+P_PhoneEdit->Text+"',";
    else sql+="NULL,";
//    if(ITBox->ItemIndex==0) sql+="'н',";
//    else if(ITBox->ItemIndex==1) sql+="'м',";
//    else if(ITBox->ItemIndex==2) sql+="'а',";
    if(!P_EMailEdit->Text.IsEmpty()) sql+="'"+P_EMailEdit->Text+"',";
    else sql+="NULL,";
    if(!ReasonEdit->Text.IsEmpty()) sql+="'"+ReasonEdit->Text+"',0)";
    else sql+="NULL,0)";
    if(fbWQ->execQuery("TAddEditPublisherForm::JXPBOKClick",sql)==false)
    { WT->Rollback();
      return;
    }
    P_ID=p_id;
  }
  WT->Commit();
  Close();
  ModalResult=mrOk;
}
//---------------------------------------------------------------------------





