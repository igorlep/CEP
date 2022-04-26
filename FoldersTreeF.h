//---------------------------------------------------------------------------


#ifndef FoldersTreeFH
#define FoldersTreeFH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include "DBClasses.h"
#include "Placemnt.hpp"
#include "JvComCtrls.hpp"
#include "JvDotNetControls.hpp"
#include "JvExComCtrls.hpp"
//---------------------------------------------------------------------------
//Struct for tree nodes
struct TStructItem {
  int AbsoluteIndex;
  AnsiString id;
  AnsiString folder_number;
  TStructItem(){AbsoluteIndex=0;}
};
//---------------------------------------------------------------------------
class TFoldersTreeFrame : public TFrame
{
__published:	// IDE-managed Components
  TPanel *TVButtonsPanel;
  TSpeedButton *AddFolderButton;
  TSpeedButton *DelFolderButton;
  TPopupMenu *foldersTreeMenu;
  TMenuItem *ftAddFolderItem;
  TMenuItem *ftDelFolderItem;
  TMenuItem *ftRenameFolderItem;
  TJvDotNetTreeView *TV;
private:	// User declarations
  AnsiString iniFileName;
  AnsiString sectionName;
  FBDatabase *fbDB;
  AnsiString tableName;

  void __fastcall loadProperties(void);
  void __fastcall saveProperties(void);
  void __fastcall SaveExpandedFolders(TStrings* list);
  void __fastcall ExpandFolders(TStrings* list);
  void __fastcall FindPositionTree(AnsiString id);
  void __fastcall BuildNode(TTreeNode* ParentNode, AnsiString ParentID);
  void __fastcall BuildTree(void);
  void __fastcall RemoveTree(void);
public:		// User declarations
  __fastcall TFoldersTreeFrame(TComponent* Owner,AnsiString iniFileName,AnsiString sectionName,FBDatabase *fbdb,AnsiString tabName);
  __fastcall ~TFoldersTreeFrame(void);
  AnsiString __fastcall getFullFolderName(void);
};
//---------------------------------------------------------------------------
//extern PACKAGE TFoldersTreeFrame *FoldersTreeFrame;
//---------------------------------------------------------------------------
#endif
