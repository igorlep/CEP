//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DBClasses.h"
#include "DifferentFunctions.h"
#include "SomeFunctions.h"
#include "NextGridF.h"
#include "AddEditPPFunctions.h"
#include "AddEditPP.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Placemnt"
#pragma link "JvComponent"
#pragma link "JvExControls"
#pragma link "JvNavigationPane"
#pragma link "FIBDataSet"
#pragma link "pFIBDataSet"
#pragma resource "*.dfm"
//TAddEditPPForm *AddEditPPForm;
//---------------------------------------------------------------------------
//**********
//* public *
//**********
__fastcall TAddEditPPForm::TAddEditPPForm(TComponent* Owner,AnsiString iniFN,class FBDatabase *fbdb,AnsiString pp_identifier)
        : TForm(Owner),iniFileName(iniFN),fbDB(fbdb),ppIdentifier(pp_identifier)
{
  iniSectionName="TAddEditPPForm_"+ppIdentifier;
  header->Caption=getConstName(this,fbDB,ppIdentifier);
  historyFrame=NULL;
  constID="";
}
//---------------------------------------------------------------------------
__fastcall TAddEditPPForm::~TAddEditPPForm(void)
{
  saveProperties();
  if(historyFrame!=NULL){ delete historyFrame; historyFrame=NULL; }
}
//---------------------------------------------------------------------------
void __fastcall TAddEditPPForm::initForm(void)
{
AnsiString sql;
FBQuery *fbRQ=fbDB->getFBQ_T1();

  loadProperties();
  sql="select ID,CONST_KIND from CONSTANTS_TABLE where IDENTIFIER='"+ppIdentifier+"'";
  if(fbRQ->execQuery("TAddEditPPForm::initForm",sql)==false) return;
  constID=fbRQ->getQuery()->FieldByName("ID")->AsString;
  const_kind=fbRQ->getQuery()->FieldByName("CONST_KIND")->AsInteger;
  fbRQ->getQuery()->Close();
  historyFrame=createHistoryFrame(this,iniFileName,iniSectionName,fbDB,workPanel,const_kind);
  if(historyFrame!=NULL)
  { historyFrame->getGridFrame()->getWorkDataSet()->BeforePost=ppBeforePost;
    historyFrame->getGridFrame()->getWorkDataSet()->BeforeInsert=ppBeforeInsert;
    loadDS();
    if(const_kind!=1)
    { Caption="«начение параметра";
      if(historyFrame->getGridFrame()->getWorkDataSet()->RecordCount>0)
        historyFrame->addRecord->Enabled=false;
    }
  }
}
//---------------------------------------------------------------------------

//***********
//* private *
//***********
void __fastcall TAddEditPPForm::loadProperties(void)
{
TIniFile *ini = new TIniFile(iniFileName);

  Left=ini->ReadInteger(iniSectionName,"Left",0);
  Top=ini->ReadInteger(iniSectionName,"Top",0);
  Width=ini->ReadInteger(iniSectionName,"Width",500);
  Height=ini->ReadInteger(iniSectionName,"Height",400);
  delete ini;
}
//---------------------------------------------------------------------------
void __fastcall TAddEditPPForm::saveProperties(void)
{
TIniFile *ini = new TIniFile(iniFileName);

  ini->WriteInteger(iniSectionName,"Left",Left);
  ini->WriteInteger(iniSectionName,"Top",Top);
  ini->WriteInteger(iniSectionName,"Width",Width);
  ini->WriteInteger(iniSectionName,"Height",Height);
  delete ini;
}
//---------------------------------------------------------------------------
void __fastcall TAddEditPPForm::loadDS(void)
{
AnsiString sql="";

  if(historyFrame==NULL) return;
  historyFrame->getGCFrame()->loadDS(formHistorySelectSQL(ppIdentifier));
}
//---------------------------------------------------------------------------

//*************
//* published *
//*************
void __fastcall TAddEditPPForm::ppBeforePost(TDataSet *DataSet)
{
  DataSet->FieldByName("CONST_ID")->AsString=constID;
}
//---------------------------------------------------------------------------
void __fastcall TAddEditPPForm::ppBeforeInsert(TDataSet *DataSet)
{
  if(const_kind!=1 && historyFrame->getGridFrame()->getWorkDataSet()->RecordCount>0)
  { MessageDlg("Ќельз€ добавить еще одно значение непериодической констаны",mtWarning,TMsgDlgButtons() << mbOK,0);
    DataSet->Cancel();
  }
}
//---------------------------------------------------------------------------

