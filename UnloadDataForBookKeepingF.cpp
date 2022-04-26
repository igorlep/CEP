//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DifferentFunctions.h"
#include "UniversalSelectPeriod.h"
#include "GridF.h"
#include "UnloadDataForBookKeepingFunctions.h"
#include "UnloadDataForBookKeepingF.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "JvComponent"
#pragma link "JvExControls"
#pragma link "JvNavigationPane"
#pragma link "JvSpeedButton"
#pragma resource "*.dfm"
//TUnloadDataForBookKeepingFrame *UnloadDataForBookKeepingFrame;
//---------------------------------------------------------------------------
//**********
//* public *
//**********
__fastcall TUnloadDataForBookKeepingFrame::TUnloadDataForBookKeepingFrame(TComponent* Owner,AnsiString iniFN,FBDatabase *fbdb)
        : TFrame(Owner),iniFileName(iniFN),fbDB(fbdb)
{
  iniSectionName="UnloadDataForBookKeepingFrame";
  dataFrame=NULL;
  loadProperties();
  setHeader();
}
//---------------------------------------------------------------------------
__fastcall TUnloadDataForBookKeepingFrame::~TUnloadDataForBookKeepingFrame(void)
{
  saveProperties();
  if(dataFrame!=NULL){ delete dataFrame; dataFrame=NULL; }
}
//---------------------------------------------------------------------------
void __fastcall TUnloadDataForBookKeepingFrame::initFrame(void)
{
  if(typeComboBox->ItemIndex==0 || typeComboBox->ItemIndex==3)
  { dataFrame=cteateUnloadDataForBookPFrame(this,iniFileName,iniSectionName+"_PFrame",fbDB,workPanel,typeComboBox->ItemIndex+1);
    if(dataFrame!=NULL)
    {
      dataFrame->setStatusBar(dataStatusBar);
    }
    else return;
  }
  else if(typeComboBox->ItemIndex==1 || typeComboBox->ItemIndex==2)
  { dataFrame=cteateUnloadDataForBookSDFrame(this,iniFileName,iniSectionName+"_SDFrame",fbDB,workPanel,typeComboBox->ItemIndex+1);
    if(dataFrame!=NULL)
    {
      dataFrame->setStatusBar(dataStatusBar);
    }
    else return;
  }
  loadDS();
}
//---------------------------------------------------------------------------
TDBGridEh * __fastcall TUnloadDataForBookKeepingFrame::getGrid(void)
{
  if(dataFrame==NULL) return NULL;
  return dataFrame->workGrid;
}
//---------------------------------------------------------------------------
TpFIBDataSet * __fastcall TUnloadDataForBookKeepingFrame::getDS(void)
{
  if(dataFrame==NULL) return NULL;
  return dataFrame->getWorkDataSet();
}
//---------------------------------------------------------------------------

//***********
//* private *
//***********
void __fastcall TUnloadDataForBookKeepingFrame::loadProperties(void)
{
TIniFile *ini = new TIniFile(iniFileName);

  typeComboBox->ItemIndex=ini->ReadInteger(iniSectionName,"typeComboBox_itemIndex",0);
  date1=ini->ReadDate(iniSectionName,"date1",TDateTime(Date().DateString()));
  date2=ini->ReadDate(iniSectionName,"date2",TDateTime(Date().DateString()));
  delete ini;
}
//---------------------------------------------------------------------------
void __fastcall TUnloadDataForBookKeepingFrame::saveProperties(void)
{
TIniFile *ini = new TIniFile(iniFileName);

  ini->WriteInteger(iniSectionName,"typeComboBox_itemIndex",typeComboBox->ItemIndex);
  ini->WriteDate(iniSectionName,"date1",date1);
  ini->WriteDate(iniSectionName,"date2",date2);
  delete ini;
}
//---------------------------------------------------------------------------
void __fastcall TUnloadDataForBookKeepingFrame::setHeader(void)
{
  header->Caption=SetHeader("Выгрузка данных для бухгалтерии УФПС",date1,date2);
}
//---------------------------------------------------------------------------
void __fastcall TUnloadDataForBookKeepingFrame::loadDS(void)
{
  if(dataFrame!=NULL)
  { AnsiString sql=formUnloadDataForBookSelectSQL(typeComboBox->ItemIndex+1,date1,date2);
    dataFrame->initSelectSQL(sql);
    dataFrame->loadDS();
  }
}
//---------------------------------------------------------------------------

//*************
//* published *
//*************
void __fastcall TUnloadDataForBookKeepingFrame::periodButtonClick(
      TObject *Sender)
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
    loadDS();
  }
  delete USPF;
}
//---------------------------------------------------------------------------
void __fastcall TUnloadDataForBookKeepingFrame::typeComboBoxChange(TObject *Sender)
{
  if(dataFrame!=NULL){ delete dataFrame; dataFrame=NULL; }
  initFrame();
}
//---------------------------------------------------------------------------
//Формирование сообщения для бухгалтерии УФПС
void __fastcall TUnloadDataForBookKeepingFrame::createButtonClick(
      TObject *Sender)
{
  createMessage(this,iniFileName,typeComboBox->ItemIndex+1);
}
//---------------------------------------------------------------------------

