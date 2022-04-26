//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "ReturnNMPfromSDFunctions.h"
#include "DifferentFunctions.h"
#include "UniversalSelectPeriod.h"
#include "SomeFunctions.h"
#include "LoadDataAboutReturnNMPFromSD.h"
#include "ReturnNMPfromSDF.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "JvArrowButton"
#pragma link "JvComponent"
#pragma link "JvExControls"
#pragma link "JvNavigationPane"
#pragma link "JvSpeedButton"
#pragma link "JvXPCheckCtrls"
#pragma link "JvXPCore"
#pragma link "frxClass"
#pragma link "frxDBSet"
#pragma link "frxExportRTF"
#pragma link "frxExportXLS"
#pragma resource "*.dfm"
//TReturnNMPfromSDFrame *ReturnNMPfromSDFrame;
//---------------------------------------------------------------------------
//**********
//* public *
//**********
__fastcall TReturnNMPfromSDFrame::TReturnNMPfromSDFrame(TComponent* Owner,AnsiString iniFN,FBDatabase *fbdb)
        : TFrame(Owner),iniFileName(iniFN), fbDB(fbdb)
{
  workDir=getAppDir();
  iniSectionName="ReturnNMPfromSubdivisions";
  dataBase=fbDB->getDatabase();

  lFrame=NULL; rFrame=NULL;
  date1=TDateTime("01.05.2010");
  date2=TDateTime("31.05.2010");
  loadProperties();
}
//---------------------------------------------------------------------------
__fastcall TReturnNMPfromSDFrame::~TReturnNMPfromSDFrame(void)
{
  saveProperties();
  if(lFrame!=NULL){ delete lFrame; lFrame=NULL; }
  if(rFrame!=NULL){ delete rFrame; rFrame=NULL; }
}
//---------------------------------------------------------------------------
void __fastcall TReturnNMPfromSDFrame::initFrame(void)
{
  //Инициируем фрейм подразделение (почтамтов)
  if(lFrame!=NULL){ delete lFrame; lFrame=NULL; }
  if(groupOnPostOffices->Checked==false)
    lFrame=cteateReturnSubdivisionsFrame(this,iniFileName,iniSectionName+"_sdFrame",fbDB,sdPanel,NULL,NULL);
  else
    lFrame=cteateReturnPostOfficesFrame(this,iniFileName,iniSectionName+"_poFrame",fbDB,sdPanel,NULL,NULL);
  if(lFrame!=NULL)
  { lFrame->getGridFrame()->workGrid->OnMouseDown=sdGridMouseDown;
    lFrame->getGridFrame()->workSource->OnDataChange=sdSourceDataChange;
    lFrame->loadDS=&loadSubdivisionsDS;
    lGrid=lFrame->getGridFrame()->workGrid;
    lDS=lFrame->getGridFrame()->getWorkDataSet();
  }
  else
  { lGrid=NULL;
    lDS=NULL;
  }

  //Инициируем фрейм ГЖП
  if(rFrame!=NULL){ delete rFrame; rFrame=NULL; }
  if(groupOnPublishers->Checked==false)
  { if(groupOnPostOffices->Checked==false)
      rFrame=createReturnNMP_SDFrame(this,iniFileName,iniSectionName+"_nmpSDFrame",fbDB,nmpPanel,SEdit,ConditionSBox);
    else
      rFrame=createReturnNMP_POFrame(this,iniFileName,iniSectionName+"_nmpPOFrame",fbDB,nmpPanel,SEdit,ConditionSBox);
  }
  else
    rFrame=createReturnPFrame(this,iniFileName,iniSectionName+"_pFrame",fbDB,nmpPanel,SEdit,ConditionSBox);
  if(rFrame!=NULL)
  { rFrame->getGridFrame()->workGrid->OnMouseDown=nmpGridMouseDown;
    rFrame->getGridFrame()->getWorkDataSet()->AfterPost=nmpDataSetAfterPost;
    rFrame->getGridFrame()->workGrid->OnGetCellParams=nmpGridGetCellParams;
//    rFrame->loadDS=&loadNMPDS;
//    rFrame->loadSearchDS=&loadNMPSearchDS;
    rGrid=rFrame->getGridFrame()->workGrid;
    rDS=rFrame->getGridFrame()->getWorkDataSet();
  }
  else
  { rGrid=NULL;
    rDS=NULL;
  }

  loadDataSets();
  setHeader();
}
//---------------------------------------------------------------------------
void __fastcall TReturnNMPfromSDFrame::loadDataSets(void)
{
  loadSubdivisionsDS();
  setFocus();
}
//---------------------------------------------------------------------------
void __fastcall TReturnNMPfromSDFrame::setFocus(void)
{
  if(activeFrame==1)
  {
    if(lFrame!=NULL) lFrame->getGridFrame()->workGrid->SetFocus();
  }
  else if(activeFrame==2)
  {
    if(rFrame!=NULL) rFrame->getGridFrame()->workGrid->SetFocus();
  }
}
//---------------------------------------------------------------------------
AnsiString __fastcall TReturnNMPfromSDFrame::getRicipientID(void)
{
AnsiString rID;

  if(groupOnPostOffices->Checked==false)
    rID=lFrame->getGridFrame()->getFieldDataOfSelectedRecord("SD_ID");
  else
    rID=lFrame->getGridFrame()->getFieldDataOfSelectedRecord("PO_ID");
  return rID;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TReturnNMPfromSDFrame::getRicipientName(void)
{
AnsiString rName;

  if(groupOnPostOffices->Checked==false)
    rName=lFrame->getGridFrame()->getFieldDataOfSelectedRecord("SD_NAME");
  else
    rName=lFrame->getGridFrame()->getFieldDataOfSelectedRecord("PO_NAME");
  return rName;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TReturnNMPfromSDFrame::getRicipientMail(void)
{
AnsiString rMail;

  if(groupOnPostOffices->Checked==false)
    rMail=lFrame->getGridFrame()->getFieldDataOfSelectedRecord("SD_EMAIL");
  else
  { AnsiString sql;
    FBQuery *fbRQ=fbDB->getFBQ_T1();
    sql="select SD_EMAIL from SUBDIVISIONS_TABLE where";
    sql+=" PO_ID="+lFrame->getGridFrame()->getFieldDataOfSelectedRecord("PO_ID");
    sql+=" and SD_TYPE=1";
    if(fbRQ->execQuery("",sql)==true)
      rMail=fbRQ->getQuery()->FieldByName("SD_EMAIL")->AsString;
  }
  return rMail;
}
//---------------------------------------------------------------------------

//***********
//* private *
//***********
void __fastcall TReturnNMPfromSDFrame::loadProperties(void)
{
TIniFile *ini = new TIniFile(iniFileName);

  leftPanel->Width=ini->ReadInteger(iniSectionName,"leftPanel_Width",250);
  groupOnPostOffices->Checked=ini->ReadBool(iniSectionName,"groupOnPostOffices_Checked",false);
  groupOnPublishers->Checked=ini->ReadBool(iniSectionName,"groupOnPublishers_Checked",false);
  showAllRecords->Checked=ini->ReadBool(iniSectionName,"showAllRecords_Checked",false);
  activeFrame=ini->ReadInteger(iniSectionName,"activeFrame",1);
  date1=ini->ReadDateTime(iniSectionName,"date1",date1);
  date2=ini->ReadDateTime(iniSectionName,"date2",date2);
  delete ini;
//  if(groupOnPostOffices->Checked || groupOnPublishers->Checked) createMessagesButton->Enabled=false;
//  if(groupOnPublishers->Checked || showAllRecords->Checked) printButton->Enabled=false;
}
//---------------------------------------------------------------------------
void __fastcall TReturnNMPfromSDFrame::saveProperties(void)
{
TIniFile *ini = new TIniFile(iniFileName);

  ini->WriteInteger(iniSectionName,"leftPanel_Width",leftPanel->Width);
  ini->WriteBool(iniSectionName,"groupOnPostOffices_Checked",groupOnPostOffices->Checked);
  ini->WriteBool(iniSectionName,"groupOnPublishers_Checked",groupOnPublishers->Checked);
  ini->WriteBool(iniSectionName,"showAllRecords_Checked",showAllRecords->Checked);
  ini->WriteInteger(iniSectionName,"activeFrame",activeFrame);
  ini->WriteDateTime(iniSectionName,"date1",date1);
  ini->WriteDateTime(iniSectionName,"date2",date2);
  delete ini;
}
//---------------------------------------------------------------------------
void __fastcall TReturnNMPfromSDFrame::loadSubdivisionsDS(void)
{
  if(lFrame!=NULL)
  { lFrame->getGCFrame()->loadDS(formReturnSubdivisionsSelectSQL(groupOnPostOffices->Checked,showAllRecords->Checked,date1,date2));
    if(groupOnPostOffices->Checked==false) lFrame->getContentFrame()->setContent();
    if(rFrame!=NULL) loadNMPDS();
  }
}
//---------------------------------------------------------------------------
void __fastcall TReturnNMPfromSDFrame::loadNMPDS(void)
{
AnsiString sdID;

  rDS->Close();
  if(lFrame==NULL || lDS->RecordCount==0) return;
  if(groupOnPostOffices->Checked==false)
    sdID=lFrame->getGridFrame()->getFieldDataOfSelectedRecord("SD_ID");
  else
    sdID=lFrame->getGridFrame()->getFieldDataOfSelectedRecord("PO_ID");
  if(rFrame!=NULL)
  { rFrame->getGCFrame()->loadDS(formReturnNMPfromSDSelectSQL(sdID,groupOnPostOffices->Checked,groupOnPublishers->Checked,showAllRecords->Checked,date1,date2));
    if(groupOnPublishers->Checked==false) rFrame->getContentFrame()->setContent();
    setRHeader();
  }
  setButtons();
}
//---------------------------------------------------------------------------
void __fastcall TReturnNMPfromSDFrame::loadNMPSearchDS(void)
{
AnsiString sdID;

  if(lFrame==NULL || lDS->RecordCount==0) return;
  if(groupOnPostOffices->Checked==false)
    sdID=lFrame->getGridFrame()->getFieldDataOfSelectedRecord("SD_ID");
  else
    sdID=lFrame->getGridFrame()->getFieldDataOfSelectedRecord("PO_ID");

  if(rFrame!=NULL && rDS->RecordCount>0)
  { rFrame->getGridFrame()->initSearchSQL(formReturnNMPfromSDSearchSQL(sdID,showAllRecords->Checked,groupOnPublishers->Checked,date1,date2));
    rFrame->getGridFrame()->loadSearchingDS();
  }
}
//---------------------------------------------------------------------------
void __fastcall TReturnNMPfromSDFrame::setHeader(void)
{
  header->Caption=SetHeader("Возврат газетно-журнальной продукции из подразделений",date1,date2);
}
//---------------------------------------------------------------------------
void __fastcall TReturnNMPfromSDFrame::setRHeader(void)
{
AnsiString str="Подразделение - \"";

  if(groupOnPostOffices->Checked==false) str+=lFrame->getGridFrame()->getFieldDataOfSelectedRecord("SD_NAME");
  else str+=lFrame->getGridFrame()->getFieldDataOfSelectedRecord("PO_NAME");
  str+="\"";
  if(rFrame->getGridFrame()->getHeader()!=NULL) rFrame->getGridFrame()->getHeader()->Caption=str;
}
//---------------------------------------------------------------------------
void __fastcall TReturnNMPfromSDFrame::setButtons(void)
{
  if(rDS==NULL || rDS->RecordCount==0)
  { createMessagesButton->Enabled=false;
    printButton->Enabled=false;
    return;
  }
  if(groupOnPublishers->Checked==false)
  { if(groupOnPostOffices->Checked==false) createMessagesButton->Enabled=true;
    else createMessagesButton->Enabled=false;
    if(showAllRecords->Checked==false)
    { torg13Item->Enabled=true;
      torg13SelectItem->Enabled=true;
    }
    else
    { torg13Item->Enabled=false;
      torg13SelectItem->Enabled=false;
    }
  }
  else
  { createMessagesButton->Enabled=false;
    torg13Item->Enabled=false;
    torg13SelectItem->Enabled=false;
  }
}
//---------------------------------------------------------------------------

//*************
//* published *
//*************
void __fastcall TReturnNMPfromSDFrame::periodButtonClick(TObject *Sender)
{
TUniversalSelectPeriodForm *USPF;

  try
  {
    USPF=new TUniversalSelectPeriodForm(this,iniFileName,"UniversalSelectPeriodForm_ReturnNMPfromSDFrame","Сформировать отчет за период",date1,date2);
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

void __fastcall TReturnNMPfromSDFrame::searchTimerTimer(TObject *Sender)
{
  searchTimer->Enabled=false;
  if(activeFrame==2) loadNMPSearchDS();
}
//---------------------------------------------------------------------------
void __fastcall TReturnNMPfromSDFrame::SEditChange(TObject *Sender)
{
  if(searchTimer->Enabled==true) searchTimer->Enabled=false;
  if(!SEdit->Text.IsEmpty()) searchTimer->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TReturnNMPfromSDFrame::SEditKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
  if(Key==VK_RETURN || Key==VK_DOWN || Key==VK_UP)
  { Key=0;
    if(activeFrame==2 && rFrame!=NULL) rFrame->getGridFrame()->workGrid->SetFocus();
  }
  else if(Key == VK_ESCAPE){ Key=0; SEdit->Text=""; }
}
//---------------------------------------------------------------------------
void __fastcall TReturnNMPfromSDFrame::ConditionSBoxClick(TObject *Sender)
{
  if(activeFrame==2) loadNMPSearchDS();
}
//---------------------------------------------------------------------------
void __fastcall TReturnNMPfromSDFrame::FSButtonClick(TObject *Sender)
{
  if(activeFrame==2 && rFrame!=NULL) rFrame->getGridFrame()->forwardSearch();
}
//---------------------------------------------------------------------------
void __fastcall TReturnNMPfromSDFrame::BSButtonClick(TObject *Sender)
{
  if(activeFrame==2 && rFrame!=NULL) rFrame->getGridFrame()->backwardSearch();
}
//---------------------------------------------------------------------------

void __fastcall TReturnNMPfromSDFrame::sdGridMouseDown(TObject *Sender,TMouseButton Button, TShiftState Shift, int X, int Y)
{
  activeFrame=1;
}
//---------------------------------------------------------------------------
void __fastcall TReturnNMPfromSDFrame::sdSourceDataChange(TObject *Sender,TField *Field)
{
  if(groupOnPostOffices->Checked==false && lFrame!=NULL)
    lFrame->getContentFrame()->setContent();
  if(rFrame!=NULL)
  { loadNMPDS();
    if(rFrame->getGridFrame()->isSearchDSActive()==true) loadNMPSearchDS();
  }
}
//---------------------------------------------------------------------------
void __fastcall TReturnNMPfromSDFrame::groupOnPostOfficesClick(
      TObject *Sender)
{
  initFrame();
}
//---------------------------------------------------------------------------
void __fastcall TReturnNMPfromSDFrame::nmpGridMouseDown(TObject *Sender,TMouseButton Button, TShiftState Shift, int X, int Y)
{
  activeFrame=2;
}
//---------------------------------------------------------------------------
void __fastcall TReturnNMPfromSDFrame::nmpDataSetAfterPost(TDataSet *DataSet)
{
//int rn=rDS->RecNo;
//AnsiString str=SEdit->Text;

  loadSubdivisionsDS();
//  SEdit->Text=str;
//  rDS->RecNo=rn;
}
//---------------------------------------------------------------------------
void __fastcall TReturnNMPfromSDFrame::groupOnPublishersClick(TObject *Sender)
{
  //Инициируем фрейм ГЖП
  if(rFrame!=NULL){ delete rFrame; rFrame=NULL; }
  if(groupOnPublishers->Checked==false)
  { if(groupOnPostOffices->Checked==false)
      rFrame=createReturnNMP_SDFrame(this,iniFileName,iniSectionName+"_nmpSDFrame",fbDB,nmpPanel,SEdit,ConditionSBox);
    else
      rFrame=createReturnNMP_POFrame(this,iniFileName,iniSectionName+"_nmpPOFrame",fbDB,nmpPanel,SEdit,ConditionSBox);
  }
  else
    rFrame=createReturnPFrame(this,iniFileName,iniSectionName+"_pFrame",fbDB,nmpPanel,SEdit,ConditionSBox);
  if(rFrame!=NULL)
  { rFrame->getGridFrame()->workGrid->OnMouseDown=nmpGridMouseDown;
//    rFrame->loadDS=&loadNMPDS;
//    rFrame->loadSearchDS=&loadNMPSearchDS;
    rGrid=rFrame->getGridFrame()->workGrid;
    rDS=rFrame->getGridFrame()->getWorkDataSet();
  }
  else
  { rGrid=NULL;
    rDS=NULL;
    return;
  }
  loadNMPDS();
}
//---------------------------------------------------------------------------
void __fastcall TReturnNMPfromSDFrame::showAllRecordsClick(TObject *Sender)
{
  loadSubdivisionsDS();
}
//---------------------------------------------------------------------------
void __fastcall TReturnNMPfromSDFrame::nmpGridGetCellParams(TObject *Sender,TColumnEh *Column,TFont *AFont,TColor &Background,TGridDrawState State)
{
  if(groupOnPostOffices->Checked==false && groupOnPublishers->Checked==false)
  { AnsiString rp=rDS->FieldByName("REAL_PRICE")->AsString;
    double p=rDS->FieldByName("E_PRICE")->AsFloat*(100+rDS->FieldByName("NDS_STAVKA")->AsInteger)/100;
    if(rp.IsEmpty() || StrToFloat(rp)<p) Background=clRed;
    else Background=clWhite;
  }
}
//---------------------------------------------------------------------------

//Возврат ГЖП из подразделений
void __fastcall TReturnNMPfromSDFrame::returnNMPReportItemClick(
      TObject *Sender)
{
int rn=lDS->RecNo;

  frxXLSExport->DefaultPath=getAppDir()+"Out\\";
  frxXLSExport->FileName="Возврат ГЖП из подразделений (краткий отчет)";

  if(frxReport->LoadFromFile(getAppDir()+"Reports\\ReturnNMPFromSDReport.fr3")==false)
  { MessageDlg("Не могу загрузить файл с отчетом: "+getAppDir()+"Reports\\ReturnNMPFromSDReport.fr3",mtWarning,TMsgDlgButtons() << mbOK,0);
    return;
  }
  frxDS->DataSet=lDS;
  frxReport->ShowReport(true);
  lDS->RecNo=rn;
}
//---------------------------------------------------------------------------
//Печать накладной Тогр-13 на возврат ГЖП на склад
void __fastcall TReturnNMPfromSDFrame::torg13ItemClick(TObject *Sender)
{
  printTorg13(this,dataBase,lDS,rDS,2,1); //SomeFunctions.h
}
//---------------------------------------------------------------------------
//Печать накладной Тогр-13 (выборочно) на возврат ГЖП на склад
void __fastcall TReturnNMPfromSDFrame::torg13SelectItemClick(
      TObject *Sender)
{
  printTorg13(this,dataBase,lDS,rDS,2,2); //SomeFunctions.h
}
//---------------------------------------------------------------------------
void __fastcall TReturnNMPfromSDFrame::frxReportGetValue(const AnsiString VarName,Variant &Value)
{
  if(VarName=="Organization")
  {
    Value=getConstValue(this,fbDB,"ORGANIZATION_NAME",date2);
  }
  else if(VarName=="SD_NAME")
  {
    Value=getRicipientName();
  }
  else if(VarName=="Title")
  { AnsiString s="Возврат газетно-журнальной продукции из подразделений";
    s=SetHeader(s,date1,date2);
    Value=s;
  }
  else if(VarName=="SENDER_NAME")
  { AnsiString s;
    if(groupOnPostOffices->Checked==false) s=lFrame->getGridFrame()->getFieldDataOfSelectedRecord("SD_NAME");
    else s=lFrame->getGridFrame()->getFieldDataOfSelectedRecord("PO_NAME");
    Value=s;
  }
  else if(VarName=="DOC_NUMBER")
  { unsigned short y,m,d;
    date2.DecodeDate(&y,&m,&d);
    AnsiString s="воз_"; //возврат
    if(groupOnPostOffices->Checked==false) s+=lFrame->getGridFrame()->getFieldDataOfSelectedRecord("SD_ID");
    else s+=lFrame->getGridFrame()->getFieldDataOfSelectedRecord("PO_ID");
    s+="_"+AnsiString().sprintf("%02d",m)+"-"+AnsiString().sprintf("%d",y);
    if(fullPrintFLG==false) s+=" - доп.";
    Value=s;
  }
  else if(VarName=="DOC_DATE")
  { unsigned short y,m,d;
    date2.DecodeDate(&y,&m,&d);
    m++; if(m>12){ m=1; y++;}
    Value=TDateTime(y,m,10).DateString();
  }
  else if(VarName=="Propis")
  { TfrxMemoView *Memo;
    Memo=(TfrxMemoView *)frxReport->FindObject("Memo79");
    Value=f_to_str(double(Memo->Value));
  }
  else if(VarName=="RJT")
  {
    Value=getConstValue(this,fbDB,"MOL_JOB_TITLE",date2);
  }
  else if(VarName=="RFIO")
  {
    Value=getConstValue(this,fbDB,"MOL_FIO",date2);
  }
}
//---------------------------------------------------------------------------
//Сообщение с накладной Торг-13
void __fastcall TReturnNMPfromSDFrame::cmFullTorg13ItemClick(TObject *Sender)
{
  createTorg13Message(this,iniFileName,2,1); //SomeFunctions.h
}
//---------------------------------------------------------------------------
//Сообщение с накладной Торг-13 (выборочно)
void __fastcall TReturnNMPfromSDFrame::cmSelectedTorg13ItemClick(TObject *Sender)
{
  createTorg13Message(this,iniFileName,2,2); //SomeFunctions.h
}
//---------------------------------------------------------------------------
//Формирование сообщения с полным шаблоном на списание ГЖП
void __fastcall TReturnNMPfromSDFrame::cmFullTemplateClick(TObject *Sender)
{
  createTemplateMessages(this,iniFileName,1); //ReturnNMPfromSDFunctions.h
}
//---------------------------------------------------------------------------
//Формирование сообщения с выборочным шаблоном на списание ГЖП
void __fastcall TReturnNMPfromSDFrame::cmSelectedTemplateClick(
      TObject *Sender)
{
  createTemplateMessages(this,iniFileName,2); //ReturnNMPfromSDFunctions.h
}
//---------------------------------------------------------------------------
//Загрузка из шаблонов сведений о писании ГЖП подразделениями
void __fastcall TReturnNMPfromSDFrame::loadDataButtonClick(TObject *Sender)
{
TLoadDataAboutReturnNMPFromSDForm *LDF;

  try
  {
    LDF=new TLoadDataAboutReturnNMPFromSDForm(this,iniFileName,fbDB);
  }
  catch (Exception &exc)
  { MessageDlg("Ошибка создания формы LoadDataAboutReturnNMPFromSDForm\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return;
  }
  LDF->ShowModal();
  delete LDF;
  int rn=rDS->RecNo;
  loadSubdivisionsDS();
  loadNMPDS();
  rDS->RecNo=rn;
}
//---------------------------------------------------------------------------
void __fastcall TReturnNMPfromSDFrame::cmMenuPopup(TObject *Sender)
{
  if(showAllRecords->Checked==false)
  { cmTemplateForReturnNMP->Enabled=false;
    cmTorg13Item->Enabled=true;
  }
  else
  { cmTemplateForReturnNMP->Enabled=true;
    cmTorg13Item->Enabled=false;
  }
}
//---------------------------------------------------------------------------

