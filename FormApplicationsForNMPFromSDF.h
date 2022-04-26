//---------------------------------------------------------------------------


#ifndef FormApplicationsForNMPFromSDFH
#define FormApplicationsForNMPFromSDFH
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
#include "JvExMask.hpp"
#include "JvSpin.hpp"
#include <Mask.hpp>
//---------------------------------------------------------------------------
class TFormApplicationsForNMPFromSDFrame : public TFrame
{
__published:	// IDE-managed Components
  TJvNavPanelHeader *header;
  TPanel *headerPanel;
  TLabel *SLabel;
  TJvSpeedButton *FSButton;
  TJvSpeedButton *BSButton;
  TJvSpeedButton *loadDataButton;
  TEdit *SEdit;
  TJvXPCheckbox *ConditionSBox;
  TPanel *leftPanel;
  TProgressBar *sdBar;
  TPanel *sdPanel;
  TSplitter *sdSplitter;
  TPanel *rightPanel;
  TPanel *nmpPanel;
  TProgressBar *nmpBar;
  TTimer *searchTimer;
  TLabel *periodLabel;
  TJvSpinEdit *yearEdit;
  TComboBox *monthBox;
  TJvSpinButton *spinButton;
  TJvSpeedButton *formMessagesButton;
  void __fastcall onChangePeriod(TObject *Sender);
  void __fastcall spinButtonBottomClick(TObject *Sender);
  void __fastcall spinButtonTopClick(TObject *Sender);

  void __fastcall sdGridMouseDown(TObject *Sender,TMouseButton Button, TShiftState Shift, int X, int Y);
  void __fastcall sdSourceDataChange(TObject *Sender,TField *Field);
  void __fastcall nmpGridMouseDown(TObject *Sender,TMouseButton Button, TShiftState Shift, int X, int Y);
  void __fastcall nmpDSBeforePost(TDataSet *DataSet);
  void __fastcall nmpDSAfterPost(TDataSet *DataSet);
  void __fastcall formMessagesButtonClick(TObject *Sender);
  void __fastcall loadDataButtonClick(TObject *Sender);
  //Поиск по модели
  void __fastcall searchTimerTimer(TObject *Sender);
  void __fastcall SEditChange(TObject *Sender);
  void __fastcall SEditKeyDown(TObject *Sender,WORD &Key,TShiftState Shift);
  void __fastcall ConditionSBoxClick(TObject *Sender);
  void __fastcall FSButtonClick(TObject *Sender);
  void __fastcall BSButtonClick(TObject *Sender);
private:	// User declarations
  AnsiString iniFileName;
  AnsiString workDir;
  AnsiString iniSectionName;
  class FBDatabase *fbDB;
  TpFIBDatabase *dataBase;

  class TNextGridFrame *lFrame,*rFrame;
  TDBGridEh *lGrid,*rGrid;
  TpFIBDataSet *lDS,*rDS;

//  bool firstLoadFLG;

  AnsiString periodStr;
  TDateTime tempDate;//,factDate1,factDate2;
  int activeFrame;

  void __fastcall loadProperties(void);
  void __fastcall saveProperties(void);
  void __fastcall loadSubdivisionsDS(void);
  void __fastcall loadNMPDS(void);
  void __fastcall loadNMPSearchDS(void);
  void __fastcall setFocus(void);
  void __fastcall setHeader(void);
public:		// User declarations
  __fastcall TFormApplicationsForNMPFromSDFrame(TComponent* Owner,AnsiString iniFN,FBDatabase *fbdb);
  __fastcall ~TFormApplicationsForNMPFromSDFrame(void);
  void __fastcall initFrame(void);
  void __fastcall loadDataSets(void);
  AnsiString __fastcall getRicipientID(void);
  AnsiString __fastcall getRicipientName(void);
  AnsiString __fastcall getRicipientMail(void);
  TDateTime __fastcall getTempDate(void){ return tempDate; }
  TpFIBDataSet *__fastcall getLDS(void){ return lFrame->getGridFrame()->getWorkDataSet(); }
  TDBGridEh *  __fastcall getLGrid(void){ return lFrame->getGridFrame()->workGrid; }
};
//---------------------------------------------------------------------------
//extern PACKAGE TFormApplicationsForNMPFromSDFrame *FormApplicationsForNMPFromSDFrame;
//---------------------------------------------------------------------------
#endif
