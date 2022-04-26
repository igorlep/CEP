//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "DifferentFunctions.h"
#include "RBFunctions.h"
#include "TreeViewF.h"
#include "ContentConfig.h"
#include "GridContentF.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "FIBDataSet"
#pragma link "pFIBDataSet"
#pragma resource "*.dfm"
TGridContentFrame *GridContentFrame;
//---------------------------------------------------------------------------
static int GridContentFrameNumber=1;
//---------------------------------------------------------------------------
//************
// published *
//************
void __fastcall TGridContentFrame::workSourceDataChange(TObject *Sender,TField *Field)
{
  if(gridFrame->workGrid->Showing)
  { if(tvFrame!=NULL && tvFrame->OnFilesBox->Checked==false)
      tvFrame->setPositionInTreeView(tvFrame->findNodeInTreeView(gridFrame->getWorkDataSet()->FieldByName("FILE_ID")->AsString));
    if(contentFrame!=NULL) contentFrame->setContent();
  }
}
//---------------------------------------------------------------------------
//Настройка окна "Содержание"
void __fastcall TGridContentFrame::contentConfigClick(TObject *Sender)
{
TContentConfigForm *CCF;

  try
  {
    CCF=new TContentConfigForm(owner,iniFileName,iniSectionName,gridFrame->getTableTitle(),gridFrame->getFieldsInfoStr(),contentsInfo);
  }
  catch(const Exception &exc)
  { MessageDlg("Ошибка создания объекта TContentConfigForm\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return;
  }
  if(CCF->ShowModal()==mrOk)
  { AnsiString newContentInfo=CCF->getContentsInfo();
    if(newContentInfo==NULL) newContentInfo="";
    contentsInfo=newContentInfo;
    contentFrame->initContentFields(contentsInfo);
    contentFrame->setContent();
  }
  delete CCF;
}
//---------------------------------------------------------------------------
//************
// private *
//************
void __fastcall TGridContentFrame::moveRecordToDelFile(AnsiString id,AnsiString delFileID)
{
FBQuery *fbWQ=fbDB->getFBQ_T2();
TpFIBTransaction *WT=fbWQ->getTransaction();
AnsiString sql;

  if(!delFileID.IsEmpty())
  { sql="update "+gridFrame->getTableName()+" set FILE_ID="+delFileID+" where "+gridFrame->getKeyFieldName()+"="+id;
    WT->StartTransaction();
    if(fbWQ->execQuery("TGridContentFrame::moveRecordToDelFile",sql)==false)
    { WT->Rollback();
      return;
    }
    WT->Commit();
  }
}
//---------------------------------------------------------------------------
void __fastcall TGridContentFrame::delRecordFromRB(AnsiString id)
{
FBQuery *fbWQ=fbDB->getFBQ_T2();
TpFIBTransaction *WT=fbWQ->getTransaction();
AnsiString sql;

  sql="delete from "+gridFrame->getTableName()+" where "+gridFrame->getKeyFieldName()+"="+id;
  WT->StartTransaction();
  if(fbWQ->execQuery("TGridContentFrame::delRecordFromRB",sql)==false)
  { WT->Rollback();
    return;
  }
  WT->Commit();
}
//---------------------------------------------------------------------------
void __fastcall TGridContentFrame::markRecordAsDel(AnsiString id)
{
FBQuery *fbWQ=fbDB->getFBQ_T2();
TpFIBTransaction *WT=fbWQ->getTransaction();
AnsiString sql;

  if(gridFrame->findPozitionFieldForGridFieldName("DEL_FLG") > 0)
  { sql="update "+gridFrame->getTableName()+" set DEL_FLG=1 where "+gridFrame->getKeyFieldName()+"="+id;
    WT->StartTransaction();
    if(fbWQ->execQuery("TGridContentFrame::moveRecordToDelFile",sql)==false)
    { WT->Rollback();
      return;
    }
    WT->Commit();
  }
}
//---------------------------------------------------------------------------

//*********
// public *
//*********
__fastcall TGridContentFrame::TGridContentFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fb_DB,AnsiString tableInf,AnsiString fieldsInf,AnsiString contentsInf,AnsiString footerFieldsInfo)
        : TFrame(Owner),owner(Owner),iniFileName(iniFN),iniSectionName(iniSN),fbDB(fb_DB),contentsInfo(contentsInf)

{
AnsiString name=this->Name+IntToStr(GridContentFrameNumber);
bool rez=Owner->FindComponent(name);

  if(rez==true) GridContentFrameNumber++;
  this->Name=this->Name+IntToStr(GridContentFrameNumber);

  TIniFile *ini;

  tvFrame=NULL;
  addEditRecord1=NULL;
  addEditRecord2=NULL;
  canDelRecord=NULL;

  AnsiString tableInfo=tableInf;
  AnsiString fieldsInfo=fieldsInf;

  try
  {
    gridFrame=new TGridFrame(owner,iniFileName,iniSectionName,fbDB,tableInfo,fieldsInfo,footerFieldsInfo);
  }
  catch(const Exception &exc)
  { MessageDlg("Ошибка создания TGridFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    gridFrame=NULL;
    return;
  }
  gridFrame->initFrame();
  gridFrame->Parent=gridPanel;
  gridFrame->setStatusBar(statusBar);

  ini=new TIniFile(iniFileName);
  int contentHeight=ini->ReadInteger(iniSectionName,"contentPanel_Height",100);
  AnsiString ci=ini->ReadString(iniSectionName,"contentsInfo","");
  delete ini;
  if(!ci.IsEmpty()) contentsInfo=ci;

  if(contentsInfo!=NULL && !contentsInfo.IsEmpty())
  {
    try
    {
      contentFrame=new TContentFrame(owner,gridFrame,contentsInfo);
    }
    catch(const Exception &exc)
    { MessageDlg("Ошибка создания TContentFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
      contentFrame=NULL;
    }
  }
  else contentFrame=NULL;
  if(contentFrame!=NULL)
  { contentFrame->Parent=contentPanel;
    gridFrame->workSource->OnDataChange=workSourceDataChange;
    contentFrame->contentMemo->PopupMenu=contentMenu;
    contentPanel->Height=contentHeight;
  }
  else
  { contentPanel->Visible=false;
    splitter->Visible=false;
  }
}
//---------------------------------------------------------------------------
__fastcall TGridContentFrame::~TGridContentFrame(void)
{
  if(contentFrame!=NULL)
  { TIniFile *ini;
    ini = new TIniFile(iniFileName);
    ini->WriteInteger(iniSectionName,"contentPanel_Height",contentPanel->Height);
    ini->WriteString(iniSectionName,"contentsInfo",contentsInfo);
    delete ini;
    delete contentFrame; contentFrame=NULL;
  }
  if(gridFrame!=NULL){ delete gridFrame; gridFrame=NULL;}
}
//---------------------------------------------------------------------------
void __fastcall TGridContentFrame::loadDS(AnsiString sql)
{
  if(sql!=NULL && !sql.IsEmpty())
  { gridFrame->initSelectSQL(sql);
    gridFrame->loadDS();
  }
}
//---------------------------------------------------------------------------
void __fastcall TGridContentFrame::setSEdit(TEdit *edit)
{
  if(gridFrame!=NULL) gridFrame->setSEdit(edit);
}
//---------------------------------------------------------------------------
void __fastcall TGridContentFrame::setConditionSBox(TJvXPCheckbox *ConditionSBox)
{
  if(gridFrame!=NULL) gridFrame->setConditionSBox(ConditionSBox);
}
//---------------------------------------------------------------------------
void __fastcall TGridContentFrame::setHeader(TJvNavPanelHeader *header)
{
  if(gridFrame!=NULL)
  { gridFrame->setHeader(header);
    gridFrame->setHeaderCaption(gridFrame->getTableTitle());
  }
}
//---------------------------------------------------------------------------
//Удаление записей
void __fastcall TGridContentFrame::delRecordsFromRB(void)
{
TpFIBDataSet *workDS=gridFrame->getWorkDataSet();
TDBGridEh *workGrid=gridFrame->workGrid;
AnsiString str;

  if(workDS->RecordCount==0) return;

  if(workGrid->SelectedRows->Count==0) workGrid->Selection->Rows->CurrentRowSelected=true;
  if(workGrid->SelectedRows->Count==1)
    str="Вы уверены, что хотите удалить текущую запись?";
  else
    str="Вы уверены, что хотите удалить выбранные записи?";
  if(MessageDlg(str,mtConfirmation,TMsgDlgButtons() << mbOK << mbCancel,0)==mrCancel) return;
  for(int i=0; i<workGrid->SelectedRows->Count; ++i)
  { workDS->GotoBookmark((void *)workGrid->SelectedRows->Items[i].c_str());
    AnsiString id=gridFrame->getFieldDataOfSelectedRecord(gridFrame->getKeyFieldName());
    //Если в таблице есть поле FILE_ID
    if(gridFrame->findPozitionFieldForGridFieldName("FILE_ID") > 0)
    { AnsiString delFileID=revisionDelFileRB(fbDB,gridFrame->getTableInfo());
      AnsiString fileID=gridFrame->getFieldDataOfSelectedRecord("FILE_ID");
      if(fileID != delFileID) //просто переносим запись в папку для удаленных записей
        moveRecordToDelFile(id,delFileID);
      else
      { //проверяем, можно ли удалить запись
        if(canDelRecord!=NULL && canDelRecord(this,fbDB,id,str)==true) delRecordFromRB(id);
        else MessageDlg(str,mtWarning,TMsgDlgButtons() << mbOK,0);
      }
    }
    //Если в таблице есть поле DEL_FLG
    else if(gridFrame->findPozitionFieldForGridFieldName("DEL_FLG") > 0)
    { //Помечаем запись на удаление
      AnsiString del_flg=gridFrame->getFieldDataOfSelectedRecord("DEL_FLG");
      if(del_flg.IsEmpty() || del_flg=="0") markRecordAsDel(id);
      else
      { //проверяем, можно ли удалить запись
        if(canDelRecord!=NULL && canDelRecord(this,fbDB,id,str)==true) delRecordFromRB(id);
        else MessageDlg(str,mtWarning,TMsgDlgButtons() << mbOK,0);
      }
    }
    else
    { //проверяем, можно ли удалить запись
      if(canDelRecord!=NULL && canDelRecord(this,fbDB,id,str)==true)
        delRecordFromRB(id);
      else MessageDlg(str,mtWarning,TMsgDlgButtons() << mbOK,0);
    }
  }
  gridFrame->loadDS();
}
//---------------------------------------------------------------------------
//Переузка gridFrame с новым значением fieldsInfo
void __fastcall TGridContentFrame::reLoadWorkGrid(AnsiString newFieldsInfo)
{
  gridFrame->reLoadWorkGrid(newFieldsInfo);
}
//---------------------------------------------------------------------------
AnsiString __fastcall TGridContentFrame::getTableInfo(void)
{
  return gridFrame->getTableInfo();
}
//---------------------------------------------------------------------------
AnsiString __fastcall TGridContentFrame::getTableTitle(void)
{
  return gridFrame->getTableTitle();
}
//---------------------------------------------------------------------------
AnsiString __fastcall TGridContentFrame::getFieldsInfo(void)
{
  return gridFrame->getFieldsInfoStr();
}
//---------------------------------------------------------------------------

