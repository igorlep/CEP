//---------------------------------------------------------------------------

#ifndef SelectRicipientH
#define SelectRicipientH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "Placemnt.hpp"
#include "DBGridEh.hpp"
#include "JvComponent.hpp"
#include "JvExControls.hpp"
#include "JvXPButtons.hpp"
#include "JvXPCore.hpp"
#include "SmrtPanel.h"
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include "FIBDataSet.hpp"
#include "pFIBDataSet.hpp"
#include <DB.hpp>
#include "DBClasses.h"
#include <ComCtrls.hpp>
#include "GridF.h"
#include "JvNavigationPane.hpp"
//---------------------------------------------------------------------------
class TSelectRicipientForm : public TForm
{
__published:	// IDE-managed Components
  TFormStorage *FormStorage;
  TPanel *buttonsPanel;
  TJvXPButton *cancelButton;
  TJvXPButton *saveButton;
  TPanel *workPanel;
  TStatusBar *statusBar;
        TJvNavPanelHeader *header;
  void __fastcall saveButtonClick(TObject *Sender);
  void __fastcall cancelButtonClick(TObject *Sender);
private:	// User declarations
  AnsiString iniFileName;
  AnsiString iniSectionName;
  FBDatabase *fbDB;
  short choicePlace;
  FBDataSet *fbDS;
  TGridFrame *rf;
public:		// User declarations
  __fastcall TSelectRicipientForm(TComponent* Owner,AnsiString iniFileName,FBDatabase *fbdb, short choice_place);
  __fastcall ~TSelectRicipientForm(void);
  AnsiString __fastcall getRicipientName(void);
  AnsiString __fastcall getRicipientEMail(void);
};
//---------------------------------------------------------------------------
//extern PACKAGE TSelectRicipientForm *SelectRicipientForm;
//---------------------------------------------------------------------------
#endif
