//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DifferentFunctions.h"
#include "SomeFunctions.h"
#include "NextGridF.h"
#include "FormApplicationsForNMPToPublishersFunctions.h"
#include "AddEditApplicationForNMP.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "JvComponent"
#pragma link "JvExControls"
#pragma link "JvNavigationPane"
#pragma link "Placemnt"
#pragma resource "*.dfm"
//TAddEditApplicationForNMPForm *AddEditApplicationForNMPForm;
//---------------------------------------------------------------------------
//**********
//* public *
//**********
__fastcall TAddEditApplicationForNMPForm::TAddEditApplicationForNMPForm(TComponent* Owner,AnsiString iniFN,FBDatabase *fbdb,AnsiString nmpid,TDateTime tDate,AnsiString nmpName)
        : TForm(Owner),iniFileName(iniFN),fbDB(fbdb),nmpID(nmpid),tempDate(tDate)
{
  workDir=getAppDir();
  iniSectionName="AddEditApplicationForNMPForm";
  FormStorage->IniFileName=iniFileName;
  FormStorage->IniSection=iniSectionName;
  header->Caption=nmpName;
  changeFLG=false;
  appFrame=createAppFrame(this,iniFileName,iniSectionName,fbDB,workPanel);
  if(appFrame!=NULL)
  { aDS=appFrame->getWorkDataSet();
    appFrame->PopupMenu=editAppMenu;
    appFrame->setStatusBar(statusBar);
    appFrame->getWorkDataSet()->SQLs->InsertSQL->Clear();
    appFrame->getWorkDataSet()->SQLs->DeleteSQL->Clear();
    appFrame->getWorkDataSet()->BeforePost=appDSBeforePost;
    appFrame->getWorkDataSet()->AfterPost=appDSAfterPost;
    loadAppDS();
  }
}
//---------------------------------------------------------------------------
__fastcall TAddEditApplicationForNMPForm::~TAddEditApplicationForNMPForm(void)
{
  if(appFrame!=NULL){ delete appFrame; appFrame=NULL; }
}
//---------------------------------------------------------------------------

//***********
//* private *
//***********
void __fastcall TAddEditApplicationForNMPForm::loadAppDS(void)
{
  if(appFrame!=NULL)
  { appFrame->workSource->DataSet->Active=false;
    AnsiString sql=formAppSelectSQL(nmpID,tempDate);
    appFrame->initSelectSQL(sql);
    appFrame->loadDS();
  }
}
//---------------------------------------------------------------------------

//*************
//* published *
//*************
void __fastcall TAddEditApplicationForNMPForm::appDSBeforePost(TDataSet *DataSet)
{
FBQuery *fbWQ=newFBQuery(this,"TAddEditApplicationForNMPForm::appDSBeforePost",fbDB->getDatabase(),false);
AnsiString sql;
AnsiString t_id;

  if(fbWQ==NULL) return;
  if(aDS->FieldByName("T_ID")->IsNull)
  { if(!DataSet->FieldByName("TEMP_Q")->IsNull)
    { t_id=getNewKeyValue(fbDB,"TEMPLATE_TABLE");
      sql="insert into TEMPLATE_TABLE(T_ID,NMP_ID,SD_ID,TEMP_Q,TEMP_DATE) values(";
      sql+=t_id+",";
      sql+=nmpID+",";
      sql+=aDS->FieldByName("SD_ID")->AsString+",";
      sql+=DataSet->FieldByName("TEMP_Q")->AsString+",";
      sql+="'"+tempDate.DateString()+"')";
      fbWQ->getTransaction()->StartTransaction();
      if(fbWQ->execQuery("TAddEditApplicationForNMPForm::appDSBeforePost",sql)==true)
        fbWQ->getTransaction()->Commit();
      else fbWQ->getTransaction()->Rollback();
    }
  }
  else
  { if(DataSet->FieldByName("TEMP_Q")->IsNull || DataSet->FieldByName("TEMP_Q")->AsInteger==0)
    { t_id=aDS->FieldByName("T_ID")->AsString;
      sql="delete from TEMPLATE_TABLE where T_ID="+t_id;
      fbWQ->getTransaction()->StartTransaction();
      if(fbWQ->execQuery("TAddEditApplicationForNMPForm::appDSBeforePost",sql)==true)
        fbWQ->getTransaction()->Commit();
      else fbWQ->getTransaction()->Rollback();
    }
  }
  delete fbWQ;
}
//---------------------------------------------------------------------------
void __fastcall TAddEditApplicationForNMPForm::appDSAfterPost(TDataSet *DataSet)
{
int rn=DataSet->RecNo;

  changeFLG=true;
  loadAppDS();
  DataSet->RecNo=rn;
}
//---------------------------------------------------------------------------
void __fastcall TAddEditApplicationForNMPForm::saveItemClick(TObject *Sender)
{
  if(aDS->State==dsEdit) aDS->Post();
}
//---------------------------------------------------------------------------
void __fastcall TAddEditApplicationForNMPForm::cancelItemClick(TObject *Sender)
{
  if(aDS->State==dsEdit) aDS->Cancel();
}
//---------------------------------------------------------------------------
void __fastcall TAddEditApplicationForNMPForm::closeItemClick(TObject *Sender)
{
  if(aDS->State==dsEdit) aDS->Post();
  Close();
}
//---------------------------------------------------------------------------
void __fastcall TAddEditApplicationForNMPForm::editAppMenuPopup(TObject *Sender)
{
  if(aDS->State==dsEdit)
  { saveItem->Enabled=true;
    cancelItem->Enabled=true;
  }
  else
  { saveItem->Enabled=false;
    cancelItem->Enabled=false;
  }
}
//---------------------------------------------------------------------------

