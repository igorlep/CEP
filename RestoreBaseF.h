//---------------------------------------------------------------------------


#ifndef RestoreBaseFH
#define RestoreBaseFH
//---------------------------------------------------------------------------
#include "DBClasses.h"
#include "JvComponent.hpp"
#include "JvExComCtrls.hpp"
#include "JvExControls.hpp"
#include "JvNavigationPane.hpp"
#include "JvProgressBar.hpp"
#include "JvWaitingGradient.hpp"
#include "JvXPButtons.hpp"
#include "JvXPCore.hpp"
#include "ToolEdit.hpp"
#include "ZipBuilder.h"
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <IBServices.hpp>
#include <Mask.hpp>
#include <StdCtrls.hpp>
#include "JvXPCheckCtrls.hpp"
#include "JvExMask.hpp"
#include "JvToolEdit.hpp"
//---------------------------------------------------------------------------
class TRestoreBaseFrame : public TFrame
{
__published:	// IDE-managed Components
  TPanel *workPanel;
  TJvNavPanelHeader *DBNameHeader;
  TRichEdit *RichEdit;
  TJvWaitingGradient *WaitingGradient;
  TJvProgressBar *ProgressBar;
  TLabel *Label1;
  TFilenameEdit *zipFilenameEdit;
  TLabel *Label2;
  TLabel *Label3;
  TDirectoryEdit *dbNetDirectoryEdit;
  TLabel *Label4;
  TDirectoryEdit *dbCopyDirectoryEdit;
  TJvXPButton *RestoreButton;
  TZipBuilder *ZipBuilder;
  TIBRestoreService *RestoreService;
  TJvXPCheckbox *protocolShowBox;
  TDirectoryEdit *backupDirectoryEdit;
  void __fastcall RestoreButtonClick(TObject *Sender);
  void __fastcall ZipBuilderMessage(TObject *Sender,int ErrCode,AnsiString Message);
  void __fastcall backupDirectoryEditChange(TObject *Sender);
private:	// User declarations
  AnsiString workDir;
  AnsiString iniFileName;
  class FBDatabase *fbDB;
  AnsiString server;
  AnsiString dbFullName;
  TDateTime restoreDateTime;
  TStatusPanel *statusPanel;
  AnsiString backupFN;
  AnsiString backupFileName;
  void __fastcall loadProperties(void);
  void __fastcall saveProperties(void);
public:		// User declarations
  __fastcall TRestoreBaseFrame(TComponent* Owner,AnsiString wd,AnsiString iniFN,FBDatabase *fbdb,TPanel *workPanel,TStatusPanel *statPanel);
  __fastcall ~TRestoreBaseFrame(void);
  void __fastcall initFrame(void);
};
//---------------------------------------------------------------------------
//extern PACKAGE TRestoreBaseFrame *RestoreBaseFrame;
//---------------------------------------------------------------------------
#endif
