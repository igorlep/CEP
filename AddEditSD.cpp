//---------------------------------------------------------------------------

#include <vcl.h>
#include "SDFunctions.h"
#include "RBWorkFunctions.h"
#include "DifferentFunctions.h"
#pragma hdrstop
#include "AddEditSD.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "JvComponent"
#pragma link "JvExControls"
#pragma link "JvXPButtons"
#pragma link "JvXPCore"
#pragma link "Placemnt"
#pragma link "Placemnt"
#pragma link "JvDotNetControls"
#pragma link "JvEdit"
#pragma link "JvExStdCtrls"
#pragma link "JvBaseEdits"
#pragma link "JvExMask"
#pragma link "JvToolEdit"
#pragma link "JvXPCheckCtrls"
#pragma resource "*.dfm"
//TAddEditSDForm *AddEditSDForm;
//---------------------------------------------------------------------------
//**********
//* public *
//**********
__fastcall TAddEditSDForm::TAddEditSDForm(TComponent* Owner,AnsiString iniFN,FBDatabase *fbdb,AnsiString &sd_id)
        : TForm(Owner),iniFileName(iniFN),fbDB(fbdb),SD_ID(sd_id)
{
AnsiString sql;

  FormStorage->IniFileName=iniFileName;
  FormStorage->IniSection="AddEditSDForm";

  SD_NameEdit->Text="";
  postNameEdit->Text="";
  dirNameEdit->Text="";
  SD_PlaceEdit->Text="";
  SD_AddressEdit->Text="";
  SD_EMailEdit->Text="";
  OrderNumberEdit->Text="";

  if(!SD_ID.IsEmpty()) //редактрование
  { FBQuery *fbRQ=fbDB->getFBQ_T1();
    sql="select s.*, p.PO_NAME, d.D_NAME from SUBDIVISIONS_TABLE s";
    sql+=" left outer join POST_OFFICES_TABLE p on p.PO_ID=s.PO_ID";
    sql+=" left outer join DIRECTS_TABLE d on d.D_ID=s.D_ID";
    sql+=" where SD_ID="+SD_ID;
    if(fbRQ->execQuery("TAddEditSDForm::TAddEditSDForm",sql)==false) return;
    TpFIBQuery *TQ=fbRQ->getQuery();
    SD_NameEdit->Text=TQ->FieldByName("SD_NAME")->AsString;
    SD_TypeBox->ItemIndex=TQ->FieldByName("SD_TYPE")->AsInteger-1;
    PO_ID=TQ->FieldByName("PO_ID")->AsString;
    postNameEdit->Text=TQ->FieldByName("PO_NAME")->AsString;
    D_ID=TQ->FieldByName("D_ID")->AsString;
    dirNameEdit->Text=TQ->FieldByName("D_NAME")->AsString;
    SD_PlaceEdit->Text=TQ->FieldByName("SD_PLACE")->AsString;
    SD_AddressEdit->Text=TQ->FieldByName("SD_ADDRESS")->AsString;
    SD_EMailEdit->Text=TQ->FieldByName("SD_EMAIL")->AsString;
    OrderNumberEdit->Value=TQ->FieldByName("ORDER_NUMBER")->AsInteger;
    progFLGCheckbox->Checked=TQ->FieldByName("PROG_FLG")->AsBoolean;
    TQ->Close();
    Caption="Редактирование подразделения";
  }
  else //создание нового
  { Caption="Новое подразделение";
    SD_TypeBox->ItemIndex=1;
    D_ID="";
    PO_ID="";
  }
}
//---------------------------------------------------------------------------

//***********
//* private *
//***********

//*************
//* published *
//*************
void __fastcall TAddEditSDForm::cancelButtonClick(TObject *Sender)
{
  Close();
  ModalResult=mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TAddEditSDForm::FormShow(TObject *Sender)
{
  SD_NameEdit->Height=21;
  postNameEdit->Height=21;
  dirNameEdit->Height=21;
  SD_PlaceEdit->Height=21;
  SD_AddressEdit->Height=21;
  SD_EMailEdit->Height=21;
  OrderNumberEdit->Height=21;
  SD_NameEdit->SetFocus();
}
//---------------------------------------------------------------------------
//Вызов справочника почтамтов
void __fastcall TAddEditSDForm::postButtonClick(TObject *Sender)
{
TRBForm *poRB;

  poRB=createPostOfficesRB(this,iniFileName,"PostOfficesRB",fbDB,"Справочник почтамтов");

  if(poRB!=NULL)
  { if(poRB->ShowModal()==mrOk)
    { PO_ID=poRB->getSelectedRecordID();
      postNameEdit->Text=poRB->getFieldDataOfSelectedRecord("PO_NAME");
    }
    delete poRB;
  }
}
//---------------------------------------------------------------------------
void __fastcall TAddEditSDForm::postClearButtonClick(TObject *Sender)
{
  postNameEdit->Text="";
  PO_ID="";
}
//---------------------------------------------------------------------------
//Вызов справочника перечней
void __fastcall TAddEditSDForm::directButtonClick(TObject *Sender)
{
TRBForm *dRB;

  dRB=createDirectsRB(this,iniFileName,"DirectsRB",fbDB,"Справочник перечней");
  if(dRB!=NULL)
  { if(dRB->ShowModal()==mrOk)
    { D_ID=dRB->getSelectedRecordID();
      dirNameEdit->Text=dRB->getFieldDataOfSelectedRecord("D_NAME");
    }
    delete dRB;
  }
}
//---------------------------------------------------------------------------
void __fastcall TAddEditSDForm::dirClearButtonClick(TObject *Sender)
{
  dirNameEdit->Text="";
  D_ID="";
}
//---------------------------------------------------------------------------
void __fastcall TAddEditSDForm::saveButtonClick(TObject *Sender)
{
FBQuery *fbWQ=fbDB->getFBQ_T2();
TpFIBTransaction *WT=fbWQ->getTransaction();
AnsiString sql;

  if(SD_NameEdit->Text.IsEmpty())
  { MessageDlg("Поле \"Наименование подразделения\" не может быть пустым",mtWarning,TMsgDlgButtons() << mbOK,0);
    return;
  }
  if(postNameEdit->Text.IsEmpty() && SD_TypeBox->ItemIndex!=2)
  { MessageDlg("Не указано наименование почтамта",mtWarning,TMsgDlgButtons() << mbOK,0);
    return;
  }

  WT->StartTransaction();
  if(!SD_ID.IsEmpty()) //редактируем существующую запись
  { sql="update SUBDIVISIONS_TABLE set";
    if(!D_ID.IsEmpty()) sql+=" D_ID="+D_ID;
    else sql+="NULL,";
    if(!PO_ID.IsEmpty()) sql+=" ,PO_ID="+PO_ID;
    else sql+="NULL,";
    sql+=" ,SD_PLACE='"+SD_PlaceEdit->Text+"'";
    sql+=" ,SD_NAME='"+SD_NameEdit->Text+"'";
    sql+=" ,SD_ADDRESS='"+SD_AddressEdit->Text+"'";
    sql+=" ,SD_EMAIL='"+SD_EMailEdit->Text+"'";
    int on=OrderNumberEdit->Value;
    sql+=" ,ORDER_NUMBER="+IntToStr(on);
    sql+=" ,SD_TYPE="+IntToStr(SD_TypeBox->ItemIndex+1);
    AnsiString flg;
    if(progFLGCheckbox->Checked) flg="1";
    else flg="0";
    sql+=" ,PROG_FLG="+flg;
    sql+=" where SD_ID="+SD_ID;
    if(fbWQ->execQuery("TAddEditSDForm::saveButtonClick",sql)==false)
    { WT->Rollback();
      return;
    }
  }
  else
  { AnsiString sd_id=getNewKeyValue(fbWQ, "SUBDIVISIONS_TABLE");
    if(sd_id.IsEmpty())
    { WT->Rollback();
      return;
    }
    //Добавляем подразделение
    sql="insert into SUBDIVISIONS_TABLE(SD_ID,D_ID,PO_ID,SD_PLACE,SD_NAME,\
SD_ADDRESS,SD_EMAIL,ORDER_NUMBER,SD_TYPE,PROG_FLG,DEL_FLG) values(";
    sql+=sd_id+",";
    if(!D_ID.IsEmpty()) sql+=D_ID+",";
    else sql+="NULL,";
    if(!PO_ID.IsEmpty()) sql+=PO_ID+",";
    else sql+="NULL,";
    sql+="'"+SD_PlaceEdit->Text+"',";
    sql+="'"+SD_NameEdit->Text+"',";
    sql+="'"+SD_AddressEdit->Text+"',";
    sql+="'"+SD_EMailEdit->Text+"',";
    int on=OrderNumberEdit->Value;
    sql+=IntToStr(on)+",";
    sql+=IntToStr(SD_TypeBox->ItemIndex+1)+",";
    AnsiString flg;
    if(progFLGCheckbox->Checked) flg="1";
    else flg="0";
    sql+=flg;
    sql+=",0)";
    if(fbWQ->execQuery("TAddEditSDForm::saveButtonClick",sql)==false)
    { WT->Rollback();
      return;
    }
    SD_ID=sd_id;
  }
  WT->Commit();
  Close();
  ModalResult=mrOk;
}
//---------------------------------------------------------------------------

