//---------------------------------------------------------------------------


#ifndef UsersWorkFH
#define UsersWorkFH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include "FIBDataSet.hpp"
#include "pFIBDataSet.hpp"
#include <DB.hpp>
//---------------------------------------------------------------------------
class TUsersWorkFrame : public TFrame
{
__published:	// IDE-managed Components
  TPanel *leftPanel;
  TSplitter *splitter;
  TPanel *rightPanel;
  void __fastcall uGridMouseDown(TObject *Sender,TMouseButton Button, TShiftState Shift, int X, int Y);
  void __fastcall uSourceDataChange(TObject *Sender,TField *Field);
  void __fastcall tvMouseDown(TObject *Sender,TMouseButton Button,TShiftState Shift,int X,int Y);
  void __fastcall saveUserRightsClick(TObject *Sender);
private:	// User declarations
  TComponent* Owner;
  AnsiString iniFileName,iniSectionName;
  class FBDatabase *fbDB;

  // - Справочник пользователей
  class TRBFrame *usersFrame;
  // - Дерево прав пользователя
  class TDBTreeViewFrame *rightsTV;

  int activeFrame;
  AnsiString userID;

  void __fastcall loadProperties(void);
  void __fastcall saveProperties(void);
  void __fastcall saveRightsKitUser(void);
  void __fastcall fillTV(AnsiString id);
public:		// User declarations
  __fastcall TUsersWorkFrame(TComponent* Owner,AnsiString iniFN,FBDatabase *fbdb);
  __fastcall ~TUsersWorkFrame(void);
  void __fastcall initFrame(void);
  void __fastcall setFocus(void);
  void __fastcall loadRightsKitUser(void);
};
//---------------------------------------------------------------------------
//extern PACKAGE TUsersWorkFrame *UsersWorkFrame;
//---------------------------------------------------------------------------
#endif
