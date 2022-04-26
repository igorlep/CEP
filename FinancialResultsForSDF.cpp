//---------------------------------------------------------------------------
//Аналитический очет - Финансовые результаты (по подразделениям)
#include <vcl.h>
#pragma hdrstop

#include "DBClasses.h"
#include "DifferentFunctions.h"
#include "GridF.h"
#include "UniversalSelectPeriod.h"
#include "AnaliticalReportsFunctions.h"
#include "FinancialResultsForSDF.h"
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
#pragma resource "*.dfm"
//TFinancialResultsForSDFrame *FinancialResultsForSDFrame;
//---------------------------------------------------------------------------
//**********
//* public *
//**********
__fastcall TFinancialResultsForSDFrame::TFinancialResultsForSDFrame(TComponent* Owner,AnsiString iniFN,FBDatabase *fbdb)
        : TFrame(Owner),iniFileName(iniFN),fbDB(fbdb)
{
  workDir=ExtractFilePath(iniFileName);
  iniSectionName="FinancialResultsFrame";
  dataBase=fbDB->getDatabase();
  resultFrame=NULL;
}
//---------------------------------------------------------------------------
__fastcall TFinancialResultsForSDFrame::~TFinancialResultsForSDFrame(void)
{
  saveProperties();
  if(resultFrame!=NULL){ delete resultFrame; resultFrame=NULL; }
}
//---------------------------------------------------------------------------
void __fastcall TFinancialResultsForSDFrame::initFrame(void)
{
  loadProperties();
  cteateFrame();
}
//---------------------------------------------------------------------------

//***********
//* private *
//***********
void __fastcall TFinancialResultsForSDFrame::loadProperties(void)
{
TIniFile *ini = new TIniFile(iniFileName);

  date1=ini->ReadDate(iniSectionName,"date1",TDateTime(Date().DateString()));
  date2=ini->ReadDate(iniSectionName,"date2",TDateTime(Date().DateString()));
  groupOnPostOffices->Checked=ini->ReadBool(iniSectionName,"groupOnPostOffices_Checked",false);
  delete ini;
}
//---------------------------------------------------------------------------
void __fastcall TFinancialResultsForSDFrame::saveProperties(void)
{
TIniFile *ini = new TIniFile(iniFileName);

  ini->WriteDate(iniSectionName,"date1",date1);
  ini->WriteDate(iniSectionName,"date2",date2);
  ini->WriteBool(iniSectionName,"groupOnPostOffices_Checked",groupOnPostOffices->Checked);
  delete ini;
}
//---------------------------------------------------------------------------
void __fastcall TFinancialResultsForSDFrame::setHeader(void)
{
  if(groupOnPostOffices->Checked==false)
    header->Caption=SetHeader("Финансовые результаты (по подразделениям)",date1,date2);
  else
    header->Caption=SetHeader("Финансовые результаты (по почтамтам)",date1,date2);
}
//---------------------------------------------------------------------------
void __fastcall TFinancialResultsForSDFrame::loadResultDS(void)
{
AnsiString sql;

  if(resultFrame!=NULL)
  { sql=formFRForSDSelectSQL(date1,date2,groupOnPostOffices->Checked);
    if(!sql.IsEmpty())
    { resultFrame->initSelectSQL(sql);
      resultFrame->loadDS();
    }
    setHeader();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFinancialResultsForSDFrame::cteateFrame(void)
{
  if(resultFrame!=NULL){ delete resultFrame; resultFrame=NULL; }
  if(groupOnPostOffices->Checked==false)
    resultFrame=createFRForSDFrame(this,iniFileName,iniSectionName+"_SD",fbDB,resultPanel);
  else
    resultFrame=createFRForPOFrame(this,iniFileName,iniSectionName+"_PO",fbDB,resultPanel);

  if(resultFrame!=NULL)
  { resultFrame->setStatusBar(resultBar);
    loadResultDS();
  }
}
//---------------------------------------------------------------------------

//*************
//* published *
//*************
void __fastcall TFinancialResultsForSDFrame::periodButtonClick(TObject *Sender)
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
void __fastcall TFinancialResultsForSDFrame::groupOnPostOfficesClick(TObject *Sender)
{
  cteateFrame();
}
//---------------------------------------------------------------------------
//Печать отчета
void __fastcall TFinancialResultsForSDFrame::printButtonClick(TObject *Sender)
{
  if(groupOnPostOffices->Checked==false)
  { frxXLSExport->DefaultPath=workDir+"Out\\";
    frxXLSExport->FileName="Финансовые результаты (по подразделениям).xls";
    if(frxReport->LoadFromFile(workDir+"Reports\\FinancialResultsForSD.fr3")==false)
    { MessageDlg("Не могу загрузить файл с отчетом: "+workDir+"Reports\\FinancialResultsForSD.fr3",mtWarning,TMsgDlgButtons() << mbOK,0);
      return;
    }
  }
  else
  { frxXLSExport->DefaultPath=workDir+"Out\\";
    frxXLSExport->FileName="Финансовые результаты (по почтамтам).xls";
    if(frxReport->LoadFromFile(workDir+"Reports\\FinancialResultsForPO.fr3")==false)
    { MessageDlg("Не могу загрузить файл с отчетом: "+workDir+"Reports\\FinancialResultsForPO.fr3",mtWarning,TMsgDlgButtons() << mbOK,0);
      return;
    }
  }
  frxDS->DataSet=resultFrame->getWorkDataSet();
  frxReport->ShowReport(true);
}
//---------------------------------------------------------------------------
void __fastcall TFinancialResultsForSDFrame::frxReportGetValue(const AnsiString VarName, Variant &Value)
{
  if(VarName=="Title")
  { AnsiString s;
    if(groupOnPostOffices->Checked==false)
     s=SetHeader("Финансовые результаты по подразделениям",date1,date2);
    else
     s=SetHeader("Финансовые результаты по почтамтам",date1,date2);
    Value=s;
  }
}
//---------------------------------------------------------------------------

