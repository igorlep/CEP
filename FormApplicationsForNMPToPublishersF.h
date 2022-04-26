//---------------------------------------------------------------------------


#ifndef FormApplicationsForNMPToPublishersFH
#define FormApplicationsForNMPToPublishersFH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "JvComponent.hpp"
#include "JvExControls.hpp"
#include "JvExMask.hpp"
#include "JvNavigationPane.hpp"
#include "JvSpeedButton.hpp"
#include "JvSpin.hpp"
#include "JvXPCheckCtrls.hpp"
#include "JvXPCore.hpp"
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Mask.hpp>
#include <Menus.hpp>
#include "frxClass.hpp"
#include "frxDBSet.hpp"
//---------------------------------------------------------------------------
class TFormApplicationsForNMPToPublishersFrame : public TFrame
{
__published:	// IDE-managed Components
  TJvNavPanelHeader *header;
  TPanel *headerPanel;
  TLabel *SLabel;
  TJvSpeedButton *FSButton;
  TJvSpeedButton *BSButton;
  TLabel *periodLabel;
  TJvSpinButton *spinButton;
  TJvSpeedButton *formMessagesButton;
  TEdit *SEdit;
  TJvXPCheckbox *ConditionSBox;
  TJvSpinEdit *yearEdit;
  TComboBox *monthBox;
  TTimer *searchTimer;
  TPanel *leftPanel;
  TProgressBar *pBar;
  TPanel *pPanel;
  TSplitter *sdSplitter;
  TPanel *rightPanel;
  TPanel *nmpPanel;
  TProgressBar *nmpBar;
  TPopupMenu *popupMenu;
  TMenuItem *appEditItem;
  TMenuItem *separator;
  TMenuItem *columnsConfigItem;
  TJvSpeedButton *printButton;
  TfrxDBDataset *frxDS;
        TfrxReport *frxReport;
        TMenuItem *printTirVedItem;
  void __fastcall onChangePeriod(TObject *Sender);
  void __fastcall spinButtonBottomClick(TObject *Sender);
  void __fastcall spinButtonTopClick(TObject *Sender);

  void __fastcall pGridMouseDown(TObject *Sender,TMouseButton Button, TShiftState Shift, int X, int Y);
  void __fastcall pSourceDataChange(TObject *Sender,TField *Field);
  void __fastcall nmpGridMouseDown(TObject *Sender,TMouseButton Button, TShiftState Shift, int X, int Y);
  void __fastcall formMessagesButtonClick(TObject *Sender);
  //Поиск по модели
  void __fastcall searchTimerTimer(TObject *Sender);
  void __fastcall SEditChange(TObject *Sender);
  void __fastcall SEditKeyDown(TObject *Sender,WORD &Key,TShiftState Shift);
  void __fastcall ConditionSBoxClick(TObject *Sender);
  void __fastcall FSButtonClick(TObject *Sender);
  void __fastcall BSButtonClick(TObject *Sender);
  void __fastcall appEditItemClick(TObject *Sender);
  void __fastcall columnsConfigItemClick(TObject *Sender);
  void __fastcall printButtonClick(TObject *Sender);
  void __fastcall frxReportGetValue(const AnsiString VarName,Variant &Value);
private:	// User declarations
  AnsiString iniFileName;
  AnsiString workDir;
  AnsiString iniSectionName;
  class FBDatabase *fbDB;
  TpFIBDatabase *dataBase;

  class TNextGridFrame *lFrame,*rFrame;
  class TDBGridEh *lGrid,*rGrid;
  class TpFIBDataSet *lDS,*rDS;

//  bool firstLoadFLG;

  AnsiString periodStr;
  TDateTime tempDate;//,factDate1,factDate2;
  int activeFrame;

  void __fastcall loadProperties(void);
  void __fastcall saveProperties(void);
  void __fastcall loadPublishersDS(void);
  void __fastcall loadNMPDS(void);
  void __fastcall loadNMPSearchDS(void);
  void __fastcall setFocus(void);
  void __fastcall setHeader(void);
public:		// User declarations
  __fastcall TFormApplicationsForNMPToPublishersFrame(TComponent* Owner,AnsiString iniFN,FBDatabase *fbdb);
  __fastcall ~TFormApplicationsForNMPToPublishersFrame(void);
  void __fastcall initFrame(void);
  void __fastcall loadDataSets(void);
  AnsiString __fastcall getRicipientID(void);
  AnsiString __fastcall getRicipientName(void);
  AnsiString __fastcall getRicipientMail(void);
  TDateTime __fastcall getTempDate(void){ return tempDate; }
  TDBGridEh *  __fastcall getLGrid(void){ return lFrame->getGridFrame()->workGrid; }
  TpFIBDataSet *__fastcall getLDS(void){ return lFrame->getGridFrame()->getWorkDataSet(); }
  TDBGridEh *  __fastcall getRGrid(void){ return rFrame->getGridFrame()->workGrid; }
  TpFIBDataSet *__fastcall getRDS(void){ return rFrame->getGridFrame()->getWorkDataSet(); }
};
//---------------------------------------------------------------------------
//extern PACKAGE TFormApplicationsForNMPToPublishersFrame *FormApplicationsForNMPToPublishersFrame;
//---------------------------------------------------------------------------
#endif
