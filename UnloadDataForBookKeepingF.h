//---------------------------------------------------------------------------


#ifndef UnloadDataForBookKeepingFH
#define UnloadDataForBookKeepingFH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "JvComponent.hpp"
#include "JvExControls.hpp"
#include "JvNavigationPane.hpp"
#include "JvSpeedButton.hpp"
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <ADODB.hpp>
#include <DB.hpp>
//---------------------------------------------------------------------------
class TUnloadDataForBookKeepingFrame : public TFrame
{
__published:	// IDE-managed Components
  TJvNavPanelHeader *header;
  TPanel *headerPanel;
  TJvSpeedButton *periodButton;
  TComboBox *typeComboBox;
  TJvSpeedButton *createButton;
  TPanel *workPanel;
  TStatusBar *dataStatusBar;
  TADOConnection *ADOConnection;
  TADOQuery *ADO_Q;
  void __fastcall periodButtonClick(TObject *Sender);
  void __fastcall typeComboBoxChange(TObject *Sender);
  void __fastcall createButtonClick(TObject *Sender);
private:	// User declarations
  AnsiString iniFileName;
  AnsiString iniSectionName;
  class FBDatabase *fbDB;

  TDateTime date1,date2;

  class TGridFrame *dataFrame;

  void __fastcall loadProperties(void);
  void __fastcall saveProperties(void);
  void __fastcall setHeader(void);
  void __fastcall loadDS(void);
public:		// User declarations
  __fastcall TUnloadDataForBookKeepingFrame(TComponent* Owner,AnsiString iniFN,FBDatabase *fbdb);
  __fastcall ~TUnloadDataForBookKeepingFrame(void);
  void __fastcall initFrame(void);
  class TDBGridEh * __fastcall getGrid(void);
  TpFIBDataSet * __fastcall getDS(void);
  TDateTime __fastcall getDate1(void){ return date1; }
  TDateTime __fastcall getDate2(void){ return date2; }
  class FBDatabase * __fastcall getFBDataBase(void){ return fbDB; }
  TADOConnection * __fastcall getADOConnection(void){ return ADOConnection; }
  TADOQuery * __fastcall getADOQuery(void){ return ADO_Q; }
};
//---------------------------------------------------------------------------
//extern PACKAGE TUnloadDataForBookKeepingFrame *UnloadDataForBookKeepingFrame;
//---------------------------------------------------------------------------
#endif
