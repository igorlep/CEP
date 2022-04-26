//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "DifferentFunctions.h"
#include "SomeFunctions.h"
#include "RBWorkFunctions.h"
#include "AddEditExits.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "JvComponent"
#pragma link "JvExControls"
#pragma link "JvXPButtons"
#pragma link "JvXPCore"
#pragma link "Placemnt"
#pragma link "SmrtPanel"
#pragma link "ToolEdit"
#pragma link "CurrEdit"
#pragma link "JvBaseEdits"
#pragma link "JvExMask"
#pragma link "JvToolEdit"
#pragma link "JvEdit"
#pragma link "JvExStdCtrls"
#pragma resource "*.dfm"
//TAddEditExitsForm *AddEditExitsForm;
//---------------------------------------------------------------------------
//**********
//* public *
//**********
__fastcall TAddEditExitsForm::TAddEditExitsForm(TComponent* Owner,AnsiString iniFN,FBDatabase *fbdb,AnsiString &e_id,AnsiString nmp_id)
        : TForm(Owner),iniFileName(iniFN),fbDB(fbdb),E_ID(e_id),NMP_ID(nmp_id)
{
AnsiString sql;
FBQuery *fbRQ=fbDB->getFBQ_T1();
TpFIBQuery *Q=fbRQ->getQuery();

  FormStorage->IniFileName=iniFileName;
  FormStorage->IniSection="AddEditExitsForm_"+getUser()->getURole();

  SP_ID=0;
  if(getUser()->getURole().LowerCase()=="оператор")
  { Label7->Visible=false;
    ReturnDateEdit->Visible=false;
    Label9->Visible=false;
    DateReturnEdit->Visible=false;
    NDS_StavkaEdit->Enabled=false;
  }
  else
  { Label7->Visible=true;
    ReturnDateEdit->Visible=true;
    Label9->Visible=true;
    DateReturnEdit->Visible=true;
    NDS_StavkaEdit->Enabled=true;
  }

  sql="select NMP_NAME,NMP_TYPE,NMP_PRICE,STAVKA_NDS from NMP_TABLE";
  sql+=" where NMP_ID="+NMP_ID;
  if(fbRQ->execQuery("TAddEditExitsForm::TAddEditExitsForm",sql)==false) return;
  if(Q->RecordCount==0)
  { MessageDlg("Не найдена запись с кодом "+NMP_ID+" в таблице NMP_TABLE",mtError,TMsgDlgButtons() << mbOK,0);
    return;
  }
  NMPText->Caption=Q->FieldByName("NMP_NAME")->AsString;
  nmpType=Q->FieldByName("NMP_TYPE")->AsString;
  Q->Close();

  if(E_ID.IsEmpty())
  { Caption="Новый выпуск";
    sql="select NMP_PRICE,STAVKA_NDS from NMP_TABLE";
    sql+=" where NMP_ID="+NMP_ID;
    if(fbRQ->execQuery("TAddEditExitsForm::TAddEditExitsForm",sql)==false) return;
    E_PriceEdit->Value=Q->FieldByName("NMP_PRICE")->AsFloat;
    int i;
    for(i=0; i<NDS_StavkaEdit->Items->Count; i++)
    { if(StrToInt(NDS_StavkaEdit->Items->Strings[i])==Q->FieldByName("STAVKA_NDS")->AsInteger)
      { NDS_StavkaEdit->ItemIndex=i;
        break;
      }
    }
    if(i==NDS_StavkaEdit->Items->Count) NDS_StavkaEdit->ItemIndex=0;
  }
  else
  { sql="select e.*,sp.* from EXITS_TABLE e";
    sql+=" left outer join STANDART_PACK_TABLE sp on sp.SP_ID=e.SP_ID";
    sql+=" where E_ID="+E_ID;
    if(fbRQ->execQuery("TAddEditExitsForm::TAddEditExitsForm",sql)==false) return;
    if(Q->RecordCount > 0)
    { E_PriceEdit->Value=Q->FieldByName("E_PRICE")->AsFloat;
      int i;
      for(i=0; i<NDS_StavkaEdit->Items->Count; i++)
      { if(StrToInt(NDS_StavkaEdit->Items->Strings[i])==Q->FieldByName("NDS_STAVKA")->AsInteger)
        { NDS_StavkaEdit->ItemIndex=i;
          break;
        }
      }
      if(i==NDS_StavkaEdit->Items->Count) NDS_StavkaEdit->ItemIndex=0;
      ExitNumberEdit->Text=Q->FieldByName("E_NUMBER")->AsString;
      ExitDateEdit->Date=Q->FieldByName("E_DATE")->AsDateTime;
      ReturnDateEdit->Date=Q->FieldByName("R_DATE")->AsDateTime;
      E_FormatBox->Caption=Q->FieldByName("E_FORMAT")->AsString;
      E_List_QEdit->Caption=Q->FieldByName("E_LIST_Q")->AsString;
      nmpWeight=Q->FieldByName("NMP_WEIGHT")->AsFloat;
      SP_ID=Q->FieldByName("SP_ID")->AsString;
      DateReturnEdit->Date=Q->FieldByName("DATE_RETURN")->AsDateTime;
    }
    Q->Close();
    Caption="Редактирование выпуска";
  }
}
//---------------------------------------------------------------------------
//***********
//* private *
//***********
//*************
//* published *
//*************
void __fastcall TAddEditExitsForm::FormShow(TObject *Sender)
{
  E_PriceEdit->Height=20;
  ExitNumberEdit->Height=20;
  ExitDateEdit->Height=20;
  E_PriceEdit->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TAddEditExitsForm::JXPBCancelClick(TObject *Sender)
{
  Close();
  ModalResult=mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TAddEditExitsForm::JXPBOKClick(TObject *Sender)
{
FBQuery *fbWQ=fbDB->getFBQ_T2();
TpFIBQuery *Q=fbWQ->getQuery();
TpFIBTransaction *WT=fbWQ->getTransaction();
AnsiString sql;

  TDateTime lockDate=getConstValue(this,fbDB,"ORDERING_LOCK_DATE",0);
  TDateTime eDate=ExitDateEdit->Date;
  TDateTime rDate=ReturnDateEdit->Date;
  TDateTime date_return=getDateReturnToPublisher(DateReturnEdit->Date);
  if(int(date_return)==0) date_return=getDateReturnToPublisher(rDate);
  if(lockDate >= eDate)
  { if(eDate < lockDate)
      MessageDlg("Дата выхода меньше даты запрета редактирования. Сохранение невозможно!",mtWarning,TMsgDlgButtons() << mbOK,0);
    else
      MessageDlg("Дата выхода равна дате запрета редактирования. Сохранение невозможно!",mtWarning,TMsgDlgButtons() << mbOK,0);
    return;
  }

  if(E_PriceEdit->Text.IsEmpty())
  { MessageDlg("Не заполнено поле \"Цена\"",mtWarning,TMsgDlgButtons() << mbOK,0);
    return;
  }
  if(ExitNumberEdit->Text.IsEmpty())
  { MessageDlg("Не заполнено поле \"Номер выхода\"",mtWarning,TMsgDlgButtons() << mbOK,0);
    return;
  }
  if(int(ExitDateEdit->Date)==0)
  { MessageDlg("Не заполнено поле \"Дата выхода\"",mtWarning,TMsgDlgButtons() << mbOK,0);
    return;
  }
  if(SP_ID.IsEmpty())
  { MessageDlg("Не выбрана стандартная пачка",mtWarning,TMsgDlgButtons() << mbOK,0);
    return;
  }
  if(eDate > rDate)
  { MessageDlg("Дата выхода не может быть меньше даты возврата",mtWarning,TMsgDlgButtons() << mbOK,0);
    return;
  }

  WT->StartTransaction();
  AnsiString e_num=ExitNumberEdit->Text.Trim();
  //Проверим нет ли уже выхода с номером E_NUMBER за дату E_DATE
  sql="select E_ID from EXITS_TABLE where NMP_ID="+NMP_ID;
  sql+=" and E_NUMBER='"+e_num+"'";
  sql+=" and E_DATE='"+eDate.DateString()+"'";
  if(!E_ID.IsEmpty()) sql+=" and E_ID <> "+E_ID;
  if(fbWQ->execQuery("TAddEditExitsForm::JXPBOKClick",sql)==false) return;
  if(Q->RecordCount > 0)
  { MessageDlg("В таблице выходов уже есть запись с номером \""+e_num+"\" от "+eDate.DateString(),mtWarning,TMsgDlgButtons() << mbOK,0);
    WT->Rollback();
    return;
  }

  if(E_ID.IsEmpty())  //создаем новую запись
  { AnsiString e_id=getNewKeyValue(fbWQ,"EXITS_TABLE");
    if(e_id.IsEmpty())
    { WT->Rollback();
      return;
    }
    sql="insert into EXITS_TABLE(E_ID,NMP_ID,E_PRICE,E_NUMBER,E_DATE,SP_ID,R_DATE,\
NDS_STAVKA,DATE_RETURN,DEL_FLG) values(";
    sql+=e_id+",";
    sql+=NMP_ID+",";
    sql+=E_PriceEdit->Text+",";
    sql+="'"+e_num+"',";
    sql+="'"+eDate.DateString()+"',";
    sql+=SP_ID+",";
    sql+="'"+rDate.DateString()+"',";
    sql+=NDS_StavkaEdit->Text+",";
    sql+="'"+date_return.DateString()+"',";
    sql+="0)";
    if(fbWQ->execQuery("TAddEditExitsForm::JXPBOKClick",sql)==false)
    { WT->Rollback();
      return;
    }
    E_ID=e_id;
  }
  else    //обновляем существующую запись
  { sql="update EXITS_TABLE set";
    sql+=" NDS_STAVKA="+NDS_StavkaEdit->Text;
    sql+=",E_PRICE="+E_PriceEdit->Text;
    sql+=",E_NUMBER='"+e_num+"'";
    sql+=",E_DATE='"+eDate.DateString()+"'";
    sql+=",SP_ID="+SP_ID;
    sql+=",R_DATE='"+rDate.DateString()+"'";
    sql+=",DATE_RETURN='"+date_return.DateString()+"'";
    sql+=" where E_ID="+E_ID;
    if(fbWQ->execQuery("TAddEditExitsForm::JXPBOKClick",sql)==false)
    { WT->Rollback();
      return;
    }
  }
  WT->Commit();
  Q->Close();

  Close();
  ModalResult=mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TAddEditExitsForm::E_PriceEditKeyPress(TObject *Sender,
      char &Key)
{
  if(Key == VK_RETURN)
  { ExitNumberEdit->SetFocus();
    Key=0;
  }
}
//---------------------------------------------------------------------------
void __fastcall TAddEditExitsForm::ExitNumberEditKeyPress(TObject *Sender,
      char &Key)
{
  if(Key==VK_RETURN)
  { ExitDateEdit->SetFocus();
    Key=0;
  }
}
//---------------------------------------------------------------------------
//Работа со справочником Стандатных пачек
void __fastcall TAddEditExitsForm::SelectSPButtonClick(TObject *Sender)
{
TRBForm *spForm=createStandartPackRB(this,iniFileName,"AddEditExitsForm_spForm",fbDB,"Справочник Стандартных пачек");

  if(spForm!=NULL)
  { if(spForm->ShowModal()==mrOk)
    { if(spForm->getFieldDataOfSelectedRecord("NMP_TYPE") == nmpType)
      { E_FormatBox->Caption=spForm->getFieldDataOfSelectedRecord("E_FORMAT");
        E_List_QEdit->Caption=spForm->getFieldDataOfSelectedRecord("E_LIST_Q");
        nmpWeight=StrToFloat(spForm->getFieldDataOfSelectedRecord("NMP_WEIGHT"));
        SP_ID=spForm->getFieldDataOfSelectedRecord("SP_ID");
      }
      else MessageDlg("Тип ГЖП выбранной записи не совпадает с типом издания",mtWarning,TMsgDlgButtons() << mbOK,0);
    }
    delete spForm;
  }
}
//---------------------------------------------------------------------------
void __fastcall TAddEditExitsForm::ExitDateEditChange(TObject *Sender)
{
  ReturnDateEdit->Date=getReturnDate(this,fbDB,ExitDateEdit->Date,NMP_ID);
  DateReturnEdit->Date=getDateReturnToPublisher(ReturnDateEdit->Date);
}
//---------------------------------------------------------------------------

