//---------------------------------------------------------------------------


#ifndef ContentFH
#define ContentFH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "JvComponent.hpp"
#include "JvExControls.hpp"
#include "JvNavigationPane.hpp"
#include "DBClasses.h"
#include "GridF.h"
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TContentFrame : public TFrame
{
__published:	// IDE-managed Components
  TJvNavPanelHeader *contentHeader;
  TMemo *contentMemo;
private:	// User declarations
  class TGridFrame *gridFrame;
  TStringList *fieldsName;
  TStringList *fieldsTitle;
public:		// User declarations
  __fastcall TContentFrame(TComponent* Owner,TGridFrame *gf,AnsiString contInfo);
  __fastcall ~TContentFrame(void);
  void __fastcall initContentFields(AnsiString contentInfo);
  void __fastcall setContent(AnsiString headerString);
  void __fastcall setContent(void);
  void __fastcall clearContent(void);
  void __fastcall setGridFrame(TGridFrame *gf){ gridFrame=gf; }
};
//---------------------------------------------------------------------------
//extern PACKAGE TContentFrame *ContentFrame;
//---------------------------------------------------------------------------
#endif
