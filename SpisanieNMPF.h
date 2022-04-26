//---------------------------------------------------------------------------


#ifndef SpisanieNMPFH
#define SpisanieNMPFH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "JvComponent.hpp"
#include "JvExControls.hpp"
#include "JvXPCheckCtrls.hpp"
#include "JvXPCore.hpp"
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TSpisanieNMPFrame : public TFrame
{
__published:	// IDE-managed Components
  TPanel *workPanel;
  TPanel *nmpButtonsPanel;
  TLabel *Label4;
  TSpeedButton *fill_NMP_DSButton;
  TLabel *labelST_1;
  TLabel *labelST_2;
  TLabel *labelST_3;
  TLabel *labelST_4;
  TComboBox *NDSBox;
  TJvXPCheckbox *delDistrRecordsBox;
  TStaticText *ST_1;
  TStaticText *ST_2;
  TStaticText *ST_3;
  TStaticText *ST_4;
  TPanel *nmpPanel;
  TPopupMenu *nmpGridMenu;
  TMenuItem *fillQReturnItem;
  TMenuItem *seporatorItem;
  TMenuItem *nmpSaveItem;
  TMenuItem *nmpCancelItem;
  TMenuItem *seporator1Item;
  TMenuItem *columnsConfigItem;
  TMenuItem *changeItem;
  TMenuItem *viewBySDItem;
  void __fastcall workPanelResize(TObject *Sender);
  void __fastcall nmpSourceStateChange(TObject *Sender);
  void __fastcall nmpDataSetBeforePost(TDataSet *DataSet);
  void __fastcall nmpGridDblClick(TObject *Sender);
  void __fastcall nmpDataSetAfterPost(TDataSet *DataSet);
  void __fastcall nmpGridColumns8GetCellParams(TObject *Sender,bool EditMode,TColCellParamsEh *Params);
  void __fastcall nmpGridColumns9GetCellParams(TObject *Sender,bool EditMode,TColCellParamsEh *Params);
  void __fastcall nmpGridColumns10GetCellParams(TObject *Sender,bool EditMode,TColCellParamsEh *Params);
  void __fastcall NDSBoxChange(TObject *Sender);
  void __fastcall delDistrRecordsBoxClick(TObject *Sender);
  void __fastcall fill_NMP_DSButtonClick(TObject *Sender);
  void __fastcall nmpSaveItemClick(TObject *Sender);
  void __fastcall nmpCancelItemClick(TObject *Sender);
  void __fastcall columnsConfigItemClick(TObject *Sender);
  void __fastcall workGridKeyDown(TObject *Sender, WORD &Key,TShiftState Shift);
  void __fastcall changeItemClick(TObject *Sender);
  void __fastcall nmpGridMenuPopup(TObject *Sender);
  void __fastcall nmpGridGetCellParams(TObject *Sender,TColumnEh *Column,TFont *AFont,TColor &Background,TGridDrawState State);
  void __fastcall viewBySDItemClick(TObject *Sender);
private:	// User declarations
  AnsiString iniFileName;
  AnsiString iniSectionName;
  class FBDatabase *fbDB;

  class TReturnNMPtoPublishersFrame *parent;

  class TGridContentFrame *nmpFrame;
  class TGridFrame *nmpF;

  bool fillFLG;

  void __fastcall loadNMPDS(void);
  void __fastcall loadNMPSearchDS(void);
  int __fastcall getStavkaNDS(void);
  void __fastcall getSum1(void);
  void __fastcall getSum2(void);
  void __fastcall getSum3(void);
  void __fastcall getSum4(void);
  void __fastcall additionalAdjustment(void);
public:		// User declarations
  __fastcall TSpisanieNMPFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbdb);
  __fastcall ~TSpisanieNMPFrame(void);
  void __fastcall initFrame(void);
  TpFIBDataSet * __fastcall getWorkDataSet(void);
  TGridContentFrame * __fastcall getGridContentFrame(void){ return nmpFrame; }
  TGridFrame * __fastcall getGridFrame(void){ return nmpFrame->getGridFrame(); }
  void __fastcall loadDS(void);
  void __fastcall loadSearchDS(void);
  void __fastcall setFocus(void);
  void __fastcall reLoadDS(void);
  void __fastcall forwardSearch(void);
  void __fastcall backwardSearch(void);
};
//---------------------------------------------------------------------------
//extern PACKAGE TSpisanieNMPFrame *SpisanieNMPFrame;
//---------------------------------------------------------------------------
#endif
