//---------------------------------------------------------------------------

#ifndef RP_FormH
#define RP_FormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "Placemnt.hpp"
#include <ExtCtrls.hpp>
#include "NextGridF.h"
//---------------------------------------------------------------------------
class TRPForm : public TForm
{
__published:	// IDE-managed Components
  TPanel *workPanel;
  TFormStorage *FormStorage;
private:	// User declarations
  AnsiString iniFileName;
  AnsiString iniSection;
  class FBDatabase *fbDB;
  class TNextGridFrame *ngFrame;
  AnsiString nmpID;
  void __fastcall loadRP_DS(void);
public:		// User declarations
  __fastcall TRPForm(TComponent* Owner,AnsiString iniFN,AnsiString iniSect,FBDatabase *fb_db,AnsiString formName,AnsiString nmp_ID);
  __fastcall ~TRPForm(void);
  void __fastcall setNextGridFrame(TNextGridFrame *NGF);
  TNextGridFrame * __fastcall getNG_Frame(void){ return ngFrame; }
};
//---------------------------------------------------------------------------
//extern PACKAGE TRPForm *RPForm;
//---------------------------------------------------------------------------
#endif
