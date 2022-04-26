//---------------------------------------------------------------------------


#ifndef ReferenceOnMovementOfGoodsFH
#define ReferenceOnMovementOfGoodsFH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "JvComponent.hpp"
#include "JvExControls.hpp"
#include "JvNavigationPane.hpp"
#include "JvSpeedButton.hpp"
#include "JvXPCheckCtrls.hpp"
#include "JvXPCore.hpp"
#include <ExtCtrls.hpp>
#include "frxClass.hpp"
#include "frxDBSet.hpp"
#include "frxExportXLS.hpp"
#include <ComCtrls.hpp>
#include "JvArrowButton.hpp"
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TReferenceOnMovementOfGoodsFrame : public TFrame
{
__published:	// IDE-managed Components
  TJvNavPanelHeader *header;
  TPanel *workPanel;
  TSplitter *pSplitter;
  TPanel *headerPanel;
  TLabel *SLabel;
  TJvSpeedButton *FSButton;
  TJvSpeedButton *BSButton;
  TJvSpeedButton *periodButton;
  TEdit *SEdit;
  TJvXPCheckbox *ConditionSBox;
  TPanel *pPanel;
  TPanel *resultPanel;
  TJvNavPanelHeader *resultHeader;
  TTimer *searchTimer;
  TfrxDBDataset *frxDS;
  TfrxXLSExport *frxXLSExport;
  TComboBox *periodBox;
  TLabel *periodLabel;
  TStatusBar *resultBar;
  TPopupMenu *printMenu;
  TMenuItem *simpleItem;
  TMenuItem *groupItem;
  TJvArrowButton *printButton;
  TLabel *resultLabel;
  TComboBox *resultBox;
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
  void __fastcall periodBoxChange(TObject *Sender);
  void __fastcall frxReportGetValue(const AnsiString VarName,Variant &Value);
  void __fastcall simpleItemClick(TObject *Sender);
  void __fastcall groupItemClick(TObject *Sender);
  void __fastcall resultBoxChange(TObject *Sender);
private:	// User declarations
  AnsiString iniFileName;
  AnsiString workDir;
  AnsiString iniSectionName;
  class FBDatabase *fbDB;
  class TpFIBDatabase *dataBase;

  TDateTime date1,date2;
  int activeFrame;
//  bool firstLoadFLG;

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
  __fastcall TReferenceOnMovementOfGoodsFrame(TComponent* Owner,AnsiString iniFN,FBDatabase *fbdb);
  __fastcall ~TReferenceOnMovementOfGoodsFrame(void);
  void __fastcall initFrame(void);
};
//---------------------------------------------------------------------------
//extern PACKAGE TReferenceOnMovementOfGoodsFrame *ReferenceOnMovementOfGoodsFrame;
//---------------------------------------------------------------------------
#endif
