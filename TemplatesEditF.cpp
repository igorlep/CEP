//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "PLACEMNT.HPP"
#include "DifferentFunctions.h"
#include "TWFunctions.h"
#include "SelectPeriod.h"
#include "CopyTemplates.h"
#include "RemovingTemplates.h"
#include "SomeFunctions.h"
#include "ReCalc.h"
#include "TemplatesEditF.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "JvComponent"
#pragma link "JvExControls"
#pragma link "JvNavigationPane"
#pragma link "JvSpeedButton"
#pragma link "JvXPCheckCtrls"
#pragma link "JvXPCore"
#pragma link "JvXPButtons"
#pragma link "JvExMask"
#pragma link "JvSpin"
#pragma link "JvExComCtrls"
#pragma link "JvUpDown"
#pragma resource "*.dfm"
//TTemplatesEditFrame *TemplatesEditFrame;
//---------------------------------------------------------------------------
//**********
//* public *
//**********
__fastcall TTemplatesEditFrame::TTemplatesEditFrame(TComponent* Owner,AnsiString iniFN,FBDatabase *fbdb)
        : TFrame(Owner),iniFileName(iniFN),fbDB(fbdb)
{
  iniSectionName="TemplatesEdit";
  pFrame=NULL;
  nmpFrame=NULL;
  tFrame=NULL;
//  firstLoadFLG=true;
}
//---------------------------------------------------------------------------
__fastcall TTemplatesEditFrame::~TTemplatesEditFrame(void)
{
  getMainForm()->templatesCopyItem->Enabled=false;
  getMainForm()->templatesCopyItem->OnClick=NULL;
  getMainForm()->templatesDeleteItem->Enabled=false;
  getMainForm()->templatesDeleteItem->OnClick=NULL;
  getMainForm()->templatesEditItem->Enabled=true;
  saveProperties();
  if(pFrame!=NULL){ delete pFrame; pFrame=NULL; }
  if(nmpFrame!=NULL){ delete nmpFrame; nmpFrame=NULL; }
  if(tFrame!=NULL){ delete tFrame; tFrame=NULL; }
}
//---------------------------------------------------------------------------
void __fastcall TTemplatesEditFrame::initFrame(void)
{
  //Инициируем фрейм поставщиков
  pFrame=createTempPublishersFrame(Owner,iniFileName,iniSectionName+"_pFrame",fbDB,pPanel,SEdit,ConditionSBox);
  if(pFrame!=NULL)
  { pFrame->getGridFrame()->workGrid->OnMouseDown=pGridMouseDown;
    pFrame->getGridFrame()->workSource->OnDataChange=pSourceDataChange;
//    pFrame->loadDS=&loadPublishersDS;
//    pFrame->loadSearchDS=&loadPublishersSearchDS;
  }
  else return;
  //Инициируем фрейм ГЖП
  nmpFrame=createTempNMPFrame(Owner,iniFileName,iniSectionName+"_nmpFrame",fbDB,nmpPanel,SEdit,ConditionSBox);
  if(nmpFrame!=NULL)
  { nmpFrame->getGridFrame()->workGrid->OnMouseDown=nmpGridMouseDown;
    nmpFrame->getGridFrame()->workSource->OnDataChange=nmpSourceDataChange;
//    nmpFrame->loadDS=&loadNMPDS;
//    nmpFrame->loadSearchDS=&loadNMPSearchDS;
  }
  else return;
  //Инициируем фрейм шаблонов
  tFrame=createTempFrame(Owner,iniFileName,iniSectionName+"_tFrame",fbDB,tPanel);
  if(tFrame!=NULL)
  { tFrame->workGrid->PopupMenu=tMenu;
    tFrame->workSource->OnStateChange=tSourceStateChange;
    tFrame->setHeader(tHeader);
    tFrame->setHeaderCaption(tFrame->getTableTitle());
    tFrame->setStatusBar(tStatusBar);
    tFrame->workGrid->OnMouseDown=tGridMouseDown;
  }
  else return;
  loadProperties();
  loadDataSets();
  initMenuItems();
}
//---------------------------------------------------------------------------
void __fastcall TTemplatesEditFrame::loadDataSets(void)
{
//  firstLoadFLG=true;
  loadPublishersDS();
//  loadNMPDS();
//  firstLoadFLG=false;
//  loadTemplateDS();
  setFocus();
}
//---------------------------------------------------------------------------
void __fastcall TTemplatesEditFrame::setFocus(void)
{
  if(activeFrame==1)
  {
    if(pFrame!=NULL) pFrame->getGridFrame()->workGrid->SetFocus();
  }
  else if(activeFrame==2)
  {
    if(nmpFrame) nmpFrame->getGridFrame()->workGrid->SetFocus();
  }
  else
  {
    if(tFrame!=NULL) tFrame->workGrid->SetFocus();
  }
}
//---------------------------------------------------------------------------

//***********
//* private *
//***********
void __fastcall TTemplatesEditFrame::loadProperties(void)
{
TIniFile *ini = new TIniFile(iniFileName);

  pPanel->Width=ini->ReadInteger(iniSectionName,"pPanel_Width",200);
  nmpPanel->Width=ini->ReadInteger(iniSectionName,"nmpPanel_Width",200);
  selectForPublisher->Checked=ini->ReadBool(iniSectionName,"selectForPublisher_Checked",true);
  monthBox->ItemIndex=ini->ReadInteger(iniSectionName,"month",0);
  yearEdit->Value=ini->ReadInteger(iniSectionName,"year",2010);
  selectToPeriod->Checked=ini->ReadBool(iniSectionName,"selectToPeriodCheck_Checked",false);
  activeFrame=ini->ReadInteger(iniSectionName,"activeFrame",1);
  delete ini;
//  upDown->Position=monthBox->ItemIndex+1;
  onChangePeriod(this);
}
//---------------------------------------------------------------------------
void __fastcall TTemplatesEditFrame::saveProperties(void)
{
TIniFile *ini = new TIniFile(iniFileName);

  ini->WriteInteger(iniSectionName,"pPanel_Width",pPanel->Width);
  ini->WriteInteger(iniSectionName,"nmpPanel_Width",nmpPanel->Width);
  ini->WriteBool(iniSectionName,"selectForPublisher_Checked",selectForPublisher->Checked);
  ini->WriteInteger(iniSectionName,"month",monthBox->ItemIndex);
  ini->WriteInteger(iniSectionName,"year",yearEdit->Value);
  ini->WriteBool(iniSectionName,"selectToPeriodCheck_Checked",selectToPeriod->Checked);
  ini->WriteInteger(iniSectionName,"activeFrame",activeFrame);
  delete ini;
}
//---------------------------------------------------------------------------
void __fastcall TTemplatesEditFrame::loadPublishersDS(void)
{
  if(pFrame!=NULL)
  { TDateTime tempDate=getDateFromMonthYearString(periodStr);
    pFrame->getGCFrame()->loadDS(formTempPublishersSelectSQL(pFrame->getViewDelRecordsFLG(),selectToPeriod->Checked,tempDate));
    pFrame->getContentFrame()->setContent();
    if(nmpFrame!=NULL) loadNMPDS();
  }
}
//---------------------------------------------------------------------------
void __fastcall TTemplatesEditFrame::loadPublishersSearchDS(void)
{
  if(pFrame!=NULL)
  { pFrame->getGridFrame()->initSearchSQL(formTempPublishersSearchSQL(pFrame->getViewDelRecordsFLG(),selectToPeriod->Checked,getDateFromMonthYearString(periodStr)));
    pFrame->getGridFrame()->loadSearchingDS();
  }
}
//---------------------------------------------------------------------------
void __fastcall TTemplatesEditFrame::loadNMPDS(void)
{
  if(nmpFrame!=NULL)
  { TDateTime tempDate=getDateFromMonthYearString(periodStr);
    AnsiString pID="";
    if(selectForPublisher->Checked==true) pID=pFrame->getGridFrame()->getSelectedRecordID();
    else pID=nmpFrame->getGridFrame()->getFieldDataOfSelectedRecord("P_ID");
    nmpFrame->getGCFrame()->loadDS(formTempNMPSelectSQL(pID,selectForPublisher->Checked,nmpFrame->getViewDelRecordsFLG(),selectToPeriod->Checked,tempDate));
    nmpFrame->getContentFrame()->setContent();
    if(tFrame!=NULL) loadTemplateDS();
  }
}
//---------------------------------------------------------------------------
void __fastcall TTemplatesEditFrame::loadNMPSearchDS(void)
{
  if(nmpFrame!=NULL)
  { AnsiString pID="";
    if(selectForPublisher->Checked==true) pID=pFrame->getGridFrame()->getSelectedRecordID();
    else pID=nmpFrame->getGridFrame()->getFieldDataOfSelectedRecord("P_ID");
    nmpFrame->getGridFrame()->initSearchSQL(formTempNMPSearchSQL(pID,selectForPublisher->Checked,nmpFrame->getViewDelRecordsFLG(),selectToPeriod->Checked,getDateFromMonthYearString(periodStr)));
    nmpFrame->getGridFrame()->loadSearchingDS();
  }
}
//---------------------------------------------------------------------------
void __fastcall TTemplatesEditFrame::loadTemplateDS(void)
{
  if(tFrame!=NULL)
  { tFrame->workSource->DataSet->Active=false;
    if(nmpFrame->getGridFrame()->workSource->DataSet->Active==false || nmpFrame->getGridFrame()->workSource->DataSet->RecordCount==0) return;
    AnsiString sql=formTempSelectSQL(tempDate,oldTempDate,factDate1,factDate2,nmpFrame->getGridFrame()->getSelectedRecordID());
    tFrame->initSelectSQL(sql);
    tFrame->loadDS();
  }
}
//---------------------------------------------------------------------------
void __fastcall TTemplatesEditFrame::initMenuItems(void)
{
CheckStyle chs;
AnsiString miName;

  chs=checkUserRightForMenuName(this,fbDB,getUser()->getUID(),"templatesCopyItem");
  if(chs==csChecked || chs==csCheckedgray)
  { getMainForm()->templatesCopyItem->Enabled=true;
    getMainForm()->templatesCopyItem->OnClick=copyTemplatesClick;
  }
  chs=checkUserRightForMenuName(this,fbDB,getUser()->getUID(),"templatesDeleteItem");
  if(chs==csChecked || chs==csCheckedgray)
  { getMainForm()->templatesDeleteItem->Enabled=true;
    getMainForm()->templatesDeleteItem->OnClick=removeTemplatesClick;
  }
  getMainForm()->templatesEditItem->Enabled=false;
  // - tMenu
  for(int i=0; i<tMenu->Items->Count; i++)
  { miName=tMenu->Items->Items[i]->Name;
    if(miName=="-") continue;
    chs=checkUserRightForMenuName(this,fbDB,getUser()->getUID(),miName);
    if(chs==csChecked) tMenu->Items->Items[i]->Enabled=true;
    else tMenu->Items->Items[i]->Enabled=false;
  }
}
//---------------------------------------------------------------------------

//*************
//* published *
//*************
void __fastcall TTemplatesEditFrame::searchTimerTimer(TObject *Sender)
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
void __fastcall TTemplatesEditFrame::SEditChange(TObject *Sender)
{
  if(searchTimer->Enabled==true) searchTimer->Enabled=false;
  if(!SEdit->Text.IsEmpty()) searchTimer->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TTemplatesEditFrame::SEditKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
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
void __fastcall TTemplatesEditFrame::ConditionSBoxClick(TObject *Sender)
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
void __fastcall TTemplatesEditFrame::FSButtonClick(TObject *Sender)
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
void __fastcall TTemplatesEditFrame::BSButtonClick(TObject *Sender)
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
void __fastcall TTemplatesEditFrame::selectForPublisherClick(
      TObject *Sender)
{
  if(pFrame!=NULL)
  { if(nmpFrame!=NULL)
    { loadNMPDS();
      if(nmpFrame->getGridFrame()->isSearchDSActive()==true) loadNMPSearchDS();
      loadTemplateDS();
    }
  }
  setFocus();
}
//---------------------------------------------------------------------------
void __fastcall TTemplatesEditFrame::selectToPeriodClick(TObject *Sender)
{
  loadDataSets();
}
//---------------------------------------------------------------------------
void __fastcall TTemplatesEditFrame::onChangePeriod(TObject *Sender)
{
unsigned short y,m,d;

  periodStr=monthBox->Text+" "+yearEdit->Text+" года";
//upDown->Position=monthBox->ItemIndex+1;

  tempDate=getDateFromMonthYearString(periodStr);
  tempDate.DecodeDate(&y,&m,&d);
  m--; if(m==0){ y--; m=12; }
  oldTempDate=TDateTime(y,m,d);
  m--; if(m==0){ m=12; y--;}
  factDate1=TDateTime(y,m,d),
  factDate2=TDateTime(y,m,lastDayOfMonth(y,m));
  if(selectToPeriod->Checked==true) loadDataSets();
  else
  { loadNMPDS();
//    loadTemplateDS();
  }
}
//---------------------------------------------------------------------------
//Следующий период
void __fastcall TTemplatesEditFrame::spinButtonTopClick(TObject *Sender)
{
  if(monthBox->ItemIndex==11)
  { monthBox->ItemIndex=0;
    yearEdit->Value++;
  }
  else monthBox->ItemIndex++;
  onChangePeriod(this);
}
//---------------------------------------------------------------------------
//Предыдущий период
void __fastcall TTemplatesEditFrame::spinButtonBottomClick(TObject *Sender)
{
  if(monthBox->ItemIndex==0)
  { monthBox->ItemIndex=11;
    yearEdit->Value--;
  }
  else monthBox->ItemIndex--;
  onChangePeriod(this);
}
//---------------------------------------------------------------------------
/*
void __fastcall TTemplatesEditFrame::upDownClick(TObject *Sender,TUDBtnType Button)
{
  if(upDown->Position > 12 && Button==btNext)
  { monthBox->ItemIndex=0;
    yearEdit->Value++;
    upDown->Position=1;
  }
  else if(upDown->Position < 1 && Button==btPrev)
  { monthBox->ItemIndex=11;
    yearEdit->Value--;
    upDown->Position=12;
  }
  else monthBox->ItemIndex=upDown->Position-1;
  onChangePeriod(this);
}
//---------------------------------------------------------------------------
*/
void __fastcall TTemplatesEditFrame::pGridMouseDown(TObject *Sender,TMouseButton Button, TShiftState Shift, int X, int Y)
{
  activeFrame=1;
}
//---------------------------------------------------------------------------
void __fastcall TTemplatesEditFrame::pSourceDataChange(TObject *Sender,TField *Field)
{
  if(pFrame->getContentFrame()!=NULL) pFrame->getContentFrame()->setContent();
//  if(firstLoadFLG==true) return;
  if(selectForPublisher->Checked==true)
  { if(nmpFrame!=NULL)
    { loadNMPDS();
      if(nmpFrame->getGridFrame()->isSearchDSActive()==true) loadNMPSearchDS();
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TTemplatesEditFrame::nmpGridMouseDown(TObject *Sender,TMouseButton Button, TShiftState Shift, int X, int Y)
{
  activeFrame=2;
}
//---------------------------------------------------------------------------
void __fastcall TTemplatesEditFrame::nmpSourceDataChange(TObject *Sender,TField *Field)
{
  if(pFrame==NULL) return;
  if(nmpFrame==NULL) return;
  if(nmpFrame->getContentFrame()!=NULL) nmpFrame->getContentFrame()->setContent();
  if(selectForPublisher->Checked==false)
  { AnsiString pID=nmpFrame->getGridFrame()->getFieldDataOfSelectedRecord("P_ID");
    if(pID.IsEmpty()) return;
    pFrame->getGridFrame()->workSource->DataSet->Locate("P_ID",pID,TLocateOptions().Clear());
  }
//  if(firstLoadFLG==true) return;
  if(tFrame!=NULL) loadTemplateDS();
}
//---------------------------------------------------------------------------
void __fastcall TTemplatesEditFrame::tGridMouseDown(TObject *Sender,TMouseButton Button, TShiftState Shift, int X, int Y)
{
  activeFrame=3;
}
//---------------------------------------------------------------------------
void __fastcall  TTemplatesEditFrame::tSourceStateChange(TObject *Sender)
{
  if(tFrame->workSource->State==dsEdit || tFrame->workSource->State==dsInsert)
  { save->Enabled=true;
    cancel->Enabled=true;
  }
  else
  { save->Enabled=false;
    cancel->Enabled=false;
  }
}
//---------------------------------------------------------------------------

void __fastcall TTemplatesEditFrame::tMenuPopup(TObject *Sender)
{
CheckStyle chs;

  if(tFrame->workSource->DataSet->RecordCount==0)
  { editTempRecord->Enabled=false;
    delTempRecord->Enabled=false;
    chs=checkUserRightForMenuName(this,fbDB,getUser()->getUID(),"loadTemplateFromOtherPeriod");
    if(chs==csChecked) loadTemplateFromOtherPeriod->Enabled=true;
    clearTemplate->Enabled=false;
  }
  else
  { chs=checkUserRightForMenuName(this,fbDB,getUser()->getUID(),"editTempRecord");
    if(chs==csChecked) editTempRecord->Enabled=true;
    chs=checkUserRightForMenuName(this,fbDB,getUser()->getUID(),"delTempRecord");
    if(chs==csChecked) delTempRecord->Enabled=true;
    loadTemplateFromOtherPeriod->Enabled=false;
    chs=checkUserRightForMenuName(this,fbDB,getUser()->getUID(),"clearTemplate");
    if(chs==csChecked) clearTemplate->Enabled=true;
  }
}
//---------------------------------------------------------------------------
//Добавление шаблона
void __fastcall TTemplatesEditFrame::addTempRecordClick(TObject *Sender)
{
AnsiString t_id="";
AnsiString nmp_id=nmpFrame->getGridFrame()->getSelectedRecordID();
AnsiString sd_id="";

  if(AddEditTemplate(this,iniFileName,fbDB,t_id,nmp_id,sd_id,tempDate)==mrOk)
  { loadTemplateDS();
    tFrame->getWorkDataSet()->Locate("T_ID",t_id,TLocateOptions().Clear());
  }
}
//---------------------------------------------------------------------------
//Редактирование шаблона
void __fastcall TTemplatesEditFrame::editTempRecordClick(TObject *Sender)
{
  if(tFrame->getFieldsInfo()[tFrame->findPozitionFieldForGridFieldName(tFrame->workGrid->Columns->Items[tFrame->workGrid->Col-1]->FieldName)-1]->isReadOnly()==true)
  { AnsiString t_id=tFrame->getSelectedRecordID();
    AnsiString nmp_id=nmpFrame->getGridFrame()->getSelectedRecordID();
    AnsiString sd_id=tFrame->getFieldDataOfSelectedRecord("SD_ID");
    if(AddEditTemplate(this,iniFileName,fbDB,t_id,nmp_id,sd_id,tempDate)==mrOk)
    { loadTemplateDS();
      tFrame->getWorkDataSet()->Locate("T_ID",t_id,TLocateOptions().Clear());
    }
  }
  else
    tFrame->getWorkDataSet()->Edit();
}
//---------------------------------------------------------------------------
//Удаление записи о шаблоне
void __fastcall TTemplatesEditFrame::delTempRecordClick(TObject *Sender)
{
TDBGridEh *grid=tFrame->workGrid;
TpFIBDataSet *ds=tFrame->getWorkDataSet();

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
    sql="delete from TEMPLATE_TABLE where T_ID="+ds->FieldByName("T_ID")->AsString;
    if(fbWQ->execQuery("TTemplatesEditFrame::delTempRecordClick",sql)==false)
    { fbWQ->getTransaction()->Rollback();
      return;
    }
  }
  fbWQ->getTransaction()->Commit();
  loadTemplateDS();
}
//---------------------------------------------------------------------------
//Добавление списком
void __fastcall TTemplatesEditFrame::AddTempRecordsClick(TObject *Sender)
{
TMultiSelectSDForm *MSF;

  MSF=createMultiSelectSDForm(this,iniFileName,iniSectionName,fbDB,"TEMPLATE_TABLE",nmpFrame->getGridFrame()->getSelectedRecordID(),getDateFromMonthYearString(periodStr));
  if(MSF!=NULL)
  { if(MSF->ShowModal()==mrOk) loadTemplateDS();
    delete MSF;
  }
}
//---------------------------------------------------------------------------
//Загрузка шаблона из другого периода
void __fastcall TTemplatesEditFrame::loadTemplateFromOtherPeriodClick(TObject *Sender)
{
TSelectPeriodForm *SPF;
int year=int(yearEdit->Value),
    month=monthBox->ItemIndex-1;

  if(month==0){ month=12; year--; }
  try
  {
    SPF=new TSelectPeriodForm(this,iniFileName,"Загрузка шаблона из другого периода",year,month);
  }
  catch (Exception &exc)
  { MessageDlg("Ошибка создания формы TSelectPeriodForm\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return;
  }
  if(SPF->ShowModal()==mrOk)
  { FBDataSet *fbDS=fbDB->getFBDS_T1_T2();
    FBQuery *fbWQ=fbDB->getFBQ_T2();
    AnsiString sql;
    unsigned short y=SPF->getYear(),
                   m=SPF->getMonthIndex()+1,
                   d=1;
    delete SPF;
    TDateTime other_date=TDateTime(y,m,d);
    AnsiString nmp_id=nmpFrame->getGridFrame()->getSelectedRecordID();
    sql="select SD_ID,TEMP_Q from TEMPLATE_TABLE where";
    sql+=" NMP_ID="+nmp_id;
    sql+=" and TEMP_DATE='"+other_date.DateString()+"'";
    if(fbDS->execSelectSQL("TTemplatesEditFrame::loadTemplateFromOtherPeriodClick",sql)==false) return;
    TpFIBTransaction *WT=fbWQ->getTransaction();
    TDataSet *DS=fbDS->getDataSet();
    //Цикл по выбранным подразделениям
    DS->First();
    while(!DS->Eof)
    { sql="insert into TEMPLATE_TABLE(T_ID,NMP_ID,SD_ID,TEMP_Q,TEMP_DATE) values(";
      sql+="GEN_ID(GEN_TEMPLATE_TABLE_ID,1),";
      sql+=nmp_id+",";
      sql+=DS->FieldByName("SD_ID")->AsString+",";
      sql+=DS->FieldByName("TEMP_Q")->AsString+",";
      sql+="'"+tempDate.DateString()+"')";
      WT->StartTransaction();
      if(fbWQ->execQuery("TTemplatesEditFrame::loadTemplateFromOtherPeriodClick",sql)==false)
      { WT->Rollback();
        return;
      }
      WT->Commit();
      DS->Next();
    }
    loadTemplateDS();
  }
  else delete SPF;
}
//---------------------------------------------------------------------------
//Сохранение изменений
void __fastcall TTemplatesEditFrame::saveClick(TObject *Sender)
{
  if(tFrame->workSource->State==dsEdit || tFrame->workSource->State==dsInsert)
    tFrame->getWorkDataSet()->Post();
}
//---------------------------------------------------------------------------
//Отмена изменений
void __fastcall TTemplatesEditFrame::cancelClick(TObject *Sender)
{
  if(tFrame->workSource->State==dsEdit || tFrame->workSource->State==dsInsert)
    tFrame->getWorkDataSet()->Cancel();
}
//---------------------------------------------------------------------------
//Очистка шаблона
void __fastcall TTemplatesEditFrame::clearTemplateClick(TObject *Sender)
{
AnsiString sql;

  if(MessageDlg("Вы уверены, что хотите удалить шаблон разнаряжения издания \""+nmpFrame->getGridFrame()->getFieldDataOfSelectedRecord("NMP_NAME")+"\"?",mtConfirmation,TMsgDlgButtons() << mbOK << mbCancel,0)==mrCancel) return;
  FBQuery *fbWQ=fbDB->getFBQ_T2();
  TpFIBTransaction *WT=fbWQ->getTransaction();
  sql="delete from TEMPLATE_TABLE where NMP_ID="+nmpFrame->getGridFrame()->getSelectedRecordID();
  sql+=" and TEMP_DATE='"+tempDate.DateString()+"'";
  WT->StartTransaction();
  if(fbWQ->execQuery("TTemplatesEditFrame::clearTemplateClick",sql)==false)
  { WT->Rollback();
    return;
  }
  WT->Commit();
  loadTemplateDS();
}
//---------------------------------------------------------------------------
//Настройка колонок
void __fastcall TTemplatesEditFrame::tColumnsConfogClick(TObject *Sender)
{
  tFrame->columnsConfigClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TTemplatesEditFrame::copyTemplatesClick(TObject *Sender)
{
TCopyTemplatesForm *CTF;
unsigned short year=(unsigned short)(yearEdit->Value),
               month=(unsigned short)(monthBox->ItemIndex+1);

  try
  {
    CTF=new TCopyTemplatesForm(this,iniFileName,fbDB,year,month);
  }
  catch (Exception &exc)
  { MessageDlg("Ошибка создания формы TCopyTemplatesForm\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return;
  }
  if(CTF->ShowModal()==mrOk)
  {
    onChangePeriod(this);
  }
  delete CTF;
}
//---------------------------------------------------------------------------
void __fastcall TTemplatesEditFrame::removeTemplatesClick(TObject *Sender)
{
TRemovingTemplatesForm *RTF;

  try
  {
    RTF=new TRemovingTemplatesForm(this,iniFileName,fbDB);
  }
  catch (Exception &exc)
  { MessageDlg("Ошибка создания формы TRemovingTemplatesForm\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return;
  }
  RTF->ShowModal();
  delete RTF;
  onChangePeriod(this);
}
//---------------------------------------------------------------------------
//Пропорциональный пересчет столбца "Количество на текущий период"
void __fastcall TTemplatesEditFrame::reCalcItemClick(TObject *Sender)
{
TReCalcForm *RCF;

  try
  {
    RCF=new TReCalcForm(this,iniFileName,fbDB,tFrame->getWorkDataSet(),tFrame->workGrid->SumList->SumCollection->Items[0]->SumValue);
  }
  catch (Exception &exc)
  { MessageDlg("Ошибка создания формы TReCalcForm\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return;
  }
  if(RCF->ShowModal()==mrOk) loadTemplateDS();
  delete RCF;
}
//---------------------------------------------------------------------------

