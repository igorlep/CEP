//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "GridF.h"
//#include "GridContentF.h"
#include "DifferentFunctions.h"
#include "SomeFunctions.h"
#include "DocumentsFunctions.h"
#include "AddRecordsToRool.h"
#include "DictationOnPaymentF.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "JvComponent"
#pragma link "JvExControls"
#pragma link "JvNavigationPane"
#pragma link "JvArrowButton"
#pragma link "JvSpeedButton"
#pragma link "JvXPCheckCtrls"
#pragma link "JvXPCore"
#pragma link "JvExComCtrls"
#pragma link "JvStatusBar"
#pragma link "JvExMask"
#pragma link "JvSpin"
#pragma link "FIBDataSet"
#pragma link "pFIBDataSet"
#pragma link "frxClass"
#pragma link "frxDBSet"
#pragma link "frxClass"
#pragma link "frxDBSet"
#pragma link "DBGridEh"
#pragma link "frxExportXLS"
#pragma resource "*.dfm"
//TDictationOnPaymentFrame *DictationOnPaymentFrame;
//---------------------------------------------------------------------------
//**********
//* public *
//**********
__fastcall TDictationOnPaymentFrame::TDictationOnPaymentFrame(TComponent* Ownerr,AnsiString iniFN,FBDatabase *fbdb)
        : TFrame(Owner),iniFileName(iniFN),fbDB(fbdb)
{
  iniSectionName="DictationOnPaymentFrame";
  workDir=getAppDir();
  dFrame=NULL;
  yearEdit->Height=21;
}
//---------------------------------------------------------------------------
__fastcall TDictationOnPaymentFrame::~TDictationOnPaymentFrame(void)
{
  saveProperties();
  if(dFrame!=NULL){ delete dFrame; dFrame=NULL; }
}
//---------------------------------------------------------------------------
void __fastcall TDictationOnPaymentFrame::initFrame(void)
{
  loadProperties();
  setHeader();
  dFrame=createDictationsFrame(this,iniFileName,iniSectionName,fbDB,workPanel);
  if(dFrame!=NULL)
  { dFrame->workGrid->PopupMenu=dictationMenu;
    dFrame->getWorkDataSet()->BeforePost=dDataSetBeforePost;
    dFrame->setStatusBar(dictBar);
  }
  else return;
  loadDictDS();
}
//---------------------------------------------------------------------------

//***********
//* private *
//***********
void __fastcall TDictationOnPaymentFrame::loadProperties(void)
{
TIniFile *ini = new TIniFile(iniFileName);
unsigned short y,m,d;

  d_date=ini->ReadDate(iniSectionName,"periodDate",Date());
  delete ini;
  periodStr=getMonthYearString(d_date);
  d_date.DecodeDate(&y,&m,&d);
  monthBox->ItemIndex=m-1;
  yearEdit->Value=y;
}
//---------------------------------------------------------------------------
void __fastcall TDictationOnPaymentFrame::saveProperties(void)
{
TIniFile *ini = new TIniFile(iniFileName);

  ini->WriteDate(iniSectionName,"periodDate",d_date);
  delete ini;
}
//---------------------------------------------------------------------------
void __fastcall TDictationOnPaymentFrame::setHeader(void)
{
  header->Caption="Реестр на оплату реализованной газетно-журнальной продукции за "+periodStr;
}
//---------------------------------------------------------------------------
void __fastcall TDictationOnPaymentFrame::loadDictDS(void)
{
  if(dFrame!=NULL)
  { AnsiString sql=formDictationsSelectSQL(d_date);
    dFrame->initSelectSQL(sql);
    dFrame->loadDS();
  }
}
//---------------------------------------------------------------------------

//*************
//* published *
//*************
void __fastcall TDictationOnPaymentFrame::onChangePeriod(void)
{
  periodStr=monthBox->Text+" "+yearEdit->Text+" года";
  d_date=endOfMonth(getDateFromMonthYearString(periodStr));
  loadDictDS();
  setHeader();
}
//---------------------------------------------------------------------------
void __fastcall TDictationOnPaymentFrame::spinButtonBottomClick(
      TObject *Sender)
{
  if(monthBox->ItemIndex==0)
  { monthBox->ItemIndex=11;
    yearEdit->Value--;
  }
  else monthBox->ItemIndex--;
  onChangePeriod();
}
//---------------------------------------------------------------------------
void __fastcall TDictationOnPaymentFrame::spinButtonTopClick(
      TObject *Sender)
{
  if(monthBox->ItemIndex==11)
  { monthBox->ItemIndex=0;
    yearEdit->Value++;
  }
  else monthBox->ItemIndex++;
  onChangePeriod();
}
//---------------------------------------------------------------------------
void __fastcall TDictationOnPaymentFrame::yearEditChange(TObject *Sender)
{
  onChangePeriod();
}
//---------------------------------------------------------------------------
void __fastcall TDictationOnPaymentFrame::monthBoxChange(TObject *Sender)
{
  onChangePeriod();
}
//---------------------------------------------------------------------------
void __fastcall TDictationOnPaymentFrame::dictationMenuPopup(TObject *Sender)
{
  if(dFrame->getWorkDataSet()->RecordCount==0)
  { fillItem->Enabled=true;
    editDictItem->Enabled=false;
    delDictItem->Enabled=false;
    clearItem->Enabled=false;
    saveItem->Enabled=false;
    cancelItem->Enabled=false;
    printRoolItem->Enabled=false;
    printDictItem->Enabled=false;
  }
  else
  { fillItem->Enabled=false;
    editDictItem->Enabled=true;
    delDictItem->Enabled=true;
    clearItem->Enabled=true;
    printRoolItem->Enabled=true;
    printDictItem->Enabled=true;
    if(dFrame->getWorkDataSet()->State==dsEdit)
    { saveItem->Enabled=true;
      cancelItem->Enabled=true;
    }
    else
    { saveItem->Enabled=false;
      cancelItem->Enabled=false;
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TDictationOnPaymentFrame::dDataSetBeforePost(TDataSet *DataSet)
{
AnsiString supply_s=DataSet->FieldByName("SUPPLY_S")->AsString;
AnsiString defect_s=DataSet->FieldByName("DEFECT_S")->AsString;
AnsiString write_off_s=DataSet->FieldByName("WRITE_OFF_S")->AsString;
double payment_s=0.0;

   if(!supply_s.IsEmpty()) payment_s=StrToFloat(supply_s);
   if(!defect_s.IsEmpty()) payment_s-=StrToFloat(defect_s);
   if(!write_off_s.IsEmpty()) payment_s-=StrToFloat(write_off_s);
   DataSet->FieldByName("PAYMENT_S")->AsFloat=payment_s;
}
//---------------------------------------------------------------------------
//Заполнение реестра на оплату
void __fastcall TDictationOnPaymentFrame::fillItemClick(TObject *Sender)
{
FBQuery *fbWQ=fbDB->getFBQ_T2();
AnsiString sql;
TDateTime date1=beginOfMonth(d_date);

  if(dFrame->getWorkDataSet()->RecordCount > 0)
  { if(MessageDlg("Внимание! При заполнении реестра существующие данные будут удалены. Продолжить?",mtInformation,TMsgDlgButtons() << mbOK << mbCancel,0)==mrCancel)
      return;
  }
  sql="execute procedure LOAD_ROOL_OF_PAYMENTS(";
  sql+="'"+date1.DateString()+"','"+d_date.DateString()+"')";
  if(fbWQ->execQuery("TDictationOnPaymentFrame::fillItemClick",sql)==false) return;
  loadDictDS();
}
//---------------------------------------------------------------------------
//Добавление записи
void __fastcall TDictationOnPaymentFrame::addRecordItomClick(TObject *Sender)
{
TAddRecordsToRoolForm *AF;

  try
  {
    AF=new TAddRecordsToRoolForm(this,iniFileName,fbDB,d_date);
  }
  catch(Exception &exc)
  { MessageDlg("Ошибка создания формы TAddRecordsToRoolForm\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    goto end;
  }
  if(AF->ShowModal()==mrOk)
    loadDictDS();
end:
  if(AF!=NULL) delete AF;
}
//---------------------------------------------------------------------------
//Редактирование
void __fastcall TDictationOnPaymentFrame::editDictItemClick(TObject *Sender)
{
  if(dFrame->workSource->State!=dsEdit) dFrame->getWorkDataSet()->Edit();
}
//---------------------------------------------------------------------------
//Удаление выбранных записей
void __fastcall TDictationOnPaymentFrame::delDictItemClick(TObject *Sender)
{
TDBGridEh *grid=dFrame->workGrid;
TpFIBDataSet *ds=dFrame->getWorkDataSet();

  if(grid==NULL || ds==NULL) return;
  if(grid->SelectedRows->Count==0) grid->SelectedRows->CurrentRowSelected=true;
  AnsiString str;
  if(grid->SelectedRows->Count==1)
    str="Вы уверены, что хотите удалить текущую запись?";
  else
    str="Вы уверены, что хотите удалить выбранные записи?";
  if(MessageDlg(str,mtConfirmation,TMsgDlgButtons() << mbOK << mbCancel,0)==mrCancel) return;

  AnsiString sql;
  FBQuery *fbWQ=fbDB->getFBQ_T2();
  fbWQ->getTransaction()->StartTransaction();
  for(int i=0; i<grid->SelectedRows->Count; i++)
  { ds->GotoBookmark((void *)grid->SelectedRows->Items[i].c_str());
    sql="delete from dictation_on_payment_table where D_ID="+ds->FieldByName("D_ID")->AsString;
    if(fbWQ->execQuery("TDictationOnPaymentFrame::delDictItemClick",sql)==false)
    { fbWQ->getTransaction()->Rollback();
      return;
    }
  }
  fbWQ->getTransaction()->Commit();
  loadDictDS();
}
//---------------------------------------------------------------------------
//Очистка реестра на оплату
void __fastcall TDictationOnPaymentFrame::clearItemClick(TObject *Sender)
{
FBQuery *fbWQ=fbDB->getFBQ_T2();
AnsiString sql;

  if(dFrame->getWorkDataSet()->RecordCount == 0) return;
  if(MessageDlg("Вы уверены, что хотите удалить существующие данные реестра?",mtConfirmation,TMsgDlgButtons() << mbOK << mbCancel,0)==mrCancel)
    return;

  sql="delete from dictation_on_payment_table where d_date='"+d_date.DateString()+"'";
  if(fbWQ->execQuery("TDictationOnPaymentFrame::fillItemClick",sql)==false) return;
  loadDictDS();
}
//---------------------------------------------------------------------------
void __fastcall TDictationOnPaymentFrame::saveItemClick(TObject *Sender)
{
  if(dFrame->workSource->State==dsEdit) dFrame->getWorkDataSet()->Post();
}
//---------------------------------------------------------------------------
void __fastcall TDictationOnPaymentFrame::cancelItemClick(TObject *Sender)
{
  if(dFrame->workSource->State==dsEdit) dFrame->getWorkDataSet()->Cancel();
}
//---------------------------------------------------------------------------
//Печать реестра на оплату
void __fastcall TDictationOnPaymentFrame::printRoolItemClick(TObject *Sender)
{
AnsiString fName=workDir+"Reports\\RoolOnPaymnetsReport.fr3";

  if(frxReport->LoadFromFile(fName)==false)
  { MessageDlg("Не обнаружен файл с шаблоном отчета - "+fName,mtWarning,TMsgDlgButtons() << mbOK,0);
    return;
  }
  frxXLSExport->FileName="Реестр на оплату.xls";
  frxDS->DataSet=dFrame->getWorkDataSet();
  frxReport->PrepareReport(true);
  frxReport->ShowPreparedReport();
}
//---------------------------------------------------------------------------
//Печать распоряжения на платеж
void __fastcall TDictationOnPaymentFrame::printDictItemClick(TObject *Sender)
{
AnsiString fName=workDir+"Reports\\DictationOnPayment.fr3";

  if(frxReport->LoadFromFile(fName)==false)
  { MessageDlg("Не обнаружен файл с шаблоном отчета - "+fName,mtWarning,TMsgDlgButtons() << mbOK,0);
    return;
  }
  frxXLSExport->FileName="Распоряжение на платеж.xls";
  frxReport->PrepareReport(true);
  frxReport->ShowPreparedReport();
}
//---------------------------------------------------------------------------
void __fastcall TDictationOnPaymentFrame::frxReportGetValue(const AnsiString VarName, Variant &Value)
{
  //Реестр на оплату
  if(VarName=="Title")
  { AnsiString s="Реестр на оплату реализованной газетно-журнальной продукции за ";
    s+=periodStr;
    Value=s;
  }
  else if(VarName=="RE_JT")
  {
    Value=getConstValue(this,fbDB,"ROOL_EXPERT_JT",d_date);
  }
  else if(VarName=="RE_FIO")
  {
    Value=getConstValue(this,fbDB,"ROOL_EXPERT_FIO",d_date);
  }
  else if(VarName=="RP_FIO")
  {
    Value=getConstValue(this,fbDB,"WORKMAN_FIO",d_date);
  }
  else if(VarName=="RP_Phone")
  {
    Value=getConstValue(this,fbDB,"WORKMAN_PHONE",d_date);
  }
  //Распоряжение на платеж
  else if(VarName=="Payee") //получатель платежа
  {
    Value="Согласно реестра на оплату реализованной ГЖП за "+periodStr;
  }
  else if(VarName=="PurposeOfPayment") //назначение платежа
  {
    Value="Оплата реализованной газетно-журнальной продукции за "+periodStr;
  }
  else if(VarName=="BaseOfPayment") //основание платежа
  {
    Value="Счета согласно реестра на оплату реализованной ГЖП за "+periodStr;
  }
  else if(VarName=="Summa") //сумма платежа
  { AnsiString summa="";
    int cn=dFrame->findPozitionColumnInGrid("PAYMENT_S");
    if(cn>0) summa=dFrame->workGrid->SumList->SumCollection->Items[3]->SumValue;
    Value=summa;
  }
  else if(VarName=="SummaPropis") //сумма прописью
  { double summa=0;
    int cn=dFrame->findPozitionColumnInGrid("PAYMENT_S");
    if(cn>0) summa=dFrame->workGrid->SumList->SumCollection->Items[3]->SumValue;
    Value=f_to_str(summa);;
  }
  else if(VarName=="PerformerFIO")
  {
    Value=getConstValue(this,fbDB,"WORKMAN_FIO",d_date);
  }
  else if(VarName=="PerformerPhone")
  {
    Value=getConstValue(this,fbDB,"WORKMAN_PHONE",d_date);
  }
  else if(VarName=="DateOfPayment")
  { unsigned short y,m,d;
    d_date.DecodeDate(&y,&m,&d);
    m++; if(m>12){ m=1; y++; }
    TDateTime dt=TDateTime(y,m,25);
    Value=dt.DateString();
  }
  else if(VarName=="DivisionName1")
  {
    Value=getConstValue(this,fbDB,"DOP_M1_JT",d_date);
  }
  else if(VarName=="FIO1")
  {
    Value=getConstValue(this,fbDB,"DOP_M1_FIO",d_date);
  }
  else if(VarName=="DivisionName2")
  {
    Value=getConstValue(this,fbDB,"DOP_M2_JT",d_date);
  }
  else if(VarName=="FIO2")
  {
    Value=getConstValue(this,fbDB,"DOP_M2_FIO",d_date);
  }
  else if(VarName=="DivisionName3")
  {
    Value=getConstValue(this,fbDB,"DOP_M3_JT",d_date);
  }
  else if(VarName=="FIO3")
  {
    Value=getConstValue(this,fbDB,"DOP_M3_FIO",d_date);
  }
  else if(VarName=="Bookman")
  { unsigned short y,m,d;
    d_date.DecodeDate(&y,&m,&d);
    m++; if(m>12){ m=1; y++; }
    TDateTime dt=TDateTime(y,m,24);
    AnsiString s;
    s=dt.DateString()+", "+getConstValue(this,fbDB,"BOOKMAN_JOB_TITLE",dt);
    s+="                                           ";
    s+=getConstValue(this,fbDB,"BOOKMAN_FIO",dt);
    Value=s;
  }
}
//---------------------------------------------------------------------------

