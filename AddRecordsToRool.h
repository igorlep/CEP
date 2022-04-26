//---------------------------------------------------------------------------

#ifndef AddRecordsToRoolH
#define AddRecordsToRoolH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "JvComponent.hpp"
#include "JvExControls.hpp"
#include "JvNavigationPane.hpp"
#include "Placemnt.hpp"
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TAddRecordsToRoolForm : public TForm
{
__published:	// IDE-managed Components
  TPanel *workPanel;
  TJvNavPanelHeader *header;
  TStatusBar *statusBar;
  TFormStorage *FormStorage;
  TPopupMenu *addrecordsMenu;
  TMenuItem *addRecordsItem;
  TMenuItem *closeItem;
  void __fastcall closeItemClick(TObject *Sender);
  void __fastcall addRecordsItemClick(TObject *Sender);
private:	// User declarations
  AnsiString iniFileName;
  AnsiString iniSectionName;
  class FBDatabase *fbDB;
  TDateTime d_date;
  class TGridFrame *pFrame;
  TDBGridEh *pGrid;
  TpFIBDataSet *pDS;
  void __fastcall loadPDS(void);
public:		// User declarations
  __fastcall TAddRecordsToRoolForm(TComponent* Owner,AnsiString iniFN,FBDatabase *fbdb,TDateTime dDate);
  __fastcall ~TAddRecordsToRoolForm(void);
};
//---------------------------------------------------------------------------
//extern PACKAGE TAddRecordsToRoolForm *AddRecordsToRoolForm;
//---------------------------------------------------------------------------
#endif
