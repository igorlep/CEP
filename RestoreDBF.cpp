//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DBClasses.h"
#include "SomeFunctions.h"
#include "RestoreBaseF.h"
#include "RestoreDBF.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "JvComponent"
#pragma link "JvExControls"
#pragma link "JvNavigationPane"
#pragma link "Placemnt"
#pragma resource "*.dfm"
//TRestoreDBFrame *RestoreDBFrame;
//---------------------------------------------------------------------------
//**********
//* public *
//**********
__fastcall TRestoreDBFrame::TRestoreDBFrame(TComponent* Owner,AnsiString iniFN,class FBDatabase *fbdb)
        : TFrame(Owner),iniFileName(iniFN),fbDB(fbdb)
{
  iniSectionName="TRestoreDBFrame";
  restoreBaseF=NULL;
}
//---------------------------------------------------------------------------
__fastcall TRestoreDBFrame::~TRestoreDBFrame(void)
{
  saveProperties();
  if(restoreBaseF!=NULL){ delete restoreBaseF; restoreBaseF=NULL; }
}
//---------------------------------------------------------------------------
void __fastcall TRestoreDBFrame::initFrame(void)
{
  loadProperties();
  try
  {
    restoreBaseF=new TRestoreBaseFrame(this,getAppDir(),iniFileName,fbDB,workPanel,getStatusPanelMainWindow());
  }
  catch(Exception &exc)
  { MessageDlg("Ошибка создания фрейма RestoreBaseFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return;
  }
  restoreBaseF->initFrame();
}
//---------------------------------------------------------------------------

//***********
//* private *
//***********
void __fastcall TRestoreDBFrame::loadProperties(void)
{
TIniFile *ini = new TIniFile(iniFileName);

  workPanel->Width=ini->ReadInteger(iniSectionName,"workPanel_Width",400);
  delete ini;
}
//---------------------------------------------------------------------------
void __fastcall TRestoreDBFrame::saveProperties(void)
{
TIniFile *ini = new TIniFile(iniFileName);

  ini->WriteInteger(iniSectionName,"workPanel_Width",workPanel->Width);
  delete ini;
}
//---------------------------------------------------------------------------

