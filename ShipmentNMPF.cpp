//---------------------------------------------------------------------------

#include <vcl.h>
//#include <math.h>
#pragma hdrstop
#include "SomeFunctions.h"
//#include "DirectWorkExcel.h"
#include "DBClasses.h"
#include "ShipmentNMPFunctions.h"
#include "UniversalSelectPeriod.h"
#include "DifferentFunctions.h"
#include "ShipmentNMPF.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "JvComponent"
#pragma link "JvExControls"
#pragma link "JvNavigationPane"
#pragma link "JvArrowButton"
#pragma link "JvSpeedButton"
#pragma link "JvXPCheckCtrls"
#pragma link "JvXPCore"
#pragma link "frxClass"
#pragma link "frxDBSet"
#pragma link "frxExportRTF"
#pragma link "frxExportXLS"
#pragma resource "*.dfm"
//TShipmentNMPFrame *ShipmentNMPFrame;
//---------------------------------------------------------------------------
//**********
//* public *
//**********
__fastcall TShipmentNMPFrame::TShipmentNMPFrame(TComponent* Owner,AnsiString iniFN,FBDatabase *fbdb)
        : TFrame(Owner),iniFileName(iniFN), fbDB(fbdb)
{
  workDir=getAppDir();
  iniSectionName="ShipmentNMPToSubdivisions";
  dataBase=fbDB->getDatabase();

  sdFrame=NULL; poFrame=NULL;
  nmpFrame=NULL; pFrame=NULL;

  date1=TDateTime("01.05.2010");
  date2=TDateTime("31.05.2010");
}
//---------------------------------------------------------------------------
__fastcall TShipmentNMPFrame::~TShipmentNMPFrame(void)
{
  saveProperties();
  if(nmpFrame!=NULL){ delete nmpFrame; nmpFrame=NULL; }
  if(pFrame!=NULL){ delete pFrame; pFrame=NULL; }
  if(sdFrame!=NULL){ delete sdFrame; sdFrame=NULL; }
  if(poFrame!=NULL){ delete poFrame; poFrame=NULL; }
}
//---------------------------------------------------------------------------
void __fastcall TShipmentNMPFrame::initFrame(void)
{
  loadProperties();
  //Инициируем фрейм подразделение (почтамтов)
  lFrame=NULL;
  if(groupOnPostOffices->Checked==false)
  { if(sdFrame==NULL)
    { sdFrame=cteateShipmentSubdivisionsFrame(this,iniFileName,iniSectionName+"_sdFrame",fbDB,sdPanel,NULL,NULL);
      if(sdFrame!=NULL)
      { sdFrame->getGridFrame()->workGrid->OnMouseDown=sdGridMouseDown;
        sdFrame->getGridFrame()->workSource->OnDataChange=sdSourceDataChange;
//        sdFrame->loadDS=&loadSubdivisionsDS;
        lFrame=sdFrame;
      }
    }
    else return;
  }
  else
  { if(poFrame==NULL)
    { poFrame=cteateShipmentPostOfficesFrame(this,iniFileName,iniSectionName+"_poFrame",fbDB,sdPanel,NULL,NULL);
      if(poFrame!=NULL)
      { poFrame->getGridFrame()->workGrid->OnMouseDown=sdGridMouseDown;
        poFrame->getGridFrame()->workSource->OnDataChange=sdSourceDataChange;
//        poFrame->loadDS=&loadSubdivisionsDS;
        lFrame=poFrame;
      }
    }
    else return;
  }
  if(lFrame!=NULL)
  { lGrid=lFrame->getGridFrame()->workGrid;
    lDS=lFrame->getGridFrame()->getWorkDataSet();
  }

  //Инициируем фрейм ГЖП
  rFrame=NULL;
  if(groupOnPublishers->Checked==false)
  { nmpFrame=createShipmentNMPFrame(this,iniFileName,iniSectionName+"_nmpFrame",fbDB,nmpPanel,SEdit,ConditionSBox);
    if(nmpFrame!=NULL)
    { nmpFrame->getGridFrame()->workGrid->OnMouseDown=nmpGridMouseDown;
//      nmpFrame->loadDS=&loadNMPDS;
//      nmpFrame->loadSearchDS=&loadNMPSearchDS;
      rFrame=nmpFrame;
    }
  }
  else
  { pFrame=createShipmentPFrame(this,iniFileName,iniSectionName+"_pFrame",fbDB,nmpPanel,SEdit,ConditionSBox);
    if(pFrame!=NULL)
    { pFrame->getGridFrame()->workGrid->OnMouseDown=nmpGridMouseDown;
//      pFrame->loadDS=&loadNMPDS;
//      pFrame->loadSearchDS=&loadNMPSearchDS;
      rFrame=pFrame;
    }
  }
  if(rFrame!=NULL)
  { rGrid=rFrame->getGridFrame()->workGrid;
    rDS=rFrame->getGridFrame()->getWorkDataSet();
  }
  loadDataSets();
  setHeader();
}
//---------------------------------------------------------------------------
void __fastcall TShipmentNMPFrame::loadDataSets(void)
{
//  firstLoadFLG=true;
  loadSubdivisionsDS();
//  firstLoadFLG=false;
//  loadNMPDS();
  setFocus();
}
//---------------------------------------------------------------------------
void __fastcall TShipmentNMPFrame::setFocus(void)
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
AnsiString __fastcall TShipmentNMPFrame::getRicipientID(void)
{
AnsiString rID;

  if(groupOnPostOffices->Checked==false)
    rID=sdFrame->getGridFrame()->getFieldDataOfSelectedRecord("SD_ID");
  else
    rID=poFrame->getGridFrame()->getFieldDataOfSelectedRecord("PO_ID");
  return rID;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TShipmentNMPFrame::getRicipientName(void)
{
AnsiString rName;

  if(groupOnPostOffices->Checked==false)
    rName=sdFrame->getGridFrame()->getFieldDataOfSelectedRecord("SD_NAME");
  else
    rName=poFrame->getGridFrame()->getFieldDataOfSelectedRecord("PO_NAME");
  return rName;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TShipmentNMPFrame::getRicipientMail(void)
{
AnsiString rMail;

  if(groupOnPostOffices->Checked==false)
    rMail=sdFrame->getGridFrame()->getFieldDataOfSelectedRecord("SD_EMAIL");
  else
  { AnsiString sql;
    FBQuery *fbRQ=fbDB->getFBQ_T1();
    sql="select SD_EMAIL from SUBDIVISIONS_TABLE where";
    sql+=" PO_ID="+poFrame->getGridFrame()->getFieldDataOfSelectedRecord("PO_ID");
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
void __fastcall TShipmentNMPFrame::loadProperties(void)
{
TIniFile *ini = new TIniFile(iniFileName);

  leftPanel->Width=ini->ReadInteger(iniSectionName,"leftPanel_Width",250);
  groupOnPostOffices->Checked=ini->ReadBool(iniSectionName,"groupOnPostOffices_Checked",false);
  groupOnPublishers->Checked=ini->ReadBool(iniSectionName,"groupOnPublishers_Checked",false);
  activeFrame=ini->ReadInteger(iniSectionName,"activeFrame",1);
  date1=ini->ReadDateTime(iniSectionName,"date1",date1);
  date2=ini->ReadDateTime(iniSectionName,"date2",date2);
  delete ini;
  if(groupOnPublishers->Checked==false)
  { torg13Item->Visible=true;
    torg13SelectItem->Visible=true;
    createMessagesButton->Enabled=true;
  }
  else
  { torg13Item->Visible=false;
    torg13SelectItem->Visible=false;
    createMessagesButton->Enabled=false;
  }
}
//---------------------------------------------------------------------------
void __fastcall TShipmentNMPFrame::saveProperties(void)
{
TIniFile *ini = new TIniFile(iniFileName);

  ini->WriteInteger(iniSectionName,"leftPanel_Width",leftPanel->Width);
  ini->WriteBool(iniSectionName,"groupOnPostOffices_Checked",groupOnPostOffices->Checked);
  ini->WriteBool(iniSectionName,"groupOnPublishers_Checked",groupOnPublishers->Checked);
  ini->WriteInteger(iniSectionName,"activeFrame",activeFrame);
  ini->WriteDateTime(iniSectionName,"date1",date1);
  ini->WriteDateTime(iniSectionName,"date2",date2);
  delete ini;
}
//---------------------------------------------------------------------------
void __fastcall TShipmentNMPFrame::loadSubdivisionsDS(void)
{
  if(lFrame!=NULL)
  { lFrame->getGCFrame()->loadDS(formShipmentSubdivisionsSelectSQL(groupOnPostOffices->Checked,date1,date2));
    if(groupOnPostOffices->Checked==false)
      lFrame->getContentFrame()->setContent();
    if(rFrame!=NULL && lDS->RecordCount>0) loadNMPDS();
  }
}
//---------------------------------------------------------------------------
void __fastcall TShipmentNMPFrame::loadNMPDS(void)
{
AnsiString sdID;

  if(lFrame==NULL || rFrame==NULL) return;
  if(groupOnPostOffices->Checked==false)
    sdID=lFrame->getGridFrame()->getFieldDataOfSelectedRecord("SD_ID");
  else
    sdID=lFrame->getGridFrame()->getFieldDataOfSelectedRecord("PO_ID");
  if(sdID!=NULL && !sdID.IsEmpty() && sdID!="0")
  { rFrame->getGCFrame()->loadDS(formShipmentNMPSelectSQL(sdID,groupOnPostOffices->Checked,groupOnPublishers->Checked,date1,date2));
    if(groupOnPublishers->Checked==false)
      rFrame->getContentFrame()->setContent();
  }
}
//---------------------------------------------------------------------------
void __fastcall TShipmentNMPFrame::loadNMPSearchDS(void)
{
  if(lFrame==NULL || rFrame==NULL) return;

  AnsiString sdID;
  if(groupOnPostOffices->Checked==false)
    sdID=lFrame->getGridFrame()->getFieldDataOfSelectedRecord("SD_ID");
  else
    sdID=lFrame->getGridFrame()->getFieldDataOfSelectedRecord("PO_ID");
  AnsiString sql=formShipmentNMPSearchSQL(sdID,groupOnPostOffices->Checked,groupOnPublishers->Checked,date1,date2);
  rFrame->getGridFrame()->initSearchSQL(sql);
  rFrame->getGridFrame()->loadSearchingDS();
}
//---------------------------------------------------------------------------
void __fastcall TShipmentNMPFrame::setHeader(void)
{
  header->Caption=SetHeader("Отгрузка газетно-журнальной продукции",date1,date2);
}
//---------------------------------------------------------------------------

//*************
//* published *
//*************
void __fastcall TShipmentNMPFrame::periodButtonClick(TObject *Sender)
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
    setHeader();
  }
  delete USPF;
}
//---------------------------------------------------------------------------
void __fastcall TShipmentNMPFrame::searchTimerTimer(TObject *Sender)
{
  searchTimer->Enabled=false;
  if(activeFrame==2) loadNMPSearchDS();
}
//---------------------------------------------------------------------------
void __fastcall TShipmentNMPFrame::SEditChange(TObject *Sender)
{
  if(searchTimer->Enabled==true) searchTimer->Enabled=false;
  if(!SEdit->Text.IsEmpty()) searchTimer->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TShipmentNMPFrame::SEditKeyDown(TObject *Sender,WORD &Key,TShiftState Shift)
{
  if(Key==VK_RETURN || Key==VK_DOWN || Key==VK_UP)
  { Key=0;
    if(activeFrame==2 && rFrame!=NULL) rFrame->getGridFrame()->workGrid->SetFocus();
  }
  else if(Key == VK_ESCAPE){ Key=0; SEdit->Text=""; }
}
//---------------------------------------------------------------------------
void __fastcall TShipmentNMPFrame::ConditionSBoxClick(TObject *Sender)
{
  if(activeFrame==2) loadNMPSearchDS();
}
//---------------------------------------------------------------------------
void __fastcall TShipmentNMPFrame::FSButtonClick(TObject *Sender)
{
  if(activeFrame==2 && rFrame!=NULL) rFrame->getGridFrame()->forwardSearch();
}
//---------------------------------------------------------------------------
void __fastcall TShipmentNMPFrame::BSButtonClick(TObject *Sender)
{
  if(activeFrame==2 && rFrame!=NULL) rFrame->getGridFrame()->backwardSearch();
}
//---------------------------------------------------------------------------

void __fastcall TShipmentNMPFrame::sdGridMouseDown(TObject *Sender,TMouseButton Button, TShiftState Shift, int X, int Y)
{
  activeFrame=1;
}
//---------------------------------------------------------------------------
void __fastcall TShipmentNMPFrame::sdSourceDataChange(TObject *Sender,TField *Field)
{
  if(groupOnPostOffices->Checked==false && lFrame!=NULL)
    lFrame->getContentFrame()->setContent();
//  if(firstLoadFLG==true) return;
  if(rFrame!=NULL)
  { loadNMPDS();
    if(rFrame->getGridFrame()->isSearchDSActive()==true) loadNMPSearchDS();
  }
}
//---------------------------------------------------------------------------
void __fastcall TShipmentNMPFrame::groupOnPostOfficesClick(TObject *Sender)
{
  lFrame=NULL;
  if(groupOnPostOffices->Checked==false)
  { if(poFrame!=NULL){ delete poFrame; poFrame=NULL; }
    if(sdFrame!=NULL)
    { lFrame=sdFrame;
      return;
    }
    sdFrame=cteateShipmentSubdivisionsFrame(this,iniFileName,iniSectionName+"_sdFrame",fbDB,sdPanel,NULL,NULL);
    if(sdFrame!=NULL)
    { sdFrame->getGridFrame()->workGrid->OnMouseDown=sdGridMouseDown;
      sdFrame->getGridFrame()->workSource->OnDataChange=sdSourceDataChange;
//      sdFrame->loadDS=&loadSubdivisionsDS;
      lFrame=sdFrame;
    }
  }
  else
  { if(sdFrame!=NULL){ delete sdFrame; sdFrame=NULL; }
    if(poFrame!=NULL)
    { lFrame=poFrame;
      return;
    }
    poFrame=cteateShipmentPostOfficesFrame(this,iniFileName,iniSectionName+"_poFrame",fbDB,sdPanel,NULL,NULL);
    if(poFrame!=NULL)
    { poFrame->getGridFrame()->workGrid->OnMouseDown=sdGridMouseDown;
      poFrame->getGridFrame()->workSource->OnDataChange=sdSourceDataChange;
//      poFrame->loadDS=&loadSubdivisionsDS;
      lFrame=poFrame;
    }
  }
  if(lFrame!=NULL)
  { lGrid=lFrame->getGridFrame()->workGrid;
    lDS=lFrame->getGridFrame()->getWorkDataSet();
  }
  loadSubdivisionsDS();
}
//---------------------------------------------------------------------------
void __fastcall TShipmentNMPFrame::nmpGridMouseDown(TObject *Sender,TMouseButton Button, TShiftState Shift, int X, int Y)
{
  activeFrame=2;
}
//---------------------------------------------------------------------------
void __fastcall TShipmentNMPFrame::groupOnPublishersClick(TObject *Sender)
{
  rFrame=NULL;
  if(groupOnPublishers->Checked==false)
  { if(pFrame!=NULL){ delete pFrame; pFrame=NULL; }
    if(nmpFrame!=NULL)
    { rFrame=nmpFrame;
      return;
    }
    nmpFrame=createShipmentNMPFrame(this,iniFileName,iniSectionName+"_nmpFrame",fbDB,nmpPanel,SEdit,ConditionSBox);
    if(nmpFrame!=NULL)
    { nmpFrame->getGridFrame()->workGrid->OnMouseDown=nmpGridMouseDown;
//      nmpFrame->loadDS=&loadNMPDS;
//      nmpFrame->loadSearchDS=&loadNMPSearchDS;
      rFrame=nmpFrame;
    }
    torg13Item->Visible=true;
    torg13SelectItem->Visible=true;
    createMessagesButton->Enabled=true;
  }
  else
  { if(nmpFrame!=NULL){ delete nmpFrame; nmpFrame=NULL; }
    if(pFrame!=NULL)
    { rFrame=pFrame;
      return;
    }
    pFrame=createShipmentPFrame(this,iniFileName,iniSectionName+"_pFrame",fbDB,nmpPanel,SEdit,ConditionSBox);
    if(pFrame!=NULL)
    { pFrame->getGridFrame()->workGrid->OnMouseDown=nmpGridMouseDown;
//      pFrame->loadDS=&loadNMPDS;
//      pFrame->loadSearchDS=&loadNMPSearchDS;
      rFrame=pFrame;
    }
    torg13Item->Visible=false;
    torg13SelectItem->Visible=false;
    createMessagesButton->Enabled=false;
  }
  if(rFrame!=NULL)
  { rGrid=rFrame->getGridFrame()->workGrid;
    rDS=rFrame->getGridFrame()->getWorkDataSet();
  }
  loadNMPDS();
}
//---------------------------------------------------------------------------

//Отчет об отгрузке ГЖП в подразделения
void __fastcall TShipmentNMPFrame::shipmentReportItemClick(TObject *Sender)
{
AnsiString sql;
FBDataSet *fbDS=NULL;
TpFIBDataSet *rds;
//AnsiString workDir=getAppDir();

  frxXLSExport->DefaultPath=workDir+"\\Out\\";

 if(groupOnPostOffices->Checked==false) frxXLSExport->FileName="Отгрузка ГЖП в подразделения.xls";
  else frxXLSExport->FileName="Отгрузка ГЖП в почтамты.xls";

  if(lDS->RecordCount==0) return;
  if(lGrid->SelectedRows->Count==0) lGrid->Selection->Rows->CurrentRowSelected=true;

  if(lGrid->SelectedRows->Count==1)
  { if(groupOnPublishers->Checked==false)
    { rGrid=nmpFrame->getGridFrame()->workGrid;
      rds=nmpFrame->getGridFrame()->getWorkDataSet();
      if(frxReport->LoadFromFile(workDir+"\\Reports\\ShipMentReport1.fr3")==false)
      { MessageDlg("Не могу загрузить файл с отчетом: "+workDir+"\\Reports\\ShipMentReport1.fr3",mtWarning,TMsgDlgButtons() << mbOK,0);
        return;
      }
    }
    else
    { rGrid=pFrame->getGridFrame()->workGrid;
      rds=pFrame->getGridFrame()->getWorkDataSet();
      if(frxReport->LoadFromFile(workDir+"\\Reports\\ShipMentReport2.fr3")==false)
      { MessageDlg("Не могу загрузить файл с отчетом: "+workDir+"\\Reports\\ShipMentReport2.fr3",mtWarning,TMsgDlgButtons() << mbOK,0);
        return;
      }
    }
  }
  else
  { try
    {
      fbDS=newFBDataSet(this,"TShipmentNMPFrame::shipmentReportItemClick",dataBase,true);
    }
    catch (Exception &exc)
    { MessageDlg("Ошибка создания объекта FBDataSet.\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
      return;
    }
    if(groupOnPublishers->Checked==false) rGrid=nmpFrame->getGridFrame()->workGrid;
    else rGrid=pFrame->getGridFrame()->workGrid;
    AnsiString sql;
    AnsiString s="";
    rGrid->Visible=false;
    for(int i=0; i<lGrid->SelectedRows->Count; ++i)
    { lDS->GotoBookmark((void *)lGrid->SelectedRows->Items[i].c_str());
      if(i > 0) s+=",";
      if(groupOnPostOffices->Checked==false) s+=lDS->FieldByName("SD_ID")->AsString;
      else s+=lDS->FieldByName("PO_ID")->AsString;
    }
    if(groupOnPostOffices->Checked==false && groupOnPublishers->Checked==false)
    { sql="select s.sd_name as name_sd,p.p_name,n.nmp_name,n.stavka_nds,";
      sql+="e.e_price,";
      sql+="sum(f.FACT_Q) as quant,";
      sql+="sum(f.FACT_Q*e.e_price) as summa";
      sql+=" from facts_table f,exits_table e,nmp_table n,subdivisions_table s,";
      sql+="publishers_table p";
      sql+=" where e.e_id=f.e_id and s.sd_id=f.sd_id and n.nmp_id=e.nmp_id";
      sql+=" and p.p_id=n.p_id";
      sql+=" and e.e_date between '"+date1.DateString()+"'";
      sql+=" and '"+date2.DateString()+"'";
      sql+=" and s.sd_id in ("+s+")";
      sql+=" group by s.sd_name,p.p_name,n.nmp_name,e.e_price,n.stavka_nds";
      sql+=" order by s.sd_name,p.p_name,n.nmp_name";
      if(frxReport->LoadFromFile(workDir+"\\Reports\\ShipMentReport1_1.fr3")==false)
      { MessageDlg("Не могу загрузить файл с отчетом: "+workDir+"\\Reports\\ShipMentReport1.fr3",mtWarning,TMsgDlgButtons() << mbOK,0);
        return;
      }
    }
    else if(groupOnPostOffices->Checked==true && groupOnPublishers->Checked==false)
    { sql="select po.po_name as name_sd,p.p_name,n.nmp_name,e.e_price,";
      sql+="n.stavka_nds,";
      sql+="sum(f.FACT_Q) as quant,";
      sql+="sum(f.FACT_Q*e.e_price) as summa";
      sql+=" from facts_table f,exits_table e,nmp_table n,subdivisions_table s,";
      sql+="post_offices_table po,publishers_table p";
      sql+=" where e.e_id=f.e_id and s.sd_id=f.sd_id and po.po_id=s.po_id";
      sql+=" and n.nmp_id=e.nmp_id and p.p_id=n.p_id";
      sql+=" and e.e_date between '"+date1.DateString()+"'";
      sql+=" and '"+date2.DateString()+"'";
      sql+=" and po.po_id in ("+s+")";
      sql+=" group by po.po_name,p.p_name,n.nmp_name,e.e_price,n.stavka_nds";
      sql+=" order by po.po_name,p.p_name,n.nmp_name";
      if(frxReport->LoadFromFile(workDir+"\\Reports\\ShipMentReport1_1.fr3")==false)
      { MessageDlg("Не могу загрузить файл с отчетом: "+workDir+"\\Reports\\ShipMentReport1.fr3",mtWarning,TMsgDlgButtons() << mbOK,0);
        return;
      }
    }
    else if(groupOnPostOffices->Checked==false && groupOnPublishers->Checked==true)
    { sql+="select s.sd_name as name_sd,p.p_name,";
      sql+="sum(f.FACT_Q) as quant,";
      sql+="sum(f.FACT_Q*e.e_price) as summa";
      sql+=" from facts_table f,exits_table e,subdivisions_table s,";
      sql+="nmp_table n,publishers_table p";
      sql+=" where e.e_id=f.e_id and s.sd_id=f.sd_id and n.nmp_id=e.nmp_id";
      sql+=" and p.p_id=n.p_id";
      sql+=" and e.e_date between '"+date1.DateString()+"'";
      sql+=" and '"+date1.DateString()+"'";
      sql+=" and s.sd_id in ("+s+")";
      sql+=" group by s.sd_name,p.p_name";
      sql+=" order by s.sd_name,p.p_name";
      if(frxReport->LoadFromFile(workDir+"\\Reports\\ShipMentReport2_1.fr3")==false)
      { MessageDlg("Не могу загрузить файл с отчетом: "+workDir+"\\Reports\\ShipMentReport1.fr3",mtWarning,TMsgDlgButtons() << mbOK,0);
        return;
      }
    }
    else if(groupOnPostOffices->Checked==true && groupOnPublishers->Checked==true)
    { sql="select po.po_name as name_sd,p.p_name,";
      sql+="sum(f.FACT_Q) as quant,";
      sql+="sum(f.FACT_Q*e.e_price) as summa";
      sql+=" from facts_table f,exits_table e,subdivisions_table s,";
      sql+="post_offices_table po,nmp_table n,publishers_table p";
      sql+=" where e.e_id=f.e_id and s.sd_id=f.sd_id and po.po_id=s.po_id";
      sql+=" and n.nmp_id=e.nmp_id and p.p_id=n.p_id";
      sql+=" and e.e_date between '"+date1.DateString()+"'";
      sql+=" and '"+date2.DateString()+"'";
      sql+=" and po.po_id in ("+s+")";
      sql+=" group by po.po_name,p.p_name";
      sql+=" order by po.po_name,p.p_name";
      if(frxReport->LoadFromFile(workDir+"\\Reports\\ShipMentReport2_1.fr3")==false)
      { MessageDlg("Не могу загрузить файл с отчетом: "+workDir+"\\Reports\\ShipMentReport1.fr3",mtWarning,TMsgDlgButtons() << mbOK,0);
        return;
      }
    }
    if(fbDS->execSelectSQL("TShipmentNMPFrame::shipmentReportItemClick",sql)==false)
    { delete fbDS;
      return;
    }
    rds=fbDS->getDataSet();
  }
  frxDS->DataSet=rds;
  rGrid->Visible=true;
  frxReport->ShowReport(true);
  if(fbDS!=NULL) delete fbDS;
}
//---------------------------------------------------------------------------
//Печать отчета об отгрузке (краткого)
void __fastcall TShipmentNMPFrame::shortShipmentReportItemClick(
      TObject *Sender)
{
int rn=lDS->RecNo;

  frxXLSExport->DefaultPath=workDir+"Out\\";
  frxXLSExport->FileName="Отгрузка ГЖП (краткий отчет)";

  if(frxReport->LoadFromFile(workDir+"Reports\\ShipmentNMPReport.fr3")==false)
  { MessageDlg("Не могу загрузить файл с отчетом: "+workDir+"Reports\\ShipmentNMPReport.fr3",mtWarning,TMsgDlgButtons() << mbOK,0);
    return;
  }
  frxDS->DataSet=lDS;
  frxReport->ShowReport(true);
  lDS->RecNo=rn;
}
//---------------------------------------------------------------------------
//Печать накладной Торг-13 на отрузку ГЖП в подразделение
void __fastcall TShipmentNMPFrame::torg13ItemClick(TObject *Sender)
{
  fullPrintFLG=true;
  printTorg13(this,dataBase,lDS,rDS,1,1); //SomeFunctions.h
}
//---------------------------------------------------------------------------
//Печать накладной Торг-13 (выборочно)
void __fastcall TShipmentNMPFrame::torg13SelectItemClick(TObject *Sender)
{
  fullPrintFLG=false;
  printTorg13(this,dataBase,lDS,rDS,1,2); //SomeFunctions.h
}
//---------------------------------------------------------------------------
void __fastcall TShipmentNMPFrame::frxReportGetValue(const AnsiString VarName, Variant &Value)
{
  if(VarName=="Title")
  { AnsiString s="Отгрузка газетно-журнальной продукции в ";
    if(groupOnPostOffices->Checked==false) s+=sdFrame->getGridFrame()->getFieldDataOfSelectedRecord("SD_NAME");
    else s+=poFrame->getGridFrame()->getFieldDataOfSelectedRecord("PO_NAME");
    s=SetHeader(s,date1,date2);
//    if(date1==date2) s+=" за "+date1.DateString();
//    else s+=" за период с "+date1.DateString()+" по "+date2.DateString();
//    s+=" года";
    if(groupOnPublishers->Checked==true || groupOnPostOffices->Checked==true)
    { bool flg=false;
      s+=" (группировка по";
      if(groupOnPublishers->Checked==true)
      { s+=" поставщику";
        flg=true;
      }
      if(groupOnPostOffices->Checked==true)
      { if(flg) s+=" и";
        s+=" почтамту";
      }
      s+=")";
    }
    Value=s;
  }
  else if(VarName=="Title1")
  { AnsiString s="Отгрузка газетно-журнальной продукции";
    s=SetHeader(s,date1,date2);
//    if(date1==date2) s+=" за "+date1.DateString();
//    else s+=" за период с "+date1.DateString()+" по "+date2.DateString();
//    s+=" года";
    Value=s;
  }
  else if(VarName=="RECIPIENT_NAME")
  { AnsiString s;
    if(groupOnPostOffices->Checked==false) s=sdFrame->getGridFrame()->getFieldDataOfSelectedRecord("SD_NAME");
    else s=poFrame->getGridFrame()->getFieldDataOfSelectedRecord("PO_NAME");
    Value=s;
  }
  else if(VarName=="SD_NAME")
  {
//    AnsiString s;
//    if(groupOnPostOffices->Checked==false) s=sdFrame->getGridFrame()->getFieldDataOfSelectedRecord("SD_NAME");
//    else s=poFrame->getGridFrame()->getFieldDataOfSelectedRecord("PO_NAME");
    Value=getRicipientName();
  }
  else if(VarName=="Organization")
  {
    Value=getConstValue(this,fbDB,"ORGANIZATION_NAME",date2);
  }
  else if(VarName=="DOC_NUMBER")
  { unsigned short y,m,d;
    date2.DecodeDate(&y,&m,&d);
    AnsiString s="от_"; //отгрузка
    if(groupOnPostOffices->Checked==false) s+=sdFrame->getGridFrame()->getFieldDataOfSelectedRecord("SD_ID");
    else s+=poFrame->getGridFrame()->getFieldDataOfSelectedRecord("PO_ID");
    s+="_"+AnsiString().sprintf("%02d",m)+"-"+AnsiString().sprintf("%d",y);
    if(fullPrintFLG==false) s+=" - доп.";
    Value=s;
  }
  else if(VarName=="DOC_DATE")
  {
    Value=date2.DateString();
  }
  else if(VarName=="SS_Name")
  {
    Value=getConstValue(this,fbDB,"SS_NAME",date2);
  }
  else if(VarName=="SJT")
  {
    Value=getConstValue(this,fbDB,"MOL_JOB_TITLE",date2);
  }
  else if(VarName=="SFIO")
  {
    Value=getConstValue(this,fbDB,"MOL_FIO",date2);
  }
  else if(VarName=="Propis")
  { TfrxMemoView *Memo;
    Memo=(TfrxMemoView *)frxReport->FindObject("Memo79");
    Value=f_to_str(double(Memo->Value));
  }
}
//---------------------------------------------------------------------------
//Сообщение с накладной Торг-13
void __fastcall TShipmentNMPFrame::cmTorg13ItemClick(TObject *Sender)
{
  createTorg13Message(this,iniFileName,1,1);
}
//---------------------------------------------------------------------------
//Сообщение с накладной Торг-13 (выборочно)
void __fastcall TShipmentNMPFrame::cmSelectTorg13ItemClick(TObject *Sender)
{
  createTorg13Message(this,iniFileName,1,2);
}
//---------------------------------------------------------------------------

