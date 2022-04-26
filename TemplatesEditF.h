//---------------------------------------------------------------------------


#ifndef TemplatesEditFH
#define TemplatesEditFH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "DBClasses.h"
#include "JvComponent.hpp"
#include "JvExControls.hpp"
#include "JvNavigationPane.hpp"
#include <ExtCtrls.hpp>
#include "JvSpeedButton.hpp"
#include "JvXPCheckCtrls.hpp"
#include "JvXPCore.hpp"
#include "NextGridF.h"
#include <Menus.hpp>
#include <ImgList.hpp>
#include <ComCtrls.hpp>
#include "JvXPButtons.hpp"
#include "JvExMask.hpp"
#include "JvSpin.hpp"
#include <Mask.hpp>
#include "JvExComCtrls.hpp"
#include "JvUpDown.hpp"
//---------------------------------------------------------------------------
class TTemplatesEditFrame : public TFrame
{
__published:	// IDE-managed Components
  TJvNavPanelHeader *Header;
  TPanel *workPanel;
  TPanel *headerPanel;
  TTimer *searchTimer;
  TLabel *SLabel;
  TEdit *SEdit;
  TJvXPCheckbox *ConditionSBox;
  TJvSpeedButton *FSButton;
  TJvSpeedButton *BSButton;
  TJvXPCheckbox *selectForPublisher;
  TLabel *periodLabel;
  TJvSpinEdit *yearEdit;
  TComboBox *monthBox;
  TJvSpinButton *spinButton;
//  TJvUpDown *upDown;
  TJvXPCheckbox *selectToPeriod;
  TPanel *pPanel;
  TSplitter *pSplitter;
  TPanel *nmpPanel;
  TSplitter *nmpSplitter;
  TPanel *tPanel;
  TJvNavPanelHeader *tHeader;
  TPopupMenu *tMenu;
  TMenuItem *addTempRecord;
  TMenuItem *editTempRecord;
  TMenuItem *delTempRecord;
  TMenuItem *AddTempRecords;
  TMenuItem *tSeparator1;
  TMenuItem *save;
  TMenuItem *cancel;
  TMenuItem *tSeparator2;
  TMenuItem *clearTemplate;
  TMenuItem *loadTemplateFromOtherPeriod;
  TMenuItem *tSeparator3;
  TMenuItem *tColumnsConfog;
  TStatusBar *tStatusBar;
  TImageList *tImageList;
  TMenuItem *reCalcItem;

  void __fastcall searchTimerTimer(TObject *Sender);
  void __fastcall SEditChange(TObject *Sender);
  void __fastcall SEditKeyDown(TObject *Sender,WORD &Key,TShiftState Shift);
  void __fastcall ConditionSBoxClick(TObject *Sender);
  void __fastcall FSButtonClick(TObject *Sender);
  void __fastcall BSButtonClick(TObject *Sender);
  void __fastcall selectForPublisherClick(TObject *Sender);
  void __fastcall selectToPeriodClick(TObject *Sender);
  void __fastcall onChangePeriod(TObject *Sender);
  void __fastcall spinButtonBottomClick(TObject *Sender);
  void __fastcall spinButtonTopClick(TObject *Sender);
//  void __fastcall upDownClick(TObject *Sender, TUDBtnType Button);

  void __fastcall pGridMouseDown(TObject *Sender,TMouseButton Button, TShiftState Shift, int X, int Y);
  void __fastcall pSourceDataChange(TObject *Sender,TField *Field);
  void __fastcall nmpGridMouseDown(TObject *Sender,TMouseButton Button, TShiftState Shift, int X, int Y);
  void __fastcall nmpSourceDataChange(TObject *Sender,TField *Field);
  void __fastcall tGridMouseDown(TObject *Sender,TMouseButton Button, TShiftState Shift, int X, int Y);
  void __fastcall tSourceStateChange(TObject *Sender);

  void __fastcall tMenuPopup(TObject *Sender);
  void __fastcall addTempRecordClick(TObject *Sender);
  void __fastcall editTempRecordClick(TObject *Sender);
  void __fastcall delTempRecordClick(TObject *Sender);
  void __fastcall saveClick(TObject *Sender);
  void __fastcall cancelClick(TObject *Sender);
  void __fastcall AddTempRecordsClick(TObject *Sender);
  void __fastcall loadTemplateFromOtherPeriodClick(TObject *Sender);
  void __fastcall clearTemplateClick(TObject *Sender);
  void __fastcall tColumnsConfogClick(TObject *Sender);

  void __fastcall copyTemplatesClick(TObject *Sender);
  void __fastcall removeTemplatesClick(TObject *Sender);
        void __fastcall reCalcItemClick(TObject *Sender);
private:	// User declarations
  AnsiString iniFileName;
  AnsiString iniSectionName;
  FBDatabase *fbDB;

  TNextGridFrame *pFrame;
  TNextGridFrame *nmpFrame;
  TGridFrame *tFrame;

  int activeFrame;
//  bool firstLoadFLG;

  AnsiString periodStr;
  TDateTime tempDate,oldTempDate,factDate1,factDate2;

  void __fastcall loadProperties(void);
  void __fastcall saveProperties(void);

  void __fastcall loadPublishersDS(void);
  void __fastcall loadPublishersSearchDS(void);
  void __fastcall loadNMPDS(void);
  void __fastcall loadNMPSearchDS(void);
  void __fastcall loadTemplateDS(void);
  void __fastcall initMenuItems(void);
public:		// User declarations
  __fastcall TTemplatesEditFrame(TComponent* Owner,AnsiString iniFN,FBDatabase *fbdb);
  __fastcall ~TTemplatesEditFrame(void);
  void __fastcall initFrame(void);
  void __fastcall loadDataSets(void);
  void __fastcall setFocus(void);
};
//---------------------------------------------------------------------------
//extern PACKAGE TTemplatesEditFrame *TemplatesEditFrame;
//---------------------------------------------------------------------------
#endif
