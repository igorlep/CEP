//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "loadDataFromApplications.h"
#include "DifferentFunctions.h"
#include "SomeFunctions.h"
#include "NextGridF.h"
#include "FormApplicationsForNMPFromSDFunctions.h"
#include "FormApplicationsForNMPFromSDF.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "JvComponent"
#pragma link "JvExControls"
#pragma link "JvNavigationPane"
#pragma link "JvArrowButton"
#pragma link "JvSpeedButton"
#pragma link "JvXPCheckCtrls"
#pragma link "JvXPCore"
#pragma link "JvExMask"
#pragma link "JvSpin"
#pragma resource "*.dfm"
//TFormApplicationsForNMPFromSDFrame *FormApplicationsForNMPFromSDFrame;
//---------------------------------------------------------------------------
//**********
//* public *
//**********
__fastcall TFormApplicationsForNMPFromSDFrame::TFormApplicationsForNMPFromSDFrame(TComponent* Owner,AnsiString iniFN,FBDatabase *fbdb)
        : TFrame(Owner),iniFileName(iniFN),fbDB(fbdb)
{
  workDir=getAppDir();
  iniSectionName="FormApplicationsForNMPFromSDFrame";
  dataBase=fbDB->getDatabase();
  lFrame=NULL; rFrame=NULL;
//  firstLoadFLG=true;

  loadProperties();
}
//---------------------------------------------------------------------------
__fastcall TFormApplicationsForNMPFromSDFrame::~TFormApplicationsForNMPFromSDFrame(void)
{
  saveProperties();
  if(lFrame!=NULL){ delete lFrame; lFrame=NULL; }
  if(rFrame!=NULL){ delete rFrame; rFrame=NULL; }
}
//---------------------------------------------------------------------------
void __fastcall TFormApplicationsForNMPFromSDFrame::initFrame(void)
{
  //Инициируем фрейм подразделение (почтамтов)
  if(lFrame!=NULL){ delete lFrame; lFrame=NULL; }
  lFrame=cteateSubdivisionsFrame(this,iniFileName,iniSectionName+"_sdFrame",fbDB,sdPanel);
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
  //Инициируем фрейм с заказанной ГЖП
  if(rFrame!=NULL){ delete rFrame; rFrame=NULL; }
  rFrame=createAppNMPFrame(this,iniFileName,iniSectionName+"_nmpFrame",fbDB,nmpPanel,SEdit,ConditionSBox);
  if(rFrame!=NULL)
  { rFrame->getGridFrame()->getWorkDataSet()->BeforePost=nmpDSBeforePost;
    rFrame->getGridFrame()->getWorkDataSet()->AfterPost=nmpDSAfterPost;
    rFrame->getGridFrame()->workGrid->OnMouseDown=nmpGridMouseDown;
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
  if(rFrame!=NULL) rFrame->gcHeader->Caption=lFrame->getGridFrame()->getFieldDataOfSelectedRecord("SD_NAME");
}
//---------------------------------------------------------------------------
void __fastcall TFormApplicationsForNMPFromSDFrame::loadDataSets(void)
{
//  firstLoadFLG=true;
  loadSubdivisionsDS();
//  firstLoadFLG=false;
//  loadNMPDS();
  setFocus();
}
//---------------------------------------------------------------------------
AnsiString __fastcall TFormApplicationsForNMPFromSDFrame::getRicipientID(void)
{
  return lFrame->getGridFrame()->getFieldDataOfSelectedRecord("SD_ID");
}
//---------------------------------------------------------------------------
AnsiString __fastcall TFormApplicationsForNMPFromSDFrame::getRicipientName(void)
{
  return lFrame->getGridFrame()->getFieldDataOfSelectedRecord("SD_NAME");
}
//---------------------------------------------------------------------------
AnsiString __fastcall TFormApplicationsForNMPFromSDFrame::getRicipientMail(void)
{
  return lFrame->getGridFrame()->getFieldDataOfSelectedRecord("SD_EMAIL");
}
//---------------------------------------------------------------------------

//***********
//* private *
//***********
void __fastcall TFormApplicationsForNMPFromSDFrame::loadProperties(void)
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
void __fastcall TFormApplicationsForNMPFromSDFrame::saveProperties(void)
{
TIniFile *ini = new TIniFile(iniFileName);

  ini->WriteInteger(iniSectionName,"leftPanel_Width",leftPanel->Width);
  ini->WriteInteger(iniSectionName,"activeFrame",activeFrame);
  ini->WriteInteger(iniSectionName,"month",monthBox->ItemIndex);
  ini->WriteInteger(iniSectionName,"year",yearEdit->Value);
  delete ini;
}
//---------------------------------------------------------------------------
void __fastcall TFormApplicationsForNMPFromSDFrame::loadSubdivisionsDS(void)
{
  if(lFrame!=NULL)
  { lFrame->getGCFrame()->loadDS(formSubdivisionsSelectSQL(tempDate));
    lFrame->getContentFrame()->setContent();
    if(rFrame!=NULL) loadNMPDS();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormApplicationsForNMPFromSDFrame::loadNMPDS(void)
{
AnsiString sdID;

  if(rFrame==NULL || rDS==NULL || lDS->RecordCount==0) return;
  rDS->Close();
  sdID=lFrame->getGridFrame()->getFieldDataOfSelectedRecord("SD_ID");
  rFrame->getGCFrame()->loadDS(formAppNMPSelectSQL(sdID,tempDate));
  rFrame->getContentFrame()->setContent();
}
//---------------------------------------------------------------------------
void __fastcall TFormApplicationsForNMPFromSDFrame::loadNMPSearchDS(void)
{
  if(rFrame!=NULL && rDS->RecordCount>0)
  { rFrame->getGridFrame()->initSearchSQL(formAppNMPSearchSQL());
    rFrame->getGridFrame()->loadSearchingDS();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormApplicationsForNMPFromSDFrame::setFocus(void)
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
void __fastcall TFormApplicationsForNMPFromSDFrame::setHeader(void)
{
char *mon[12]={"январь","февраль","март","апрель","май","июнь","июль","август","сентябрь","октябрь","ноябрь","декабрь"};

  header->Caption="Формирование заявок на газетно-журнальную продукцию от подразделений на "+AnsiString(mon[monthBox->ItemIndex])+" "+yearEdit->Text+" года";
}
//---------------------------------------------------------------------------

//*************
//* published *
//*************
void __fastcall TFormApplicationsForNMPFromSDFrame::onChangePeriod(TObject *Sender)
{
  periodStr=monthBox->Text+" "+yearEdit->Text+" года";

  tempDate=getDateFromMonthYearString(periodStr);
  loadDataSets();
  setHeader();
}
//---------------------------------------------------------------------------
//Следующий период
void __fastcall TFormApplicationsForNMPFromSDFrame::spinButtonBottomClick(TObject *Sender)
{
  if(monthBox->ItemIndex==0)
  { monthBox->ItemIndex=11;
    yearEdit->Value--;
  }
  else monthBox->ItemIndex--;
  onChangePeriod(this);
}
//---------------------------------------------------------------------------
//Предыдущий период
void __fastcall TFormApplicationsForNMPFromSDFrame::spinButtonTopClick(TObject *Sender)
{
  if(monthBox->ItemIndex==11)
  { monthBox->ItemIndex=0;
    yearEdit->Value++;
  }
  else monthBox->ItemIndex++;
  onChangePeriod(this);
}
//---------------------------------------------------------------------------

void __fastcall TFormApplicationsForNMPFromSDFrame::sdGridMouseDown(TObject *Sender,TMouseButton Button, TShiftState Shift, int X, int Y)
{
  activeFrame=1;
}
//---------------------------------------------------------------------------
void __fastcall TFormApplicationsForNMPFromSDFrame::sdSourceDataChange(TObject *Sender,TField *Field)
{
  if(lFrame!=NULL)
    lFrame->getContentFrame()->setContent();
//  if(firstLoadFLG==true) return;
  if(rFrame!=NULL)
  { loadNMPDS();
    if(rFrame->getGridFrame()->isSearchDSActive()==true) loadNMPSearchDS();
    rFrame->gcHeader->Caption=lFrame->getGridFrame()->getFieldDataOfSelectedRecord("SD_NAME");
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormApplicationsForNMPFromSDFrame::nmpGridMouseDown(TObject *Sender,TMouseButton Button, TShiftState Shift, int X, int Y)
{
  activeFrame=2;
}
//---------------------------------------------------------------------------
void __fastcall TFormApplicationsForNMPFromSDFrame::nmpDSBeforePost(TDataSet *DataSet)
{
FBQuery *fbWQ=newFBQuery(this,"TFormApplicationsForNMPFromSDFrame::nmpDSBeforePost",fbDB->getDatabase(),false);
AnsiString sql;
AnsiString t_id;

  if(fbWQ==NULL) return;
  if(rDS->FieldByName("T_ID")->IsNull)
  { if(!DataSet->FieldByName("TEMP_Q")->IsNull)
    { t_id=getNewKeyValue(fbDB,"TEMPLATE_TABLE");
      sql="insert into TEMPLATE_TABLE(T_ID,NMP_ID,SD_ID,TEMP_Q,TEMP_DATE) values(";
      sql+=t_id+",";
      sql+=rDS->FieldByName("NMP_ID")->AsString+",";
      sql+=lDS->FieldByName("SD_ID")->AsString+",";
      sql+=DataSet->FieldByName("TEMP_Q")->AsString+",";
      sql+="'"+tempDate.DateString()+"')";
      fbWQ->getTransaction()->StartTransaction();
      if(fbWQ->execQuery("TFormApplicationsForNMPFromSDFrame::nmpDSBeforePost",sql)==true)
        fbWQ->getTransaction()->Commit();
      else fbWQ->getTransaction()->Rollback();
    }
  }
  else
  { if(DataSet->FieldByName("TEMP_Q")->IsNull || DataSet->FieldByName("TEMP_Q")->AsInteger==0)
    { t_id=rDS->FieldByName("T_ID")->AsString;
      sql="delete from TEMPLATE_TABLE where T_ID="+t_id;
      fbWQ->getTransaction()->StartTransaction();
      if(fbWQ->execQuery("TFormApplicationsForNMPFromSDFrame::nmpDSBeforePost",sql)==true)
        fbWQ->getTransaction()->Commit();
      else fbWQ->getTransaction()->Rollback();
    }
  }
  delete fbWQ;
}
//---------------------------------------------------------------------------
void __fastcall TFormApplicationsForNMPFromSDFrame::nmpDSAfterPost(TDataSet *DataSet)
{
int rn=DataSet->RecNo;

  loadDataSets();
  DataSet->RecNo=rn;
}
//---------------------------------------------------------------------------
//Формирование сообщений для подразделений
void __fastcall TFormApplicationsForNMPFromSDFrame::formMessagesButtonClick(TObject *Sender)
{
  createMessage(this,iniFileName); //FormApplicationsForNMPFromSDFunctions.h
}
//---------------------------------------------------------------------------
//Загрузка из шаблона сведений о заказанной газетно-журнальной продукции
void __fastcall TFormApplicationsForNMPFromSDFrame::loadDataButtonClick(TObject *Sender)
{
TLoadDataFromApplicationsForm *LDF;

  try
  {
    LDF=new TLoadDataFromApplicationsForm(this,iniFileName,fbDB);
  }
  catch (Exception &exc)
  { MessageDlg("Ошибка создания формы LoadDataFromApplicationsForm\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return;
  }
  LDF->ShowModal();
  delete LDF;
//  int rn=rDS->RecNo;
  loadSubdivisionsDS();
//  loadNMPDS();
//  rDS->RecNo=rn;
}
//---------------------------------------------------------------------------
//Поиск по модели
void __fastcall TFormApplicationsForNMPFromSDFrame::searchTimerTimer(TObject *Sender)
{
  searchTimer->Enabled=false;
  if(activeFrame==2) loadNMPSearchDS();
}
//---------------------------------------------------------------------------
void __fastcall TFormApplicationsForNMPFromSDFrame::SEditChange(TObject *Sender)
{
  if(searchTimer->Enabled==true) searchTimer->Enabled=false;
  if(!SEdit->Text.IsEmpty()) searchTimer->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TFormApplicationsForNMPFromSDFrame::SEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
  if(Key==VK_RETURN || Key==VK_DOWN || Key==VK_UP)
  { Key=0;
    if(activeFrame==2 && rFrame!=NULL) rFrame->getGridFrame()->workGrid->SetFocus();
  }
  else if(Key == VK_ESCAPE){ Key=0; SEdit->Text=""; }
}
//---------------------------------------------------------------------------
void __fastcall TFormApplicationsForNMPFromSDFrame::ConditionSBoxClick(TObject *Sender)
{
  if(activeFrame==2) loadNMPSearchDS();
}
//---------------------------------------------------------------------------
void __fastcall TFormApplicationsForNMPFromSDFrame::FSButtonClick(TObject *Sender)
{
  if(activeFrame==2 && rFrame!=NULL) rFrame->getGridFrame()->forwardSearch();
}
//---------------------------------------------------------------------------
void __fastcall TFormApplicationsForNMPFromSDFrame::BSButtonClick(TObject *Sender)
{
  if(activeFrame==2 && rFrame!=NULL) rFrame->getGridFrame()->backwardSearch();
}
//---------------------------------------------------------------------------

