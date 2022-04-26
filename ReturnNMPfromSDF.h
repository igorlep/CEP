//---------------------------------------------------------------------------


#ifndef ReturnNMPfromSDFH
#define ReturnNMPfromSDFH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "FIBDataSet.hpp"
#include "pFIBDataSet.hpp"
#include "DBClasses.h"
#include "JvArrowButton.hpp"
#include "JvComponent.hpp"
#include "JvExControls.hpp"
#include "JvNavigationPane.hpp"
#include "JvSpeedButton.hpp"
#include "JvXPCheckCtrls.hpp"
#include "JvXPCore.hpp"
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include "NextGridF.h"
#include <Menus.hpp>
#include "frxClass.hpp"
#include "frxDBSet.hpp"
#include "frxExportRTF.hpp"
#include "frxExportXLS.hpp"
//---------------------------------------------------------------------------
class TReturnNMPfromSDFrame : public TFrame
{
__published:	// IDE-managed Components
  TJvNavPanelHeader *header;
  TPanel *headerPanel;
  TLabel *SLabel;
  TJvSpeedButton *FSButton;
  TJvSpeedButton *BSButton;
  TJvSpeedButton *periodButton;
  TJvArrowButton *printButton;
  TJvArrowButton *createMessagesButton;
  TEdit *SEdit;
  TJvXPCheckbox *ConditionSBox;
  TJvXPCheckbox *groupOnPostOffices;
  TJvXPCheckbox *groupOnPublishers;
  TPanel *leftPanel;
  TProgressBar *sdBar;
  TSplitter *sdSplitter;
  TPanel *rightPanel;
  TPanel *nmpPanel;
  TProgressBar *nmpBar;
  TTimer *searchTimer;
  TPopupMenu *printMenu;
  TMenuItem *torg13Item;
  TMenuItem *torg13SelectItem;
  TPopupMenu *cmMenu;
  TMenuItem *cmTemplateForReturnNMP;
  TfrxDBDataset *frxDS;
  TfrxRTFExport *frxRTFExport;
  TfrxXLSExport *frxXLSExport;
  TMenuItem *cmFullTemplate;
  TMenuItem *cmSelectedTemplate;
  TMenuItem *cmTorg13Item;
  TMenuItem *cmFullTorg13Item;
  TMenuItem *cmSelectedTorg13Item;
  TJvXPCheckbox *showAllRecords;
  TJvSpeedButton *loadDataButton;
  TPanel *sdPanel;
  TMenuItem *returnNMPReportItem;
  TfrxReport *frxReport;

  void __fastcall periodButtonClick(TObject *Sender);

  void __fastcall searchTimerTimer(TObject *Sender);
  void __fastcall SEditChange(TObject *Sender);
  void __fastcall SEditKeyDown(TObject *Sender,WORD &Key,TShiftState Shift);
  void __fastcall ConditionSBoxClick(TObject *Sender);
  void __fastcall FSButtonClick(TObject *Sender);
  void __fastcall BSButtonClick(TObject *Sender);

  void __fastcall sdGridMouseDown(TObject *Sender,TMouseButton Button, TShiftState Shift, int X, int Y);
  void __fastcall sdSourceDataChange(TObject *Sender,TField *Field);
  void __fastcall groupOnPostOfficesClick(TObject *Sender);
  void __fastcall nmpGridMouseDown(TObject *Sender,TMouseButton Button, TShiftState Shift, int X, int Y);
  void __fastcall nmpDataSetAfterPost(TDataSet *DataSet);
  void __fastcall groupOnPublishersClick(TObject *Sender);
  void __fastcall showAllRecordsClick(TObject *Sender);
  void __fastcall nmpGridGetCellParams(TObject *Sender,TColumnEh *Column,TFont *AFont,TColor &Background,TGridDrawState State);

  void __fastcall torg13ItemClick(TObject *Sender);
  void __fastcall torg13SelectItemClick(TObject *Sender);
  void __fastcall frxReportGetValue(const AnsiString VarName,Variant &Value);
  void __fastcall cmFullTemplateClick(TObject *Sender);
  void __fastcall cmSelectedTemplateClick(TObject *Sender);
  void __fastcall cmFullTorg13ItemClick(TObject *Sender);
  void __fastcall cmSelectedTorg13ItemClick(TObject *Sender);
  void __fastcall loadDataButtonClick(TObject *Sender);
  void __fastcall cmMenuPopup(TObject *Sender);
  void __fastcall returnNMPReportItemClick(TObject *Sender);
private:	// User declarations
  AnsiString iniFileName;
  AnsiString workDir;
  AnsiString iniSectionName;
  FBDatabase *fbDB;
  TpFIBDatabase *dataBase;

  TNextGridFrame *lFrame,*rFrame;
  TDBGridEh *lGrid,*rGrid;
  TpFIBDataSet *lDS,*rDS;

  int activeFrame;

  TDateTime date1, date2;

  bool fullPrintFLG;

  void __fastcall loadProperties(void);
  void __fastcall saveProperties(void);
  void __fastcall loadSubdivisionsDS(void);
  void __fastcall loadNMPDS(void);
  void __fastcall loadNMPSearchDS(void);
  void __fastcall setHeader(void);
  void __fastcall setRHeader(void);
  void __fastcall setButtons(void);
public:		// User declarations
  __fastcall TReturnNMPfromSDFrame(TComponent* Owner,AnsiString iniFN,FBDatabase *fbdb);
  __fastcall ~TReturnNMPfromSDFrame(void);
  void __fastcall initFrame(void);
  void __fastcall loadDataSets(void);
  void __fastcall setFocus(void);
  AnsiString __fastcall getRicipientID(void);
  AnsiString __fastcall getRicipientName(void);
  AnsiString __fastcall getRicipientMail(void);
  FBDatabase * __fastcall getFBDataBase(void){ return fbDB; }
  TpFIBDataSet *__fastcall getRDS(void){ return rDS; }
  TpFIBDataSet *__fastcall getLDS(void){ return lDS; }
  TDBGridEh *  __fastcall getRGrid(void){ return rGrid; }
  TDBGridEh *  __fastcall getLGrid(void){ return lGrid; }
  TDateTime __fastcall getDate1(void){ return date1; }
  TDateTime __fastcall getDate2(void){ return date2; }
  void __fastcall setFullPrintFLG(bool flg){ fullPrintFLG=flg; }
};
//---------------------------------------------------------------------------
//extern PACKAGE TReturnNMPfromSDFrame *ReturnNMPfromSDFrame;
//---------------------------------------------------------------------------
#endif
