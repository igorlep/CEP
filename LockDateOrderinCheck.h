//---------------------------------------------------------------------------

#ifndef LockDateOrderinCheckH
#define LockDateOrderinCheckH
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
class LockDateOrderinCheckThread : public TThread
{
private:
protected:
  FBDatabase *fbDB;
  TDateTime lockDate;
  void __fastcall Execute();
  void __fastcall UpdateCaption();
public:
  __fastcall LockDateOrderinCheckThread(bool CreateSuspended,FBDatabase *fbdb);
};
//---------------------------------------------------------------------------
#endif
