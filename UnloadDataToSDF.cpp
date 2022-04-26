//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DBClasses.h"
#include "DifferentFunctions.h"
#include "UniversalSelectPeriod.h"
#include "ExchangeDataFunctions.h"
#include "UnloadDataToSDF.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "JvComponent"
#pragma link "JvExControls"
#pragma link "JvNavigationPane"
#pragma link "JvSpeedButton"
#pragma resource "*.dfm"
//TUnloadDataToSDFrame *UnloadDataToSDFrame;
//---------------------------------------------------------------------------
//**********
//* public *
//**********
__fastcall TUnloadDataToSDFrame::TUnloadDataToSDFrame(TComponent* Owner,AnsiString iniFN,FBDatabase *fbdb)
        : TFrame(Owner),iniFileName(iniFN),fbDB(fbdb)
{
  iniSectionName="UnloadDataToSDFrame";
  sdFrame=NULL;
  nmpFrame=NULL;
}
//---------------------------------------------------------------------------
__fastcall TUnloadDataToSDFrame::~TUnloadDataToSDFrame(void)
{
  saveProperties();
  if(sdFrame!=NULL){ delete sdFrame; sdFrame=NULL; }
  if(nmpFrame!=NULL){ delete nmpFrame; nmpFrame=NULL; }
}
//---------------------------------------------------------------------------
void __fastcall TUnloadDataToSDFrame::initFrame(void)
{
  loadProperties();
  sdFrame=createED_SDFrame(this,iniFileName,iniSectionName+"_sdFrame",fbDB,sdPanel);
  if(sdFrame!=NULL)
  { sdFrame->setHeader(sdHeader);
    sdFrame->setHeaderCaption(sdFrame->getTableTitle());
    sdFrame->setStatusBar(sdStatusBar);
    sdFrame->workSource->OnDataChange=sdSourceDataChange;
  }
  nmpFrame=createED_NMPFrame(this,iniFileName,iniSectionName+"_nmpFrame",fbDB,nmpPanel);
  if(nmpFrame!=NULL)
  { nmpFrame->setHeader(nmpHeader);
    nmpFrame->setHeaderCaption(nmpFrame->getTableTitle());
    nmpFrame->setStatusBar(nmpStatusBar);
  }
  loadSD_DS();
  setHeader();
}
//---------------------------------------------------------------------------
AnsiString __fastcall TUnloadDataToSDFrame::getRicipientID(void)
{
  return sdFrame->getFieldDataOfSelectedRecord("SD_ID");
}
//---------------------------------------------------------------------------
AnsiString __fastcall TUnloadDataToSDFrame::getRicipientName(void)
{
  return sdFrame->getFieldDataOfSelectedRecord("SD_NAME");
}
//---------------------------------------------------------------------------
AnsiString __fastcall TUnloadDataToSDFrame::getRicipientMail(void)
{
  return sdFrame->getFieldDataOfSelectedRecord("SD_EMAIL");
}
//---------------------------------------------------------------------------

//***********
//* private *
//***********
void __fastcall TUnloadDataToSDFrame::loadProperties(void)
{
TIniFile *ini = new TIniFile(iniFileName);

  sdPanel->Width=ini->ReadInteger(iniSectionName,"sdPanel_Width",200);
  date1=ini->ReadDate(iniSectionName,"date1",TDateTime(Date().DateString()));
  date2=ini->ReadDate(iniSectionName,"date2",TDateTime(Date().DateString()));
  delete ini;
}
//---------------------------------------------------------------------------
void __fastcall TUnloadDataToSDFrame::saveProperties(void)
{
TIniFile *ini = new TIniFile(iniFileName);

  ini->WriteInteger(iniSectionName,"sdPanel_Width",sdPanel->Width);
  ini->WriteDate(iniSectionName,"date1",date1);
  ini->WriteDate(iniSectionName,"date2",date2);
  delete ini;
}
//---------------------------------------------------------------------------
void __fastcall TUnloadDataToSDFrame::setHeader(void)
{
  header->Caption=SetHeader("Выгрузка для подразделений данных о поступлении ГЖП",date1,date2);
}
//---------------------------------------------------------------------------
void __fastcall TUnloadDataToSDFrame::loadSD_DS(void)
{
  if(sdFrame!=NULL)
  { sdFrame->initSelectSQL(formED_SDSelectSQL());
    sdFrame->loadDS();
    loadNMP_DS();
  }
}
//---------------------------------------------------------------------------
void __fastcall TUnloadDataToSDFrame::loadNMP_DS(void)
{
  if(nmpFrame!=NULL && sdFrame!=NULL && sdFrame->getWorkDataSet()->RecordCount > 0)
  { AnsiString sdID=sdFrame->getFieldDataOfSelectedRecord("SD_ID");
    nmpFrame->initSelectSQL(formED_NMPSelectSQL(sdID,date1,date2));
    nmpFrame->loadDS();
  }
}
//---------------------------------------------------------------------------

//*************
//* published *
//*************
void __fastcall TUnloadDataToSDFrame::sdSourceDataChange(TObject *Sender,TField *Field)
{
  loadNMP_DS();
}
//---------------------------------------------------------------------------
//Выбор периода
void __fastcall TUnloadDataToSDFrame::periodButtonClick(TObject *Sender)
{
TUniversalSelectPeriodForm *USPF;

  try
  {
    USPF=new TUniversalSelectPeriodForm(this,iniFileName,"UniversalSelectPeriodForm_SpisanieNMPFrame","Сформировать отчет за период",date1,date2);
  }
  catch (Exception &exc)
  { MessageDlg("Ошибка создания формы UniversalSelectPeriodForm\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return;
  }
  if(USPF->ShowModal()==mrOk)
  { setHeader();
    loadNMP_DS();
  }
  delete USPF;
}
//---------------------------------------------------------------------------
//формирование сообщений
void __fastcall TUnloadDataToSDFrame::createMessagesButtonClick(
      TObject *Sender)
{
  createShipmentDataToSubdivisions(this,iniFileName);
}
//---------------------------------------------------------------------------

