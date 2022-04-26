//---------------------------------------------------------------------------


#ifndef GoodsRepotrFH
#define GoodsRepotrFH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "JvComponent.hpp"
#include "JvExControls.hpp"
#include "JvExMask.hpp"
#include "JvNavigationPane.hpp"
#include "JvSpin.hpp"
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Mask.hpp>
#include "frxClass.hpp"
#include "frxDBSet.hpp"
#include "frxExportXLS.hpp"
//---------------------------------------------------------------------------
class TGoodsRepotrFrame : public TFrame
{
__published:	// IDE-managed Components
  TJvNavPanelHeader *header;
  TPanel *workPanel;
  TPanel *headerPanel;
  TLabel *periodLabel;
  TJvSpinButton *spinButton;
  TJvSpinEdit *yearEdit;
  TComboBox *monthBox;
  TSpeedButton *printButton;
  TComboBox *selectTypeBox;
  TLabel *selectTypeLabel;
  TPanel *lPanel;
  TStatusBar *lBar;
  TSplitter *vSplitter;
  TPanel *rPanel;
  TStatusBar *rBar;
  TJvNavPanelHeader *lHeader;
  TJvNavPanelHeader *rHeader;
  TfrxDBDataset *frxDS;
  TfrxReport *frxReport;
  TfrxXLSExport *frxXLSExport;
  void __fastcall yearEditChange(TObject *Sender);
  void __fastcall monthBoxChange(TObject *Sender);
  void __fastcall spinButtonBottomClick(TObject *Sender);
  void __fastcall spinButtonTopClick(TObject *Sender);
  void __fastcall selectTypeBoxChange(TObject *Sender);
  void __fastcall printButtonClick(TObject *Sender);
  void __fastcall frxReportGetValue(const AnsiString VarName,Variant &Value);
private:	// User declarations
  AnsiString iniFileName;
  AnsiString iniSectionName;
  AnsiString workDir;
  class FBDatabase *fbDB;

  class TGridFrame *lFrame,*rFrame;

  TDateTime r_date;
  AnsiString periodStr;
  FBDataSet *fbDS;
  double as,ss,rs;

  void __fastcall loadProperties(void);
  void __fastcall saveProperties(void);
  void __fastcall onChangePeriod(void);
  void __fastcall loadLeftDS(void);
  void __fastcall loadRihgtDS(void);
  void __fastcall loadDataSets(void);
  void __fastcall setHeader(void);
public:		// User declarations
  __fastcall TGoodsRepotrFrame(TComponent* Owner,AnsiString iniFN,FBDatabase *fbdb);
  __fastcall ~TGoodsRepotrFrame(void);
  void __fastcall initFrame(void);
};
//---------------------------------------------------------------------------
//extern PACKAGE TGoodsRepotrFrame *GoodsRepotrFrame;
//---------------------------------------------------------------------------
#endif
