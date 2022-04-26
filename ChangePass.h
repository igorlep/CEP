//---------------------------------------------------------------------------

#ifndef ChangePassH
#define ChangePassH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ADODB.hpp>
#include <DB.hpp>
#include "FIBDataSet.hpp"
#include "pFIBDataSet.hpp"
#include "FIBQuery.hpp"
#include "pFIBQuery.hpp"
#include <Buttons.hpp>
#include "FIBDatabase.hpp"
#include "pFIBDatabase.hpp"
#include "DBClasses.h"
#include "JvComponent.hpp"
#include "JvExControls.hpp"
#include "JvXPButtons.hpp"
#include "JvXPCore.hpp"
//---------------------------------------------------------------------------
class TChangePassForm : public TForm
{
__published:	// IDE-managed Components
        TPanel *Panel1;
        TPanel *Panel2;
        TEdit *OldPassword;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label3;
        TEdit *NewPassword1;
        TEdit *NewPassword2;
        TLabel *Name;
        TJvXPButton *SaveButton;
        TJvXPButton *CancelButton;
        void __fastcall OldPasswordKeyPress(TObject *Sender, char &Key);
        void __fastcall NewPassword1KeyPress(TObject *Sender, char &Key);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall SaveButtonClick(TObject *Sender);
        void __fastcall CancelButtonClick(TObject *Sender);
private:	// User declarations
        AnsiString ID;
        AnsiString OldPassWD;
        TpFIBDatabase *database;
        FBQuery *fbQ;
        TpFIBQuery *Q;
        SecretWord *sw;
public:		// User declarations
        __fastcall TChangePassForm(TComponent* Owner, AnsiString id, TpFIBDatabase *database);
};
//---------------------------------------------------------------------------
//extern PACKAGE TChangePassForm *ChangePassForm;
//---------------------------------------------------------------------------
#endif
