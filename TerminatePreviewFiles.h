//---------------------------------------------------------------------------

#ifndef TerminatePreviewFilesH
#define TerminatePreviewFilesH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "DBClasses.h"
//---------------------------------------------------------------------------
class TerminatePreviewFilesThread : public TThread
{            
private:
protected:
  FBDatabase *fbDB;
  AnsiString userID;
  void __fastcall Execute();
  void __fastcall UpdateCaption(void);
public:
  __fastcall TerminatePreviewFilesThread(bool CreateSuspended,FBDatabase *fbdb,AnsiString uID);
};
//---------------------------------------------------------------------------
#endif
