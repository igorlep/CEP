//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DifferentFunctions.h"
#include "SomeFunctions.h"
#include "ArrivalNMPF.h"
#include "ViewNMPBySDFunctions.h"
#include "ViewNMPBySD.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "JvComponent"
#pragma link "JvExControls"
#pragma link "JvNavigationPane"
#pragma link "Placemnt"
#pragma resource "*.dfm"
//TViewNMPBySDForm *ViewNMPBySDForm;
//---------------------------------------------------------------------------
__fastcall TViewNMPBySDForm::TViewNMPBySDForm(TComponent* Owner,AnsiString iniFN,FBDatabase *fbdb,int type,AnsiString id,TDateTime dt1,TDateTime dt2,AnsiString nmpName)
        : TForm(Owner),iniFileName(iniFN),fbDB(fbdb),typeForm(type),ID(id),date1(dt1),date2(dt2)
{
  workDir=getAppDir();
  FormStorage->IniFileName=iniFileName;
  header->Caption=nmpName;
  if(typeForm==1)
  { Caption="Разнарядение ГЖП по подразделениям";
    bool groupOnPublishing=(static_cast<TArrivalNMPFrame*>(Owner))->groupOnPublishing->Checked;
    iniSectionName="ViewOrderingNMPBySDForm";
    gf=createOrderingViewNMPBySDFrame(this,iniFileName,iniSectionName,fbDB,workPanel,groupOnPublishing);
    if(gf!=NULL)
    { gf->setStatusBar(statusBar);
      AnsiString sql=formOrderingNMPBySDSelectSQL(groupOnPublishing,ID,date1,date2);
      gf->initSelectSQL(sql);
      gf->loadDS();
    }
  }
  else
  { Caption="Возврат ГЖП из подразделениям";
    iniSectionName="ViewReturnNMPBySDForm";
    gf=createViewReturnNMPBySDFrame(this,iniFileName,iniSectionName,fbDB,workPanel);
    if(gf!=NULL)
    { gf->setStatusBar(statusBar);
      AnsiString sql=formReturnNMPBySDSelectSQL(ID);
      gf->initSelectSQL(sql);
      gf->loadDS();
    }
  }
  FormStorage->IniSection=iniSectionName;
}
//---------------------------------------------------------------------------
__fastcall TViewNMPBySDForm::~TViewNMPBySDForm(void)
{
  if(gf!=NULL){ delete gf; gf=NULL; }
}
//---------------------------------------------------------------------------
