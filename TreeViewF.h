//---------------------------------------------------------------------------
//Фрейм - Дерево разделов справочника

#ifndef TreeViewFH
#define TreeViewFH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include "DBClasses.h"
#include <ImgList.hpp>
#include <Menus.hpp>
#include "PLACEMNT.HPP"
//---------------------------------------------------------------------------
// Данные элемента дерева
struct RBTreeData
{
  AnsiString ID; //идентификатор записи из таблицы rbTableName
};
//---------------------------------------------------------------------------
class TTreeViewFrame : public TFrame
{
__published:	// IDE-managed Components
  TPanel *buttonsPanel;
  TSpeedButton *AddFileButton;
  TSpeedButton *DelFileButton;
  TCheckBox *OnFilesBox;
  TTreeView *treeView;
  TImageList *TVImageList;
  TPopupMenu *TVPopupMenu;
  TMenuItem *TV_AddFile;
  TMenuItem *TV_DelFile;
  TMenuItem *TV_RenameFile;
  void __fastcall FrameResize(TObject *Sender);
  void __fastcall treeViewChange(TObject *Sender, TTreeNode *Node);
  void __fastcall AddFileButtonClick(TObject *Sender);
  void __fastcall TV_RenameFileClick(TObject *Sender);
  void __fastcall treeViewEdited(TObject *Sender, TTreeNode *Node,AnsiString &S);
  void __fastcall DelFileButtonClick(TObject *Sender);
  void __fastcall OnFilesBoxClick(TObject *Sender);
  void __fastcall treeViewMouseUp(TObject *Sender,TMouseButton Button, TShiftState Shift, int X, int Y);
private:	// User declarations
  AnsiString iniFileName;
  AnsiString iniSectionName;
  FBDatabase *fbDatabase;
  AnsiString rbTableInfo;
  AnsiString rbTableName;
  AnsiString keyFieldName;
  AnsiString viewFieldName;
  AnsiString rootFileID;        //идентификатор корневой папки
  AnsiString delFileID;         //идентификатор папки для удаленных записей
  AnsiString currentFileID;

  class TGridFrame *gridFrame; //указатель на фрейм, отображающий содержимое текущей папки

  void __fastcall buildNode(TTreeNode* ParentNode, AnsiString ParentID);
  void __fastcall buildTree(AnsiString rootID);
  void __fastcall removeTree(void);
public:		// User declarations
  __fastcall TTreeViewFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,AnsiString rbTI,AnsiString rootID);
  __fastcall ~TTreeViewFrame(void);
  void __fastcall initTreeViewFrame(void);
  void __fastcall setPositionInTreeView(TTreeNode *node);
  TTreeNode * __fastcall findNodeInTreeView(AnsiString id);
  void __fastcall setGridFrame(TGridFrame *gridF){ gridFrame=gridF; }
  AnsiString __fastcall getViewFieldName(void){ return viewFieldName; }
  AnsiString __fastcall getRootFileID(void){ return rootFileID; }
  AnsiString __fastcall getDelFileID(void){ return delFileID; }
  AnsiString __fastcall getFileID(void);

  //Функии, определяемые во внешнем приложении и инициируемые при создании фрейма
  // - Фукция формирует select SQL запрос для основного DataSet
  typedef AnsiString __fastcall (*pfISSQL)(AnsiString FILE_ID);
  pfISSQL formSelectSQL;
  // - Фукция формирует search SQL запрос для  searchDS
  typedef AnsiString __fastcall (*pfISearchSQL)(AnsiString fileID,AnsiString fieldName,AnsiString editText,bool condition);
  pfISearchSQL formSearchSQL;
};
//---------------------------------------------------------------------------
//extern PACKAGE TTreeViewFrame *TreeViewFrame;
//---------------------------------------------------------------------------
#endif
