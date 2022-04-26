//---------------------------------------------------------------------------


#ifndef UsersRightsFH
#define UsersRightsFH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "JvComponent.hpp"
#include "JvExControls.hpp"
#include "JvNavigationPane.hpp"
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TUsersRightsFrame : public TFrame
{
__published:	// IDE-managed Components
  TJvNavPanelHeader *header;
  TPanel *rPanel;
  TSplitter *vSplitter;
  TPanel *lPanel;
  TJvNavPanelHeader *uHeader;
  TStatusBar *uStatusBar;
  TPopupMenu *uMenu;
  TMenuItem *saveRightsKitUserItem;
  void __fastcall uGridMouseDown(TObject *Sender,TMouseButton Button, TShiftState Shift, int X, int Y);
  void __fastcall uSourceDataChange(TObject *Sender,TField *Field);
  void __fastcall tvMouseDown(TObject *Sender,TMouseButton Button,TShiftState Shift,int X,int Y);
  void __fastcall saveRightsKitUserItemClick(TObject *Sender);
private:	// User declarations
  AnsiString iniFileName;
  AnsiString iniSectionName;
  class FBDatabase *fbDB;
  int userID;

  class TGridFrame *usersFrame;
  class TDBTreeViewFrame *rightsTV;
  int activeFrame;

  void __fastcall loadProperties(void);
  void __fastcall saveProperties(void);
  void __fastcall setFocus(void);
  void __fastcall saveRightsKitUser(void);
  void __fastcall loadRightsKitUser(void);
public:		// User declarations
  __fastcall TUsersRightsFrame(TComponent* Owner,AnsiString iniFN,class FBDatabase *fbdb);
  __fastcall ~TUsersRightsFrame(void);
  void __fastcall initFrame(void);
};
//---------------------------------------------------------------------------
//extern PACKAGE TUsersRightsFrame *UsersRightsFrame;
//---------------------------------------------------------------------------
#endif
