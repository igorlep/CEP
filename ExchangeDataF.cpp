//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ExchangeDataFunctions.h"
#include "ExchangeDataF.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "JvComponent"
#pragma link "JvExControls"
#pragma link "JvNavigationPane"
#pragma link "JvBaseDlg"
#pragma link "JvSelectDirectory"
#pragma link "JvWaitingGradient"
#pragma link "JvWaitingProgress"
#pragma link "JvExComCtrls"
#pragma link "JvProgressBar"
#pragma link "JvXPButtons"
#pragma link "JvXPCore"
#pragma resource "*.dfm"
//TExchangeDataFrame *RExchangeDataFrame;
//---------------------------------------------------------------------------
//**********
//* public *
//**********
__fastcall TExchangeDataFrame::TExchangeDataFrame(TComponent* Owner,AnsiString iniFN,FBDatabase *fbdb,AnsiString Type)
        : TFrame(Owner),iniFileName(iniFN),fbDB(fbdb),type(Type)
{
  iniSectionName="ExchangeDataFrame";
  exchangeButton->Caption="Выгрузить";
  if(type=="rbSinchro")
    Header->Caption="Выгрузка справочников для синхронизации данных с подразделениями";
  else if(type=="prepareKeys")
    Header->Caption="Подготовка ключей настройки базы данных на подразделение";
  sdFrame=NULL;
  richEdit->Clear();
}
//---------------------------------------------------------------------------
__fastcall TExchangeDataFrame::~TExchangeDataFrame(void)
{
  saveProperties();
  if(sdFrame!=NULL){ delete sdFrame; sdFrame=NULL; }
}
//---------------------------------------------------------------------------
void __fastcall TExchangeDataFrame::initFrame(void)
{
  loadProperties();
  sdFrame=createED_SDFrame(this,iniFileName,iniSectionName,fbDB,workPanel);
  if(sdFrame!=NULL)
  { sdFrame->workGrid->PopupMenu=exchangeDataPopupMenu;
    sdFrame->setHeader(sdHeader);
    sdFrame->setHeaderCaption(sdFrame->getTableTitle());
    sdFrame->setStatusBar(sdStatusBar);
    AnsiString sql=formED_SDSelectSQL();
    sdFrame->initSelectSQL(sql);
    sdFrame->loadDS();
    sdFrame->workGrid->SetFocus();
  }
}
//---------------------------------------------------------------------------
class TGridFrame * __fastcall TExchangeDataFrame::getSDFrame(void)
{
  return sdFrame;
}
//---------------------------------------------------------------------------

//***********
//* private *
//***********
void __fastcall TExchangeDataFrame::loadProperties(void)
{
TIniFile *ini = new TIniFile(iniFileName);

  lPanel->Width=ini->ReadInteger(iniSectionName,"lPanel_Width",200);
  richEdit->Height=ini->ReadInteger(iniSectionName,"richEdit_Height",300);
  delete ini;
}
//---------------------------------------------------------------------------
void __fastcall TExchangeDataFrame::saveProperties(void)
{
TIniFile *ini = new TIniFile(iniFileName);

  ini->WriteInteger(iniSectionName,"lPanel_Width",lPanel->Width);
  ini->WriteInteger(iniSectionName,"richEdit_Height",richEdit->Height);
  delete ini;
}
//---------------------------------------------------------------------------

//*************
//* published *
//*************
//Формирование сообщений для отправки в подразделения
void __fastcall TExchangeDataFrame::messagesFormItemClick(TObject *Sender)
{
  if(type=="rbSinchro") createRBSinchroMessages(this,iniFileName);
  else if(type=="prepareKeys") prepareKeysOnSDMessages(this,iniFileName);
}
//---------------------------------------------------------------------------

