//---------------------------------------------------------------------------
//Справка о товародвижении по поставщикам
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
#include "ReferenceOnMovementOfGoodsF.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "JvComponent"
#pragma link "JvExControls"
#pragma link "JvNavigationPane"
#pragma link "JvSpeedButton"
#pragma link "JvXPCheckCtrls"
#pragma link "JvXPCore"
#pragma link "frxClass"
#pragma link "frxDBSet"
#pragma link "frxExportXLS"
#pragma link "JvArrowButton"
#pragma resource "*.dfm"
//TReferenceOnMovementOfGoodsFrame *ReferenceOnMovementOfGoodsFrame;
//---------------------------------------------------------------------------
//**********
//* public *
//**********
__fastcall TReferenceOnMovementOfGoodsFrame::TReferenceOnMovementOfGoodsFrame(TComponent* Owner,AnsiString iniFN,FBDatabase *fbdb)
        : TFrame(Owner),iniFileName(iniFN),fbDB(fbdb)
{
  workDir=getAppDir();
  iniSectionName="ReferenceOnMovementOfGoods";
  dataBase=fbDB->getDatabase();
  pFrame=NULL;
  resultFrame=NULL;
}
//---------------------------------------------------------------------------
__fastcall TReferenceOnMovementOfGoodsFrame::~TReferenceOnMovementOfGoodsFrame(void)
{
  saveProperties();
  if(pFrame!=NULL){ delete pFrame; pFrame=NULL; }
  if(resultFrame!=NULL){ delete resultFrame; resultFrame=NULL; }
}
//---------------------------------------------------------------------------
void __fastcall TReferenceOnMovementOfGoodsFrame::initFrame(void)
{
//  if(resultFrame!=NULL){ delete resultFrame; resultFrame=NULL; }
//  if(pFrame!=NULL){ delete pFrame; pFrame=NULL; }
  //Инициируем фрейм поставщиков
  pFrame=createTempPublishersFrame(this,iniFileName,iniSectionName+"_pFrame",fbDB,pPanel,SEdit,ConditionSBox);
  if(pFrame!=NULL)
  { pFrame->getGridFrame()->workGrid->OnMouseDown=pGridMouseDown;
    pFrame->getGridFrame()->workSource->OnDataChange=pSourceDataChange;
  }
  else return;
  resultFrame=cteateReferenceOnMovementOfGoodsFrame(this,iniFileName,iniSectionName+"_resultFrame",fbDB,resultPanel,SEdit,ConditionSBox);
  if(resultFrame!=NULL)
  { resultFrame->workGrid->OnMouseDown=resultGridMouseDown;
    resultFrame->setStatusBar(resultBar);
    resultFrame->setHeader(resultHeader);
  }
  else return;
  loadProperties();
  loadDataSets();
}
//---------------------------------------------------------------------------

//***********
//* private *
//***********
void __fastcall TReferenceOnMovementOfGoodsFrame::loadProperties(void)
{
TIniFile *ini = new TIniFile(iniFileName);

  pPanel->Width=ini->ReadInteger(iniSectionName,"pPanel_Width",200);
  activeFrame=ini->ReadInteger(iniSectionName,"activeFrame",1);
  date1=ini->ReadDate(iniSectionName,"date1",TDateTime(Date().DateString()));
  date2=ini->ReadDate(iniSectionName,"date2",TDateTime(Date().DateString()));
  periodBox->ItemIndex=ini->ReadInteger(iniSectionName,"periodBox_ItemIndex",0);
  resultBox->ItemIndex=ini->ReadInteger(iniSectionName,"resultBox_ItemIndex",1);
  delete ini;
  setHeader();
}
//---------------------------------------------------------------------------
void __fastcall TReferenceOnMovementOfGoodsFrame::saveProperties(void)
{
TIniFile *ini = new TIniFile(iniFileName);

  ini->WriteInteger(iniSectionName,"pPanel_Width",pPanel->Width);
  ini->WriteInteger(iniSectionName,"activeFrame",activeFrame);
  ini->WriteDate(iniSectionName,"date1",date1);
  ini->WriteDate(iniSectionName,"date2",date2);
  ini->WriteInteger(iniSectionName,"periodBox_ItemIndex",periodBox->ItemIndex);
  ini->WriteInteger(iniSectionName,"resultBox_ItemIndex",resultBox->ItemIndex);
  delete ini;
}
//---------------------------------------------------------------------------
void __fastcall TReferenceOnMovementOfGoodsFrame::setHeader(void)
{
  header->Caption=SetHeader("Справка о товародвижении (по поставщикам)",date1,date2);
}
//---------------------------------------------------------------------------
void __fastcall TReferenceOnMovementOfGoodsFrame::setResultHeader(void)
{
AnsiString str;

  str="Поставшик - ";
  if(pFrame!=NULL) str+=pFrame->getGridFrame()->getFieldDataOfSelectedRecord("P_NAME");
  resultHeader->Caption=str;
}
//---------------------------------------------------------------------------
void __fastcall TReferenceOnMovementOfGoodsFrame::setFocus(void)
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
void __fastcall TReferenceOnMovementOfGoodsFrame::loadDataSets(void)
{
  loadPDS();
  setFocus();
}
//---------------------------------------------------------------------------
void __fastcall TReferenceOnMovementOfGoodsFrame::loadPSearchDS(void)
{
  if(pFrame==NULL) return;

  bool viewDelRecords=pFrame->getViewDelRecordsFLG();
  AnsiString sql=formPublishersSearchSQL(viewDelRecords);
  pFrame->getGridFrame()->initSearchSQL(sql);
  pFrame->getGridFrame()->loadSearchingDS();
}
//---------------------------------------------------------------------------
void __fastcall TReferenceOnMovementOfGoodsFrame::loadPDS(void)
{
  if(pFrame!=NULL)
  { AnsiString sql=formPublishersSelectSQL(pFrame->getViewDelRecordsFLG());
    pFrame->getGCFrame()->loadDS(sql);
    pFrame->getContentFrame()->setContent();
    if(resultFrame!=NULL) loadResultDS();
    resultHeader->Caption="Поставщик - "+pFrame->getGridFrame()->getFieldDataOfSelectedRecord("P_NAME");
  }
}
//---------------------------------------------------------------------------
void __fastcall TReferenceOnMovementOfGoodsFrame::loadResultDS(void)
{
  if(resultFrame!=NULL)
  { AnsiString pid=pFrame->getGridFrame()->getFieldDataOfSelectedRecord("P_ID");
    AnsiString sql=formMovementOfGoodsSelectSQL(pid,date1,date2,periodBox->ItemIndex,resultBox->ItemIndex);
    resultFrame->initSelectSQL(sql);
    resultFrame->loadDS();
  }
}
//---------------------------------------------------------------------------
void __fastcall TReferenceOnMovementOfGoodsFrame::loadResultSearchDS(void)
{
  if(resultFrame!=NULL)
  { AnsiString pid=pFrame->getGridFrame()->getFieldDataOfSelectedRecord("P_ID");
    AnsiString sql=formMovementOfGoodsSearchSQL(pid,date1,date2,periodBox->ItemIndex);
    resultFrame->initSearchSQL(sql);
    resultFrame->loadSearchingDS();
  }
}
//---------------------------------------------------------------------------

//*************
//* published *
//*************
void __fastcall TReferenceOnMovementOfGoodsFrame::searchTimerTimer(TObject *Sender)
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
void __fastcall TReferenceOnMovementOfGoodsFrame::SEditChange(TObject *Sender)
{
  if(searchTimer->Enabled==true) searchTimer->Enabled=false;
  if(!SEdit->Text.IsEmpty()) searchTimer->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TReferenceOnMovementOfGoodsFrame::SEditKeyDown(TObject *Sender,WORD &Key,TShiftState Shift)
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
void __fastcall TReferenceOnMovementOfGoodsFrame::ConditionSBoxClick(TObject *Sender)
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
void __fastcall TReferenceOnMovementOfGoodsFrame::FSButtonClick(TObject *Sender)
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
void __fastcall TReferenceOnMovementOfGoodsFrame::BSButtonClick(TObject *Sender)
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

void __fastcall TReferenceOnMovementOfGoodsFrame::pGridMouseDown(TObject *Sender,TMouseButton Button, TShiftState Shift, int X, int Y)
{
  activeFrame=1;
}
//---------------------------------------------------------------------------
void __fastcall TReferenceOnMovementOfGoodsFrame::pSourceDataChange(TObject *Sender,TField *Field)
{
  setResultHeader();
  pFrame->getContentFrame()->setContent();
  if(resultFrame==NULL) return;
  { loadResultDS();
    if(resultFrame->isSearchDSActive()==true) loadResultSearchDS();
  }
}
//---------------------------------------------------------------------------
void __fastcall TReferenceOnMovementOfGoodsFrame::resultGridMouseDown(TObject *Sender,TMouseButton Button, TShiftState Shift, int X, int Y)
{
  activeFrame=2;
}
//---------------------------------------------------------------------------
//Выбор периода
void __fastcall TReferenceOnMovementOfGoodsFrame::periodButtonClick(TObject *Sender)
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
void __fastcall TReferenceOnMovementOfGoodsFrame::periodBoxChange(
      TObject *Sender)
{
  loadResultDS();
}
//---------------------------------------------------------------------------
//Печать документа
void __fastcall TReferenceOnMovementOfGoodsFrame::frxReportGetValue(const AnsiString VarName,Variant &Value)
{
  if(VarName=="Title")
  { AnsiString s;
    if(resultBox->ItemIndex==0) //по данным подразделений
      s=SetHeader("Справка о товародвижении (по данным подразделений)",date1,date2);
    else
      s=SetHeader("Справка о товародвижении (по данным поставщиков)",date1,date2);
    s+="\n"+pFrame->getGridFrame()->getFieldDataOfSelectedRecord("P_NAME");
    Value=s;
  }
  else if(VarName=="LEADER_JT")
  {
    Value=getConstValue(this,fbDB,"LEADER_JOB_TITLE",date2);
  }
  else if(VarName=="LEADER_FIO")
  {
    Value=getConstValue(this,fbDB,"LEADER_FIO",date2);
  }
  else if(VarName=="Title1")
  { AnsiString s;
    if(resultBox->ItemIndex==0) //по данным подразделений
      s=SetHeader("Групповой отчет - Справка о товародвижении (по данным подразделений)",date1,date2);
    else
      s=SetHeader("Групповой отчет - Справка о товародвижении (по данным поставщиков)",date1,date2);
    Value=s;
  }
}
//---------------------------------------------------------------------------
//Простой отчет
void __fastcall TReferenceOnMovementOfGoodsFrame::simpleItemClick(TObject *Sender)
{
  if(pFrame==NULL || resultFrame==NULL || resultFrame->getWorkDataSet()->RecordCount==0) return;
  TDBGridEh *pGrid=pFrame->getGridFrame()->workGrid;
  TpFIBDataSet *pDS=pFrame->getGridFrame()->getWorkDataSet();
  TpFIBDataSet *rDS=resultFrame->getWorkDataSet();

  frxXLSExport->DefaultPath=workDir+"Out\\";
  frxXLSExport->FileName="Справка по товародвижению (по поставщикам).xls";
  if(frxReport->LoadFromFile(workDir+"Reports\\MovementOfGoodsReport.fr3")==false)
  { MessageDlg("Не могу загрузить файл с отчетом: "+workDir+"Reports\\MovementOfGoodsReport.fr3",mtWarning,TMsgDlgButtons() << mbOK,0);
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
//Групповой отчет
void __fastcall TReferenceOnMovementOfGoodsFrame::groupItemClick(TObject *Sender)
{
FBDataSet *fbDS=NULL;
AnsiString sql;

  if(pFrame==NULL || resultFrame==NULL || resultFrame->getWorkDataSet()->RecordCount==0) return;
  frxXLSExport->DefaultPath=workDir+"Out\\";
  if(resultBox->ItemIndex==0) //по данным подразделений
    frxXLSExport->FileName="Групповой отчет - Справка по товародвижению (по данным подразделений).xls";
  else  //по данным поставщиков
    frxXLSExport->FileName="Групповой отчет - Справка по товародвижению (по данным поставщиков).xls";
  if(frxReport->LoadFromFile(workDir+"Reports\\MovementOfGoodsForPubReport.fr3")==false)
  { MessageDlg("Не могу загрузить файл с отчетом: "+workDir+"Reports\\MovementOfGoodsForPubReport.fr3",mtWarning,TMsgDlgButtons() << mbOK,0);
    goto end;
  }
  fbDS=newFBDataSet(this,"TReferenceOnMovementOfGoodsFrame::groupItemClick",dataBase,true);
  if(fbDS==NULL) goto end;
  sql="select * from MOVEMENT_OF_GOODS_FOR_PEB(";
  sql+="'"+date1.DateString()+"',";
  sql+="'"+date2.DateString()+"',";
  sql+=IntToStr(periodBox->ItemIndex)+",";
  sql+=IntToStr(resultBox->ItemIndex)+")";
  sql+=" order by PERIOD,P_NAME";
  if(fbDS->execSelectSQL("",sql)==false) goto end;
  frxDS->DataSet=fbDS->getDataSet();
  frxReport->ShowReport(true);
end:
  if(fbDS!=NULL) delete fbDS;
}
//---------------------------------------------------------------------------
//При изменении типа результата
void __fastcall TReferenceOnMovementOfGoodsFrame::resultBoxChange(TObject *Sender)
{
  loadResultDS();
}
//---------------------------------------------------------------------------

