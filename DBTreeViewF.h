//---------------------------------------------------------------------------
//Класс для отображения иерархических структур в виде дерева с пометками (CheckBox-ами)
// Дерево формируется на основе таблицы tableName из базы данных fbDB
// Значимыми для формирования дерева являются 2 поля таблицы:
//  keyFieldName - ключевое поле. Содержит код записи, определяющий ее положение в структуре
//   Строка с кодом записи должна содержать коды (номера) всех родительских элементов дерева,
//   начиная с первого уровня, разделенные символом '.', и номер самого элемента
//   Пример: 01.02.05, где 01 и 02 - коды родительских элементов, 05- код самого элемента
//  viewFieldName - поле с описанием, отображаемым в дереве
//
// В конструктор класса передаются следующие параметры:
//  Owner - компонент, на панели которого отображается фрейм с деревом,
//  Parent - управляющий элемент, в рамках которого будет выведен фрейм с деревом,
//  iniFN - полное имя файла инициализации,
//  fbdb - указатель на объект с базой данных,
//  structTInfo - информационная строка с описанием параметров таблицы
//  Формат строки structTInfo:
//  наименование_таблицы,
//  заголовок_таблицы,
//  наименование_поля_с_кодом_записи,
//  наименование_поля_с_описание_записи,
//  максимальное_количество_значащих_цифр_в_номере_элемента_дерева
//  Примечание: 1.Заголовок_таблицы будет выведен в DBTreeViewHeader
//              2.Если количестко значащих цифр фактического номера элемента меньше
//                заявленного,то недостающие позиции слева должны заполняться нулями.
//
// В файле инициализации iniFileName сохраняются код текущего элемента дерева и коды
// всех распахнутых узлов.
// Для сохранения и восствновления содов помеченных элементов служат две функции:
//  getCheckedItems(void) - возвращает строку с кодами помеченных элементов.
//   Строка имее следующий формат:
//    код_элемента1,стиль_пометки1;код_элемента2,стиль_пометки2;...
//  setCheckForItems(AnsiString checkedItems) - восстанавливает помеченные элементы
//   на основании данных, содержащихся в строке checkedItems

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
// Данные элемента дерева
struct DBTreeData
{
  AnsiString ID; //идентификатор записи из таблицы tableName
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
  int levelExponent; //максимальное количество значащих цифр в номере уровня иерархии
  AnsiString levelTemplate; //строка-шаблон, содержащая символ '_', длиной в levelExponent
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
