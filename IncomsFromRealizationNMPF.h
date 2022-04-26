//---------------------------------------------------------------------------


#ifndef IncomsFromRealizationNMPFH
#define IncomsFromRealizationNMPFH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "JvComponent.hpp"
#include "JvExControls.hpp"
#include "JvNavigationPane.hpp"
#include <ExtCtrls.hpp>
#include "JvArrowButton.hpp"
#include "JvSpeedButton.hpp"
#include "JvXPCheckCtrls.hpp"
#include "JvXPCore.hpp"
#include "frxClass.hpp"
#include "frxDBSet.hpp"
#include "frxExportRTF.hpp"
#include "frxExportXLS.hpp"
#include <ComCtrls.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TIncomsFromRealizationNMPFrame : public TFrame
{
__published:	// IDE-managed Components
  TJvNavPanelHeader *header;
  TPanel *workPanel;
  TPanel *headerPanel;
  TLabel *SLabel;
  TJvSpeedButton *FSButton;
  TJvSpeedButton *BSButton;
  TEdit *SEdit;
  TJvXPCheckbox *ConditionSBox;
  TPanel *pPanel;
  TSplitter *pSplitter;
  TTimer *searchTimer;
  TPanel *resultPanel;
  TJvNavPanelHeader *resultHeader;
  TJvSpeedButton *periodButton;
  TfrxDBDataset *frxDS;
  TfrxXLSExport *frxXLSExport;
  TStatusBar *resultBar;
  TPopupMenu *printMenu;
  TMenuItem *simpleItem;
  TMenuItem *groupItem;
  TJvArrowButton *printButton;
  TfrxReport *frxReport;

  void __fastcall searchTimerTimer(TObject *Sender);
  void __fastcall SEditChange(TObject *Sender);
  void __fastcall SEditKeyDown(TObject *Sender,WORD &Key,TShiftState Shift);
  void __fastcall ConditionSBoxClick(TObject *Sender);
  void __fastcall FSButtonClick(TObject *Sender);
  void __fastcall BSButtonClick(TObject *Sender);

  void __fastcall pGridMouseDown(TObject *Sender,TMouseButton Button, TShiftState Shift, int X, int Y);
  void __fastcall pSourceDataChange(TObject *Sender,TField *Field);
  void __fastcall resultGridMouseDown(TObject *Sender,TMouseButton Button, TShiftState Shift, int X, int Y);
  void __fastcall periodButtonClick(TObject *Sender);
  void __fastcall frxReportGetValue(const AnsiString VarName,Variant &Value);
  void __fastcall simpleItemClick(TObject *Sender);
  void __fastcall groupItemClick(TObject *Sender);
private:	// User declarations
  AnsiString iniFileName;
  AnsiString workDir;
  AnsiString iniSectionName;
  class FBDatabase *fbDB;
  class TpFIBDatabase *dataBase;

  TDateTime date1,date2;
  int activeFrame;

  class TNextGridFrame *pFrame;
  class TGridFrame *resultFrame;

  void __fastcall loadProperties(void);
  void __fastcall saveProperties(void);
  void __fastcall setHeader(void);
  void __fastcall setResultHeader(void);
  void __fastcall setFocus(void);
  void __fastcall loadDataSets(void);
  void __fastcall loadPSearchDS(void);
  void __fastcall loadPDS(void);
  void __fastcall loadResultDS(void);
  void __fastcall loadResultSearchDS(void);
public:		// User declarations
  __fastcall TIncomsFromRealizationNMPFrame(TComponent* Owner,AnsiString iniFN,FBDatabase *fbdb);
  __fastcall ~TIncomsFromRealizationNMPFrame(void);
  void __fastcall initFrame(void);
};
//---------------------------------------------------------------------------
//extern PACKAGE TIncomsFromRealizationNMPFrame *IncomsFromRealizationNMPFrame;
//---------------------------------------------------------------------------
#endif
