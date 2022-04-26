//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "MWFunctions.h"
#include "SelectRicipient.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Placemnt"
#pragma link "DBGridEh"
#pragma link "JvComponent"
#pragma link "JvExControls"
#pragma link "JvXPButtons"
#pragma link "JvXPCore"
#pragma link "SmrtPanel"
#pragma link "FIBDataSet"
#pragma link "pFIBDataSet"
#pragma link "JvNavigationPane"
#pragma resource "*.dfm"
//TSelectRicipientForm *SelectRicipientForm;
//---------------------------------------------------------------------------
//**********
//* public *
//**********
__fastcall TSelectRicipientForm::TSelectRicipientForm(TComponent* Owner,AnsiString iniFN,FBDatabase *fbdb, short choice_place)
        : TForm(Owner),iniFileName(iniFN),fbDB(fbdb),choicePlace(choice_place)
{
AnsiString sql;

  if(choicePlace==1) iniSectionName="SelectRicipientForm_SUBDIVISIONS_TABLE";
  else iniSectionName="SelectRicipientForm_PUBLISHERS_TABLE";
  FormStorage->IniFileName=iniFileName;
  FormStorage->IniSection=iniSectionName;
  //Инициируем фрейм для выбора получателя сообщения
  rf=createRicipientsFrame(Owner,iniFileName,iniSectionName,fbDB,workPanel,choicePlace);
  if(rf!=NULL)
  { rf->setStatusBar(statusBar);
    rf->setHeader(header);
    rf->setHeaderCaption(rf->getTableTitle());
    rf->workGrid->OnDblClick=saveButtonClick;
    rf->initSelectSQL(formRicipientSelectSQL(choicePlace));
    rf->loadDS();
  }
}
//---------------------------------------------------------------------------
__fastcall TSelectRicipientForm::~TSelectRicipientForm(void)
{
  if(rf!=NULL){ delete rf; rf=NULL; }
}
//---------------------------------------------------------------------------
AnsiString __fastcall TSelectRicipientForm::getRicipientName(void)
{
  if(rf!=NULL)
  { if(choicePlace==1)
      return rf->getFieldDataOfSelectedRecord("SD_NAME");
    else return rf->getFieldDataOfSelectedRecord("P_NAME");
  }
  else return "";
}
//---------------------------------------------------------------------------
AnsiString __fastcall TSelectRicipientForm::getRicipientEMail(void)
{
  if(rf!=NULL)
  { if(choicePlace==1)
      return rf->getFieldDataOfSelectedRecord("SD_EMAIL");
    else return rf->getFieldDataOfSelectedRecord("P_EMAIL");
  }
  else return "";
}
//---------------------------------------------------------------------------

//*************
//* published *
//*************
void __fastcall TSelectRicipientForm::saveButtonClick(TObject *Sender)
{
  Close();
  ModalResult=mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TSelectRicipientForm::cancelButtonClick(TObject *Sender)
{
  Close();
  ModalResult=mrCancel;
}
//---------------------------------------------------------------------------

