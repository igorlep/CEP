//---------------------------------------------------------------------------

#include "DBClasses.h"
#pragma hdrstop
#include "AddEditSD.h"
#include "SDFunctions.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "DBGridEh"
//---------------------------------------------------------------------------
int __fastcall AddEditSubdivision(TComponent* Owner,AnsiString iniFN,FBDatabase *fbDB,AnsiString &id)
{
TAddEditSDForm *AESD;
int modalResult=mrCancel;

  try
  {
    AESD=new TAddEditSDForm(Owner,iniFN,fbDB,id);
  }
  catch(const Exception &exc)
  { MessageDlg("Ошибка создания формы TAddEditSDForm\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return modalResult;
  }
  modalResult=AESD->ShowModal();
  delete AESD;
  return modalResult;
}
//---------------------------------------------------------------------------
