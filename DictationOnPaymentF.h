//---------------------------------------------------------------------------


#ifndef DictationOnPaymentFH
#define DictationOnPaymentFH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "JvComponent.hpp"
#include "JvExControls.hpp"
#include "JvNavigationPane.hpp"
#include "JvArrowButton.hpp"
#include "JvSpeedButton.hpp"
#include "JvXPCheckCtrls.hpp"
#include "JvXPCore.hpp"
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include "JvExComCtrls.hpp"
#include "JvStatusBar.hpp"
#include "JvExMask.hpp"
#include "JvSpin.hpp"
#include <Mask.hpp>
#include <Menus.hpp>
#include "FIBDataSet.hpp"
#include "pFIBDataSet.hpp"
#include <DB.hpp>
#include "frxClass.hpp"
#include "frxDBSet.hpp"
#include "DBGridEh.hpp"
#include <Grids.hpp>
#include "frxExportXLS.hpp"
//---------------------------------------------------------------------------
class TDictationOnPaymentFrame : public TFrame
{
__published:	// IDE-managed Components
  TJvNavPanelHeader *header;
  TPanel *workPanel;
  TPanel *headerPanel;
  TLabel *periodLabel;
  TJvSpinEdit *yearEdit;
  TComboBox *monthBox;
  TJvSpinButton *spinButton;
  TPopupMenu *dictationMenu;
  TMenuItem *fillItem;
  TMenuItem *editDictItem;
  TMenuItem *delDictItem;
  TMenuItem *spliter;
  TMenuItem *printDictItem;
  TStatusBar *dictBar;
  TMenuItem *clearItem;
  TMenuItem *saveItem;
  TMenuItem *spliter1;
  TMenuItem *cancelItem;
  TMenuItem *printRoolItem;
  TMenuItem *addRecordItom;
  TJvArrowButton *printButton;
  TPopupMenu *printMenu;
  TMenuItem *roolItem;
  TMenuItem *dictItem;
  TfrxDBDataset *frxDS;
  TfrxXLSExport *frxXLSExport;
        TfrxReport *frxReport;
  void __fastcall spinButtonBottomClick(TObject *Sender);
  void __fastcall spinButtonTopClick(TObject *Sender);
  void __fastcall yearEditChange(TObject *Sender);
  void __fastcall monthBoxChange(TObject *Sender);
  void __fastcall dictationMenuPopup(TObject *Sender);
  void __fastcall dDataSetBeforePost(TDataSet *DataSet);

  void __fastcall fillItemClick(TObject *Sender);
  void __fastcall addRecordItomClick(TObject *Sender);
  void __fastcall editDictItemClick(TObject *Sender);
  void __fastcall delDictItemClick(TObject *Sender);
  void __fastcall clearItemClick(TObject *Sender);
  void __fastcall saveItemClick(TObject *Sender);
  void __fastcall cancelItemClick(TObject *Sender);
  void __fastcall printRoolItemClick(TObject *Sender);
  void __fastcall printDictItemClick(TObject *Sender);
  void __fastcall frxReportGetValue(const AnsiString VarName,Variant &Value);
private:	// User declarations
  AnsiString iniFileName;
  AnsiString iniSectionName;
  AnsiString workDir;
  class FBDatabase *fbDB;

  class TGridFrame *dFrame;

  TDateTime d_date;
  AnsiString periodStr;

  void __fastcall loadProperties(void);
  void __fastcall saveProperties(void);
  void __fastcall onChangePeriod(void);
  void __fastcall setHeader(void);
//  void __fastcall setDHeader(void);
  void __fastcall loadDictDS(void);
public:		// User declarations
  __fastcall TDictationOnPaymentFrame(TComponent* Ownerr,AnsiString iniFN,FBDatabase *fbdb);
  __fastcall ~TDictationOnPaymentFrame(void);
  void __fastcall initFrame(void);
};
//---------------------------------------------------------------------------
//extern PACKAGE TDictationOnPaymentFrame *DictationOnPaymentFrame;
//---------------------------------------------------------------------------
#endif
