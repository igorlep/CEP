//---------------------------------------------------------------------------


#ifndef GridContentFH
#define GridContentFH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "GridF.h"
#include "ContentF.h"
#include <Menus.hpp>
#include "FIBDataSet.hpp"
#include "pFIBDataSet.hpp"
#include <DB.hpp>
#include <ImgList.hpp>
#include "JvXPCheckCtrls.hpp"
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TGridContentFrame : public TFrame
{
__published:	// IDE-managed Components
  TStatusBar *statusBar;
  TPanel *workPanel;
  TPanel *gridPanel;
  TSplitter *splitter;
  TPanel *contentPanel;
  TPopupMenu *contentMenu;
  TMenuItem *contentConfig;
  void __fastcall workSourceDataChange(TObject *Sender,TField *Field);
  void __fastcall contentConfigClick(TObject *Sender);
private:	// User declarations
  TComponent* owner;
  AnsiString iniFileName;
  AnsiString iniSectionName;
  FBDatabase *fbDB;
  AnsiString contentsInfo;

  TGridFrame *gridFrame;
  TContentFrame *contentFrame;
  class TTreeViewFrame *tvFrame; //указатель на фрейм, отображающий дерео папок справочника

  void __fastcall moveRecordToDelFile(AnsiString id,AnsiString delFileID);
  void __fastcall delRecordFromRB(AnsiString id);
  void __fastcall markRecordAsDel(AnsiString id);
public:		// User declarations
  __fastcall TGridContentFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,AnsiString tableInf,AnsiString fieldsInfo,AnsiString contentsInf,AnsiString footerFieldsInfo="");
  __fastcall ~TGridContentFrame(void);
  void __fastcall loadDS(AnsiString sql);
  void __fastcall setSEdit(TEdit *edit);
  void __fastcall setConditionSBox(TJvXPCheckbox *ConditionSBox);
  void __fastcall setHeader(TJvNavPanelHeader *header);
  void __fastcall delRecordsFromRB(void);
  void __fastcall reLoadWorkGrid(AnsiString newFieldsInfo);
  AnsiString __fastcall getTableInfo(void);
  AnsiString __fastcall getTableTitle(void);
  AnsiString __fastcall getFieldsInfo(void);

  TGridFrame * __fastcall getGridFrame(void){ return gridFrame; }
  TContentFrame * __fastcall getContentFrame(void){ return contentFrame; }
  AnsiString __fastcall getContentsInfo(void){  return contentsInfo; }
  void __fastcall setTVFrame(TTreeViewFrame *TVFrame){ tvFrame=TVFrame; }
  typedef void __fastcall (__closure *pfGridMouseDown)(TObject *Sender,TMouseButton Button,TShiftState Shift,int X,int Y);
  typedef void __fastcall (__closure *pfSourceDataChange)(TObject *Sender,TField *Field);
  typedef void __fastcall (*pfAddAdjusments)(TGridFrame *nf);

  //Функии, определяемые во внешнем приложении и инициируемые при создании фрейма
  // - Добавление (редактирование) записи
  typedef int __fastcall (*pfAERecord1)(TComponent* Owner,AnsiString iniFN,FBDatabase *fbDB,AnsiString &id);
  pfAERecord1 addEditRecord1;
  typedef int __fastcall (*pfAERecord2)(TComponent* Owner,AnsiString iniFN,FBDatabase *fbDB,AnsiString &id,AnsiString parentID);
  pfAERecord2 addEditRecord2;
  // - Функция проверки возможности удаления записи
  typedef bool __fastcall (*pfCDR)(TComponent* owner,FBDatabase *fbDB,AnsiString id,AnsiString &str);
  pfCDR canDelRecord;
};
//---------------------------------------------------------------------------
extern PACKAGE TGridContentFrame *GridContentFrame;
//---------------------------------------------------------------------------
#endif
