//---------------------------------------------------------------------------
//����� ��� ����������� ������������� �������� � ���� ������ � ��������� (CheckBox-���)
// ������ ����������� �� ������ ������� tableName �� ���� ������ fbDB
// ��������� ��� ������������ ������ �������� 2 ���� �������:
//  keyFieldName - �������� ����. �������� ��� ������, ������������ �� ��������� � ���������
//   ������ � ����� ������ ������ ��������� ���� (������) ���� ������������ ��������� ������,
//   ������� � ������� ������, ����������� �������� '.', � ����� ������ ��������
//   ������: 01.02.05, ��� 01 � 02 - ���� ������������ ���������, 05- ��� ������ ��������
//  viewFieldName - ���� � ���������, ������������ � ������
//
// � ����������� ������ ���������� ��������� ���������:
//  Owner - ���������, �� ������ �������� ������������ ����� � �������,
//  Parent - ����������� �������, � ������ �������� ����� ������� ����� � �������,
//  iniFN - ������ ��� ����� �������������,
//  fbdb - ��������� �� ������ � ����� ������,
//  structTInfo - �������������� ������ � ��������� ���������� �������
//  ������ ������ structTInfo:
//  ������������_�������,
//  ���������_�������,
//  ������������_����_�_�����_������,
//  ������������_����_�_��������_������,
//  ������������_����������_��������_����_�_������_��������_������
//  ����������: 1.���������_������� ����� ������� � DBTreeViewHeader
//              2.���� ���������� �������� ���� ������������ ������ �������� ������
//                �����������,�� ����������� ������� ����� ������ ����������� ������.
//
// � ����� ������������� iniFileName ����������� ��� �������� �������� ������ � ����
// ���� ����������� �����.
// ��� ���������� � �������������� ����� ���������� ��������� ������ ��� �������:
//  getCheckedItems(void) - ���������� ������ � ������ ���������� ���������.
//   ������ ���� ��������� ������:
//    ���_��������1,�����_�������1;���_��������2,�����_�������2;...
//  setCheckForItems(AnsiString checkedItems) - ��������������� ���������� ��������
//   �� ��������� ������, ������������ � ������ checkedItems

#ifndef DBTreeViewFH
#define DBTreeViewFH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "ChTreeView.h"
#include "JvComponent.hpp"
#include "JvExControls.hpp"
#include "JvNavigationPane.hpp"
#include <ComCtrls.hpp>
#include "DBClasses.h"
#include <Menus.hpp>
//---------------------------------------------------------------------------
// ������ �������� ������
struct DBTreeData
{
  AnsiString ID; //������������� ������ �� ������� tableName
};
//---------------------------------------------------------------------------
class TDBTreeViewFrame : public TFrame
{
__published:	// IDE-managed Components
  TChTreeView *dbTreeView;
  TJvNavPanelHeader *DBTreeViewHeader;
  TPopupMenu *tvPopupMenu;
  TMenuItem *selectAll;
  TMenuItem *unselectAll;
  TMenuItem *expand;
  TMenuItem *collapse;
  TMenuItem *separator1;
  TMenuItem *separator2;
  TMenuItem *saveItem;
  void __fastcall dbTreeViewChange(TObject *Sender, TTreeNode *Node);
  void __fastcall selectAllClick(TObject *Sender);
  void __fastcall unselectAllClick(TObject *Sender);
  void __fastcall expandClick(TObject *Sender);
  void __fastcall collapseClick(TObject *Sender);
  void __fastcall dbTreeViewEnter(TObject *Sender);
  void __fastcall dbTreeViewExit(TObject *Sender);
  void __fastcall tvPopupMenuPopup(TObject *Sender);
  void __fastcall dbTreeViewSelectNode(TObject *Sender,TTreeNode *Node);
  void __fastcall dbTreeViewDeselectNode(TObject *Sender,TTreeNode *Node);
private:	// User declarations
  AnsiString ownerName;
  AnsiString iniFileName;
  FBDatabase *fbDB;
  AnsiString structureTableInfo;
  AnsiString tableName;
  AnsiString tableTitle;
  AnsiString keyFieldName;
  AnsiString viewFieldName;
  int levelExponent; //������������ ���������� �������� ���� � ������ ������ ��������
  AnsiString levelTemplate; //������-������, ���������� ������ '_', ������ � levelExponent
  AnsiString currentItemID;
  bool changeFLG;
  void __fastcall buildNode(TTreeNode* ParentNode, AnsiString ParentID);
  void __fastcall buildTree(void);
  void __fastcall removeTree(void);
public:		// User declarations
 __fastcall TDBTreeViewFrame(TComponent* Owner,TWinControl* parent,AnsiString iniFN,FBDatabase *fbdb,AnsiString structTInfo);
  __fastcall ~TDBTreeViewFrame(void);
  void __fastcall initTreeViewFrame(void);
  TTreeNode * __fastcall findNodeInTreeView(AnsiString id);
  void __fastcall setPositionInTreeView(TTreeNode *node);
  AnsiString __fastcall getCheckedItems(void);
  void __fastcall setCheckForItems(AnsiString checkedItems);
  void __fastcall setHeaderCaption(AnsiString caption){ DBTreeViewHeader->Caption=caption; }
  bool __fastcall getChangeFLG(void){ return changeFLG; }
  void __fastcall setChangeFLG(bool flg){ changeFLG=flg; }
};
//---------------------------------------------------------------------------
//extern PACKAGE TDBTreeViewFrame *DBTreeViewFrame;
//---------------------------------------------------------------------------
#endif
