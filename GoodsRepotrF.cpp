//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DifferentFunctions.h"
#include "SomeFunctions.h"
#include "UniversalSelectPeriod.h"
#include "DocumentsFunctions.h"
#include "GoodsRepotrF.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "JvComponent"
#pragma link "JvExControls"
#pragma link "JvExMask"                  
#pragma link "JvNavigationPane"
#pragma link "JvSpin"
#pragma link "frxClass"
#pragma link "frxDBSet"
#pragma link "frxExportXLS"
#pragma resource "*.dfm"
//TGoodsRepotrFrame *GoodsRepotrFrame;
//---------------------------------------------------------------------------
//**********
//* public *
//**********
__fastcall TGoodsRepotrFrame::TGoodsRepotrFrame(TComponent* Owner,AnsiString iniFN,FBDatabase *fbdb)
        : TFrame(Owner),iniFileName(iniFN),fbDB(fbdb)
{
  iniSectionName="GoodsRepotFrame";
  workDir=getAppDir();
  yearEdit->Height=21;
  lFrame=NULL;
  rFrame=NULL;
}
//---------------------------------------------------------------------------
__fastcall TGoodsRepotrFrame::~TGoodsRepotrFrame(void)
{
  saveProperties();
  if(lFrame!=NULL){ delete lFrame; lFrame=NULL; }
  if(rFrame!=NULL){ delete rFrame; rFrame=NULL; }
}
//---------------------------------------------------------------------------
void __fastcall TGoodsRepotrFrame::initFrame(void)
{
  loadProperties();
  lFrame=createLeftFrame(this,iniFileName,iniSectionName,fbDB,lPanel,selectTypeBox->ItemIndex+1,lHeader,lBar);
  rFrame=createRightFrame(this,iniFileName,iniSectionName,fbDB,rPanel,selectTypeBox->ItemIndex+1,rHeader,rBar);
  loadDataSets();
  setHeader();
}
//---------------------------------------------------------------------------

//***********
//* private *
//***********
void __fastcall TGoodsRepotrFrame::loadProperties(void)
{
TIniFile *ini = new TIniFile(iniFileName);
unsigned short y,m,d;

  r_date=ini->ReadDate(iniSectionName,"periodDate",Date());
  selectTypeBox->ItemIndex=ini->ReadInteger(iniSectionName,"reportType",0);
  lPanel->Width=ini->ReadInteger(iniSectionName,"lPanel_Width",300);
  delete ini;
  periodStr=getMonthYearString(r_date);
  r_date.DecodeDate(&y,&m,&d);
  monthBox->ItemIndex=m-1;
  yearEdit->Value=y;
}
//---------------------------------------------------------------------------
void __fastcall TGoodsRepotrFrame::saveProperties(void)
{
TIniFile *ini = new TIniFile(iniFileName);

  ini->WriteDate(iniSectionName,"periodDate",r_date);
  ini->WriteInteger(iniSectionName,"reportType",selectTypeBox->ItemIndex);
  ini->WriteInteger(iniSectionName,"lPanel_Width",lPanel->Width);
  delete ini;
}
//---------------------------------------------------------------------------
void __fastcall TGoodsRepotrFrame::onChangePeriod(void)
{
  periodStr=monthBox->Text+" "+yearEdit->Text+" года";
  r_date=endOfMonth(getDateFromMonthYearString(periodStr));
  loadDataSets();
  setHeader();
}
//---------------------------------------------------------------------------
void __fastcall TGoodsRepotrFrame::loadLeftDS(void)
{
  if(lFrame!=NULL)
  { AnsiString sql=formLeftSelectSQL(beginOfMonth(r_date),r_date,selectTypeBox->ItemIndex+1);
    lFrame->initSelectSQL(sql);
    lFrame->loadDS();
  }
}
//---------------------------------------------------------------------------
void __fastcall TGoodsRepotrFrame::loadRihgtDS(void)
{
  if(rFrame!=NULL)
  { AnsiString sql=formRihgtSelectSQL(beginOfMonth(r_date),r_date,selectTypeBox->ItemIndex+1);
    rFrame->initSelectSQL(sql);
    rFrame->loadDS();
  }
}
//---------------------------------------------------------------------------
void __fastcall TGoodsRepotrFrame::loadDataSets(void)
{
  loadLeftDS();
  loadRihgtDS();
}
//---------------------------------------------------------------------------
void __fastcall TGoodsRepotrFrame::setHeader(void)
{
AnsiString s;

  s="Товарный отчет о ";
  if(selectTypeBox->ItemIndex==0) s+="поступлении";
  else s+="возврате";
  s+=" ГЖП за "+periodStr;
  header->Caption=s;
}
//---------------------------------------------------------------------------

//*************
//* published *
//*************
void __fastcall TGoodsRepotrFrame::yearEditChange(TObject *Sender)
{
  onChangePeriod();
}
//---------------------------------------------------------------------------
void __fastcall TGoodsRepotrFrame::monthBoxChange(TObject *Sender)
{
  onChangePeriod();
}
//---------------------------------------------------------------------------
void __fastcall TGoodsRepotrFrame::spinButtonBottomClick(TObject *Sender)
{
  if(monthBox->ItemIndex==0)
  { monthBox->ItemIndex=11;
    yearEdit->Value--;
  }
  else monthBox->ItemIndex--;
  onChangePeriod();
}
//---------------------------------------------------------------------------
void __fastcall TGoodsRepotrFrame::spinButtonTopClick(TObject *Sender)
{
  if(monthBox->ItemIndex==11)
  { monthBox->ItemIndex=0;
    yearEdit->Value++;
  }
  else monthBox->ItemIndex++;
  onChangePeriod();
}
//---------------------------------------------------------------------------
void __fastcall TGoodsRepotrFrame::selectTypeBoxChange(TObject *Sender)
{
  if(lFrame!=NULL){ delete lFrame; lFrame=NULL; }
  lFrame=createLeftFrame(this,iniFileName,iniSectionName,fbDB,lPanel,selectTypeBox->ItemIndex+1,lHeader,lBar);
  if(rFrame!=NULL){ delete rFrame; rFrame=NULL; }
  rFrame=createRightFrame(this,iniFileName,iniSectionName,fbDB,rPanel,selectTypeBox->ItemIndex+1,rHeader,rBar);
  loadDataSets();
  setHeader();
}
//---------------------------------------------------------------------------
//Печать очтета
void __fastcall TGoodsRepotrFrame::printButtonClick(TObject *Sender)
{
AnsiString fName=workDir+"Reports\\GoodsReport.fr3";
AnsiString sql;
FBQuery *fbRQ=fbDB->getFBQ_T1();

  if(FileExists(fName)==false)
  { MessageDlg("Не обнаружен файл с шаблоном отчета - "+fName,mtWarning,TMsgDlgButtons() << mbOK,0);
    return;
  }
  if(frxReport->LoadFromFile(fName)==false)
  { MessageDlg("Ошибка открытия файла с шаблоном отчета - "+fName,mtWarning,TMsgDlgButtons() << mbOK,0);
    return;
  }
  if(selectTypeBox->ItemIndex==0)
  { sql="select R_SUMMA from GET_REMAINDER(";
    sql+="'"+beginOfMonth(r_date).DateString()+"',1)";
    if(fbRQ->execQuery("TGoodsRepotrFrame::printButtonClick",sql)==false) return;
    rs=fbRQ->getQuery()->FieldByName("R_SUMMA")->AsFloat;
    sql="select ID,NAME,DOC_DATE,DOC_NUMBER,SUMMA,SUMMA,D_TYPE";
    sql+=" from GET_GM_FOR_AS(";
    sql+="'"+beginOfMonth(r_date).DateString()+"',";
    sql+="'"+r_date.DateString()+"')";
  }
  else
  { sql="select R_SUMMA from GET_REMAINDER(";
    sql+="'"+beginOfMonth(r_date).DateString()+"',2)";
    if(fbRQ->execQuery("TGoodsRepotrFrame::printButtonClick",sql)==false) return;
    rs=fbRQ->getQuery()->FieldByName("R_SUMMA")->AsFloat;
    sql="select ID,NAME,DOC_DATE,DOC_NUMBER,SUMMA,SUMMA,D_TYPE";
    sql+=" from GET_GM_FOR_RETURN(";
    sql+="'"+beginOfMonth(r_date).DateString()+"',";
    sql+="'"+r_date.DateString()+"')";
  }
  fbDS=fbDB->getFBDS_T1_T2();
  if(fbDS->execSelectSQL("TGoodsRepotrFrame::printButtonClick",sql)==false) return;
  frxXLSExport->FileName="Товарный отчет.xls";
  frxDS->DataSet=fbDS->getDataSet();
  frxReport->PrepareReport(true);
  frxReport->ShowPreparedReport();
  fbDS->getDataSet()->Close();
}
//---------------------------------------------------------------------------
void __fastcall TGoodsRepotrFrame::frxReportGetValue(const AnsiString VarName, Variant &Value)
{
  if(VarName=="OrgName")
  {
    Value=getConstValue(this,fbDB,"ORGANIZATION_NAME",r_date);
  }
  else if(VarName=="SS_Name")
  {
    Value=getConstValue(this,fbDB,"SS_NAME",r_date);
  }
  else if(VarName=="DocNum")
  { AnsiString s;
    unsigned short y,m,d;
    r_date.DecodeDate(&y,&m,&d);
    s="ТО-"+AnsiString().sprintf("%02d-%d",m,y);
    if(selectTypeBox->ItemIndex==0) s+="(п)";
    else s+="(в)";
    Value=s;
  }
  else if(VarName=="DocDate")
  { if(selectTypeBox->ItemIndex==0)
      Value=r_date.DateString();
    else
    { unsigned short y,m,d;
      r_date.DecodeDate(&y,&m,&d);
      m++; if(m>12){ m=1; y++; }
      d=10;
      Value=TDateTime(y,m,d).DateString();
    }
  }
  else if(VarName=="Date1")
  { if(selectTypeBox->ItemIndex==0)
      Value=beginOfMonth(r_date).DateString();
    else
    { unsigned short y,m,d;
      r_date.DecodeDate(&y,&m,&d);
      m++; if(m>12){ m=1; y++; }
      d=1;
      Value=TDateTime(y,m,d).DateString();
    }
  }
  else if(VarName=="Date2")
  { if(selectTypeBox->ItemIndex==0)
      Value=r_date.DateString();
    else
    { unsigned short y,m,d;
      r_date.DecodeDate(&y,&m,&d);
      m++; if(m>12){ m=1; y++; }
      d=10;
      Value=TDateTime(y,m,d).DateString();
    }
  }
  else if(VarName=="MOL")
  { AnsiString s;
    s=getConstValue(this,fbDB,"MOL_JOB_TITLE",r_date);
    s+=", ";
    s+=getConstValue(this,fbDB,"MOL_FIO",r_date);
    Value=s;
  }
  else if(VarName=="R_Type")
  { AnsiString s;
    if(selectTypeBox->ItemIndex==0) s="Поступление ГЖП";
    else s="Возврат ГЖП";
    Value=s;
  }
  else if(VarName=="GH_Name")
  { AnsiString s;
    if(fbDS->getDataSet()->FieldByName("D_TYPE")->AsInteger==1)
      s="Остаток на "+beginOfMonth(r_date).DateString();
    else s="Итого с остатком";
    Value=s;
  }
  else if(VarName=="GH_Summa")
  { AnsiString s;
    if(fbDS->getDataSet()->FieldByName("D_TYPE")->AsInteger==1)
      s=AnsiString().sprintf("%.02lf",rs);
    else
    { TfrxMemoView *Memo;
      Memo=(TfrxMemoView *)frxReport->FindObject("Memo58");
      as=Memo->Value;
      s=AnsiString().sprintf("%.02lf",as+rs);
    }
    Value=s;
  }
  else if(VarName=="GH")
  { AnsiString s;
    if(fbDS->getDataSet()->FieldByName("D_TYPE")->AsInteger==1) s="Приход";
    else s="Расход";
    Value=s;
  }
  else if(VarName=="GF_Name")
  { AnsiString s;
    if(fbDS->getDataSet()->FieldByName("D_TYPE")->AsInteger==1) s="Итого по приходу";
    else s="Итого по расходу";
    Value=s;
  }
  else if(VarName=="RS_Name")
  {
    Value="Остаток на "+r_date.DateString();
  }
  else if(VarName=="RS_Summa")
  { TfrxMemoView *Memo;
    Memo=(TfrxMemoView *)frxReport->FindObject("Memo58");
    ss=Memo->Value;
    Value=AnsiString().sprintf("%.02lf",as-ss+rs);
  }
  else if(VarName=="MOL_JT")
  {
    Value=getConstValue(this,fbDB,"MOL_JOB_TITLE",r_date);
  }
  else if(VarName=="MOL_FIO")
  {
    Value=getConstValue(this,fbDB,"MOL_FIO",r_date);
  }
}
//---------------------------------------------------------------------------


