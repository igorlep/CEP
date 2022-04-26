//---------------------------------------------------------------------------


#ifndef VozvratNMPFH
#define VozvratNMPFH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TVozvratNMPFrame : public TFrame
{
__published:	// IDE-managed Components
  TPanel *nmpPanel;
  TPopupMenu *nmpGridMenu;
  TMenuItem *nmpSaveItem;
  TMenuItem *nmpCancelItem;
  TMenuItem *seporator2;
  TMenuItem *columnsConfigItem;
  TPanel *buttonPanel;
  TSpeedButton *fill_NMP_DSButton;
  TMenuItem *fillQReturnItem;
  TMenuItem *separator1;
  TMenuItem *viewBySDItem;
  void __fastcall nmpSourceStateChange(TObject *Sender);
  void __fastcall nmpDataSetBeforePost(TDataSet *DataSet);
  void __fastcall nmpDataSetAfterPost(TDataSet *DataSet);
  void __fastcall nmpSaveItemClick(TObject *Sender);
  void __fastcall nmpCancelItemClick(TObject *Sender);
  void __fastcall columnsConfigItemClick(TObject *Sender);
  void __fastcall fill_NMP_DSButtonClick(TObject *Sender);
  void __fastcall workGridKeyDown(TObject *Sender, WORD &Key,TShiftState Shift);
  void __fastcall viewBySDItemClick(TObject *Sender);
private:	// User declarations
  AnsiString iniFileName;
  AnsiString iniSectionName;
  class FBDatabase *fbDB;

  class TReturnNMPtoPublishersFrame *parent;

  class TGridFrame *nmpFrame;

  void __fastcall loadNMPDS(void);
  void __fastcall loadNMPSearchDS(void);
public:		// User declarations
  __fastcall TVozvratNMPFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbdb);
  __fastcall ~TVozvratNMPFrame(void);
  void __fastcall initFrame(void);
  TpFIBDataSet * __fastcall getWorkDataSet(void);
  TGridFrame * __fastcall getGridFrame(void){ return nmpFrame; }
  void __fastcall loadDS(void);
  void __fastcall loadSearchDS(void);
  void __fastcall setFocus(void);
  void __fastcall reLoadDS(void);
  void __fastcall forwardSearch(void);
  void __fastcall backwardSearch(void);
};
//---------------------------------------------------------------------------
//extern PACKAGE TVozvratNMPFrame *VozvratNMPFrame;
//---------------------------------------------------------------------------
#endif
