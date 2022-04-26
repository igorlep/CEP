//---------------------------------------------------------------------------


#ifndef ArchivingBDFH
#define ArchivingBDFH
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
#include "JvCaptionPanel.hpp"
#include "JvExExtCtrls.hpp"
//---------------------------------------------------------------------------
class TArchivingBDFrame : public TFrame
{
__published:	// IDE-managed Components
  TPanel *workPanel;
        TSplitter *vSplitter;
        TJvNavPanelHeader *header;
private:	// User declarations
  AnsiString iniFileName;
  AnsiString iniSectionName;
  FBDatabase *fbDB;

  class TBackUpBaseFrame *archBaseFrame;

  void __fastcall loadProperties(void);
  void __fastcall saveProperties(void);
public:		// User declarations
  __fastcall TArchivingBDFrame(TComponent* Owner,AnsiString iniFN,class FBDatabase *fbfb);
  __fastcall ~TArchivingBDFrame(void);
  void __fastcall initFrame(void);
};
//---------------------------------------------------------------------------
//extern PACKAGE TArchivingBDFrame *ArchivingBDFrame;
//---------------------------------------------------------------------------
#endif
