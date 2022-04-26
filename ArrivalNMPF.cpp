//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "DBClasses.h"
#include "SomeFunctions.h"
#include "ArrivalNMPFunctions.h"
#include "UniversalSelectPeriod.h"
#include "DifferentFunctions.h"
#include "ViewNMPBySD.h"
#include "ArrivalNMPF.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "JvComponent"
#pragma link "JvExControls"
#pragma link "JvNavigationPane"
#pragma link "FIBDataSet"
#pragma link "pFIBDataSet"
#pragma link "JvSpeedButton"
#pragma link "JvXPCheckCtrls"
#pragma link "JvXPCore"
#pragma link "JvArrowButton"
#pragma link "frxClass"
#pragma link "frxDBSet"
#pragma link "frxExportRTF"
#pragma link "frxExportXLS"
#pragma resource "*.dfm"
//TArrivalNMPFrame *ArrivalNMPFrame;
//---------------------------------------------------------------------------
//**********
//* public *
//**********
__fastcall TArrivalNMPFrame::TArrivalNMPFrame(TComponent* Owner,AnsiString iniFN,FBDatabase *fbdb)
        : TFrame(Owner),iniFileName(iniFN),fbDB(fbdb)
{
  iniSectionName="ArrivalNMPFromPublishers";
  dataBase=fbDB->getDatabase();

  pFrame=NULL;
  nmpFrame=NULL;
  firstLoadFLG=false;
  
  date1=TDateTime("01.05.2010");
  date2=TDateTime("31.05.2010");
}
//---------------------------------------------------------------------------
__fastcall TArrivalNMPFrame::~TArrivalNMPFrame(void)
{
  saveProperties();
  if(nmpFrame!=NULL){ delete nmpFrame; nmpFrame=NULL; }
  if(pFrame!=NULL){ delete pFrame; pFrame=NULL; }
}
//---------------------------------------------------------------------------
void __fastcall TArrivalNMPFrame::initFrame(void)
{
  loadProperties();
  //Инициируем фрейм поставщиков
  pFrame=cteateArrivalPublishersFtame(this,iniFileName,iniSectionName+"_pFrame",fbDB,pPanel,SEdit,ConditionSBox);
  if(pFrame!=NULL)
  { pFrame->getGridFrame()->workGrid->OnMouseDown=pGridMouseDown;
    pFrame->getGridFrame()->workSource->OnDataChange=pSourceDataChange;
//    pFrame->loadDS=&loadPublishersDS;
//    pFrame->loadSearchDS=&loadPublishersSearchDS;
  }
  else return;
  //Инициируем фрейм ГЖП
  AnsiString snStr=iniSectionName+"_nmpFrame"+IntToStr(int(groupOnPublishing->Checked));
  nmpFrame=createArrivalNMPFrame(this,iniFileName,snStr,fbDB,nmpPanel,SEdit,ConditionSBox,groupOnPublishing->Checked);
  if(nmpFrame!=NULL)
  { nmpFrame->getGridFrame()->workGrid->OnMouseDown=nmpGridMouseDown;
    nmpFrame->getGridFrame()->workSource->OnDataChange=nmpSourceDataChange;
    nmpFrame->additional1->OnClick=viewBySDItemClick;
    nmpFrame->additional1->Bitmap->LoadFromResourceName((int)HInstance,"BYSDBMP");
//    nmpFrame->loadDS=&loadNMPDS;
//    nmpFrame->loadSearchDS=&loadNMPSearchDS;
  }
  else return;
  loadDataSets();
  setHeader();
}
//---------------------------------------------------------------------------
void __fastcall TArrivalNMPFrame::loadDataSets(void)
{
//  firstLoadFLG=true;
  loadPublishersDS();
//  firstLoadFLG=false;
//  loadNMPDS();
  setFocus();
}
//---------------------------------------------------------------------------
void __fastcall TArrivalNMPFrame::setFocus(void)
{
  if(activeFrame==1)
  {
    if(pFrame!=NULL) pFrame->getGridFrame()->workGrid->SetFocus();
  }
  else if(activeFrame==2)
  {
    if(nmpFrame!=NULL) nmpFrame->getGridFrame()->workGrid->SetFocus();
  }
}
//---------------------------------------------------------------------------

//***********
//* private *
//***********
void __fastcall TArrivalNMPFrame::loadProperties(void)
{
TIniFile *ini = new TIniFile(iniFileName);

  pPanel->Width=ini->ReadInteger(iniSectionName,"pPanel_Width",200);
  selectForPublisher->Checked=ini->ReadBool(iniSectionName,"selectForPublisher_Checked",true);
  activeFrame=ini->ReadInteger(iniSectionName,"activeFrame",1);
  date1=ini->ReadDateTime(iniSectionName,"date1",date1);
  date2=ini->ReadDateTime(iniSectionName,"date2",date2);
  delete ini;
  if(selectForPublisher->Checked==true) receiptWarrantsItem->Visible=true;
  else receiptWarrantsItem->Visible=false;
}
//---------------------------------------------------------------------------
void __fastcall TArrivalNMPFrame::saveProperties(void)
{
TIniFile *ini = new TIniFile(iniFileName);

  ini->WriteInteger(iniSectionName,"pPanel_Width",pPanel->Width);
  ini->WriteBool(iniSectionName,"selectForPublisher_Checked",selectForPublisher->Checked);
  ini->WriteInteger(iniSectionName,"activeFrame",activeFrame);
  ini->WriteDateTime(iniSectionName,"date1",date1);
  ini->WriteDateTime(iniSectionName,"date2",date2);
  delete ini;
}
//---------------------------------------------------------------------------
void __fastcall TArrivalNMPFrame::loadPublishersDS(void)
{
  if(pFrame!=NULL)
  { pFrame->getGCFrame()->loadDS(formArrivalPublishersSelectSQL(date1,date2));
    pFrame->getContentFrame()->setContent();
    if(nmpFrame!=NULL) loadNMPDS();
  }
}
//---------------------------------------------------------------------------
void __fastcall TArrivalNMPFrame::loadPublishersSearchDS(void)
{
  if(pFrame==NULL) return;

//  AnsiString fieldName=pFrame->getCurrentFieldName();
//  bool viewDelRecords=pFrame->getViewDelRecordsFLG();
  pFrame->getGridFrame()->initSearchSQL(formArrivalPublishersSearchSQL(date1,date2));
  pFrame->getGridFrame()->loadSearchingDS();
}
//---------------------------------------------------------------------------
void __fastcall TArrivalNMPFrame::loadNMPDS(void)
{
  if(pFrame==NULL || nmpFrame==NULL) return;

  AnsiString pID=pFrame->getGridFrame()->getFieldDataOfSelectedRecord("P_ID");
  nmpFrame->getGCFrame()->loadDS(formArrivalNMPSelectSQL(pID,selectForPublisher->Checked,groupOnPublishing->Checked,date1,date2));
  nmpFrame->getContentFrame()->setContent();
}
//---------------------------------------------------------------------------
void __fastcall TArrivalNMPFrame::loadNMPSearchDS(void)
{
AnsiString fieldName=nmpFrame->getGridFrame()->getFieldInfoByGridFieldName(nmpFrame->getCurrentFieldName())->getSQLFieldName();
AnsiString pID=pFrame->getGridFrame()->getFieldDataOfSelectedRecord("P_ID");

  AnsiString searchSQLstr=formArrivalNMPSearchSQL(pID,selectForPublisher->Checked,groupOnPublishing->Checked,date1,date2);
  nmpFrame->getGridFrame()->initSearchSQL(searchSQLstr);
  nmpFrame->getGridFrame()->loadSearchingDS();
}
//---------------------------------------------------------------------------
void __fastcall TArrivalNMPFrame::setHeader(void)
{
  header->Caption=SetHeader("Поступление газетно-журнальной продукции",date1,date2);
}
//---------------------------------------------------------------------------

//*************
//* published *
//*************
void __fastcall TArrivalNMPFrame::periodButtonClick(TObject *Sender)
{
TUniversalSelectPeriodForm *USPF;

  try
  {
    USPF=new TUniversalSelectPeriodForm(this,iniFileName,"UniversalSelectPeriodForm_ArrivalNMPFrame","Сформировать отчет за период",date1,date2);
  }
  catch (Exception &exc)
  { MessageDlg("Ошибка создания формы UniversalSelectPeriodForm\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return;
  }
  if(USPF->ShowModal()==mrOk)
  { loadDataSets();
    setHeader();
  }
  delete USPF;
}
//---------------------------------------------------------------------------
//Поиск по модели
void __fastcall TArrivalNMPFrame::searchTimerTimer(TObject *Sender)
{
  searchTimer->Enabled=false;
  if(activeFrame==1)
  {
    loadPublishersSearchDS();
  }
  else if(activeFrame==2)
  {
    if(nmpFrame!=NULL) loadNMPSearchDS();
  }
}
//---------------------------------------------------------------------------
void __fastcall TArrivalNMPFrame::SEditChange(TObject *Sender)
{
  if(searchTimer->Enabled==true) searchTimer->Enabled=false;
  if(!SEdit->Text.IsEmpty()) searchTimer->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TArrivalNMPFrame::SEditKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  if(Key==VK_RETURN || Key==VK_DOWN || Key==VK_UP)
  { Key=0;
    if(activeFrame==1)
    {
      if(pFrame!=NULL && pFrame->getGridFrame()!=NULL) pFrame->getGridFrame()->workGrid->SetFocus();
    }
    else if(activeFrame==2)
    {
      if(nmpFrame!=NULL && nmpFrame->getGridFrame()!=NULL) nmpFrame->getGridFrame()->workGrid->SetFocus();
    }
  }
  else if(Key == VK_ESCAPE){ Key=0; SEdit->Text=""; }
}
//---------------------------------------------------------------------------
void __fastcall TArrivalNMPFrame::ConditionSBoxClick(TObject *Sender)
{
  if(activeFrame==1)
  {
    loadPublishersSearchDS();
  }
  else if(activeFrame==2)
  {
    if(nmpFrame!=NULL) loadNMPSearchDS();
  }
}
//---------------------------------------------------------------------------
void __fastcall TArrivalNMPFrame::FSButtonClick(TObject *Sender)
{
  if(activeFrame==1)
  {
    if(pFrame!=NULL) pFrame->getGridFrame()->forwardSearch();
  }
  else if(activeFrame==2)
  {
    if(nmpFrame!=NULL) nmpFrame->getGridFrame()->forwardSearch();
  }
}
//---------------------------------------------------------------------------
void __fastcall TArrivalNMPFrame::BSButtonClick(TObject *Sender)
{
  if(activeFrame==1)
  {
    if(pFrame!=NULL) pFrame->getGridFrame()->backwardSearch();
  }
  else if(activeFrame==2)
  {
    if(nmpFrame!=NULL) nmpFrame->getGridFrame()->backwardSearch();
  }
}
//---------------------------------------------------------------------------

void __fastcall TArrivalNMPFrame::pGridMouseDown(TObject *Sender,TMouseButton Button, TShiftState Shift, int X, int Y)
{
  activeFrame=1;
}
//---------------------------------------------------------------------------
void __fastcall TArrivalNMPFrame::pSourceDataChange(TObject *Sender,TField *Field)
{
  if(pFrame->getContentFrame()!=NULL) pFrame->getContentFrame()->setContent();
  if(firstLoadFLG==true) return;
  if(selectForPublisher->Checked==true)
  { if(nmpFrame!=NULL)
    { loadNMPDS();
      if(nmpFrame->getGridFrame()->isSearchDSActive()==true) loadNMPSearchDS();
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TArrivalNMPFrame::selectForPublisherClick(TObject *Sender)
{
  if(selectForPublisher->Checked==true) receiptWarrantsItem->Visible=true;
  else receiptWarrantsItem->Visible=false;
  if(pFrame!=NULL)
  { if(nmpFrame!=NULL)
    { loadNMPDS();
      if(nmpFrame->getGridFrame()->isSearchDSActive()==true) loadNMPSearchDS();
    }
  }
  setFocus();
}
//---------------------------------------------------------------------------
void __fastcall TArrivalNMPFrame::nmpGridMouseDown(TObject *Sender,TMouseButton Button, TShiftState Shift, int X, int Y)
{
  activeFrame=2;
}
//---------------------------------------------------------------------------
void __fastcall TArrivalNMPFrame::nmpSourceDataChange(TObject *Sender,TField *Field)
{
  if(pFrame==NULL) return;
  if(nmpFrame==NULL) return;
  if(nmpFrame->getContentFrame()!=NULL) nmpFrame->getContentFrame()->setContent();
  if(selectForPublisher->Checked==false)
  { AnsiString pID=nmpFrame->getGridFrame()->getFieldDataOfSelectedRecord("P_ID");
    if(pID.IsEmpty()) return;
    pFrame->getGridFrame()->workSource->DataSet->Locate("P_ID",pID,TLocateOptions().Clear());
  }
}
//---------------------------------------------------------------------------
void __fastcall TArrivalNMPFrame::groupOnPublishingClick(TObject *Sender)
{
  if(nmpFrame!=NULL){ delete nmpFrame; nmpFrame=NULL; }
  AnsiString snStr=iniSectionName+"_nmpFrame"+IntToStr(int(groupOnPublishing->Checked));
  nmpFrame=createArrivalNMPFrame(this,iniFileName,snStr,fbDB,nmpPanel,SEdit,ConditionSBox,groupOnPublishing->Checked);
  if(nmpFrame!=NULL)
  { nmpFrame->getGridFrame()->workGrid->OnMouseDown=nmpGridMouseDown;
    nmpFrame->getGridFrame()->workSource->OnDataChange=nmpSourceDataChange;
    nmpFrame->additional1->OnClick=viewBySDItemClick;
//    nmpFrame->loadDS=&loadNMPDS;
//    nmpFrame->loadSearchDS=&loadNMPSearchDS;
  }
  loadNMPDS();
}
//---------------------------------------------------------------------------
//Отчет о поступлении ГЖП
void __fastcall TArrivalNMPFrame::arrivalReportItemClick(TObject *Sender)
{
AnsiString sql;
TDBGridEh *pGrid=pFrame->getGridFrame()->workGrid;
TDBGridEh *nmpGrid=nmpFrame->getGridFrame()->workGrid;
TpFIBDataSet *pDS=pFrame->getGridFrame()->getWorkDataSet();
TpFIBDataSet *nmpDS=nmpFrame->getGridFrame()->getWorkDataSet();

  if(pDS->RecordCount==0) return;

  frxXLSExport->DefaultPath=getAppDir()+"Out\\";
  if(pGrid->SelectedRows->Count==0) pGrid->Selection->Rows->CurrentRowSelected=true;
  if(groupOnPublishing->Checked==true)
    frxXLSExport->FileName="Поступление ГЖП (группировка по изданиям)";
  else
    frxXLSExport->FileName="Поступление ГЖП (в разрезе выходов)";
  if(pGrid->SelectedRows->Count==1)
  { if(groupOnPublishing->Checked==false)
    { if(frxReport->LoadFromFile(getAppDir()+"Reports\\ComingReport2.fr3")==false)
      { MessageDlg("Не могу загрузить файл с отчетом: "+getAppDir()+"Reports\\CominReport2.fr3",mtWarning,TMsgDlgButtons() << mbOK,0);
        return;
      }
    }
    else
    { if(frxReport->LoadFromFile(getAppDir()+"Reports\\ComingReport4.fr3")==false)
      { MessageDlg("Не могу загрузить файл с отчетом: "+getAppDir()+"Reports\\CominReport4.fr3",mtWarning,TMsgDlgButtons() << mbOK,0);
        return;
      }
    }
    frxDS->DataSet=nmpDS;
    frxReport->PrepareReport(true);
  }
  else
  { if(groupOnPublishing->Checked==false)
    { if(frxReport->LoadFromFile(getAppDir()+"Reports\\ComingReport3.fr3")==false)
      { MessageDlg("Не могу загрузить файл с отчетом: "+getAppDir()+"Reports\\CominReport3.fr3",mtWarning,TMsgDlgButtons() << mbOK,0);
        return;
      }
    }
    else
    { if(frxReport->LoadFromFile(getAppDir()+"Reports\\ComingReport5.fr3")==false)
      { MessageDlg("Не могу загрузить файл с отчетом: "+getAppDir()+"Reports\\CominReport3.fr3",mtWarning,TMsgDlgButtons() << mbOK,0);
        return;
      }
    }
    //Формируем строку с идентификаторами выбранных записей
    int rn=pDS->RecNo;
    nmpGrid->Visible=false;
    firstLoadFLG=true;
    AnsiString p_id_str="";
    for(int i=0; i<pGrid->SelectedRows->Count; ++i)
    { pDS->GotoBookmark((void *)pGrid->SelectedRows->Items[i].c_str());
      if(i > 0) p_id_str+=",";
      p_id_str+=pDS->FieldByName("P_ID")->AsString;
    }
    firstLoadFLG=false;
    pDS->RecNo=rn;;
    nmpGrid->Visible=true;

    if(groupOnPublishing->Checked==false)
      sql="select p.p_name, n.nmp_name, e.e_number, e.e_price as NMP_PRICE, e.nds_stavka,";
    else
      sql="select p.p_name, n.nmp_name, e.e_price as NMP_PRICE, e.nds_stavka,";
    sql+=" sum(f.FACT_Q) as SUM_FACT_Q, sum(f.FACT_Q*e.e_price*(100+e.nds_stavka)/100) as vsego";
    sql+=" from facts_table f";
    sql+=" left outer join exits_table e on e.e_id=f.e_id";
    sql+=" left outer join nmp_table n on n.nmp_id=e.nmp_id";
    sql+=" left outer join publishers_table p on p.p_id=n.p_id";
    sql+=" where e.E_DATE>='"+date1.DateString()+"'";
    sql+=" and e.E_DATE<='"+date2.DateString()+"'";
    sql+=" and p.P_ID in ("+p_id_str+")";
    if(groupOnPublishing->Checked==false)
      sql+=" group by p.p_name, n.nmp_name, e.e_number, e.e_price, e.nds_stavka";
    else
      sql+=" group by p.p_name, n.nmp_name, e.e_price, e.nds_stavka";
    sql+=" order by p.p_name, n.nmp_name";
    if(groupOnPublishing->Checked==false) sql+=",e.e_number";
    TpFIBDataSet *DS=fbDB->getDS_T1_T2();
    DS->Close();
    DS->SQLs->SelectSQL->Clear();
    DS->SQLs->SelectSQL->Add(sql);
    try
    {
      DS->Open();
    }
    catch (Exception &exc)
    { MessageDlg("Ошибка - "+sql+"\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
      return;
    }
    frxDS->DataSet=DS;
    frxReport->PrepareReport(true);
  }
  frxReport->ShowPreparedReport();
}
//---------------------------------------------------------------------------
//Печать краткого отчета
void __fastcall TArrivalNMPFrame::shortArrivalReportItemClick(
      TObject *Sender)
{
TpFIBDataSet *pDS=pFrame->getGridFrame()->getWorkDataSet();
int rn=pDS->RecNo;

  frxXLSExport->DefaultPath=getAppDir()+"Out\\";
  frxXLSExport->FileName="Поступление ГЖП (краткий отчет)";

  if(frxReport->LoadFromFile(getAppDir()+"Reports\\ArrivalNMPReport.fr3")==false)
  { MessageDlg("Не могу загрузить файл с отчетом: "+getAppDir()+"Reports\\ArrivalNMPReport.fr3",mtWarning,TMsgDlgButtons() << mbOK,0);
    return;
  }
  frxDS->DataSet=pDS;
  frxReport->ShowReport(true);
  pDS->RecNo=rn;
}
//---------------------------------------------------------------------------
//Печать приходного ордера
void __fastcall TArrivalNMPFrame::receiptWarrantsItemClick(TObject *Sender)
{
TDBGridEh *pGrid=pFrame->getGridFrame()->workGrid;
//TDBGridEh *nmpGrid=nmpFrame->getGridFrame()->workGrid;
TpFIBDataSet *pDS=pFrame->getGridFrame()->getWorkDataSet();
TpFIBDataSet *nmpDS=nmpFrame->getGridFrame()->getWorkDataSet();
FBQuery *fbWQ=fbDB->getFBQ_T2();
TpFIBQuery *WQ=fbWQ->getQuery();
TpFIBTransaction *WT=fbWQ->getTransaction();
AnsiString sql;

  int rn=pDS->RecNo;
  frxXLSExport->DefaultPath=getAppDir()+"Out\\";
  if(pGrid->SelectedRows->Count==0) pGrid->Selection->Rows->CurrentRowSelected=true;

  if(frxReport->LoadFromFile(getAppDir()+"Reports\\RWReport.fr3")==false)
  { MessageDlg("Не могу загрузить файл с отчетом: "+getAppDir()+"Reports\\RWReport.fr3",mtWarning,TMsgDlgButtons() << mbOK,0);
    return;
  }

  nmpPanel->Visible=false;

  for(int i=0; i<pGrid->SelectedRows->Count; ++i)
  { pDS->GotoBookmark((void *)pGrid->SelectedRows->Items[i].c_str());
    int p_id=pDS->FieldByName("P_ID")->AsInteger;
    unsigned short y,m,d;
    date2.DecodeDate(&y,&m,&d);
    rwDate=TDateTime(y,m,lastDayOfMonth(y,m)); //дата приходного ордера
    if(nmpDS->RecordCount > 0)
    { frxDS->DataSet=nmpDS;
      //Определяем номер приходного ордера
      sql="select RW_NUMBER from RECEIPT_WARRANTS_TABLE where P_ID="+IntToStr(p_id);
      sql+=" and RW_DATE='"+rwDate.DateString()+"'";
      if(fbWQ->execQuery("TArrivalNMPFrame::receiptWarrantsItemClick",sql)==false) return;
      if(WQ->RecordCount > 0) rwNumber=WQ->FieldByName("RW_NUMBER")->AsInteger;
      else
      { sql="select max(RW_NUMBER) as MAX_RW_NUMBER from RECEIPT_WARRANTS_TABLE";
        sql+=" where RW_YEAR="+IntToStr(y);
        if(fbWQ->execQuery("TArrivalNMPFrame::receiptWarrantsItemClick",sql)==false) return;
        rwNumber=WQ->FieldByName("MAX_RW_NUMBER")->AsInteger+1;
        sql="insert into RECEIPT_WARRANTS_TABLE(RW_NUMBER,RW_YEAR,RW_DATE,P_ID) values(";
        sql+=IntToStr(rwNumber)+",";
        sql+=IntToStr(y)+",";
        sql+="'"+rwDate.DateString()+"',";
        sql+=IntToStr(p_id)+")";
        WT->StartTransaction();
        if(fbWQ->execQuery("TArrivalNMPFrame::receiptWarrantsItemClick",sql)==false)
        { WT->Rollback();
          return;
        }
        WT->Commit();
      }
      //Формируем очередной приходный ордер
      if(i==0) frxReport->PrepareReport(true);
      else frxReport->PrepareReport(false);
    }
  }
  pDS->RecNo=rn;
  nmpPanel->Visible=true;
  frxReport->ShowPreparedReport();
}
//---------------------------------------------------------------------------
void __fastcall TArrivalNMPFrame::frxReportGetValue(const AnsiString VarName, Variant &Value)
{
  if(VarName=="Title")
  { AnsiString s="Поступление газетно-журнальной продукции";
    if(selectForPublisher->Checked==true) s+=" от \""+pFrame->getGridFrame()->getWorkDataSet()->FieldByName("P_NAME")->AsString;
    s=SetHeader(s,date1,date2);
//    if(date1==date2) s+="\" за "+date1.DateString();
//    else s+=" за период с "+date1.DateString()+" по "+date2.DateString();
//    s+=" года";
    if(groupOnPublishing->Checked==true) s+=" (группировка по изданиям)";
    else s+=" (в разрезе выходов)";
    Value=s;
  }
  else if(VarName=="Title_3")
  { AnsiString s="Поступление газетно-журнальной продукции ";
    s=SetHeader(s,date1,date2);
//    if(date1==date2) s+="\" за "+date1.DateString();
//    else s+=" за период с "+date1.DateString()+" по "+date2.DateString();
//    s+=" года";
    Value=s;
  }
  //Приходный ордер
  else if(VarName=="SS_Name")
  {
    Value=getConstValue(this,fbDB,"SS_NAME",rwDate);
  }
  else if(VarName=="RWNumber")
  {
    Value="ГЖП-"+AnsiString().sprintf("%03d",rwNumber);
  }
  else if(VarName=="RWDate")
  {
    Value=rwDate.DateString();
  }
  else if(VarName=="Organization")
  {
    Value=getConstValue(this,fbDB,"ORGANIZATION_NAME",rwDate);
  }
  else if(VarName=="Provider")
  {
    Value=pFrame->getGridFrame()->getWorkDataSet()->FieldByName("P_NAME")->AsString;
  }
  else if(VarName=="P_NAME")
  {
    Value=pFrame->getGridFrame()->getWorkDataSet()->FieldByName("P_NAME")->AsString;
  }
  else if(VarName=="OKPO")
  {
    Value=getConstValue(this,fbDB,"OKPO",rwDate);
  }
  else if(VarName=="NAME_NMP")
  { AnsiString s=nmpFrame->getGridFrame()->getWorkDataSet()->FieldByName("NMP_NAME")->AsString;
    if(groupOnPublishing->Checked==false)
      s+=", №"+nmpFrame->getGridFrame()->getWorkDataSet()->FieldByName("E_NUMBER")->AsString;
    Value=s;
  }
  else if(VarName=="RJT")
  {
    Value=getConstValue(this,fbDB,"MOL_JOB_TITLE",rwDate);
  }
  else if(VarName=="RFIO")
  {
    Value=getConstValue(this,fbDB,"MOL_FIO",rwDate);
  }
  else if(VarName=="SJT")
  {
    Value=getConstValue(this,fbDB,"WORKMAN_JOB_TITLE",rwDate);
  }
  else if(VarName=="SFIO")
  {
    Value=getConstValue(this,fbDB,"WORKMAN_FIO",rwDate);
  }
}
//---------------------------------------------------------------------------
void __fastcall TArrivalNMPFrame::viewBySDItemClick(TObject *Sender)
{
TViewNMPBySDForm *VSD;
AnsiString id;
AnsiString nmp_name=nmpFrame->getGridFrame()->getFieldDataOfSelectedRecord("NMP_NAME");

  if(groupOnPublishing->Checked)
    id=nmpFrame->getGridFrame()->getFieldDataOfSelectedRecord("NMP_ID");
  else
    id=nmpFrame->getGridFrame()->getFieldDataOfSelectedRecord("E_ID");
  try
  {
    VSD=new TViewNMPBySDForm(this,iniFileName,fbDB,1,id,date1,date2,nmp_name);
  }
  catch(const Exception &exc)
  { MessageDlg("Ошибка создания формы TViewNMPBySDForm\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return;
  }
  VSD->ShowModal();
  delete VSD;
}
//---------------------------------------------------------------------------

