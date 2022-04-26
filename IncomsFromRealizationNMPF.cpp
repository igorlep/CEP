//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DBClasses.h"
#include "DifferentFunctions.h"
#include "SomeFunctions.h"
#include "NextGridF.h"
#include "TWFunctions.h"
#include "RBWorkFunctions.h"
#include "UniversalSelectPeriod.h"
#include "AnaliticalReportsFunctions.h"
#include "IncomsFromRealizationNMPF.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "JvComponent"
#pragma link "JvExControls"
#pragma link "JvNavigationPane"
#pragma link "JvArrowButton"
#pragma link "JvSpeedButton"
#pragma link "JvXPCheckCtrls"
#pragma link "JvXPCore"
#pragma link "frxClass"
#pragma link "frxDBSet"
#pragma link "frxExportRTF"
#pragma link "frxExportXLS"
#pragma resource "*.dfm"
//TIncomsFromRealizationNMPFrame *IncomsFromRealizationNMPFrame;
//---------------------------------------------------------------------------
//**********
//* public *
//**********
__fastcall TIncomsFromRealizationNMPFrame::TIncomsFromRealizationNMPFrame(TComponent* Owner,AnsiString iniFN,FBDatabase *fbdb)
        : TFrame(Owner),iniFileName(iniFN),fbDB(fbdb)
{
  workDir=getAppDir();
  iniSectionName="IncomsFromRealizationNMP";
  dataBase=fbDB->getDatabase();
  pFrame=NULL;
  resultFrame=NULL;
}
//---------------------------------------------------------------------------
__fastcall TIncomsFromRealizationNMPFrame::~TIncomsFromRealizationNMPFrame(void)
{
  saveProperties();

  if(resultFrame!=NULL){ delete resultFrame; resultFrame=NULL; }
  if(pFrame!=NULL){ delete pFrame; pFrame=NULL; }
}
//---------------------------------------------------------------------------
void __fastcall TIncomsFromRealizationNMPFrame::initFrame(void)
{
  //Инициируем фрейм поставщиков
  pFrame=createTempPublishersFrame(this,iniFileName,iniSectionName+"_pFrame",fbDB,pPanel,SEdit,ConditionSBox);
  if(pFrame!=NULL)
  { pFrame->getGridFrame()->workGrid->OnMouseDown=pGridMouseDown;
    pFrame->getGridFrame()->workSource->OnDataChange=pSourceDataChange;
  }
  else return;
  resultFrame=cteateIncomsFromRealizationFrame(this,iniFileName,iniSectionName+"_resultFrame",fbDB,resultPanel,SEdit,ConditionSBox);
  if(resultFrame!=NULL)
  { resultFrame->workGrid->OnMouseDown=resultGridMouseDown;
    resultFrame->setStatusBar(resultBar);
    resultFrame->setHeader(resultHeader);
  }

  loadProperties();
  loadDataSets();
}
//---------------------------------------------------------------------------

//***********
//* private *
//***********
void __fastcall TIncomsFromRealizationNMPFrame::loadProperties(void)
{
TIniFile *ini = new TIniFile(iniFileName);

  pPanel->Width=ini->ReadInteger(iniSectionName,"pPanel_Width",200);
  activeFrame=ini->ReadInteger(iniSectionName,"activeFrame",1);
  date1=ini->ReadDate(iniSectionName,"date1",TDateTime(Date().DateString()));
  date2=ini->ReadDate(iniSectionName,"date2",TDateTime(Date().DateString()));
  delete ini;
  setHeader();
}
//---------------------------------------------------------------------------
void __fastcall TIncomsFromRealizationNMPFrame::saveProperties(void)
{
TIniFile *ini = new TIniFile(iniFileName);

  ini->WriteInteger(iniSectionName,"pPanel_Width",pPanel->Width);
  ini->WriteInteger(iniSectionName,"activeFrame",activeFrame);
  ini->WriteDate(iniSectionName,"date1",date1);
  ini->WriteDate(iniSectionName,"date2",date2);
  delete ini;
}
//---------------------------------------------------------------------------
void __fastcall TIncomsFromRealizationNMPFrame::setHeader(void)
{
  header->Caption=SetHeader("Доходы от реализации газетно-журнальной продукции",date1,date2);
}
//---------------------------------------------------------------------------
void __fastcall  TIncomsFromRealizationNMPFrame::setResultHeader(void)
{
AnsiString str;

  str="Поставшик - ";
  if(pFrame!=NULL) str+=pFrame->getGridFrame()->getFieldDataOfSelectedRecord("P_NAME");
  resultHeader->Caption=str;
}
//---------------------------------------------------------------------------
void __fastcall TIncomsFromRealizationNMPFrame::setFocus(void)
{
  if(activeFrame==1)
  {
    if(pFrame!=NULL) pFrame->getGridFrame()->workGrid->SetFocus();
  }
  else if(activeFrame==2)
  {
    if(resultFrame!=NULL) resultFrame->workGrid->SetFocus();
  }
}
//---------------------------------------------------------------------------
void __fastcall TIncomsFromRealizationNMPFrame::loadDataSets(void)
{
  loadPDS();
  resultHeader->Caption="Поставщик - "+pFrame->getGridFrame()->getFieldDataOfSelectedRecord("P_NAME");
  setFocus();
}
//---------------------------------------------------------------------------
void __fastcall TIncomsFromRealizationNMPFrame::loadPSearchDS(void)
{
  if(pFrame==NULL) return;

  bool viewDelRecords=pFrame->getViewDelRecordsFLG();
  AnsiString sql=formPublishersSearchSQL(viewDelRecords);
  pFrame->getGridFrame()->initSearchSQL(sql);
  pFrame->getGridFrame()->loadSearchingDS();
}
//---------------------------------------------------------------------------
void __fastcall TIncomsFromRealizationNMPFrame::loadPDS(void)
{
  if(pFrame!=NULL)
  { AnsiString sql=formPublishersSelectSQL(pFrame->getViewDelRecordsFLG());
    pFrame->getGCFrame()->loadDS(sql);
    pFrame->getContentFrame()->setContent();
    loadResultDS();
  }
}
//---------------------------------------------------------------------------
void __fastcall TIncomsFromRealizationNMPFrame::loadResultDS(void)
{
  if(resultFrame!=NULL)
  { AnsiString pid=pFrame->getGridFrame()->getFieldDataOfSelectedRecord("P_ID");
    AnsiString sql=formIncomsFromRealizationSelectSQL(pid,date1,date2);
    resultFrame->initSelectSQL(sql);
    resultFrame->loadDS();
  }
}
//---------------------------------------------------------------------------
void __fastcall TIncomsFromRealizationNMPFrame::loadResultSearchDS(void)
{
  if(resultFrame!=NULL)
  { AnsiString pid=pFrame->getGridFrame()->getFieldDataOfSelectedRecord("P_ID");
    AnsiString sql=formIncomsFromRealizationSearchSQL(pid,date1,date2);
    resultFrame->initSearchSQL(sql);
    resultFrame->loadSearchingDS();
  }
}
//---------------------------------------------------------------------------

//*************
//* published *
//*************
void __fastcall TIncomsFromRealizationNMPFrame::searchTimerTimer(TObject *Sender)
{
  searchTimer->Enabled=false;
  if(activeFrame==1)
  {
    loadPSearchDS();
  }
  else if(activeFrame==2)
  {
    loadResultSearchDS();
  }
}
//---------------------------------------------------------------------------
void __fastcall TIncomsFromRealizationNMPFrame::SEditChange(TObject *Sender)
{
  if(searchTimer->Enabled==true) searchTimer->Enabled=false;
  if(!SEdit->Text.IsEmpty()) searchTimer->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TIncomsFromRealizationNMPFrame::SEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
  if(Key==VK_RETURN || Key==VK_DOWN || Key==VK_UP)
  { Key=0;
    if(activeFrame==1)
    {
      if(pFrame!=NULL && pFrame->getGridFrame()!=NULL) pFrame->getGridFrame()->workGrid->SetFocus();
    }
    else if(activeFrame==2)
    {
      if(resultFrame!=NULL) resultFrame->workGrid->SetFocus();
    }
  }
  else if(Key == VK_ESCAPE){ Key=0; SEdit->Text=""; }
}
//---------------------------------------------------------------------------
void __fastcall TIncomsFromRealizationNMPFrame::ConditionSBoxClick(TObject *Sender)
{
  if(activeFrame==1)
  {
    loadPSearchDS();
  }
  else if(activeFrame==2)
  {
    if(resultFrame!=NULL) loadResultSearchDS();
  }
}
//---------------------------------------------------------------------------
void __fastcall TIncomsFromRealizationNMPFrame::FSButtonClick(TObject *Sender)
{
  if(activeFrame==1)
  {
    if(pFrame!=NULL) pFrame->getGridFrame()->forwardSearch();
  }
  else if(activeFrame==2)
  {
    if(resultFrame!=NULL) resultFrame->forwardSearch();
  }
}
//---------------------------------------------------------------------------
void __fastcall TIncomsFromRealizationNMPFrame::BSButtonClick(TObject *Sender)
{
  if(activeFrame==1)
  {
    if(pFrame!=NULL) pFrame->getGridFrame()->backwardSearch();
  }
  else if(activeFrame==2)
  {
    if(resultFrame!=NULL) resultFrame->backwardSearch();
  }
}
//---------------------------------------------------------------------------

void __fastcall TIncomsFromRealizationNMPFrame::pGridMouseDown(TObject *Sender,TMouseButton Button, TShiftState Shift, int X, int Y)
{
  activeFrame=1;
}
//---------------------------------------------------------------------------
void __fastcall TIncomsFromRealizationNMPFrame::pSourceDataChange(TObject *Sender,TField *Field)
{
  setResultHeader();
  pFrame->getContentFrame()->setContent();
  if(resultFrame==NULL) return;
  { loadResultDS();
    if(resultFrame->isSearchDSActive()==true) loadResultSearchDS();
  }
}
//---------------------------------------------------------------------------
void __fastcall TIncomsFromRealizationNMPFrame::resultGridMouseDown(TObject *Sender,TMouseButton Button, TShiftState Shift, int X, int Y)
{
  activeFrame=2;
}
//---------------------------------------------------------------------------
//Выбор периода
void __fastcall TIncomsFromRealizationNMPFrame::periodButtonClick(TObject *Sender)
{
TUniversalSelectPeriodForm *USPF;

  try
  {
    USPF=new TUniversalSelectPeriodForm(this,iniFileName,iniSectionName,"Сформировать отчет за период",date1,date2);
  }
  catch (Exception &exc)
  { MessageDlg("Ошибка создания формы UniversalSelectPeriodForm\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return;
  }
  if(USPF->ShowModal()==mrOk)
  { setHeader();
    loadDataSets();
  }
  delete USPF;
}
//---------------------------------------------------------------------------
//Печать документа
void __fastcall TIncomsFromRealizationNMPFrame::frxReportGetValue(const AnsiString VarName, Variant &Value)
{
  if(VarName=="Title")
  { AnsiString s=Caption=SetHeader("Доходы от реализации газетно-журнальной продукции",date1,date2);
    s+="\n"+pFrame->getGridFrame()->getFieldDataOfSelectedRecord("P_NAME");
    Value=s;
  }
  else if(VarName=="Title1")
  { AnsiString s=Caption=SetHeader("Доходы от реализации газетно-журнальной продукции",date1,date2);
    Value=s;
  }
}
//---------------------------------------------------------------------------
//Печать простого отчета
void __fastcall TIncomsFromRealizationNMPFrame::simpleItemClick(TObject *Sender)
{
  if(pFrame==NULL || resultFrame==NULL || resultFrame->getWorkDataSet()->RecordCount==0) return;
  TDBGridEh *pGrid=pFrame->getGridFrame()->workGrid;
  TpFIBDataSet *pDS=pFrame->getGridFrame()->getWorkDataSet();
  TpFIBDataSet *rDS=resultFrame->getWorkDataSet();

  frxXLSExport->DefaultPath=workDir+"Out\\";
  frxXLSExport->FileName="Доходы от реализации ГЖП.xls";
  if(frxReport->LoadFromFile(workDir+"Reports\\IncomFromRealizationReport.fr3")==false)
  { MessageDlg("Не могу загрузить файл с отчетом: "+workDir+"Reports\\IncomFromRealizationReport.fr3",mtWarning,TMsgDlgButtons() << mbOK,0);
    return;
  }
  frxDS->DataSet=rDS;

  if(pGrid->SelectedRows->Count==0) pGrid->Selection->Rows->CurrentRowSelected=true;
  for(int i=0; i<pGrid->SelectedRows->Count; ++i)
  { pDS->GotoBookmark((void *)pGrid->SelectedRows->Items[i].c_str());
    if(i==0) frxReport->PrepareReport(true);
    else frxReport->PrepareReport(false);
  }
  frxReport->ShowPreparedReport();
}
//---------------------------------------------------------------------------
//Печать группового отчета
void __fastcall TIncomsFromRealizationNMPFrame::groupItemClick(TObject *Sender)
{
FBDataSet *fbDS=NULL;
AnsiString sql;

  if(pFrame==NULL || resultFrame==NULL || resultFrame->getWorkDataSet()->RecordCount==0) return;
  frxXLSExport->DefaultPath=workDir+"Out\\";
  frxXLSExport->FileName="Доходы от реализации ГЖП (групповой отчет).xls";
  if(frxReport->LoadFromFile(workDir+"Reports\\IncomFromRealizForPubReport.fr3")==false)
  { MessageDlg("Не могу загрузить файл с отчетом: "+workDir+"Reports\\IncomFromRealizForPubReport.fr3",mtWarning,TMsgDlgButtons() << mbOK,0);
    goto end;
  }
  fbDS=newFBDataSet(this,"TIncomsFromRealizationNMPFrame::groupItemClick",dataBase,true);
  if(fbDS==NULL) goto end;
  sql="select * from INCOM_FROM_RALIZ_FOR_PUB(";
  sql+="'"+date1.DateString()+"',";
  sql+="'"+date2.DateString()+"')";
  if(fbDS->execSelectSQL("",sql)==false) goto end;
  frxDS->DataSet=fbDS->getDataSet();
  frxReport->ShowReport(true);
end:
  if(fbDS!=NULL) delete fbDS;
}
//---------------------------------------------------------------------------

