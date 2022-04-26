//---------------------------------------------------------------------------


#ifndef ReferenceOnMGForSDFH
#define ReferenceOnMGForSDFH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "JvComponent.hpp"
#include "JvExControls.hpp"
#include "JvNavigationPane.hpp"
#include "frxClass.hpp"
#include "frxDBSet.hpp"
#include "frxExportXLS.hpp"
#include "JvArrowButton.hpp"
#include "JvSpeedButton.hpp"
#include "JvXPCheckCtrls.hpp"
#include "JvXPCore.hpp"
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TReferenceOnMGForSDFrame : public TFrame
{
__published:	// IDE-managed Components
  TJvNavPanelHeader *header;
  TTimer *searchTimer;
  TPopupMenu *printMenu;
  TMenuItem *simpleItem;
  TMenuItem *groupItem;
  TfrxDBDataset *frxDS;
  TfrxXLSExport *frxXLSExport;
  TPanel *workPanel;
  TSplitter *sdSplitter;
  TPanel *headerPanel;
  TLabel *SLabel;
  TJvSpeedButton *FSButton;
  TJvSpeedButton *BSButton;
  TJvSpeedButton *periodButton;
  TLabel *periodLabel;
  TJvArrowButton *printButton;
  TEdit *SEdit;
  TJvXPCheckbox *ConditionSBox;
  TPanel *lPanel;
  TPanel *resultPanel;
  TJvNavPanelHeader *resultHeader;
  TStatusBar *resultBar;
  TJvXPCheckbox *groupOnPostOffices;
  TComboBox *periodBox;
        TfrxReport *frxReport;
  //Поиск по модели
  void __fastcall searchTimerTimer(TObject *Sender);
  void __fastcall SEditChange(TObject *Sender);
  void __fastcall SEditKeyDown(TObject *Sender,WORD &Key,TShiftState Shift);
  void __fastcall ConditionSBoxClick(TObject *Sender);
  void __fastcall FSButtonClick(TObject *Sender);
  void __fastcall BSButtonClick(TObject *Sender);
  //
  void __fastcall sdGridMouseDown(TObject *Sender,TMouseButton Button, TShiftState Shift, int X, int Y);
  void __fastcall sdSourceDataChange(TObject *Sender,TField *Field);
  void __fastcall resultGridMouseDown(TObject *Sender,TMouseButton Button, TShiftState Shift, int X, int Y);
  void __fastcall periodButtonClick(TObject *Sender);
  void __fastcall groupOnPostOfficesClick(TObject *Sender);
  void __fastcall periodBoxChange(TObject *Sender);
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

  class TNextGridFrame *lFrame;
  TDBGridEh *lGrid;
  TpFIBDataSet *lDS;
  class TGridFrame *resultFrame;

  void __fastcall loadProperties(void);
  void __fastcall saveProperties(void);
  void __fastcall setHeader(void);
  void __fastcall setResultHeader(void);
  void __fastcall setFocus(void);
  void __fastcall loadDataSets(void);
  void __fastcall loadSDDS(void);
  void __fastcall loadResultDS(void);
  void __fastcall loadResultSearchDS(void);
public:		// User declarations
  __fastcall TReferenceOnMGForSDFrame(TComponent* Owner,AnsiString iniFN,FBDatabase *fbdb);
  __fastcall ~TReferenceOnMGForSDFrame(void);
  void __fastcall initFrame(void);
};
//---------------------------------------------------------------------------
//extern PACKAGE TReferenceOnMGForSDFrame *ReferenceOnMGForSDFrame;
//---------------------------------------------------------------------------
#endif
