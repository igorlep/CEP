//---------------------------------------------------------------------------
//Поток, проверяющий дату запрета редактирования разнаряжения ГЖП
#include <vcl.h>
#pragma hdrstop
#include "DBClasses.h"
#include "DifferentFunctions.h"
#include "OrderingNMPF.h"
#include "MainCAP_2010.h"
#include "SomeFunctions.h"
#include "LockDateOrderinCheck.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall LockDateOrderinCheckThread::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall LockDateOrderinCheckThread::LockDateOrderinCheckThread(bool CreateSuspended,FBDatabase *fb_DB)
        : TThread(CreateSuspended),fbDB(fb_DB)
{
  FreeOnTerminate=false;
  (static_cast<TOrderingNMPFrame*>(MainCAP_2010Form->getWorkFrame()))->setCreateLDOThread(true);
  lockDate=0;
}
//---------------------------------------------------------------------------
void __fastcall LockDateOrderinCheckThread::Execute()
{
  do
  { if(fbDB->isConnected()==true)
    { TDateTime date=StrToDateTime(getConstValue(NULL,fbDB,"ORDERING_LOCK_DATE",Date()));
      if(date!=lockDate)
      { lockDate=date;
        Synchronize(UpdateCaption);
      }
    }
    for(int i=0; i<150; i++)
    { Sleep(200);
      if(Terminated) break;
    }
  }while(!Terminated);
  (static_cast<TOrderingNMPFrame*>(MainCAP_2010Form->getWorkFrame()))->setCreateLDOThread(false);
}
//---------------------------------------------------------------------------
void __fastcall LockDateOrderinCheckThread::UpdateCaption()
{
  set3MainStatusBar("Дата запрета редактирования - "+lockDate.DateString());
}
//---------------------------------------------------------------------------
