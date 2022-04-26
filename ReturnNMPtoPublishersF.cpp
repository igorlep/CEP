//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "GridF.h"
#include "GridContentF.h"
#include "DifferentFunctions.h"
#include "SomeFunctions.h"
#include "UniversalSelectPeriod.h"
#include "SpisanieNMPF.h"
#include "VozvratNMPF.h"
#include "ReturnNMPtoPublishersFunctions.h"
#include "ReturnNMPtoPublishersF.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "JvArrowButton"
#pragma link "JvComponent"
#pragma link "JvExComCtrls"
#pragma link "JvExControls"
#pragma link "JvNavigationPane"
#pragma link "JvSpeedButton"
#pragma link "JvStatusBar"
#pragma link "JvXPCheckCtrls"
#pragma link "JvXPCore"
#pragma link "frxClass"
#pragma link "frxDBSet"
#pragma link "frxExportRTF"
#pragma link "frxExportXLS"
#pragma link "JvExMask"
#pragma link "JvSpin"
#pragma resource "*.dfm"
//TReturnNMPtoPublishersFrame *ReturnNMPtoPublishersFrame;
//---------------------------------------------------------------------------
//**********
//* public *
//**********
__fastcall TReturnNMPtoPublishersFrame::TReturnNMPtoPublishersFrame(TComponent* Owner,AnsiString iniFN,FBDatabase *fbdb)
        : TFrame(Owner),iniFileName(iniFN),fbDB(fbdb)
{
  workDir=getAppDir();
  iniSectionName="ReturnNMPtoPublishersFrame";
  pFrame=NULL;
  nmpFrame=NULL;
  nmpF=NULL;
}
//---------------------------------------------------------------------------
__fastcall TReturnNMPtoPublishersFrame::~TReturnNMPtoPublishersFrame(void)
{
  saveProperties();
  if(pFrame!=NULL){ delete pFrame; pFrame=NULL; }
  if(nmpFrame!=NULL){ delete nmpFrame; nmpFrame=NULL; }
}
//---------------------------------------------------------------------------
void __fastcall TReturnNMPtoPublishersFrame::initFrame(void)
{
  loadProperties();
  setHeader();
  //Фрейм поставщиков
  pFrame=createPublisherSelectFrame(this,iniFileName,iniSectionName+"_pFrame",fbDB,pPanel);
  if(pFrame!=NULL)
  { pFrame->getGridFrame()->workSource->OnDataChange=pSourceDataChange;
    pFrame->setHeader(pHeader);
    pFrame->getGridFrame()->workGrid->OnMouseDown=pGridMouseDown;
  }
  else return;
  //Фрейм ГЖП
  nmpF=NULL;
  nmpFrame=createReturnNMPFrame(this,iniFileName,iniSectionName,fbDB,nmpPanel,typeComboBox->ItemIndex);
  if(nmpFrame==NULL) return;

  loadPublishersDS();
//  loadNMPDS();
//  setNMPHeader();
  setFocus();
}
//---------------------------------------------------------------------------
AnsiString __fastcall TReturnNMPtoPublishersFrame::getPublisherID(void)
{
  if(pFrame!=NULL && pFrame->getGridFrame()->getWorkDataSet()->Active && pFrame->getGridFrame()->getWorkDataSet()->RecordCount > 0)
    return pFrame->getGridFrame()->getSelectedRecordID();
  else return "";
}
//---------------------------------------------------------------------------
TDBGridEh *  __fastcall TReturnNMPtoPublishersFrame::getPGrid(void)
{
  if(pFrame!=NULL) return pFrame->getGridFrame()->workGrid;
  else return NULL;
}
//---------------------------------------------------------------------------
TDBGridEh *  __fastcall TReturnNMPtoPublishersFrame::getNMPGrid(void)
{
  if(nmpF!=NULL) return nmpF->workGrid;
  else return NULL;
}
//---------------------------------------------------------------------------
void __fastcall TReturnNMPtoPublishersFrame::reloadP_DS(void)
{
//AnsiString p_id=getPublisherID();
//  pFrame->getGridFrame()->workSource->OnDataChange=NULL;
  loadPublishersDS();
//  pFrame->getGridFrame()->getWorkDataSet()->Locate("P_ID",p_id,TLocateOptions().Clear());
//  pFrame->getGridFrame()->workSource->OnDataChange=pSourceDataChange;
  setFocus();
}
//---------------------------------------------------------------------------
AnsiString __fastcall TReturnNMPtoPublishersFrame::getRicipientID(void)
{
  return pFrame->getGridFrame()->getFieldDataOfSelectedRecord("P_ID");
}
//---------------------------------------------------------------------------
AnsiString __fastcall TReturnNMPtoPublishersFrame::getRicipientName(void)
{
  return pFrame->getGridFrame()->getFieldDataOfSelectedRecord("P_NAME");
}
//---------------------------------------------------------------------------
AnsiString __fastcall TReturnNMPtoPublishersFrame::getRicipientMail(void)
{
  return pFrame->getGridFrame()->getFieldDataOfSelectedRecord("P_EMAIL");
}
//---------------------------------------------------------------------------
TpFIBDataSet *__fastcall TReturnNMPtoPublishersFrame::getRDS(void)
{
  if(typeComboBox->ItemIndex==0) return static_cast<TSpisanieNMPFrame*>(nmpFrame)->getWorkDataSet();
  else return static_cast<TVozvratNMPFrame*>(nmpFrame)->getWorkDataSet();
}
//---------------------------------------------------------------------------
TDBGridEh *  __fastcall TReturnNMPtoPublishersFrame::getRGrid(void)
{
  if(typeComboBox->ItemIndex==0) return static_cast<TSpisanieNMPFrame*>(nmpFrame)->getGridFrame()->workGrid;
  else return static_cast<TVozvratNMPFrame*>(nmpFrame)->getGridFrame()->workGrid;
}
//---------------------------------------------------------------------------

//***********
//* private *
//***********
void __fastcall TReturnNMPtoPublishersFrame::loadProperties(void)
{
TIniFile *ini = new TIniFile(iniFileName);

  leftPanel->Width=ini->ReadInteger(iniSectionName,"leftPanel_Width",250);
  typeComboBox->ItemIndex=ini->ReadInteger(iniSectionName,"typeComboBox_itemIndex",0);
  selectForPublisherBox->Checked=ini->ReadBool(iniSectionName,"selectForPublisherBox_Checked",false);
  monthBox->ItemIndex=ini->ReadInteger(iniSectionName,"month",0);
  yearEdit->Value=ini->ReadInteger(iniSectionName,"year",2010);
  activeFrame=ini->ReadInteger(iniSectionName,"activeFrame",1);
  delete ini;
  onChangePeriod(this);
}
//---------------------------------------------------------------------------
void __fastcall TReturnNMPtoPublishersFrame::saveProperties(void)
{
TIniFile *ini = new TIniFile(iniFileName);

  ini->WriteInteger(iniSectionName,"leftPanel_Width",leftPanel->Width);
  ini->WriteInteger(iniSectionName,"typeComboBox_itemIndex",typeComboBox->ItemIndex);
  ini->WriteBool(iniSectionName,"selectForPublisherBox_Checked",selectForPublisherBox->Checked);
  ini->WriteInteger(iniSectionName,"month",monthBox->ItemIndex);
  ini->WriteInteger(iniSectionName,"year",yearEdit->Value);
  ini->WriteInteger(iniSectionName,"activeFrame",activeFrame);
  delete ini;
}
//---------------------------------------------------------------------------
void __fastcall TReturnNMPtoPublishersFrame::setHeader(void)
{
  header->Caption="Возврат нереализованной газетно-журнальной продукции поставщикам за "+getMonthYearString(date1);
}
//---------------------------------------------------------------------------
void __fastcall TReturnNMPtoPublishersFrame::setNMPHeader(void)
{
AnsiString str;

  if(pFrame==NULL || pFrame->getGridFrame()->getWorkDataSet()->Active==false || pFrame->getGridFrame()->getWorkDataSet()->RecordCount==0)
    str="Газетно-журнальной продукции, подлежащая ";
  else
    str=pFrame->getGridFrame()->getFieldDataOfSelectedRecord("P_NAME")+" - газетно-журнальной продукции, подлежащая ";
  if(typeComboBox->ItemIndex==0) str+="списанию";
  else str+="возврату";
  nmpHeader->Caption=str;
}
//---------------------------------------------------------------------------
void __fastcall TReturnNMPtoPublishersFrame::setFocus(void)
{
  if(activeFrame==1)
  {
    if(pFrame!=NULL) pFrame->getGridFrame()->workGrid->SetFocus();
  }
  else if(activeFrame==2)
  {
    if(nmpF!=NULL) nmpF->workGrid->SetFocus();
  }
}
//---------------------------------------------------------------------------
void __fastcall TReturnNMPtoPublishersFrame::loadPublishersDS(void)
{
  if(pFrame!=NULL)
  { AnsiString sql=formPublishersSelectSQL(date1,date2,typeComboBox->ItemIndex);
    pFrame->loadDS(sql);
    pFrame->getContentFrame()->setContent();
    if(nmpFrame!=NULL) loadNMPDS();
  }
}
//---------------------------------------------------------------------------
void __fastcall TReturnNMPtoPublishersFrame::loadNMPDS(void)
{
  if(nmpFrame==NULL) return;
  if(typeComboBox->ItemIndex==0)
  { TSpisanieNMPFrame *sf=static_cast<TSpisanieNMPFrame*>(nmpFrame);
    sf->loadDS();
    AnsiString s=nmpF->getFieldDataOfSelectedRecord("NAME_NMP");
    sf->getGridContentFrame()->getContentFrame()->setContent(s);
  }
  else
  { TVozvratNMPFrame *vf=static_cast<TVozvratNMPFrame*>(nmpFrame);
    vf->loadDS();
  }
  setNMPHeader();
}
//---------------------------------------------------------------------------
void __fastcall TReturnNMPtoPublishersFrame::loadNMPSearchDS(void)
{
AnsiString pID;

  if(nmpFrame==NULL) return;// || nmpFrame->getWorkDataSet()->RecordCount==0) return;
  pID=getPublisherID();
  if(nmpFrame!=NULL && !pID.IsEmpty())// && nmpFrame->getWorkDataSet()->RecordCount > 0)
  { if(typeComboBox->ItemIndex==0)
    { TSpisanieNMPFrame *sf=static_cast<TSpisanieNMPFrame*>(nmpFrame);
      sf->loadSearchDS();
    }
    else
    { TVozvratNMPFrame *vf=static_cast<TVozvratNMPFrame*>(nmpFrame);
      vf->loadSearchDS();
    }
  }
}
//---------------------------------------------------------------------------

//*************
//* published *
//*************
void __fastcall TReturnNMPtoPublishersFrame::onChangePeriod(TObject *Sender)
{
  periodStr=monthBox->Text+" "+yearEdit->Text+" года";

  date1=getDateFromMonthYearString(periodStr);
  date2=endOfMonth(date1);
  loadPublishersDS();
  setHeader();
}
//---------------------------------------------------------------------------
//Следующий период
void __fastcall TReturnNMPtoPublishersFrame::spinButtonBottomClick(TObject *Sender)
{
  if(monthBox->ItemIndex==0)
  { monthBox->ItemIndex=11;
    yearEdit->Value--;
  }
  else monthBox->ItemIndex--;
  onChangePeriod(this);
}
//Предыдущий период
void __fastcall TReturnNMPtoPublishersFrame::spinButtonTopClick(TObject *Sender)
{
  if(monthBox->ItemIndex==11)
  { monthBox->ItemIndex=0;
    yearEdit->Value++;
  }
  else monthBox->ItemIndex++;
  onChangePeriod(this);
}
//---------------------------------------------------------------------------

void __fastcall TReturnNMPtoPublishersFrame::typeComboBoxChange(TObject *Sender)
{
  if(nmpFrame!=NULL){ delete nmpFrame; nmpFrame=NULL; }
  nmpF=NULL;
  loadPublishersDS();
  setNMPHeader();
  nmpFrame=createReturnNMPFrame(this,iniFileName,iniSectionName,fbDB,nmpPanel,typeComboBox->ItemIndex);
  if(nmpFrame==NULL) return;
  loadNMPDS();
  setFocus();
}
//---------------------------------------------------------------------------
void __fastcall TReturnNMPtoPublishersFrame::pSourceDataChange(TObject *Sender,TField *Field)
{
  setNMPHeader();
  pFrame->getContentFrame()->setContent();
  if(nmpFrame==NULL) return;
  if(selectForPublisherBox->Checked==true)
  { if(nmpFrame!=NULL)
    { loadNMPDS();
      if(nmpF!=NULL && nmpF->isSearchDSActive()==true) loadNMPSearchDS();
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TReturnNMPtoPublishersFrame::pGridMouseDown(TObject *Sender,TMouseButton Button, TShiftState Shift, int X, int Y)
{
  activeFrame=1;
}
//---------------------------------------------------------------------------
void __fastcall TReturnNMPtoPublishersFrame::nmpSourceDataChange(TObject *Sender,TField *Field)
{
  if(nmpF==NULL) return;
  if(nmpF->getWorkDataSet()->RecordCount==0) return;

  if(selectForPublisherBox->Checked==false)
  { AnsiString pID=nmpF->getFieldDataOfSelectedRecord("P_ID");
    if(pID.IsEmpty()) return;
    pFrame->getGridFrame()->workSource->DataSet->Locate("P_ID",pID,TLocateOptions().Clear());
  }
  if(nmpF->workGrid->Showing && typeComboBox->ItemIndex==0)
  { TContentFrame *cf=(static_cast<TSpisanieNMPFrame*>(nmpFrame))->getGridContentFrame()->getContentFrame();
    if(cf!=NULL)
    { AnsiString s=nmpF->getFieldDataOfSelectedRecord("NAME_NMP");
      cf->setContent(s);
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TReturnNMPtoPublishersFrame::nmpGridMouseDown(TObject *Sender,TMouseButton Button, TShiftState Shift, int X, int Y)
{
  activeFrame=2;
}
//---------------------------------------------------------------------------
void __fastcall TReturnNMPtoPublishersFrame::selectForPublisherBoxClick(TObject *Sender)
{
  if(selectForPublisherBox->Checked==false)
  { printTorg12Item->Enabled=false;
    printTorg12SelectItem->Enabled=false;
    cmTorg12Item->Enabled=false;
    cmTorg12SelectItem->Enabled=false;
  }
  else
  { printTorg12Item->Enabled=true;
    printTorg12SelectItem->Enabled=true;
    cmTorg12Item->Enabled=true;
    cmTorg12SelectItem->Enabled=true;
  }

  if(nmpFrame==NULL) return;
  if(typeComboBox->ItemIndex==0)
  { TSpisanieNMPFrame *sf=static_cast<TSpisanieNMPFrame*>(nmpFrame);
    sf->reLoadDS();
  }
  else
  { TVozvratNMPFrame *vf=static_cast<TVozvratNMPFrame*>(nmpFrame);
    vf->reLoadDS();
  }
  setFocus();
}
//---------------------------------------------------------------------------

//Поиск по модели
void __fastcall TReturnNMPtoPublishersFrame::searchTimerTimer(TObject *Sender)
{
  searchTimer->Enabled=false;
  if(activeFrame==2) loadNMPSearchDS();
}
//---------------------------------------------------------------------------
void __fastcall TReturnNMPtoPublishersFrame::SEditChange(TObject *Sender)
{
  if(searchTimer->Enabled==true) searchTimer->Enabled=false;
  if(!SEdit->Text.IsEmpty()) searchTimer->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TReturnNMPtoPublishersFrame::SEditKeyDown(TObject *Sender, WORD &Key,TShiftState Shift)
{
  if(Key==VK_RETURN || Key==VK_DOWN || Key==VK_UP)
  { Key=0;
    if(activeFrame==2 && nmpFrame!=NULL) setFocus();
  }
  else if(Key == VK_ESCAPE){ Key=0; SEdit->Text=""; }
}
//---------------------------------------------------------------------------
void __fastcall TReturnNMPtoPublishersFrame::conditionSBoxClick(TObject *Sender)
{
  if(activeFrame==2 && !SEdit->Text.IsEmpty()) loadNMPSearchDS();
  setFocus();
}
//---------------------------------------------------------------------------
void __fastcall TReturnNMPtoPublishersFrame::FSButtonClick(TObject *Sender)
{
  if(activeFrame==2 && nmpF!=NULL) nmpF->forwardSearch();
}
//---------------------------------------------------------------------------
void __fastcall TReturnNMPtoPublishersFrame::BSButtonClick(TObject *Sender)
{
  if(activeFrame==2 && nmpF!=NULL) nmpF->backwardSearch();
}
//---------------------------------------------------------------------------
//Возврат ГЖП поставщикам (краткий отчет)
void __fastcall TReturnNMPtoPublishersFrame::returnNMPItemClick(
      TObject *Sender)
{
TpFIBDataSet *lDS=pFrame->getGridFrame()->getWorkDataSet();
int rn=lDS->RecNo;

  frxXLSExport->DefaultPath=workDir+"Out\\";
  frxXLSExport->FileName="Возврат ГЖП из подразделений (краткий отчет)";

  if(frxReport->LoadFromFile(workDir+"Reports\\ReturnNMPToPublishers.fr3")==false)
  { MessageDlg("Не могу загрузить файл с отчетом: "+workDir+"Reports\\ReturnNMPToPublishers.fr3",mtWarning,TMsgDlgButtons() << mbOK,0);
    return;
  }
  frxDS->DataSet=lDS;
  frxReport->ShowReport(true);
  lDS->RecNo=rn;
}
//---------------------------------------------------------------------------
//Печать накладной Торг-12
// - полная
void __fastcall TReturnNMPtoPublishersFrame::printTorg12ItemClick(TObject *Sender)
{
  if(pFrame!=NULL && nmpF!=NULL)
    printTorg12(this,fbDB->getDatabase(),pFrame->getGridFrame()->getWorkDataSet(),nmpF->getWorkDataSet(),1);
}
//---------------------------------------------------------------------------
// - выборочная
void __fastcall TReturnNMPtoPublishersFrame::printTorg12SelectItemClick(TObject *Sender)
{
  if(pFrame!=NULL && nmpF!=NULL)
    printTorg12(this,fbDB->getDatabase(),pFrame->getGridFrame()->getWorkDataSet(),nmpF->getWorkDataSet(),2);
}
//---------------------------------------------------------------------------
void __fastcall TReturnNMPtoPublishersFrame::frxReportGetValue(const AnsiString VarName, Variant &Value)
{
  //Накладная Торг-12
  if(VarName=="Organisation")
  { AnsiString s=getConstValue(this,fbDB,"ORGANIZATION_NAME",date2)+", ";
    s+=getConstValue(this,fbDB,"ORGANIZATION_ADDRESS",date2)+", ";
    s+=getConstValue(this,fbDB,"BANK_REQUISITIONS",date2);
    Value=s;
  }
  else if(VarName=="SS_Name") //получатель
  {
    Value=getConstValue(this,fbDB,"SS_NAME",date2);
  }
  else if(VarName=="RECIPIENT_NAME") //получатель
  {
    Value=pFrame->getGridFrame()->getWorkDataSet()->FieldByName("P_NAME")->AsString;
  }
  else if(VarName=="Title")
  { AnsiString s="Возврат газетно-журнальной продукции поставщикам";
    s=SetHeader(s,date1,date2);
    Value=s;
  }
  else if(VarName=="RET_TYPE")
  { AnsiString s;
    if(typeComboBox->ItemIndex==0) s="Списано";
    else s="Возвращено";
    Value=s;
  }
  else if(VarName=="Consignee") //грузополучатель
  { AnsiString s;
    TpFIBDataSet *ds=pFrame->getGridFrame()->getWorkDataSet();
    if(!ds->FieldByName("P_OFFICIAL_NAME")->IsNull) s=ds->FieldByName("P_OFFICIAL_NAME")->AsString;
    else s=ds->FieldByName("P_NAME")->AsString;
    if(!ds->FieldByName("P_ADDRESS")->IsNull)
      s+=", "+ds->FieldByName("P_ADDRESS")->AsString;
    if(!ds->FieldByName("P_INN_KPP")->IsNull)
      s+=", "+ds->FieldByName("P_INN_KPP")->AsString;
    if(!ds->FieldByName("P_PHONE")->IsNull)
      s+=", "+ds->FieldByName("P_PHONE")->AsString;
    if(!ds->FieldByName("P_BANK_REQUISITIONS")->IsNull)
      s+=", "+ds->FieldByName("P_BANK_REQUISITIONS")->AsString;
    Value=s;
  }
  else if(VarName=="Supplier") //поставщик
  {
    Value=getConstValue(this,fbDB,"MAIN_ORGANIZATION",date2);
  }
  else if(VarName=="Payer")    //плательщик
  { AnsiString s;
    TpFIBDataSet *ds=pFrame->getGridFrame()->getWorkDataSet();
    if(!ds->FieldByName("P_OFFICIAL_NAME")->IsNull) s=ds->FieldByName("P_OFFICIAL_NAME")->AsString;
    else s=ds->FieldByName("P_NAME")->AsString;
    if(!ds->FieldByName("P_ADDRESS")->IsNull)
      s+=", "+ds->FieldByName("P_ADDRESS")->AsString;
    if(!ds->FieldByName("P_INN_KPP")->IsNull)
      s+=", "+ds->FieldByName("P_INN_KPP")->AsString;
    if(!ds->FieldByName("P_PHONE")->IsNull)
      s+=", "+ds->FieldByName("P_PHONE")->AsString;
    if(!ds->FieldByName("P_BANK_REQUISITIONS")->IsNull)
      s+=", "+ds->FieldByName("P_BANK_REQUISITIONS")->AsString;
    Value=s;
//    Value=pFrame->getGridFrame()->getWorkDataSet()->FieldByName("P_NAME")->AsString;
  }
  else if(VarName=="Reason")    //Основание для выписки накладной
  {
    Value=pFrame->getGridFrame()->getWorkDataSet()->FieldByName("REASON")->AsString;
  }
  else if(VarName=="OKPO")
  {
    Value=getConstValue(this,fbDB,"OKPO",date2);
  }
  else if(VarName=="OKDP")
  {
    Value=getConstValue(this,fbDB,"OKDP",date2);
  }
  else if(VarName=="DocNum")
  { AnsiString s;
    AnsiString pID=getPublisherID();//pFrame->getGridFrame()->getWorkDataSet()->FieldByName("P_ID")->AsString;
    unsigned short y,m,d;
    date2.DecodeDate(&y,&m,&d);
    s=pID+"_"+AnsiString().sprintf("%02d",m)+"-"+AnsiString().sprintf("%d",y);
    if(fullPrintFLG==false) s+="-доп";
    Value=s;
  }
  else if(VarName=="DocDate")
  { if(fullPrintFLG==true)
    { unsigned short y,m,d;
      date2.DecodeDate(&y,&m,&d);
      m++; if(m>12){ m=1; y++;}
      Value=TDateTime(y,m,10).DateString();
    }
    else Value=date2.DateString();
  }
  else if(VarName=="KLP") //количество строк прописью
  { TfrxMemoView *memo;
    memo=(TfrxMemoView *)frxReport->FindObject("Memo89");
    int sum=int(memo->Value);
    Value=i_to_str(sum);
  }
  else if(VarName=="SummaProp") //сумма с НДС прописью
  { TfrxMemoView *memo;
    memo=(TfrxMemoView *)frxReport->FindObject("Memo105");
    double sum=double(memo->Value);
    Value=f_to_str(sum);
  }
  else if(VarName=="LJT")
  {
    Value=getConstValue(this,fbDB,"LEADER_JOB_TITLE",date2);
  }
  else if(VarName=="LFIO")
  {
    Value=getConstValue(this,fbDB,"LEADER_FIO",date2);
  }
  else if(VarName=="GBFIO")
  {
    Value=getConstValue(this,fbDB,"GBOOK_FIO",date2);
  }
  else if(VarName=="RJT")
  {
    Value=getConstValue(this,fbDB,"WORKMAN_JOB_TITLE",date2);
  }
  else if(VarName=="RFIO")
  {
    Value=getConstValue(this,fbDB,"WORKMAN_FIO",date2);
  }
  else if(VarName=="D")
  { if(fullPrintFLG==true) Value="10";
    else
    { unsigned short y,m,d;
      date2.DecodeDate(&y,&m,&d);
      Value=IntToStr(d);
    }
  }
  else if(VarName=="M")
  { unsigned short y,m,d;
    date2.DecodeDate(&y,&m,&d);
    char *mes[12]={"января","февраля","марта","апреля","мая","июня","июля","августа","сентября","октября","ноября","декабря"};
    if(fullPrintFLG==true)
    { m++;
      if(m>12) m=1;
    }
    Value=AnsiString(mes[m-1]);
  }
  else if(VarName=="Y")
  { unsigned short y,m,d;
    date2.DecodeDate(&y,&m,&d);
    if(fullPrintFLG==true)
    { m++;
      if(m>12) y++;
    }
    Value=IntToStr(y);
  }
  //Акт Торг-16
  if(VarName=="Organisation16")
  {
    Value=getConstValue(this,fbDB,"ORGANIZATION_NAME",date2);
  }
  else if(VarName=="Subdivision")
  {
    Value="Основной склад";
  }
  else if(VarName=="DocNum16")
  { unsigned short y,m,d;
    date2.DecodeDate(&y,&m,&d);
    AnsiString s="29_"+IntToStr(m)+"-"+IntToStr(y);
    Value=s;
  }
  else if(VarName=="DocDate16")
  { unsigned short y,m,d;
    date2.DecodeDate(&y,&m,&d);
    m++;
    if(m>12){m=1; y++;}
    Value=TDateTime(y,m,25).DateString();
  }
  else if(VarName=="DPT")
  {
     Value="c "+date1.DateString()+" по "+date2.DateString();
  }
  else if(VarName=="DST")
  { unsigned short y,m,d;
    date2.DecodeDate(&y,&m,&d);
    m++;
    if(m>12){m=1; y++;}
    Value=TDateTime(y,m,25).DateString();
  }
  else if(VarName=="NTN")
  { unsigned short y,m,d;
    date2.DecodeDate(&y,&m,&d);
    Value="29_"+IntToStr(m)+"-"+IntToStr(y);
  }
  else if(VarName=="DTN")
  { unsigned short y,m,d;
    date2.DecodeDate(&y,&m,&d);
    Value=TDateTime(y,m,lastDayOfMonth(y,m));
  }
  else if(VarName=="SumProp")
  { TfrxMemoView *memo;
    memo=(TfrxMemoView *)frxReport->FindObject("Memo152");
    double sum=double(memo->Value);
    Value=f_to_str(sum);
  }
}
//---------------------------------------------------------------------------
//Печать акта Торг-2
// - полного
void __fastcall TReturnNMPtoPublishersFrame::printTorg2ItemClick(TObject *Sender)
{
  printTorg2(this,1);
}
//---------------------------------------------------------------------------
// - выборочного
void __fastcall TReturnNMPtoPublishersFrame::printTorg2SelectItemClick(
      TObject *Sender)
{
  printTorg2(this,2);
}
//---------------------------------------------------------------------------
//Печать акта Торг-16
void __fastcall TReturnNMPtoPublishersFrame::printTorg16ItemClick(TObject *Sender)
{
  printTorg16(this,fbDB);
}
//---------------------------------------------------------------------------
//Формирование сообщения с накладной Торг-12
// - полная накладная
void __fastcall TReturnNMPtoPublishersFrame::cmTorg12ItemClick(TObject *Sender)
{
  if(typeComboBox->ItemIndex==0) createMessage(this,iniFileName,1,1);
  else createMessage(this,iniFileName,2,1);
}
//---------------------------------------------------------------------------
// - выборочная накладная
void __fastcall TReturnNMPtoPublishersFrame::cmTorg12SelectItemClick(TObject *Sender)
{
  if(typeComboBox->ItemIndex==0) createMessage(this,iniFileName,1,2);
  else createMessage(this,iniFileName,2,2);
}
//---------------------------------------------------------------------------
//Формирование сообщения с актом Торг-2
// - полный акт
void __fastcall TReturnNMPtoPublishersFrame::cmTorg2ItemClick(TObject *Sender)
{
  createMessage(this,iniFileName,3,1);
}
//---------------------------------------------------------------------------
// - выборочный акт
void __fastcall TReturnNMPtoPublishersFrame::cmTorg2SelectItemClick(TObject *Sender)
{
  createMessage(this,iniFileName,3,2);
}
//---------------------------------------------------------------------------
//

