//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "DifferentFunctions.h"
#include "GetDate.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "JvComponent"
#pragma link "JvExControls"
#pragma link "JvXPButtons"
#pragma link "JvXPCore"
#pragma link "Placemnt"
#pragma link "SmrtPanel"
#pragma link "ToolEdit"
#pragma link "JvNavigationPane"
#pragma resource "*.dfm"
//TGetDateForm *GetDateForm;
//---------------------------------------------------------------------------
//**********
//* public *
//**********
__fastcall TGetDateForm::TGetDateForm(TComponent* Owner,AnsiString iniFN,FBDatabase *fbdb,AnsiString nmp_id)
        : TForm(Owner),iniFileName(iniFN),fbDB(fbdb)
{
FBQuery *fbRQ=fbDB->getFBQ_T1();
TpFIBQuery *Q=fbRQ->getQuery();
AnsiString sql;

  FormStorage->IniFileName=iniFileName;
  FormStorage->IniSection="GetDateForm";

  gridFrame=NULL;

  sql="select NMP_NAME from NMP_TABLE";
  sql+=" where NMP_ID="+nmp_id;
  if(fbRQ->execQuery("TGetDateForm::TGetDateForm",sql)==false) return;
  header->Caption="Издание: \""+Q->FieldByName("NMP_NAME")->AsString+"\"";
  Q->Close();
}
//---------------------------------------------------------------------------
__fastcall TGetDateForm::~TGetDateForm(void)
{
  if(gridFrame!=NULL){ delete gridFrame; gridFrame=NULL; }
}
//---------------------------------------------------------------------------
//***********
//* private *
//***********
void __fastcall TGetDateForm::FormShow(TObject *Sender)
{
  if(gridFrame->getWorkDataSet()->RecordCount==0) JXPBOK->Enabled=false;
}
//---------------------------------------------------------------------------
//*************
//* published *
//*************
void __fastcall TGetDateForm::JXPBOKClick(TObject *Sender)
{
  Close();
  ModalResult=mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TGetDateForm::JXPBCancelClick(TObject *Sender)
{
  Close();
  ModalResult=mrCancel;
}
//---------------------------------------------------------------------------

