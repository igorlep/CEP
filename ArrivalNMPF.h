//---------------------------------------------------------------------------


#ifndef ArrivalNMPFH
#define ArrivalNMPFH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "pFIBDatabase.hpp"
#include "JvComponent.hpp"
#include "JvExControls.hpp"
#include "JvNavigationPane.hpp"
#include "FIBDataSet.hpp"
#include "pFIBDataSet.hpp"
#include <DB.hpp>
#include "JvSpeedButton.hpp"
#include "JvXPCheckCtrls.hpp"
#include "JvXPCore.hpp"
#include <ExtCtrls.hpp>
#include "NextGridF.h"
#include "JvArrowButton.hpp"
#include <Menus.hpp>
#include "frxClass.hpp"
#include "frxDBSet.hpp"
#include "frxExportRTF.hpp"
#include "frxExportXLS.hpp"
//---------------------------------------------------------------------------
class TArrivalNMPFrame : public TFrame
{
__published:	// IDE-managed Components
  TPanel *headerPanel;
  TLabel *SLabel;
  TEdit *SEdit;
  TJvSpeedButton *FSButton;
  TJvSpeedButton *BSButton;
  TJvXPCheckbox *ConditionSBox;
  TJvXPCheckbox *selectForPublisher;
  TPanel *pPanel;
  TSplitter *pSplitter;
  TPanel *nmpPanel;
  TTimer *searchTimer;
  TJvSpeedButton *periodButton;
  TJvArrowButton *printButton;
  TPopupMenu *printMenu;
  TMenuItem *arrivalReportItem;
  TMenuItem *receiptWarrantsItem;
  TJvXPCheckbox *groupOnPublishing;
  TfrxDBDataset *frxDS;
  TJvNavPanelHeader *header;
  TMenuItem *shortArrivalReportItem;
  TfrxXLSExport *frxXLSExport;
  TfrxReport *frxReport;

  void __fastcall periodButtonClick(TObject *Sender);

  void __fastcall searchTimerTimer(TObject *Sender);
  void __fastcall SEditChange(TObject *Sender);
  void __fastcall SEditKeyDown(TObject *Sender,WORD &Key,TShiftState Shift);
  void __fastcall ConditionSBoxClick(TObject *Sender);
  void __fastcall FSButtonClick(TObject *Sender);
  void __fastcall BSButtonClick(TObject *Sender);

  void __fastcall pGridMouseDown(TObject *Sender,TMouseButton Button, TShiftState Shift, int X, int Y);
  void __fastcall pSourceDataChange(TObject *Sender,TField *Field);
  void __fastcall selectForPublisherClick(TObject *Sender);
  void __fastcall nmpGridMouseDown(TObject *Sender,TMouseButton Button, TShiftState Shift, int X, int Y);
  void __fastcall nmpSourceDataChange(TObject *Sender,TField *Field);
  void __fastcall groupOnPublishingClick(TObject *Sender);
  void __fastcall arrivalReportItemClick(TObject *Sender);
  void __fastcall frxReportGetValue(const AnsiString VarName,Variant &Value);
  void __fastcall receiptWarrantsItemClick(TObject *Sender);
  void __fastcall shortArrivalReportItemClick(TObject *Sender);
  void __fastcall viewBySDItemClick(TObject *Sender);
private:	// User declarations
  AnsiString iniFileName;
  AnsiString iniSectionName;
  FBDatabase *fbDB;
  TpFIBDatabase *dataBase;

  TNextGridFrame *pFrame;
  TNextGridFrame *nmpFrame;

  int activeFrame;
  bool firstLoadFLG;

  TDateTime date1, date2;
  TDateTime rwDate; //дата приходного ордера
  int rwNumber;     //номер приходного ордера

  void __fastcall loadProperties(void);
  void __fastcall saveProperties(void);
  void __fastcall loadPublishersDS(void);
  void __fastcall loadPublishersSearchDS(void);
  void __fastcall loadNMPDS(void);
  void __fastcall loadNMPSearchDS(void);
  void __fastcall setHeader(void);
public:		// User declarations
  __fastcall TArrivalNMPFrame(TComponent* Owner,AnsiString iniFN,FBDatabase *fbdb);
  __fastcall ~TArrivalNMPFrame(void);
  void __fastcall initFrame(void);
  void __fastcall loadDataSets(void);
  void __fastcall setFocus(void);
};
//---------------------------------------------------------------------------
//extern PACKAGE TArrivalNMPFrame *ArrivalNMPFrame;
//---------------------------------------------------------------------------
#endif
