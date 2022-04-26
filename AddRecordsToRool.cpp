//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DifferentFunctions.h"
#include "SomeFunctions.h"
#include "DocumentsFunctions.h"
#include "AddRecordsToRool.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "JvComponent"
#pragma link "JvExControls"
#pragma link "JvNavigationPane"
#pragma link "Placemnt"
#pragma resource "*.dfm"
//TAddRecordsToRoolForm *AddRecordsToRoolForm;
//---------------------------------------------------------------------------
//**********
//* public *
//**********
__fastcall TAddRecordsToRoolForm::TAddRecordsToRoolForm(TComponent* Owner,AnsiString iniFN,FBDatabase *fbdb,TDateTime dDate)
        : TForm(Owner),iniFileName(iniFN),fbDB(fbdb),d_date(dDate)
{
  iniSectionName="AddRecordsToRoolForm";
  FormStorage->IniFileName=iniFileName;
  FormStorage->IniSection=iniSectionName;
  pFrame=createAddRecordsToRoolFrame(this,iniFileName,iniSectionName,fbDB,workPanel);
  if(pFrame!=NULL)
  { pGrid=pFrame->workGrid;
    pDS=pFrame->getWorkDataSet();
    pFrame->PopupMenu=addrecordsMenu;
    pFrame->setStatusBar(statusBar);
    pFrame->getWorkDataSet()->SQLs->InsertSQL->Clear();
    pFrame->getWorkDataSet()->SQLs->DeleteSQL->Clear();
    loadPDS();
  }
}
//---------------------------------------------------------------------------
__fastcall TAddRecordsToRoolForm::~TAddRecordsToRoolForm(void)
{
  if(pFrame!=NULL){ delete pFrame; pFrame=NULL; }
}
//---------------------------------------------------------------------------

//***********
//* private *
//***********
void __fastcall TAddRecordsToRoolForm::loadPDS(void)
{
  if(pFrame!=NULL)
  { pFrame->workSource->DataSet->Active=false;
    AnsiString sql=formPubSelectSQL(d_date);
    pFrame->initSelectSQL(sql);
    pFrame->loadDS();
  }
}
//---------------------------------------------------------------------------

//*************
//* published *
//*************
void __fastcall TAddRecordsToRoolForm::closeItemClick(TObject *Sender)
{
  Close();        
}
//---------------------------------------------------------------------------
void __fastcall TAddRecordsToRoolForm::addRecordsItemClick(TObject *Sender)
{
TDateTime date1=beginOfMonth(d_date);
TDateTime date2=d_date;

  if(pGrid==NULL || pDS==NULL) return;
  if(pGrid->SelectedRows->Count==0) pGrid->SelectedRows->CurrentRowSelected=true;
  AnsiString sql;
  FBQuery *fbWQ=fbDB->getFBQ_T2();
  fbWQ->getTransaction()->StartTransaction();
  for(int i=0; i<pGrid->SelectedRows->Count; i++)
  { pDS->GotoBookmark((void *)pGrid->SelectedRows->Items[i].c_str());
    AnsiString pID=pDS->FieldByName("P_ID")->AsString;
    sql="execute procedure ADD_RECORD_TO_ROOL("+pID+",";
    sql+="'"+date1.DateString()+"',";
    sql+="'"+date2.DateString()+"')";
    if(fbWQ->execQuery("TDictationOnPaymentFrame::delDictItemClick",sql)==false)
    { fbWQ->getTransaction()->Rollback();
      return;
    }
  }
  fbWQ->getTransaction()->Commit();

  Close();
  ModalResult=mrOk;
}
//---------------------------------------------------------------------------
