//---------------------------------------------------------------------------


#ifndef ReturnNMPtoPublishersFH
#define ReturnNMPtoPublishersFH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "JvArrowButton.hpp"
#include "JvComponent.hpp"
#include "JvExComCtrls.hpp"
#include "JvExControls.hpp"
#include "JvNavigationPane.hpp"
#include "JvSpeedButton.hpp"
#include "JvStatusBar.hpp"
#include "JvXPCheckCtrls.hpp"
#include "JvXPCore.hpp"
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include "frxClass.hpp"
#include "frxDBSet.hpp"
#include "frxExportRTF.hpp"
#include "frxExportXLS.hpp"
#include "JvExMask.hpp"
#include "JvSpin.hpp"
#include <Mask.hpp>
//---------------------------------------------------------------------------
class TReturnNMPtoPublishersFrame : public TFrame
{
__published:	// IDE-managed Components
  TJvNavPanelHeader *header;
  TPanel *headerPanel;
  TLabel *SLabel;
  TJvSpeedButton *FSButton;
  TJvSpeedButton *BSButton;
  TJvArrowButton *printButton;
  TJvArrowButton *createMessagesButton;
  TEdit *SEdit;
  TJvXPCheckbox *conditionSBox;
  TComboBox *typeComboBox;
  TJvXPCheckbox *selectForPublisherBox;
  TPanel *leftPanel;
  TProgressBar *pBar;
  TSplitter *pSplitter;
  TPanel *rightPanel;
  TProgressBar *nmpBar;
  TJvStatusBar *nmpStatusBar;
  TJvStatusBar *pStatusBar;
  TJvNavPanelHeader *pHeader;
  TJvNavPanelHeader *nmpHeader;
  TPanel *pPanel;
  TPanel *nmpPanel;
  TTimer *searchTimer;
  TPopupMenu *printMenu;
  TMenuItem *printTorg12Item;
  TMenuItem *printTorg12SelectItem;
  TPopupMenu *cmMenu;
  TMenuItem *cmTorg12Item;
  TMenuItem *cmTorg12SelectItem;
  TfrxDBDataset *frxDS;
  TfrxXLSExport *frxXLSExport;
  TMenuItem *printTorg2Item;
  TMenuItem *cmTorg2Item;
  TMenuItem *printTorg2SelectItem;
  TMenuItem *cmTorg2SelectItem;
  TMenuItem *printTorg16Item;
  TMenuItem *returnNMPItem;
  TfrxReport *frxReport;
  TLabel *periodLabel;
  TJvSpinEdit *yearEdit;
  TComboBox *monthBox;
  TJvSpinButton *spinButton;

  void __fastcall onChangePeriod(TObject *Sender);
  void __fastcall spinButtonBottomClick(TObject *Sender);
  void __fastcall spinButtonTopClick(TObject *Sender);

  void __fastcall typeComboBoxChange(TObject *Sender);
  void __fastcall pSourceDataChange(TObject *Sender,TField *Field);
  void __fastcall pGridMouseDown(TObject *Sender,TMouseButton Button, TShiftState Shift, int X, int Y);
  void __fastcall nmpSourceDataChange(TObject *Sender,TField *Field);
  void __fastcall nmpGridMouseDown(TObject *Sender,TMouseButton Button, TShiftState Shift, int X, int Y);
  void __fastcall selectForPublisherBoxClick(TObject *Sender);
  //Поиск по модели
  void __fastcall searchTimerTimer(TObject *Sender);
  void __fastcall SEditChange(TObject *Sender);
  void __fastcall SEditKeyDown(TObject *Sender, WORD &Key,TShiftState Shift);
  void __fastcall conditionSBoxClick(TObject *Sender);
  void __fastcall FSButtonClick(TObject *Sender);
  void __fastcall BSButtonClick(TObject *Sender);
  // - печать отчета о возврате ГЖП
  void __fastcall returnNMPItemClick(TObject *Sender);
  //Работа с накладной Торг-12,актом Торг-2 и актом Торг-16
  // - печать накладной
  void __fastcall printTorg12ItemClick(TObject *Sender);
  void __fastcall printTorg12SelectItemClick(TObject *Sender);
  void __fastcall frxReportGetValue(const AnsiString VarName,Variant &Value);
  // - печать акта Торг-2
  void __fastcall printTorg2ItemClick(TObject *Sender);
  void __fastcall printTorg2SelectItemClick(TObject *Sender);
  // - печать акта Торг-16
  void __fastcall printTorg16ItemClick(TObject *Sender);
  // - формирование сообщения с накладной
  void __fastcall cmTorg12ItemClick(TObject *Sender);
  void __fastcall cmTorg12SelectItemClick(TObject *Sender);
  // - формирование сообщения с актом
  void __fastcall cmTorg2ItemClick(TObject *Sender);
  void __fastcall cmTorg2SelectItemClick(TObject *Sender);
private:	// User declarations
  AnsiString iniFileName;
  AnsiString iniSectionName;
  AnsiString workDir;
  class FBDatabase *fbDB;

  AnsiString periodStr;
  TDateTime date1,date2;

  int activeFrame;

  class TGridContentFrame *pFrame;
  class TFrame *nmpFrame;
  class TGridFrame *nmpF;

  bool fullPrintFLG;

  void __fastcall loadProperties(void);
  void __fastcall saveProperties(void);
  void __fastcall setHeader(void);
  void __fastcall setNMPHeader(void);
  void __fastcall setFocus(void);
  void __fastcall loadPublishersDS(void);
  void __fastcall loadNMPDS(void);
  void __fastcall loadNMPSearchDS(void);
public:		// User declarations
  __fastcall TReturnNMPtoPublishersFrame(TComponent* Owner,AnsiString iniFN,FBDatabase *fbdb);
  __fastcall ~TReturnNMPtoPublishersFrame(void);
  void __fastcall initFrame(void);
  AnsiString __fastcall getPublisherID(void);
  class TDBGridEh *  __fastcall getPGrid(void);
  class TDBGridEh *  __fastcall getNMPGrid(void);
  void __fastcall reloadP_DS(void);
  AnsiString __fastcall getRicipientID(void);
  AnsiString __fastcall getRicipientName(void);
  AnsiString __fastcall getRicipientMail(void);
  TpFIBDataSet *__fastcall getRDS(void);
  TDBGridEh *  __fastcall getRGrid(void);
  //
  class FBDatabase * __fastcall getFB_DB(void){ return fbDB; }
  TGridContentFrame * __fastcall getPublishersFrame(void){ return pFrame; }
  bool __fastcall selectForPublisherChecked(void){ return selectForPublisherBox->Checked; }
  void __fastcall setNMPGridFrame(TGridFrame *gf){ nmpF=gf; }
  TDateTime __fastcall getDate1(void){ return date1; }
  TDateTime __fastcall getDate2(void){ return date2; }
  void __fastcall setFullPrintFLG(bool flg){ fullPrintFLG=flg; }
  bool __fastcall getFullPrintFLG(void){ return fullPrintFLG; }
  TpFIBDataSet *__fastcall getLDS(void){ return pFrame->getGridFrame()->getWorkDataSet(); }
  TDBGridEh *  __fastcall getLGrid(void){ return pFrame->getGridFrame()->workGrid; }
};
//---------------------------------------------------------------------------
//extern PACKAGE TReturnNMPtoPublishersFrame *ReturnNMPtoPublishersFrame;
//---------------------------------------------------------------------------
#endif
