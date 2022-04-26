//---------------------------------------------------------------------------
//Поток, удаляющий файлы, открытые для просмотра, и соответствующие записи из
//таблицы PREVIEW_FILES_TABLE
#include <vcl.h>
#pragma hdrstop
#include "MainCAP_2010.h"
#include "TerminatePreviewFiles.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall TerminatePreviewFilesThread::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall TerminatePreviewFilesThread::TerminatePreviewFilesThread(bool CreateSuspended,FBDatabase *fbdb,AnsiString uID)
        : TThread(CreateSuspended),fbDB(fbdb),userID(uID)
{
  FreeOnTerminate=false;
  MainCAP_2010Form->setTerminatePreviewFilesThreadFGG(true);
}
//---------------------------------------------------------------------------
void __fastcall TerminatePreviewFilesThread::Execute()
{
  do
  { for(int i=0; i<30; i++)
    { Sleep(200);
      if(Terminated)
        MainCAP_2010Form->setTerminatePreviewFilesThreadFGG(false);
    }
    Synchronize(UpdateCaption);
  } while(!Terminated);

}
//---------------------------------------------------------------------------
void __fastcall TerminatePreviewFilesThread::UpdateCaption(void)
{
AnsiString sql;
FBDataSet *fbDS=newFBDataSet(NULL,"TerminatePreviewFilesThread::UpdateCaption",fbDB->getDatabase(),true);
FBQuery *fbWQ=newFBQuery(NULL,"TerminatePreviewFilesThread::UpdateCaption",fbDB->getDatabase(),false);
TpFIBDataSet *ds;
TpFIBTransaction *wt;

  if(fbDS!=NULL && fbWQ!=NULL)
  { if(userID.IsEmpty()) goto ret;
    sql="select * from PREVIEW_FILES_TABLE where USER_ID="+userID;
    if(fbDS->execSelectSQL("TerminatePreviewFilesThread::UpdateCaption",sql)==false) goto ret;
    ds=fbDS->getDataSet();
    if(ds->Active==false) goto ret;
    wt=fbWQ->getTransaction();
    ds->First();
    while(!ds->Eof)
    { AnsiString fn=ds->FieldByName("FILE_NAME")->AsString;
      bool delFLG=false;
      if(FileExists(fn)) delFLG=DeleteFile(fn);
      else delFLG=true;
      if(delFLG)
      { sql="delete from PREVIEW_FILES_TABLE where PF_ID="+ds->FieldByName("PF_ID")->AsString;
        wt->StartTransaction();
        if(fbWQ->execQuery("TerminatePreviewFilesThread::UpdateCaption",sql)==false)
        { wt->Rollback();
          goto ret;
        }
        wt->Commit();
      }
      ds->Next();
    }
  }
ret:
  if(fbDS!=NULL) delete fbDS;
  if(fbWQ!=NULL) delete fbWQ;
}
//---------------------------------------------------------------------------
