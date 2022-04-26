//---------------------------------------------------------------------------

#ifndef GetDateH
#define GetDateH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "JvComponent.hpp"
#include "JvExControls.hpp"
#include "JvXPButtons.hpp"
#include "JvXPCore.hpp"
#include "Placemnt.hpp"
#include "SmrtPanel.h"
#include <ExtCtrls.hpp>
#include "ToolEdit.hpp"
#include <Mask.hpp>
#include "DBClasses.h"
#include "JvNavigationPane.hpp"
#include "GridF.h"
//---------------------------------------------------------------------------
class TGetDateForm : public TForm
{
__published:	// IDE-managed Components
  TFormStorage *FormStorage;
  TPanel *workPanel;
  TPanel *buttonsPanel;
  TJvXPButton *JXPBOK;
  TJvXPButton *JXPBCancel;
  TJvNavPanelHeader *header;
  void __fastcall FormShow(TObject *Sender);
  void __fastcall JXPBOKClick(TObject *Sender);
  void __fastcall JXPBCancelClick(TObject *Sender);
private:	// User declarations
  AnsiString iniFileName;
  FBDatabase *fbDB;
  TGridFrame *gridFrame;
public:		// User declarations
  __fastcall TGetDateForm(TComponent* Owner,AnsiString iniFN,FBDatabase *fbdb,AnsiString e_id);
  __fastcall ~TGetDateForm(void);
  void __fastcall setGridFrame(TGridFrame *gf){ gridFrame=gf; }
  TGridFrame * __fastcall getGridFrame(void){ return gridFrame; }
};
//---------------------------------------------------------------------------
//extern PACKAGE TGetDateForm *GetDateForm;
//---------------------------------------------------------------------------
#endif
