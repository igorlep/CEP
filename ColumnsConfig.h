//---------------------------------------------------------------------------

#ifndef ColumnsConfigH
#define ColumnsConfigH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "PLACEMNT.HPP"
#include "Placemnt.hpp"
#include "JvComponent.hpp"
#include "JvExControls.hpp"
#include "JvNavigationPane.hpp"
#include "JvXPButtons.hpp"
#include "JvXPCore.hpp"
#include <ExtCtrls.hpp>
#include "DBGridEh.hpp"
#include "kbmMemTable.hpp"
#include <DB.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------
class TColumnsConfigForm : public TForm
{
__published:	// IDE-managed Components
  TFormStorage *FormStorage;
  TPanel *buttonsPanel;
  TJvXPButton *SaveButton;
  TJvXPButton *CancelButton;
  TPanel *workPanel;
  TJvNavPanelHeader *header;
  TkbmMemTable *table;
  TSmallintField *tableNumber;
  TStringField *tableFieldTitle;
  TBooleanField *tableEditInGrid;
  TBooleanField *tableGridViewAble;
  TBooleanField *tableTitleButton;
  TBooleanField *tableSearchAble;
  TDataSource *source;
  TDBGridEh *grid;
        TStringField *tableFormatStr;
  void __fastcall CancelButtonClick(TObject *Sender);
  void __fastcall SaveButtonClick(TObject *Sender);
  void __fastcall tableEditInGridChange(TField *Sender);
  void __fastcall tableSearchAbleChange(TField *Sender);
private:	// User declarations
  AnsiString iniFileName;
  AnsiString iniSectionName;
  AnsiString tableTitle;
  AnsiString fieldsInfo;
  void __fastcall loadTableFromStr(void);
  void __fastcall unLoadTableToStr(void);
public:		// User declarations
  __fastcall TColumnsConfigForm(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,AnsiString tableT,AnsiString fieldsInf);
  __fastcall ~TColumnsConfigForm(void);
  AnsiString __fastcall getFieldsInfo(void){ return fieldsInfo; }
};
//---------------------------------------------------------------------------
//extern PACKAGE TColumnsConfigForm *ColumnsConfigForm;
//---------------------------------------------------------------------------
#endif
