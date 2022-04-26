//---------------------------------------------------------------------------

#ifndef CopyTemplatesH
#define CopyTemplatesH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include "Placemnt.hpp"
#include "JvComponent.hpp"
#include "JvEdit.hpp"
#include "JvExControls.hpp"
#include "JvExStdCtrls.hpp"
#include "JvSpin.hpp"
#include "JvStaticText.hpp"
#include "SmrtPanel.h"
#include "CSPIN.h"
#include <ComCtrls.hpp>
#include "JvXPButtons.hpp"
#include "JvXPCore.hpp"
#include "JvExComCtrls.hpp"
#include "JvProgressBar.hpp"
#include "JvNavigationPane.hpp"
#include "DBClasses.h"
//---------------------------------------------------------------------------
class TCopyTemplatesForm : public TForm
{
__published:	// IDE-managed Components
  TFormStorage *FormStorage;
  TJvNavPanelHeader *header;
  TPanel *workPanel;
  TLabel *fromLabel;
  TComboBox *fromPeriodBox;
  TLabel *toLabel;
  TJvEdit *toPeriodEdit;
  TJvSpinButton *spinButton;
  TJvProgressBar *progressBar;
  TPanel *buttonsPanel;
  TJvXPButton *copyButton;
  TJvXPButton *cancelButton;
  void __fastcall spinButtonBottomClick(TObject *Sender);
  void __fastcall spinButtonTopClick(TObject *Sender);
  void __fastcall cancelButtonClick(TObject *Sender);
  void __fastcall copyButtonClick(TObject *Sender);
private:	// User declarations
  AnsiString iniFileName;
  FBDatabase *fbDB;
public:		// User declarations
  __fastcall TCopyTemplatesForm(TComponent* Owner,AnsiString iniFN,FBDatabase *fb_db,unsigned short year,unsigned short monthIndex);
};
//---------------------------------------------------------------------------
//extern PACKAGE TCopyTemplatesForm *CopyTemplatesForm;
//---------------------------------------------------------------------------
#endif
