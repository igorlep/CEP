//---------------------------------------------------------------------------
//Справка о товародвижении по подразделениям (почтамтам)
#include <vcl.h>
#pragma hdrstop

#include "DBClasses.h"
#include "DifferentFunctions.h"
#include "SomeFunctions.h"
#include "GridF.h"
#include "NextGridF.h"
#include "UniversalSelectPeriod.h"
#include "AnaliticalReportsFunctions.h"
#include "ReferenceOnMGForSDF.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "JvComponent"
#pragma link "JvExControls"
#pragma link "JvNavigationPane"
#pragma link "frxClass"
#pragma link "frxDBSet"
#pragma link "frxExportXLS"
#pragma link "JvArrowButton"
#pragma link "JvSpeedButton"
#pragma link "JvXPCheckCtrls"
#pragma link "JvXPCore"
#pragma resource "*.dfm"
//TReferenceOnMGForSDFrame *ReferenceOnMGForSDFrame;
//---------------------------------------------------------------------------
//**********
//* public *
//**********
__fastcall TReferenceOnMGForSDFrame::TReferenceOnMGForSDFrame(TComponent* Owner,AnsiString iniFN,FBDatabase *fbdb)
        : TFrame(Owner),iniFileName(iniFN),fbDB(fbdb)
{
  workDir=getAppDir();
  iniSectionName="ReferenceOnMGForSD";
  dataBase=fbDB->getDatabase();
  lFrame=NULL;
  resultFrame=NULL;
  date1=Date();
  date2=Date();
  loadProperties();
}
//---------------------------------------------------------------------------
__fastcall TReferenceOnMGForSDFrame::~TReferenceOnMGForSDFrame(void)
{
  saveProperties();
  if(lFrame!=NULL){ delete lFrame; lFrame=NULL; }
  if(resultFrame!=NULL){ delete resultFrame; resultFrame=NULL; }
}
//---------------------------------------------------------------------------
void __fastcall TReferenceOnMGForSDFrame::initFrame(void)
{
  //Инициируем фрейм подразделение (почтамтов)
  if(lFrame!=NULL){ delete lFrame; lFrame=NULL; }
  if(resultFrame!=NULL){ delete resultFrame; resultFrame=NULL; }
  if(groupOnPostOffices->Checked==false)
    lFrame=cteateSDFrame(this,iniFileName,iniSectionName+"_sdFrame",fbDB,lPanel);
  else
    lFrame=cteatePOFrame(this,iniFileName,iniSectionName+"_poFrame",fbDB,lPanel);
  if(lFrame!=NULL)
  { lFrame->getGridFrame()->workGrid->OnMouseDown=sdGridMouseDown;
    lFrame->getGridFrame()->workSource->OnDataChange=sdSourceDataChange;
    lGrid=lFrame->getGridFrame()->workGrid;
    lDS=lFrame->getGridFrame()->getWorkDataSet();
  }
  else
  { lGrid=NULL;
    lDS=NULL;
  }
  resultFrame=cteateGMForSDFrame(this,iniFileName,iniSectionName+"_resultFrame",fbDB,resultPanel,SEdit,ConditionSBox);
  if(resultFrame!=NULL)
  { resultFrame->workGrid->OnMouseDown=resultGridMouseDown;
    resultFrame->setStatusBar(resultBar);
    resultFrame->setHeader(resultHeader);
  }
  else return;
  loadDataSets();
//  setHeader();
}
//---------------------------------------------------------------------------

//***********
//* private *
//***********
void __fastcall TReferenceOnMGForSDFrame::loadProperties(void)
{
TIniFile *ini = new TIniFile(iniFileName);

  lPanel->Width=ini->ReadInteger(iniSectionName,"lPanel_Width",250);
  date1=ini->ReadDateTime(iniSectionName,"date1",date1);
  date2=ini->ReadDateTime(iniSectionName,"date2",date2);
  periodBox->ItemIndex=ini->ReadInteger(iniSectionName,"periodBox_ItemIndex",0);
  groupOnPostOffices->Checked=ini->ReadBool(iniSectionName,"groupOnPostOffices_Checked",false);
  delete ini;
}
//---------------------------------------------------------------------------
void __fastcall TReferenceOnMGForSDFrame::saveProperties(void)
{
TIniFile *ini = new TIniFile(iniFileName);

  ini->WriteInteger(iniSectionName,"lPanel_Width",lPanel->Width);
  ini->WriteDateTime(iniSectionName,"date1",date1);
  ini->WriteDateTime(iniSectionName,"date2",date2);
  ini->WriteInteger(iniSectionName,"periodBox_ItemIndex",periodBox->ItemIndex);
  ini->WriteBool(iniSectionName,"groupOnPostOffices_Checked",groupOnPostOffices->Checked);
  delete ini;
}
//---------------------------------------------------------------------------
void __fastcall TReferenceOnMGForSDFrame::setHeader(void)
{
AnsiString s;

  s="Справка о товародвижении (по";
  if(groupOnPostOffices->Checked==false) s+=" подразделениям)";
  else s+=" почтамтам)";
  header->Caption=SetHeader(s,date1,date2);
}
//---------------------------------------------------------------------------
void __fastcall TReferenceOnMGForSDFrame::setResultHeader(void)
{
AnsiString str="";

  if(lFrame!=NULL)
  { if(groupOnPostOffices->Checked==false)
     str=lFrame->getGridFrame()->getFieldDataOfSelectedRecord("SD_NAME");
    else
     str=lFrame->getGridFrame()->getFieldDataOfSelectedRecord("PO_NAME");
  }
  resultHeader->Caption=str;
}
//---------------------------------------------------------------------------
void __fastcall TReferenceOnMGForSDFrame::setFocus(void)
{
  if(activeFrame==1)
  {
    if(lFrame!=NULL) lFrame->getGridFrame()->workGrid->SetFocus();
  }
  else if(activeFrame==2)
  {
    if(resultFrame!=NULL) resultFrame->workGrid->SetFocus();
  }
}
//---------------------------------------------------------------------------
void __fastcall TReferenceOnMGForSDFrame::loadDataSets(void)
{
  loadSDDS();
  setFocus();
}
//---------------------------------------------------------------------------
void __fastcall TReferenceOnMGForSDFrame::loadSDDS(void)
{
  if(lFrame!=NULL)
  { lFrame->getGCFrame()->loadDS(formSD_POSelectSQL(groupOnPostOffices->Checked,date1,date2));
    if(groupOnPostOffices->Checked==false) lFrame->getContentFrame()->setContent();
    if(resultFrame!=NULL) loadResultDS();
    setHeader();
  }
}
//---------------------------------------------------------------------------
void __fastcall TReferenceOnMGForSDFrame::loadResultDS(void)
{
  if(resultFrame!=NULL)
  { AnsiString id;
    if(groupOnPostOffices->Checked==false)
      id=lFrame->getGridFrame()->getFieldDataOfSelectedRecord("SD_ID");
    else
      id=lFrame->getGridFrame()->getFieldDataOfSelectedRecord("PO_ID");
    AnsiString sql=formGMForSDSelectSQL(id,date1,date2,periodBox->ItemIndex,groupOnPostOffices->Checked);
    resultFrame->initSelectSQL(sql);
    resultFrame->loadDS();
    setResultHeader();
  }
}
//---------------------------------------------------------------------------
void __fastcall TReferenceOnMGForSDFrame::loadResultSearchDS(void)
{
  if(resultFrame!=NULL)
  { AnsiString id;
    if(groupOnPostOffices->Checked==false)
      id=lFrame->getGridFrame()->getFieldDataOfSelectedRecord("SD_ID");
    else
      id=lFrame->getGridFrame()->getFieldDataOfSelectedRecord("PO_ID");
    AnsiString sql=formGMRorSDSearchSQL(id,date1,date2,periodBox->ItemIndex,groupOnPostOffices->Checked);
    resultFrame->initSearchSQL(sql);
    resultFrame->loadSearchingDS();
  }
}
//---------------------------------------------------------------------------

//*************
//* published *
//*************
//Поиск по модели
void __fastcall TReferenceOnMGForSDFrame::searchTimerTimer(TObject *Sender)
{
  searchTimer->Enabled=false;
  if(activeFrame==2)
  {
    loadResultSearchDS();
  }
}
//---------------------------------------------------------------------------
void __fastcall TReferenceOnMGForSDFrame::SEditChange(TObject *Sender)
{
  if(searchTimer->Enabled==true) searchTimer->Enabled=false;
  if(!SEdit->Text.IsEmpty()) searchTimer->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TReferenceOnMGForSDFrame::SEditKeyDown(TObject *Sender,WORD &Key,TShiftState Shift)
{
  if(Key==VK_RETURN || Key==VK_DOWN || Key==VK_UP)
  { Key=0;
    if(activeFrame==2)
    {
      if(resultFrame!=NULL) resultFrame->workGrid->SetFocus();
    }
  }
  else if(Key == VK_ESCAPE){ Key=0; SEdit->Text=""; }
}
//---------------------------------------------------------------------------
void __fastcall TReferenceOnMGForSDFrame::ConditionSBoxClick(
      TObject *Sender)
{
  if(activeFrame==2)
  {
    if(resultFrame!=NULL) loadResultSearchDS();
  }
}
//---------------------------------------------------------------------------
void __fastcall TReferenceOnMGForSDFrame::FSButtonClick(TObject *Sender)
{
  if(activeFrame==2)
  {
    if(resultFrame!=NULL) resultFrame->forwardSearch();
  }
}
//---------------------------------------------------------------------------
void __fastcall TReferenceOnMGForSDFrame::BSButtonClick(TObject *Sender)
{
  if(activeFrame==2)
  {
    if(resultFrame!=NULL) resultFrame->backwardSearch();
  }
}
//---------------------------------------------------------------------------
//
void __fastcall TReferenceOnMGForSDFrame::sdGridMouseDown(TObject *Sender,TMouseButton Button, TShiftState Shift, int X, int Y)
{
  activeFrame=1;
}
//---------------------------------------------------------------------------
void __fastcall TReferenceOnMGForSDFrame::sdSourceDataChange(TObject *Sender,TField *Field)
{
  if(groupOnPostOffices->Checked==false && lFrame!=NULL)
    lFrame->getContentFrame()->setContent();
  if(resultFrame!=NULL)
  { loadResultDS();
    if(resultFrame->isSearchDSActive()==true) loadResultSearchDS();
  }
}
//---------------------------------------------------------------------------
void __fastcall TReferenceOnMGForSDFrame::resultGridMouseDown(TObject *Sender,TMouseButton Button, TShiftState Shift, int X, int Y)
{
  activeFrame=2;
}
//---------------------------------------------------------------------------
void __fastcall TReferenceOnMGForSDFrame::periodButtonClick(
      TObject *Sender)
{
TUniversalSelectPeriodForm *USPF;

  try
  {
    USPF=new TUniversalSelectPeriodForm(this,iniFileName,"UniversalSelectPeriodForm_ShipmentNMPFrame","Сформировать отчет за период",date1,date2);
  }
  catch (Exception &exc)
  { MessageDlg("Ошибка создания формы UniversalSelectPeriodForm\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return;
  }
  if(USPF->ShowModal()==mrOk)
  { loadDataSets();
//    setHeader();
  }
  delete USPF;
}
//---------------------------------------------------------------------------
void __fastcall TReferenceOnMGForSDFrame::groupOnPostOfficesClick(
      TObject *Sender)
{
  initFrame();
}
//---------------------------------------------------------------------------
void __fastcall TReferenceOnMGForSDFrame::periodBoxChange(TObject *Sender)
{
  loadResultDS();
}
//---------------------------------------------------------------------------
//Печать отчетов
void __fastcall TReferenceOnMGForSDFrame::frxReportGetValue(const AnsiString VarName, Variant &Value)
{
  if(VarName=="Title")
  { AnsiString s;
    s="Справка о товародвижении (по";
    if(groupOnPostOffices->Checked==false) s+=" подразделениям)";
    else s+=" почтамтам)";
    s=SetHeader(s,date1,date2);
    if(groupOnPostOffices->Checked==false)
      s+="\n"+lFrame->getGridFrame()->getFieldDataOfSelectedRecord("SD_NAME");
    else
      s+="\n"+lFrame->getGridFrame()->getFieldDataOfSelectedRecord("PO_NAME");
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
    s="Справка о товародвижении по";
    if(groupOnPostOffices->Checked==false) s+=" подразделениям";
    else s+=" почтамтам";
    s=SetHeader(s+" (групповой отчет)",date1,date2);
    Value=s;
  }
}
//---------------------------------------------------------------------------
//Простой отчет
void __fastcall TReferenceOnMGForSDFrame::simpleItemClick(TObject *Sender)
{
  if(lFrame==NULL || resultFrame==NULL || resultFrame->getWorkDataSet()->RecordCount==0) return;
  TDBGridEh *lGrid=lFrame->getGridFrame()->workGrid;
  TpFIBDataSet *lDS=lFrame->getGridFrame()->getWorkDataSet();
  TpFIBDataSet *rDS=resultFrame->getWorkDataSet();

  frxXLSExport->DefaultPath=workDir+"Out\\";
  AnsiString s="Справка о товародвижении (по";
  if(groupOnPostOffices->Checked==false) s+=" подразделениям)";
  else s+=" почтамтам)";
  frxXLSExport->FileName=s+".xls";
  if(frxReport->LoadFromFile(workDir+"Reports\\MovementOfGoodsReport.fr3")==false)
  { MessageDlg("Не могу загрузить файл с отчетом: "+workDir+"Reports\\MovementOfGoodsReport.fr3",mtWarning,TMsgDlgButtons() << mbOK,0);
    return;
  }
  frxDS->DataSet=rDS;

  if(lGrid->SelectedRows->Count==0) lGrid->Selection->Rows->CurrentRowSelected=true;
  for(int i=0; i<lGrid->SelectedRows->Count; ++i)
  { lDS->GotoBookmark((void *)lGrid->SelectedRows->Items[i].c_str());
    if(i==0) frxReport->PrepareReport(true);
    else frxReport->PrepareReport(false);
  }
  frxReport->ShowPreparedReport();
}
//---------------------------------------------------------------------------
//Групповой отчет
void __fastcall TReferenceOnMGForSDFrame::groupItemClick(TObject *Sender)
{
FBDataSet *fbDS=NULL;
AnsiString sql;

  if(lFrame==NULL || resultFrame==NULL || resultFrame->getWorkDataSet()->RecordCount==0) return;
  frxXLSExport->DefaultPath=workDir+"Out\\";
  AnsiString s="Справка о товародвижении (по";
  if(groupOnPostOffices->Checked==false) s+=" подразделениям)";
  else s+=" почтамтам)";
  frxXLSExport->FileName=s+" (групповой отчет).xls";
  if(frxReport->LoadFromFile(workDir+"Reports\\MovementOfGoodsForSDReport.fr3")==false)
  { MessageDlg("Не могу загрузить файл с отчетом: "+workDir+"Reports\\MovementOfGoodsForSDReport.fr3",mtWarning,TMsgDlgButtons() << mbOK,0);
    goto end;
  }
  fbDS=newFBDataSet(this,"TReferenceOnMovementOfGoodsFrame::groupItemClick",dataBase,true);
  if(fbDS==NULL) goto end;
  sql="select * from MOVEMENT_OF_GOODS_FOR_SD(";
  if(groupOnPostOffices->Checked==false) sql+="1,";
  else sql+="2,";
  sql+="'"+date1.DateString()+"',";
  sql+="'"+date2.DateString()+"',";
  sql+=IntToStr(periodBox->ItemIndex)+")";
  if(fbDS->execSelectSQL("",sql)==false) goto end;
  frxDS->DataSet=fbDS->getDataSet();
  frxReport->ShowReport(true);
end:
  if(fbDS!=NULL) delete fbDS;
}
//---------------------------------------------------------------------------

