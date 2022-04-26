//---------------------------------------------------------------------------
//Аналитический очет - Финансовые результаты (по поставщикам)
#include <vcl.h>
#pragma hdrstop

#include "DBClasses.h"
#include "DifferentFunctions.h"
#include "SomeFunctions.h"
#include "GridF.h"
#include "UniversalSelectPeriod.h"
#include "AnaliticalReportsFunctions.h"
#include "FinancialResultsForPubF.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "JvArrowButton"
#pragma link "JvComponent"
#pragma link "JvExControls"
#pragma link "JvNavigationPane"
#pragma link "JvSpeedButton"
#pragma link "frxClass"
#pragma link "frxDBSet"
#pragma link "frxExportXLS"
#pragma resource "*.dfm"
//TFinancialResultsForPubFrame *FinancialResultsForPubFrame;
//---------------------------------------------------------------------------
//**********
//* public *
//**********
__fastcall TFinancialResultsForPubFrame::TFinancialResultsForPubFrame(TComponent* Owner,AnsiString iniFN,FBDatabase *fbdb)
        : TFrame(Owner),iniFileName(iniFN),fbDB(fbdb)
{
  workDir=getAppDir();
  iniSectionName="FinancialResultsForPubFrame";
  dataBase=fbDB->getDatabase();
  resultFrame=NULL;
}
//---------------------------------------------------------------------------
__fastcall TFinancialResultsForPubFrame::~TFinancialResultsForPubFrame(void)
{
  saveProperties();
  if(resultFrame!=NULL){ delete resultFrame; resultFrame=NULL; }
}
//---------------------------------------------------------------------------
void __fastcall TFinancialResultsForPubFrame::initFrame(void)
{
  loadProperties();
  resultFrame=cteateFRForPubFrame(this,iniFileName,iniSectionName,fbDB,resultPanel);
  if(resultFrame!=NULL)
  {
    resultFrame->setStatusBar(resultBar);
  }
  else return;
  loadResultDS();
}
//---------------------------------------------------------------------------

//***********
//* private *
//***********
void __fastcall TFinancialResultsForPubFrame::loadProperties(void)
{
TIniFile *ini = new TIniFile(iniFileName);

  date1=ini->ReadDate(iniSectionName,"date1",TDateTime(Date().DateString()));
  date2=ini->ReadDate(iniSectionName,"date2",TDateTime(Date().DateString()));
  resultBox->ItemIndex=ini->ReadInteger(iniSectionName,"resultBox_ItemIndex",1);
  delete ini;
}
//---------------------------------------------------------------------------
void __fastcall TFinancialResultsForPubFrame::saveProperties(void)
{
TIniFile *ini = new TIniFile(iniFileName);

  ini->WriteDate(iniSectionName,"date1",date1);
  ini->WriteDate(iniSectionName,"date2",date2);
  ini->WriteInteger(iniSectionName,"resultBox_ItemIndex",resultBox->ItemIndex);
  delete ini;
}
//---------------------------------------------------------------------------
void __fastcall TFinancialResultsForPubFrame::setHeader(void)
{
  header->Caption=SetHeader("Финансовые результаты (по поставщикам)",date1,date2);
}
//---------------------------------------------------------------------------
void __fastcall TFinancialResultsForPubFrame::loadResultDS(void)
{
  if(resultFrame!=NULL)
  { AnsiString sql=formFRForPubSelectSQL(date1,date2,resultBox->ItemIndex+1);
    resultFrame->initSelectSQL(sql);
    resultFrame->loadDS();
    setHeader();
  }
}
//---------------------------------------------------------------------------

//*************
//* published *
//*************
void __fastcall TFinancialResultsForPubFrame::periodButtonClick(
      TObject *Sender)
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
  {
    loadResultDS();
  }
  delete USPF;
}
//---------------------------------------------------------------------------
void __fastcall TFinancialResultsForPubFrame::resultBoxChange(
      TObject *Sender)
{
  loadResultDS();
}
//---------------------------------------------------------------------------
//Печать докумнета
void __fastcall TFinancialResultsForPubFrame::printButtonClick(
      TObject *Sender)
{
  frxXLSExport->DefaultPath=workDir+"Out\\";
  frxXLSExport->FileName="Финансовые результаты (по поставщикам).xls";
  if(frxReport->LoadFromFile(workDir+"Reports\\FinancialResultsForPub.fr3")==false)
  { MessageDlg("Не могу загрузить файл с отчетом: "+workDir+"Reports\\FinancialResultsForPub.fr3",mtWarning,TMsgDlgButtons() << mbOK,0);
    return;
  }
  frxDS->DataSet=resultFrame->getWorkDataSet();
  frxReport->ShowReport(true);
}
//---------------------------------------------------------------------------
void __fastcall TFinancialResultsForPubFrame::frxReportGetValue(
      const AnsiString VarName, Variant &Value)
{
  if(VarName=="Title")
  { AnsiString s;
    if(resultBox->ItemIndex==0)
      s=SetHeader("Финансовые результаты по поставщикам (по данным поставщиков)",date1,date2);
    else
      s=SetHeader("Финансовые результаты по поставщикам (по данным подразделений)",date1,date2);
    Value=s;
  }
}
//---------------------------------------------------------------------------

