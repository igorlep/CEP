//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DBClasses.h"
#include "SomeFunctions.h"
#include "BackUpBaseF.h"
#include "ArchivingBDF.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "JvComponent"
#pragma link "JvExControls"
#pragma link "JvNavigationPane"
#pragma link "Placemnt"
#pragma link "JvCaptionPanel"
#pragma link "JvExExtCtrls"
#pragma resource "*.dfm"
//TArchivingBDFrame *ArchivingBDFrame;
//---------------------------------------------------------------------------
//**********
//* public *
//**********
__fastcall TArchivingBDFrame::TArchivingBDFrame(TComponent* Owner,AnsiString iniFN,class FBDatabase *fbdb)
        : TFrame(Owner),iniFileName(iniFN),fbDB(fbdb)
{
  iniSectionName="ArchivingBDFrame";
  archBaseFrame=NULL;
}
//---------------------------------------------------------------------------
__fastcall TArchivingBDFrame::~TArchivingBDFrame(void)
{
  saveProperties();
  if(archBaseFrame!=NULL){ delete archBaseFrame; archBaseFrame=NULL; }
}
//---------------------------------------------------------------------------
void __fastcall TArchivingBDFrame::initFrame(void)
{
  loadProperties();
  try
  {
    archBaseFrame=new TBackUpBaseFrame(this,getAppDir(),iniFileName,fbDB,workPanel,getStatusPanelMainWindow());
  }
  catch(Exception &exc)
  { MessageDlg("Ошибка создания фрейма BackUpBaseFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return;
  }
  archBaseFrame->initFrame();
}
//---------------------------------------------------------------------------

//***********
//* private *
//***********
void __fastcall TArchivingBDFrame::loadProperties(void)
{
TIniFile *ini = new TIniFile(iniFileName);
AnsiString s;

  workPanel->Width=ini->ReadInteger(iniSectionName,"workPanel_Width",400);
  delete ini;
}
//---------------------------------------------------------------------------
void __fastcall TArchivingBDFrame::saveProperties(void)
{
TIniFile *ini = new TIniFile(iniFileName);

  ini->WriteInteger(iniSectionName,"workPanel_Width",workPanel->Width);
  delete ini;
}
//---------------------------------------------------------------------------

//*************
//* published *
//*************
