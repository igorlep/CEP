//---------------------------------------------------------------------------


#ifndef ExchangeDataFH
#define ExchangeDataFH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "JvComponent.hpp"
#include "JvExControls.hpp"
#include "JvNavigationPane.hpp"
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include "JvBaseDlg.hpp"
#include "JvSelectDirectory.hpp"
#include <Dialogs.hpp>
#include "JvWaitingGradient.hpp"
#include "JvWaitingProgress.hpp"
#include "JvExComCtrls.hpp"
#include "JvProgressBar.hpp"
#include "JvXPButtons.hpp"
#include "JvXPCore.hpp"
//---------------------------------------------------------------------------
class TExchangeDataFrame : public TFrame
{
__published:	// IDE-managed Components
  TJvNavPanelHeader *Header;
  TSplitter *vSplitter;
  TPopupMenu *exchangeDataPopupMenu;
  TMenuItem *messagesFormItem;
  TPanel *lPanel;
  TRichEdit *richEdit;
  TSplitter *hSplitter;
  TPanel *topPanel;
  TPanel *workPanel;
  TStatusBar *sdStatusBar;
  TJvNavPanelHeader *sdHeader;
  TJvXPButton *exchangeButton;
  void __fastcall messagesFormItemClick(TObject *Sender);
private:	// User declarations
  AnsiString iniFileName;
  AnsiString iniSectionName;
  class FBDatabase *fbDB;
  AnsiString type;

  class TGridFrame *sdFrame;

  void __fastcall loadProperties(void);
  void __fastcall saveProperties(void);
public:		// User declarations
  __fastcall TExchangeDataFrame(TComponent* Owner,AnsiString iniFN,FBDatabase *fbdb,AnsiString Type);
  __fastcall ~TExchangeDataFrame(void);
  void __fastcall initFrame(void);
  class TGridFrame * __fastcall getSDFrame(void);
  AnsiString __fastcall getType(void){ return type; }
};
//---------------------------------------------------------------------------
//extern PACKAGE TExchangeDataFrame *ExchangeDataFrame;
//---------------------------------------------------------------------------
#endif
