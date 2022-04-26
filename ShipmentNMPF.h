//---------------------------------------------------------------------------


#ifndef ShipmentNMPFH
#define ShipmentNMPFH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "JvComponent.hpp"
#include "JvExControls.hpp"
#include "JvNavigationPane.hpp"
#include "FIBDataSet.hpp"
#include "pFIBDataSet.hpp"
#include "JvArrowButton.hpp"
#include "JvSpeedButton.hpp"
#include "JvXPCheckCtrls.hpp"
#include "JvXPCore.hpp"
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <Menus.hpp>
#include "NextGridF.h"
#include "frxClass.hpp"
#include "frxDBSet.hpp"
#include "frxExportRTF.hpp"
#include "frxExportXLS.hpp"
#include <ComCtrls.hpp>
#include "DirectWorkExcel.h"
//---------------------------------------------------------------------------
class TShipmentNMPFrame : public TFrame
{
__published:	// IDE-managed Components
  TJvNavPanelHeader *header;
  TPanel *headerPanel;
  TLabel *SLabel;
  TJvSpeedButton *FSButton;
  TJvSpeedButton *BSButton;
  TJvSpeedButton *periodButton;
  TJvArrowButton *printButton;
  TEdit *SEdit;
  TJvXPCheckbox *ConditionSBox;
  TJvXPCheckbox *groupOnPostOffices;
  TJvXPCheckbox *groupOnPublishers;
  TSplitter *sdSplitter;
  TPopupMenu *printMenu;
  TMenuItem *shipmentReportItem;
  TMenuItem *torg13Item;
  TMenuItem *torg13SelectItem;
  TTimer *searchTimer;
  TfrxDBDataset *frxDS;
  TfrxXLSExport *frxXLSExport;
  TPanel *leftPanel;
  TProgressBar *sdBar;
  TPanel *sdPanel;
  TPanel *rightPanel;
  TPanel *nmpPanel;
  TProgressBar *nmpBar;
  TJvArrowButton *createMessagesButton;
  TPopupMenu *cmMenu;
  TMenuItem *cmTorg13Item;
  TMenuItem *cmSelectTorg13Item;
  TMenuItem *shortShipmentReportItem;
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
  void __fastcall groupOnPublishersClick(TObject *Sender);

  void __fastcall shipmentReportItemClick(TObject *Sender);
  void __fastcall torg13ItemClick(TObject *Sender);
  void __fastcall torg13SelectItemClick(TObject *Sender);
  void __fastcall frxReportGetValue(const AnsiString VarName,Variant &Value);
  void __fastcall cmTorg13ItemClick(TObject *Sender);
  void __fastcall cmSelectTorg13ItemClick(TObject *Sender);
  void __fastcall shortShipmentReportItemClick(TObject *Sender);
private:	// User declarations
  AnsiString iniFileName;
  AnsiString workDir;
  AnsiString iniSectionName;
  FBDatabase *fbDB;
  TpFIBDatabase *dataBase;

  TNextGridFrame *sdFrame,*poFrame,*lFrame; //в sdPanel выводим или список подразделений или список почтамтов
  TNextGridFrame *nmpFrame,*pFrame,*rFrame; //в nmpGrid выводим данные по отгрузе в разрезе ГЖП или поставщиков
  TDBGridEh *lGrid,*rGrid;
  TpFIBDataSet *lDS,*rDS;

  int activeFrame;
//  bool firstLoadFLG;

  TDateTime date1, date2;

  bool fullPrintFLG;

  void __fastcall loadProperties(void);
  void __fastcall saveProperties(void);
  void __fastcall loadSubdivisionsDS(void);
  void __fastcall loadNMPDS(void);
  void __fastcall loadNMPSearchDS(void);
  void __fastcall setHeader(void);
public:		// User declarations
  __fastcall TShipmentNMPFrame(TComponent* Owner,AnsiString iniFN,FBDatabase *fbdb);
  __fastcall ~TShipmentNMPFrame(void);
  void __fastcall initFrame(void);
  void __fastcall loadDataSets(void);
  void __fastcall setFocus(void);
  AnsiString __fastcall getRicipientID(void);
  AnsiString __fastcall getRicipientName(void);
  AnsiString __fastcall getRicipientMail(void);
  TpFIBDataSet * __fastcall getRDS(void){ return rDS; }
  TpFIBDataSet * __fastcall getLDS(void){ return lDS; }
  TDBGridEh * __fastcall getRGrid(void){ return rGrid; }
  TDBGridEh *  __fastcall getLGrid(void){ return lGrid; }
  TDateTime __fastcall getDate1(void){ return date1; }
  TDateTime __fastcall getDate2(void){ return date2; }
  void __fastcall setFullPrintFLG(bool flg){ fullPrintFLG=flg; }
};
//---------------------------------------------------------------------------
//extern PACKAGE TShipmentNMPFrame *ShipmentNMPFrame;
//---------------------------------------------------------------------------
#endif
