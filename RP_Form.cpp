//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "RBWorkFunctions.h"
#include "RP_Form.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Placemnt"
#pragma resource "*.dfm"
//TNextGridForm *RPForm;
//---------------------------------------------------------------------------
//**********
//* public *
//**********
__fastcall TRPForm::TRPForm(TComponent* Owner,AnsiString iniFN,AnsiString iniSect,FBDatabase *fb_db,AnsiString formName,AnsiString nmp_ID)
        : TForm(Owner),iniFileName(iniFN),iniSection(iniSect),fbDB(fb_db),nmpID(nmp_ID)
{
  FormStorage->IniFileName=iniFileName;
  FormStorage->IniSection=iniSect;
  ngFrame=NULL;
  Caption=formName;
}
//---------------------------------------------------------------------------
__fastcall TRPForm::~TRPForm(void)
{
  if(ngFrame!=NULL) delete ngFrame;
}
//---------------------------------------------------------------------------
void __fastcall TRPForm::setNextGridFrame(TNextGridFrame *NGF)
{
  ngFrame=NGF;
  if(ngFrame!=NULL) ngFrame->loadDS=loadRP_DS;
}
//---------------------------------------------------------------------------
//**********
//* public *
//**********
void __fastcall TRPForm::loadRP_DS(void)
{
  if(ngFrame!=NULL && !nmpID.IsEmpty())
    ngFrame->getGCFrame()->loadDS(formRPSelectSQL(nmpID));
}
//---------------------------------------------------------------------------

