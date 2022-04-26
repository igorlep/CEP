//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DBClasses.h"
#include "DifferentFunctions.h"
#include "RBF.h"
#include "ReferenceBook.h"
#include "AddEditDict.h"
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
#pragma link "JvExMask"
#pragma link "JvSpin"
#pragma link "CurrEdit"
#pragma link "ToolEdit"
#pragma resource "*.dfm"
//TAddEditDictForm *AddEditDictForm;
//---------------------------------------------------------------------------
//**********
//* public *
//**********
__fastcall TAddEditDictForm::TAddEditDictForm(TComponent* Owner,AnsiString iniFN,FBDatabase *fb_DB,AnsiString &id,AnsiString periodStr)
        : TForm(Owner),iniFileName(iniFN),fbDB(fb_DB),D_ID(id)
{
FBQuery *fbRQ=fbDB->getFBQ_T1();
AnsiString sql;

  FormStorage->IniFileName=iniFileName;
  FormStorage->IniSection="AddEditDictForm";
  periodText->Caption=periodStr;

  if(!D_ID.IsEmpty() && StrToInt(D_ID)!=0) //редактрование
  { sql="select d.*,p.p_name from DICTATION_ON_PAYMENT_TABLE d,";
    sql+="PUBLISERS_TABLE p";
    sql+=" where p.P_ID=d.P_ID and d.D_ID="+D_ID;
    if(fbRQ->execQuery("TAddEditDictForm::TAddEditDictForm",sql)==false) return;
    TpFIBQuery *TQ=fbRQ->getQuery();
    P_ID=TQ->FieldByName("P_ID")->AsString;
    P_NameEdit->Text=TQ->FieldByName("P_NAME")->AsString;
    D_NumberEdit->Text=TQ->FieldByName("D_NUMBER")->AsString;
    periodText->Caption=getMonthYearString(TQ->FieldByName("D_DATE")->AsDateTime);
    baseEdit->Text=TQ->FieldByName("REASON_FOR_PAYMENT")->AsString;
    summaEdit->Text=TQ->FieldByName("D_SUMMA")->AsString;
    Caption="Редактирование издания";
  }
  else //создание нового
  {
    Caption="Новое издание";
  }
}
//---------------------------------------------------------------------------

//***********
//* private *
//***********


//*************
//* published *
//*************
void __fastcall TAddEditDictForm::JXPBCancelClick(TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------
void __fastcall TAddEditDictForm::pButtonClick(TObject *Sender)
{
TRBForm *pRB=NULL;
TRBFrame *pRBF=NULL;

AnsiString tableInfo="PUBISHERS_TABLE,Поставщики ГЖП,P_ID,P_NAME,,";
AnsiString
  fieldsInfo= "P_ID,P_ID,Код,Center,1,1,1,0,;";
  fieldsInfo+="P_NAME,P_NAME,Наименование поставщика,Left,1,1,1,0,;";
  fieldsInfo+="REASON,REASON,Основание,Left,1,1,1,0,";
AnsiString contentsInfo="";
bool tvViewAble=false,
     needsSearchingForModels=false;
AnsiString sql;

  try
  {
    pRBF=new TRBFrame(this,iniFileName,"SelectPublisherForm",fbDB,tableInfo,fieldsInfo,contentsInfo,tvViewAble,needsSearchingForModels);
  }
  catch (Exception &exc)
  {
    MessageDlg("TAddEditDictForm::pButtonClick - Ошибка создания TReferenceBookFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    goto end;
  }
  //Убираем лишние элементы меню
  pRBF->rbAddRecord->Visible=false;
  pRBF->rbEditRecord->Visible=false;
  pRBF->rbDelRecord->Visible=false;
  pRBF->rbMarkDelRecord->Visible=false;
  pRBF->rbUnMarkDelRecord->Visible=false;
  pRBF->rbMoveRecord->Visible=false;
  pRBF->rbSeparator1->Visible=false;
  pRBF->rbSaveEdit->Visible=false;
  pRBF->rbCancelEdit->Visible=false;
  pRBF->rbSeparator2->Visible=false;
  pRBF->rbUpdate->Visible=false;
  pRBF->rbViewDelRecords->Visible=false;

  sql="select P_ID,P_NAME,REASON from PUBLISHERS_TABLE where (DEL_FLG is NULL or DEL_FLG=0)";
  pRBF->getRB_GC()->loadDS(sql);

  try
  {
    pRB=new TRBForm(this,iniFileName,"SelectPublisherForm",fbDB,pRBF,"Справочник поставщиков");
  }
  catch (Exception &exc)
  {
    MessageDlg("TAddEditDictForm::pButtonClick - Ошибка создания TRBForm\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    goto end;
  }
  pRBF->setRBForm(pRB);
  if(pRB->ShowModal()==mrOk)
  { P_NameEdit->Text=pRB->getFieldDataOfSelectedRecord("P_NAME");
    P_ID=pRB->getSelectedRecordID();
    baseEdit->Text=pRB->getFieldDataOfSelectedRecord("REASON");
  }
end:
  if(pRB!=NULL) delete pRB;
  else if(pRBF!=NULL) delete pRBF;
}
//---------------------------------------------------------------------------

