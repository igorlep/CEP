//---------------------------------------------------------------------------


#ifndef FinancialResultsForSDFH
#define FinancialResultsForSDFH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "JvComponent.hpp"
#include "JvExControls.hpp"
#include "JvNavigationPane.hpp"
#include "JvSpeedButton.hpp"
#include "JvXPCheckCtrls.hpp"
#include "JvXPCore.hpp"
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include "frxClass.hpp"
#include "frxDBSet.hpp"
#include "frxExportXLS.hpp"
//---------------------------------------------------------------------------
class TFinancialResultsForSDFrame : public TFrame
{
__published:	// IDE-managed Components
  TJvNavPanelHeader *header;
  TPanel *headerPanel;
  TJvSpeedButton *periodButton;
  TJvSpeedButton *printButton;
  TPanel *resultPanel;
  TStatusBar *resultBar;
  TJvXPCheckbox *groupOnPostOffices;
        TfrxDBDataset *frxDS;
        TfrxXLSExport *frxXLSExport;
        TfrxReport *frxReport;
        void __fastcall periodButtonClick(TObject *Sender);
        void __fastcall groupOnPostOfficesClick(TObject *Sender);
        void __fastcall printButtonClick(TObject *Sender);
        void __fastcall frxReportGetValue(const AnsiString VarName,
          Variant &Value);
private:	// User declarations
  AnsiString iniFileName;
  AnsiString workDir;
  AnsiString iniSectionName;
  class FBDatabase *fbDB;
  class TpFIBDatabase *dataBase;

  TDateTime date1,date2;
  class TGridFrame *resultFrame;

  void __fastcall loadProperties(void);
  void __fastcall saveProperties(void);
  void __fastcall setHeader(void);
  void __fastcall loadResultDS(void);
  void __fastcall cteateFrame(void);
public:		// User declarations
  __fastcall TFinancialResultsForSDFrame(TComponent* Owner,AnsiString iniFN,FBDatabase *fbdb);
  __fastcall ~TFinancialResultsForSDFrame(void);
  void __fastcall initFrame(void);
};
//---------------------------------------------------------------------------
//extern PACKAGE TFinancialResultsForSDFrame *FinancialResultsForSDFrame;
//---------------------------------------------------------------------------
#endif
