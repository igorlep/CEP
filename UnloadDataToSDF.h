//---------------------------------------------------------------------------


#ifndef UnloadDataToSDFH
#define UnloadDataToSDFH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "JvComponent.hpp"
#include "JvExControls.hpp"
#include "JvNavigationPane.hpp"
#include "JvSpeedButton.hpp"
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include "DBGridEh.hpp"
//---------------------------------------------------------------------------
class TUnloadDataToSDFrame : public TFrame
{
__published:	// IDE-managed Components
  TJvNavPanelHeader *header;
  TPanel *sdPanel;
  TJvNavPanelHeader *sdHeader;
  TSplitter *vSplitter;
  TStatusBar *sdStatusBar;
  TPanel *nmpPanel;
  TJvNavPanelHeader *nmpHeader;
  TStatusBar *nmpStatusBar;
  TPanel *headerPanel;
  TJvSpeedButton *periodButton;
  TJvSpeedButton *createMessagesButton;
  TProgressBar *sdBar;
  TProgressBar *nmpBar;
  void __fastcall sdSourceDataChange(TObject *Sender,TField *Field);
  void __fastcall periodButtonClick(TObject *Sender);
        void __fastcall createMessagesButtonClick(TObject *Sender);
private:	// User declarations
  AnsiString iniFileName;
  AnsiString iniSectionName;
  class FBDatabase *fbDB;

  TDateTime date1,date2;
  class TGridFrame *sdFrame;
  class TGridFrame *nmpFrame;

  void __fastcall loadProperties(void);
  void __fastcall saveProperties(void);
  void __fastcall setHeader(void);
  void __fastcall loadSD_DS(void);
  void __fastcall loadNMP_DS(void);
public:		// User declarations
  __fastcall TUnloadDataToSDFrame(TComponent* Owner,AnsiString iniFN,FBDatabase *fbdb);
  __fastcall ~TUnloadDataToSDFrame(void);
  void __fastcall initFrame(void);
  AnsiString __fastcall getRicipientID(void);
  AnsiString __fastcall getRicipientName(void);
  AnsiString __fastcall getRicipientMail(void);
  TDateTime __fastcall getDate1(void){ return date1; }
  TDateTime __fastcall getDate2(void){ return date2; }
  TpFIBDataSet * __fastcall getLDS(void){ return sdFrame->getWorkDataSet(); }
  TDBGridEh * __fastcall getLGrid(void){ return sdFrame->workGrid; }
  TpFIBDataSet * __fastcall getRDS(void){ return nmpFrame->getWorkDataSet(); }
  TDBGridEh * __fastcall getRGrid(void){ return nmpFrame->workGrid; }
};
//---------------------------------------------------------------------------
//extern PACKAGE TUnloadDataToSDFrame *UnloadDataToSDFrame;
//---------------------------------------------------------------------------
#endif
