//---------------------------------------------------------------------------


#ifndef GridFH
#define GridFH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <DB.hpp>
#include <Grids.hpp>
#include "DBClasses.h"
#include "JvComponent.hpp"
#include "JvExControls.hpp"
#include "JvNavigationPane.hpp"
#include <ImgList.hpp>
#include "FIBDataSet.hpp"
#include "pFIBDataSet.hpp"
#include "DBGridEh.hpp"
#pragma link "JvNavigationPane"
#include "JvXPCheckCtrls.hpp"
//#include "TreeViewF.h"
//---------------------------------------------------------------------------
enum OrderString{forWorkDS,forSearchDS};
//---------------------------------------------------------------------------
struct FieldInfo
{
private:
  AnsiString sqlFieldName;  //наименование пол€ в SQL запросе
  AnsiString gridFieldName; //наименование пол€ в Grid-e
  AnsiString gridFieldTitle;//заголовок пол€ в Grid-e
  AnsiString align;         //расположение в столбце Grid-а (Left, Center, Right)
  bool readOnly;            //‘лаг - “олько чтение
  bool gridViewAble;        //‘лаг - ќтображать в Grid-е
  bool titleButton;         //‘лаг - «аголовок колонки Grid-а €вл€етс€ кнопкой
  bool searchAble;          //флаг поиска по модели
  AnsiString formatString;  //формат вывода в Grid-е
public:
  void init(AnsiString fieldInfo);
  AnsiString getSQLFieldName(void){ return sqlFieldName; }
  AnsiString getGridFieldName(void){ return gridFieldName; }
  AnsiString getGridFieldTitle(void){ return gridFieldTitle; }
  AnsiString getAlign(void){ return align; }
  bool isReadOnly(void){ return readOnly; }
  bool isGridViewAble(void){ return gridViewAble; }
  bool isTitleButton(void){ return titleButton; }
  bool isSearchAble(void){ return searchAble; }
  AnsiString getFormatString(void){ return formatString; }
};
//---------------------------------------------------------------------------
struct FooterFieldInfo
{
private:
  AnsiString fieldName;
  AnsiString valueType;
  AnsiString formatString;
  AnsiString value;
  AnsiString align;
public:
  void init(AnsiString fieldInfo);
  AnsiString getFieldName(void){ return fieldName; }
  AnsiString getValueType(void){ return valueType; }
  AnsiString getFormatString(void){ return formatString; }
  AnsiString getValue(void){ return value; }
  AnsiString getAlign(void){ return align; }
};
//---------------------------------------------------------------------------
class TGridFrame : public TFrame
{
__published:	// IDE-managed Components
  TDBGridEh *workGrid;
  TDataSource *workSource;
  TImageList *del_flg_ImageList;
  TImageList *lock_flg_ImageList;

  void __fastcall workGridSortMarkingChanged(TObject *Sender);
  void __fastcall workGridKeyPress(TObject *Sender, char &Key);
  void __fastcall workGridEnter(TObject *Sender);
  void __fastcall workGridExit(TObject *Sender);
  void __fastcall workGridColEnter(TObject *Sender);
  void __fastcall dsBeforeClose(TDataSet *DataSet);
  void __fastcall dsAfterOpen(TDataSet *DataSet);
  void __fastcall dsAfterInsert(TDataSet *DataSet);
  void __fastcall dsAfterPost(TDataSet *DataSet);
  void __fastcall columnsConfigClick(TObject *Sender);
private:	// User declarations
  TComponent* owner;
  AnsiString iniFileName;
  AnsiString iniSectionName;
  AnsiString tableInfo;
  AnsiString fieldsInfoStr;
  AnsiString footerFieldsInfoStr;
  AnsiString tableName,tableTitle,keyFieldName,viewFieldName;

  FieldInfo **fieldsInfo;       //массив структур с описанием полей таблицы
  int fieldsQuantity;           //количество полей таблицы
  int gridViewFieldsQuantity;   //количество полей, отображаемых в Grid
  int editAbleFieldsQuantity;   //количество полей, которые могут редактироватьс€
  int searchAbleFieldsQuantity; //количество полей, по которым можно осуществл€ть поиск по модели

  FooterFieldInfo **footerFieldsInfo;  //массив структур с описанием Footer полей
  int footerFieldsQuantity;            //количество Footer полей

  TStatusBar *statusBar;

  FBDatabase *fbDatabase;

  FBDataSet *fbDS;
  TpFIBDataSet *workDS;
  AnsiString selectSQL;
  AnsiString selectedRecordID;

  class TJvNavPanelHeader *gcHeader;

  //ѕоиск по модели
  TEdit *SEdit;
  TJvXPCheckbox *conditionSBox;
  FBDataSet *fbSDS;
  TpFIBDataSet *searchDS;
  AnsiString searchSQL;

  bool columnsConfigFieldEditInGridVisible; //флаг - показывать колонку EditInGrid в форме конфигурации колонок
  bool columnsConfigFieldViewInGridVisible; //флаг - показывать колонку ViewInGrid в форме конфигурации колонок
  bool columnsConfigFieldTitleButtonVisible; //флаг - показывать колонку TitleButton в форме конфигурации колонок
  bool columnsConfigFieldSearchAbleVisible; //флаг - показывать колонку SearchAble в форме конфигурации колонок
  void __fastcall initSQLs(void);
  bool __fastcall locateDS(void);
  AnsiString __fastcall assemblySearchSQLString(void);
public:		// User declarations
  __fastcall TGridFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,AnsiString tableInfo,AnsiString fieldsInfo,AnsiString footerFieldsInfo="");
  __fastcall ~TGridFrame(void);
  void __fastcall initWorkGrid(AnsiString fieldsInf);
  void __fastcall initFooterStr(AnsiString footerFieldsInf);
  void __fastcall reLoadWorkGrid(AnsiString newFieldsInfo);
  void __fastcall initFrame(void);
  void __fastcall initSelectSQL(AnsiString sql);
  void __fastcall loadDS(void);
  void __fastcall initSearchSQL(AnsiString sql);
  void __fastcall loadSearchingDS(void);
  AnsiString __fastcall formatSortStringDS(OrderString forWho);
  void __fastcall forwardSearch(void);
  void __fastcall backwardSearch(void);
  int __fastcall findPozitionFieldForSQLFieldName(AnsiString sqlFieldName); //ищет положение пол€ по его sql наименованию
  int __fastcall findPozitionFieldForGridFieldName(AnsiString gridFieldName); //ищет положение пол€ по его наименованию в сетке
  int __fastcall findPozitionColumnInGrid(AnsiString gridFieldName); //ищет положение пол€ в сетке по его наименованию
  FieldInfo * getFieldInfoByGridFieldName(AnsiString FieldName);
  void __fastcall setHeaderCaption(AnsiString caption);
  AnsiString __fastcall getFieldDataOfSelectedRecord(AnsiString fieldName);
  AnsiString __fastcall getFieldDataOfSelectedRecord(AnsiString refTableName,AnsiString keyFieldName,AnsiString refFieldName,AnsiString refFieldValue,AnsiString retFieldName);
  AnsiString __fastcall getSelectedRecordID(void);
  AnsiString __fastcall getSEditText(void);
  bool __fastcall getConditionChecked(void);
  bool __fastcall isSearchDSActive(void);
  bool __fastcall isReadOnly(AnsiString fieldName);
  bool __fastcall isGridViewAble(AnsiString fieldName);
  bool __fastcall isTitleButton(AnsiString fieldName);
  bool __fastcall isSearchAble(AnsiString fieldName);

  TpFIBDataSet * __fastcall getWorkDataSet(void){ return workDS; }
  FieldInfo ** __fastcall getFieldsInfo(void){ return fieldsInfo;}
  AnsiString __fastcall getTableInfo(void){ return tableInfo;}
  AnsiString __fastcall getTableName(void){ return tableName;}
  AnsiString __fastcall getTableTitle(void){ return tableTitle;}
  AnsiString __fastcall getKeyFieldName(void){ return keyFieldName;}
  AnsiString __fastcall getViewFieldName(void){ return viewFieldName;}
  AnsiString __fastcall getFieldsInfoStr(void){ return fieldsInfoStr;}
  int __fastcall getFieldsQuantity(void){ return fieldsQuantity;}
  int __fastcall getEditAbleFieldsQuantity(void){ return editAbleFieldsQuantity;}
  int __fastcall getSearchAbleFieldsQuantity(void){ return searchAbleFieldsQuantity;}
  //
  TEdit * __fastcall getSEdit(void){ return SEdit; }
  TJvXPCheckbox * __fastcall getConditionSBox(void){ return conditionSBox; }
  AnsiString __fastcall getSelectSQL(void){ return selectSQL; }
  AnsiString __fastcall getSearchSQL(void){ return searchSQL; }
  TJvNavPanelHeader * __fastcall getHeader(void){ return gcHeader; }
  TStatusBar * __fastcall getStatusBar(void){ return statusBar; }
  //
  void __fastcall setSEdit(TEdit *edit){ SEdit=edit; }
  void __fastcall setConditionSBox(TJvXPCheckbox *ConditionSBox){ conditionSBox=ConditionSBox; }
  void __fastcall setSelectedRecordID(AnsiString id){ selectedRecordID=id; }
  void __fastcall setSelectSQL(AnsiString sql){ selectSQL=sql; }
  void __fastcall setSearchSQL(AnsiString sql){ searchSQL=sql; }
  void __fastcall setHeader(TJvNavPanelHeader *header){ gcHeader=header; }
  void __fastcall setStatusBar(TStatusBar *sb){ statusBar=sb; }
  void __fastcall setColumnsConfigFieldEditInGridVisible(bool ccf){ columnsConfigFieldEditInGridVisible=ccf; }
  void __fastcall setColumnsConfigFieldViewInGridVisible(bool ccf){ columnsConfigFieldViewInGridVisible=ccf; }
  void __fastcall setColumnsConfigFieldTitleButtonVisible(bool ccf){ columnsConfigFieldTitleButtonVisible=ccf; }
  void __fastcall setColumnsConfigFieldSearchAbleVisible(bool ccf){ columnsConfigFieldSearchAbleVisible=ccf; }

  typedef void __fastcall (*pfAA)(TGridFrame *nf);
  pfAA additionalAdjustmentsForGrid;
//  pfAA getAdditionalAdjustmentsForGrid(void){ return additionalAdjustmentsForGrid; }
//  void __fastcall setAdditionalAdjustmentsForGrid(pfAA AA){ additionalAdjustmentsForGrid=AA; }

};
//---------------------------------------------------------------------------
//extern PACKAGE TGridFrame *GridFrame;
//---------------------------------------------------------------------------
#endif
