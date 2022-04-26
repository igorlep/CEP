//---------------------------------------------------------------------------
//����� - ������ �������� �����������

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
// ������ �������� ������
struct RBTreeData
{
  AnsiString ID; //������������� ������ �� ������� rbTableName
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
  AnsiString rootFileID;        //������������� �������� �����
  AnsiString delFileID;         //������������� ����� ��� ��������� �������
  AnsiString currentFileID;

  class TGridFrame *gridFrame; //��������� �� �����, ������������ ���������� ������� �����

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

  //������, ������������ �� ������� ���������� � ������������ ��� �������� ������
  // - ������ ��������� select SQL ������ ��� ��������� DataSet
  typedef AnsiString __fastcall (*pfISSQL)(AnsiString FILE_ID);
  pfISSQL formSelectSQL;
  // - ������ ��������� search SQL ������ ���  searchDS
  typedef AnsiString __fastcall (*pfISearchSQL)(AnsiString fileID,AnsiString fieldName,AnsiString editText,bool condition);
  pfISearchSQL formSearchSQL;
};
//---------------------------------------------------------------------------
//extern PACKAGE TTreeViewFrame *TreeViewFrame;
//---------------------------------------------------------------------------
#endif
