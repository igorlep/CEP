//---------------------------------------------------------------------------


#ifndef BackUpBaseFH
#define BackUpBaseFH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "JvComponent.hpp"
#include "JvExComCtrls.hpp"
#include "JvExControls.hpp"
#include "JvNavigationPane.hpp"
#include "JvProgressBar.hpp"
#include "JvWaitingGradient.hpp"
#include "JvXPButtons.hpp"
#include "JvXPCore.hpp"
#include "Placemnt.hpp"
#include "ToolEdit.hpp"
#include "ZipBuilder.h"
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <IBServices.hpp>
#include <Mask.hpp>
#include "JvXPCheckCtrls.hpp"
//---------------------------------------------------------------------------
class TBackUpBaseFrame : public TFrame
{
__published:	// IDE-managed Components
  TPanel *workPanel;
  TLabel *Label3;
  TLabel *Label2;
  TLabel *Label1;
  TFilenameEdit *ZipFilenameEdit;
  TDirectoryEdit *BackUpDirEdit;
  TJvNavPanelHeader *DBNameHeader;
  TFilenameEdit *BackUpFilenameEdit;
  TRichEdit *RichEdit;
  TJvWaitingGradient *WaitingGradient;
  TJvProgressBar *ProgressBar;
  TZipBuilder *ZipBuilder;
  TIBBackupService *BackupService;
  TJvXPButton *ArchButton;
  TJvXPCheckbox *protocolShowBox;
  void __fastcall BackUpFilenameEditAfterDialog(TObject *Sender,AnsiString &Name,bool &Action);
  void __fastcall BackUpDirEditAfterDialog(TObject *Sender,AnsiString &Name,bool &Action);
  void __fastcall ZipFilenameEditAfterDialog(TObject *Sender,AnsiString &Name,bool &Action);
  void __fastcall ArchButtonClick(TObject *Sender);
private:	// User declarations
  AnsiString workDir;
  AnsiString iniFileName;
  class FBDatabase *fbDB;
  AnsiString server;
  AnsiString dbFullName;
  TDateTime backUpDateTime;
  TStatusPanel *statusPanel;
  void __fastcall loadProperties(void);
  void __fastcall saveProperties(void);
public:		// User declarations
  __fastcall TBackUpBaseFrame(TComponent* Owner,AnsiString wd,AnsiString iniFN,FBDatabase *fbdb,TPanel *workPanel,TStatusPanel *statPanel);
  __fastcall ~TBackUpBaseFrame(void);
  void __fastcall initFrame(void);
  TDateTime __fastcall getBackUpDateTime(void){ return backUpDateTime; }
};
//---------------------------------------------------------------------------
//extern PACKAGE TBackUpBaseFrame *BackUpBaseFrame;
//---------------------------------------------------------------------------
#endif
