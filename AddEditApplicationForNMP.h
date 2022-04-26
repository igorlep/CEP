//---------------------------------------------------------------------------

#ifndef AddEditApplicationForNMPH
#define AddEditApplicationForNMPH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "JvComponent.hpp"
#include "JvExControls.hpp"
#include "JvNavigationPane.hpp"
#include <ExtCtrls.hpp>
#include "Placemnt.hpp"
#include <ComCtrls.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TAddEditApplicationForNMPForm : public TForm
{
__published:	// IDE-managed Components
  TPanel *workPanel;
  TJvNavPanelHeader *header;
  TFormStorage *FormStorage;
  TStatusBar *statusBar;
  TPopupMenu *editAppMenu;
  TMenuItem *saveItem;
  TMenuItem *cancelItem;
  TMenuItem *separator;
  TMenuItem *closeItem;
  void __fastcall appDSBeforePost(TDataSet *DataSet);
  void __fastcall appDSAfterPost(TDataSet *DataSet);
  void __fastcall saveItemClick(TObject *Sender);
  void __fastcall cancelItemClick(TObject *Sender);
  void __fastcall closeItemClick(TObject *Sender);
  void __fastcall editAppMenuPopup(TObject *Sender);
private:	// User declarations
  AnsiString iniFileName;
  AnsiString workDir;
  AnsiString iniSectionName;
  class FBDatabase *fbDB;
  AnsiString nmpID;
  TDateTime tempDate;
  class TGridFrame *appFrame;
  TpFIBDataSet *aDS;
  bool changeFLG;
  void __fastcall loadAppDS(void);
public:		// User declarations
  __fastcall TAddEditApplicationForNMPForm(TComponent* Owner,AnsiString iniFN,FBDatabase *fbdb,AnsiString nmpid,TDateTime tDate,AnsiString nmpName);
  __fastcall ~TAddEditApplicationForNMPForm(void);
  bool __fastcall getChangeFLG(void){ return changeFLG; }
};
//---------------------------------------------------------------------------
//extern PACKAGE TAddEditApplicationForNMPForm *AddEditApplicationForNMPForm;
//---------------------------------------------------------------------------
#endif
