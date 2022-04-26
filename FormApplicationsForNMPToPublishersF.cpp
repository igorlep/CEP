//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DifferentFunctions.h"
#include "SomeFunctions.h"
#include "NextGridF.h"
#include "AddEditApplicationForNMP.h"
#include "FormApplicationsForNMPToPublishersFunctions.h"
#include "FormApplicationsForNMPToPublishersF.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "JvComponent"
#pragma link "JvExControls"
#pragma link "JvExMask"
#pragma link "JvNavigationPane"
#pragma link "JvSpeedButton"
#pragma link "JvSpin"
#pragma link "JvXPCheckCtrls"
#pragma link "JvXPCore"
#pragma link "frxClass"
#pragma link "frxDBSet"
#pragma resource "*.dfm"
//TFormApplicationsForNMPToPublishersFrame *FormApplicationsForNMPToPublishersFrame;
//---------------------------------------------------------------------------
//**********
//* public *
//**********
__fastcall TFormApplicationsForNMPToPublishersFrame::TFormApplicationsForNMPToPublishersFrame(TComponent* Owner,AnsiString iniFN,FBDatabase *fbdb)
        : TFrame(Owner),iniFileName(iniFN),fbDB(fbdb)
{
  workDir=getAppDir();
  iniSectionName="FormApplicationsForNMPToPublishersFrame";
  dataBase=fbDB->getDatabase();
  lFrame=NULL; rFrame=NULL;
//  firstLoadFLG=true;

  loadProperties();
}
//---------------------------------------------------------------------------
__fastcall TFormApplicationsForNMPToPublishersFrame::~TFormApplicationsForNMPToPublishersFrame(void)
{
  saveProperties();
  if(lFrame!=NULL){ delete lFrame; lFrame=NULL; }
  if(rFrame!=NULL){ delete rFrame; rFrame=NULL; }
}
//---------------------------------------------------------------------------
void __fastcall TFormApplicationsForNMPToPublishersFrame::initFrame(void)
{
  //Инициируем фрейм поставщиков
  if(lFrame!=NULL){ delete lFrame; lFrame=NULL; }
  lFrame=cteatePublishersFrame(this,iniFileName,iniSectionName+"_pFrame",fbDB,pPanel);
  if(lFrame!=NULL)
  { lFrame->getGridFrame()->workGrid->OnMouseDown=pGridMouseDown;
    lFrame->getGridFrame()->workSource->OnDataChange=pSourceDataChange;
    lFrame->loadDS=&loadPublishersDS;
    lGrid=lFrame->getGridFrame()->workGrid;
    lDS=lFrame->getGridFrame()->getWorkDataSet();
  }
  else
  { lGrid=NULL;
    lDS=NULL;
  }
  //Инициируем фрейм с заказанной ГЖП
  if(rFrame!=NULL){ delete rFrame; rFrame=NULL; }
  rFrame=createAppNMPToPubFrame(this,iniFileName,iniSectionName+"_nmpFrame",fbDB,nmpPanel,SEdit,ConditionSBox);
  if(rFrame!=NULL)
  { rFrame->getGridFrame()->workGrid->OnMouseDown=nmpGridMouseDown;
    rFrame->getGridFrame()->workGrid->PopupMenu=popupMenu;
    rFrame->loadDS=&loadNMPDS;
    rFrame->loadSearchDS=&loadNMPSearchDS;
    rGrid=rFrame->getGridFrame()->workGrid;
    rDS=rFrame->getGridFrame()->getWorkDataSet();
  }
  else
  { rGrid=NULL;
    rDS=NULL;
  }

  loadDataSets();
  setHeader();
  if(rFrame!=NULL && lFrame!=NULL) rFrame->gcHeader->Caption=lFrame->getGridFrame()->getFieldDataOfSelectedRecord("P_NAME");
}
//---------------------------------------------------------------------------
void __fastcall TFormApplicationsForNMPToPublishersFrame::loadDataSets(void)
{
//  firstLoadFLG=true;
  loadPublishersDS();
//  firstLoadFLG=false;
//  loadNMPDS();
  setFocus();
}
//---------------------------------------------------------------------------
AnsiString __fastcall TFormApplicationsForNMPToPublishersFrame::getRicipientID(void)
{
  return lFrame->getGridFrame()->getFieldDataOfSelectedRecord("P_ID");
}
//---------------------------------------------------------------------------
AnsiString __fastcall TFormApplicationsForNMPToPublishersFrame::getRicipientName(void)
{
  if(!lFrame->getGridFrame()->getFieldDataOfSelectedRecord("P_OFFICIAL__NAME").IsEmpty())
    return lFrame->getGridFrame()->getFieldDataOfSelectedRecord("P_OFFICIAL_NAME");
  else
    return lFrame->getGridFrame()->getFieldDataOfSelectedRecord("P_NAME");
}
//---------------------------------------------------------------------------
AnsiString __fastcall TFormApplicationsForNMPToPublishersFrame::getRicipientMail(void)
{
  return lFrame->getGridFrame()->getFieldDataOfSelectedRecord("P_EMAIL");
}
//---------------------------------------------------------------------------

//***********
//* private *
//***********
void __fastcall TFormApplicationsForNMPToPublishersFrame::loadProperties(void)
{
TIniFile *ini = new TIniFile(iniFileName);

  leftPanel->Width=ini->ReadInteger(iniSectionName,"leftPanel_Width",250);
  activeFrame=ini->ReadInteger(iniSectionName,"activeFrame",1);
  monthBox->ItemIndex=ini->ReadInteger(iniSectionName,"month",0);
  yearEdit->Value=ini->ReadInteger(iniSectionName,"year",2010);
  delete ini;
  onChangePeriod(this);
}
//---------------------------------------------------------------------------
void __fastcall TFormApplicationsForNMPToPublishersFrame::saveProperties(void)
{
TIniFile *ini = new TIniFile(iniFileName);

  ini->WriteInteger(iniSectionName,"leftPanel_Width",leftPanel->Width);
  ini->WriteInteger(iniSectionName,"activeFrame",activeFrame);
  ini->WriteInteger(iniSectionName,"month",monthBox->ItemIndex);
  ini->WriteInteger(iniSectionName,"year",yearEdit->Value);
  delete ini;
}
//---------------------------------------------------------------------------
void __fastcall TFormApplicationsForNMPToPublishersFrame::loadPublishersDS(void)
{
  if(lFrame!=NULL)
  { lFrame->getGCFrame()->loadDS(formPublishersSelectSQL(tempDate));
    lFrame->getContentFrame()->setContent();
    if(rFrame!=NULL) loadNMPDS();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormApplicationsForNMPToPublishersFrame::loadNMPDS(void)
{
AnsiString pID;

  if(rFrame==NULL || rDS==NULL) return;
  rDS->Close();
  if(lDS->RecordCount==0) return;
  pID=lFrame->getGridFrame()->getFieldDataOfSelectedRecord("P_ID");
  if(rFrame!=NULL)
  { AnsiString sql=formAppNMPToPubSelectSQL(pID,tempDate);
    rFrame->getGCFrame()->loadDS(sql);
    rFrame->getContentFrame()->setContent();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormApplicationsForNMPToPublishersFrame::loadNMPSearchDS(void)
{
  if(rFrame!=NULL && rDS->RecordCount>0)
  { AnsiString pID=lFrame->getGridFrame()->getFieldDataOfSelectedRecord("P_ID");
    rFrame->getGridFrame()->initSearchSQL(formAppNMPToPubSearchSQL(pID));
    rFrame->getGridFrame()->loadSearchingDS();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormApplicationsForNMPToPublishersFrame::setFocus(void)
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
void __fastcall TFormApplicationsForNMPToPublishersFrame::setHeader(void)
{
char *mon[12]={"январь","февраль","март","апрель","май","июнь","июль","август","сентябрь","октябрь","ноябрь","декабрь"};

  header->Caption="Формирование заявок на газетно-журнальную продукцию для поставщиков на "+AnsiString(mon[monthBox->ItemIndex])+" "+yearEdit->Text+" года";
}
//---------------------------------------------------------------------------

//*************
//* published *
//*************
void __fastcall TFormApplicationsForNMPToPublishersFrame::onChangePeriod(TObject *Sender)
{
  periodStr=monthBox->Text+" "+yearEdit->Text+" года";

  tempDate=getDateFromMonthYearString(periodStr);
  loadDataSets();
  setHeader();
}
//---------------------------------------------------------------------------
void __fastcall TFormApplicationsForNMPToPublishersFrame::spinButtonBottomClick(TObject *Sender)
{
  if(monthBox->ItemIndex==0)
  { monthBox->ItemIndex=11;
    yearEdit->Value--;
  }
  else monthBox->ItemIndex--;
  onChangePeriod(this);
}
//---------------------------------------------------------------------------
void __fastcall TFormApplicationsForNMPToPublishersFrame::spinButtonTopClick(TObject *Sender)
{
  if(monthBox->ItemIndex==11)
  { monthBox->ItemIndex=0;
    yearEdit->Value++;
  }
  else monthBox->ItemIndex++;
  onChangePeriod(this);
}
//---------------------------------------------------------------------------
void __fastcall TFormApplicationsForNMPToPublishersFrame::pGridMouseDown(TObject *Sender,TMouseButton Button, TShiftState Shift, int X, int Y)
{
  activeFrame=1;
}
//---------------------------------------------------------------------------
void __fastcall TFormApplicationsForNMPToPublishersFrame::pSourceDataChange(TObject *Sender,TField *Field)
{
  if(lFrame!=NULL)
    lFrame->getContentFrame()->setContent();
//  if(firstLoadFLG==true) return;
  if(rFrame!=NULL)
  { loadNMPDS();
    if(rFrame->getGridFrame()->isSearchDSActive()==true) loadNMPSearchDS();
  }
  rFrame->gcHeader->Caption=lFrame->getGridFrame()->getFieldDataOfSelectedRecord("P_NAME");
}
//---------------------------------------------------------------------------
void __fastcall TFormApplicationsForNMPToPublishersFrame::nmpGridMouseDown(TObject *Sender,TMouseButton Button, TShiftState Shift, int X, int Y)
{
  activeFrame=2;
}
//---------------------------------------------------------------------------
//Формирование сообщений
void __fastcall TFormApplicationsForNMPToPublishersFrame::formMessagesButtonClick(TObject *Sender)
{
  createMessageToPub(this,iniFileName); //FormApplicationsForNMPToPublishersFunctions.h
}
//---------------------------------------------------------------------------
//Поиск по модели
void __fastcall TFormApplicationsForNMPToPublishersFrame::searchTimerTimer(TObject *Sender)
{
  searchTimer->Enabled=false;
  if(activeFrame==2) loadNMPSearchDS();
}
//---------------------------------------------------------------------------
void __fastcall TFormApplicationsForNMPToPublishersFrame::SEditChange(TObject *Sender)
{
  if(searchTimer->Enabled==true) searchTimer->Enabled=false;
  if(!SEdit->Text.IsEmpty()) searchTimer->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TFormApplicationsForNMPToPublishersFrame::SEditKeyDown(TObject *Sender,WORD &Key,TShiftState Shift)
{
  if(Key==VK_RETURN || Key==VK_DOWN || Key==VK_UP)
  { Key=0;
    if(activeFrame==2 && rFrame!=NULL) rFrame->getGridFrame()->workGrid->SetFocus();
  }
  else if(Key == VK_ESCAPE){ Key=0; SEdit->Text=""; }
}
//---------------------------------------------------------------------------
void __fastcall TFormApplicationsForNMPToPublishersFrame::ConditionSBoxClick(TObject *Sender)
{
  if(activeFrame==2) loadNMPSearchDS();
}
//---------------------------------------------------------------------------
void __fastcall TFormApplicationsForNMPToPublishersFrame::FSButtonClick(TObject *Sender)
{
  if(activeFrame==2 && rFrame!=NULL) rFrame->getGridFrame()->forwardSearch();
}
//---------------------------------------------------------------------------
void __fastcall TFormApplicationsForNMPToPublishersFrame::BSButtonClick(TObject *Sender)
{
  if(activeFrame==2 && rFrame!=NULL) rFrame->getGridFrame()->backwardSearch();
}
//---------------------------------------------------------------------------
//Редактирование заявок
void __fastcall TFormApplicationsForNMPToPublishersFrame::appEditItemClick(TObject *Sender)
{
TAddEditApplicationForNMPForm *EAF;
AnsiString nmpid=rFrame->getGridFrame()->getFieldDataOfSelectedRecord("NMP_ID");
AnsiString nmpName=rFrame->getGridFrame()->getFieldDataOfSelectedRecord("NMP_NAME");
  try
  {
    EAF=new TAddEditApplicationForNMPForm(this,iniFileName,fbDB,nmpid,tempDate,nmpName);
  }
  catch(Exception &exc)
  { MessageDlg("Ошибка создания формы TAddEditApplicationForNMPForm\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    EAF=NULL;
    return;
  }
  EAF->ShowModal();
  if(EAF->getChangeFLG()==true)
    loadDataSets();
  delete EAF;
}
//---------------------------------------------------------------------------
//Конфигурирование колонок
void __fastcall TFormApplicationsForNMPToPublishersFrame::columnsConfigItemClick(
      TObject *Sender)
{
  rFrame->getGridFrame()->columnsConfigClick(Sender);
}
//---------------------------------------------------------------------------
//Печать тиражной ведомости
void __fastcall TFormApplicationsForNMPToPublishersFrame::printButtonClick(
      TObject *Sender)
{
  PrintTirVed(this,tempDate);
}
//---------------------------------------------------------------------------
void __fastcall TFormApplicationsForNMPToPublishersFrame::frxReportGetValue(
      const AnsiString VarName, Variant &Value)
{
  if(VarName=="Title")
  { AnsiString s="Тиражная ведомость разнаряжения газетно-журнальной продукции\n";
    s+="Издательство - \""+lDS->FieldByName("P_NAME")->AsString+"\"\n";
    s+="Издание - \""+rDS->FieldByName("NMP_NAME")->AsString+"\"\n";
    s+="за "+periodStr;
    Value=s;
  }
}
//---------------------------------------------------------------------------


