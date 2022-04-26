//---------------------------------------------------------------------------

#ifndef AddEditPPH
#define AddEditPPH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include "JvComponent.hpp"
#include "JvExControls.hpp"
#include "JvNavigationPane.hpp"
#include "FIBDataSet.hpp"
#include "pFIBDataSet.hpp"
#include <DB.hpp>
//---------------------------------------------------------------------------
class TAddEditPPForm : public TForm
{
__published:	// IDE-managed Components
  TPanel *workPanel;
  TJvNavPanelHeader *header;
  void __fastcall ppBeforePost(TDataSet *DataSet);
  void __fastcall ppBeforeInsert(TDataSet *DataSet);
private:	// User declarations
  AnsiString iniFileName;
  AnsiString iniSectionName;
  class FBDatabase *fbDB;
  AnsiString ppIdentifier;
  AnsiString constID;
  int const_kind;

  class TNextGridFrame *historyFrame;

  void __fastcall loadProperties(void);
  void __fastcall saveProperties(void);
  void __fastcall loadDS(void);
public:		// User declarations
  __fastcall TAddEditPPForm(TComponent* Owner,AnsiString iniFN,class FBDatabase *fbdb,AnsiString pp_identifier);
  __fastcall ~TAddEditPPForm(void);
  void __fastcall initForm(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TAddEditPPForm *AddEditPPForm;
//---------------------------------------------------------------------------
#endif
