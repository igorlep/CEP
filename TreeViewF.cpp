//---------------------------------------------------------------------------
//Фрейм - Дерево разделов справочника

#include <vcl.h>
#pragma hdrstop
#include "DBClasses.h"
#include "RBFunctions.h"
#include "GridF.h"
#include "DifferentFunctions.h"
#include "PLACEMNT.HPP"
#include "TreeViewF.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Placemnt"
#pragma resource "*.dfm"
//TTreeViewFrame *TreeViewFrame;
//---------------------------------------------------------------------------
//************
// published *
//************
void __fastcall TTreeViewFrame::FrameResize(TObject *Sender)
{
  treeView->Repaint();
}
//---------------------------------------------------------------------------
void __fastcall TTreeViewFrame::treeViewChange(TObject *Sender,
      TTreeNode *Node)
{
  if(treeView->Visible==false) return;
  if(treeView->Selected->Data!=NULL)
  { currentFileID=(static_cast<RBTreeData*>(treeView->Selected->Data))->ID;
    if(gridFrame!=NULL)
    { if(OnFilesBox->Checked==true) gridFrame->setHeaderCaption("Папка - \""+treeView->Selected->Text+"\"");
      else gridFrame->setHeaderCaption("Справочник \""+getSubstring(rbTableInfo,',',2)+"\"");
      if(OnFilesBox->Checked==true && formSelectSQL!=NULL)
      { gridFrame->initSelectSQL(formSelectSQL(currentFileID));
        gridFrame->loadDS();
      }
      if(OnFilesBox->Checked==true && formSearchSQL!=NULL && gridFrame!=NULL)
      { AnsiString fileID=getFileID();
        AnsiString fieldName=gridFrame->workGrid->Columns->Items[gridFrame->workGrid->Col-1]->FieldName;
        AnsiString editText=gridFrame->getSEditText();
        bool conditionChecked=gridFrame->getConditionChecked();
        gridFrame->initSearchSQL(formSearchSQL(fileID,fieldName,editText,conditionChecked));
        gridFrame->loadSearchingDS();
      }
    }
  }
}
//---------------------------------------------------------------------------
//Добавление дочерней папки
void __fastcall TTreeViewFrame::AddFileButtonClick(TObject *Sender)
{
TTreeNode *node=treeView->Selected;
AnsiString fileID=currentFileID;
AnsiString sql;
FBQuery * fbWQ=fbDatabase->getFBQ_T2();
AnsiString newID;

  fbWQ->getTransaction()->StartTransaction();
  newID=getNewKeyValue(fbDatabase,rbTableName);
  if(newID.IsEmpty())
  { fbWQ->getTransaction()->Rollback();
    return;
  }
  sql="insert into "+rbTableName+"("+keyFieldName+",FILE_ID,REC_TYPE,"+viewFieldName+") values(";
  sql+=newID+",";
  sql+=currentFileID+",0,'')";
  if(fbWQ->execQuery("TRB_Tree_Frame::TV_AddFileClick",sql)==false)
  { fbWQ->getTransaction()->Rollback();
    return;
  }
  fbWQ->getTransaction()->Commit();
  node=treeView->Items->AddChild(node,"");
  node->ImageIndex=0;
  node->SelectedIndex=1;
  RBTreeData *rbTD= new RBTreeData();
  rbTD->ID=newID;
  node->Data=rbTD;
  treeView->Selected=node;
  node->EditText();
}
//---------------------------------------------------------------------------
//Пепеименование текущей папки
void __fastcall TTreeViewFrame::TV_RenameFileClick(TObject *Sender)
{
AnsiString rootFileID=revisionRootFileRB(fbDatabase,rbTableInfo);
AnsiString delFileID=revisionDelFileRB(fbDatabase,rbTableInfo);

  if(currentFileID == rootFileID || currentFileID == delFileID)
  { MessageDlg("Нельзя изменить наименование этой парки",mtWarning,TMsgDlgButtons() << mbOK,0);
    return;
  }
  treeView->Selected->EditText();
}
//---------------------------------------------------------------------------
//Завершение редактирования наименования папки
void __fastcall TTreeViewFrame::treeViewEdited(TObject *Sender,
      TTreeNode *Node, AnsiString &S)
{
AnsiString sql="update "+rbTableName+" set "+viewFieldName+"='"+S+"' where "+keyFieldName+"="+currentFileID;
FBQuery * fbWQ=fbDatabase->getFBQ_T2();

  fbWQ->execQuery("TTreeViewFrame::treeViewEdited",sql);
}
//---------------------------------------------------------------------------
//Удаление текущей папки
void __fastcall TTreeViewFrame::DelFileButtonClick(TObject *Sender)
{
AnsiString sql;
FBQuery * fbRQ=fbDatabase->getFBQ_T1();
AnsiString rootFileID=revisionRootFileRB(fbDatabase,rbTableInfo);
AnsiString delFileID=revisionDelFileRB(fbDatabase,rbTableInfo);

  //Корневую папку и папку для удаленных записей удалять нельзя
  if(currentFileID==rootFileID || currentFileID==delFileID)
  { if(MessageDlg("Выбранную папку удалить нельзя!",mtInformation,TMsgDlgButtons() << mbOK << mbCancel,0)!=mrOk) return;
    return;
  }
  //Проверяем, нет ли в удаляемой папке записей
  sql="select count("+keyFieldName+") as COUNT_ID from "+rbTableName+" where FILE_ID="+currentFileID;
  if(fbRQ->execQuery("TTreeViewFrame::DelFileButtonClick",sql)==false) return;
  if(fbRQ->getQuery()->FieldByName("COUNT_ID")->AsInteger > 0)
  { if(MessageDlg("Нельзя удалить папку, содержащую записи",mtInformation,TMsgDlgButtons() << mbOK << mbCancel,0)!=mrOk) return;
    return;
  }

  if(MessageDlg("Вы уверены, что хотите удалить текущую папку?",mtConfirmation,TMsgDlgButtons() << mbOK << mbCancel,0)!=mrOk) return;

  FBQuery * fbWQ=fbDatabase->getFBQ_T2();
  sql="delete from "+rbTableName+" where "+keyFieldName+"="+currentFileID;
  if(fbWQ->execQuery("TRB_Tree_Frame::TV_DelFileClick",sql)==false) return;
  treeView->Selected->Delete();
}
//---------------------------------------------------------------------------
void __fastcall TTreeViewFrame::OnFilesBoxClick(TObject *Sender)
{
  if(gridFrame!=NULL)
  { AnsiString fID="";
    if(OnFilesBox->Checked==true)
    { fID=currentFileID;
      gridFrame->setHeaderCaption("Папка - \""+treeView->Selected->Text+"\"");
    }
    else gridFrame->setHeaderCaption("Справочник \""+getSubstring(rbTableInfo,',',2)+"\"");
    if(formSelectSQL!=NULL)
      gridFrame->initSelectSQL(formSelectSQL(fID));
    gridFrame->loadDS();

    if(formSearchSQL!=NULL)
    { AnsiString fileID=getFileID();
      AnsiString fieldName=gridFrame->workGrid->Columns->Items[gridFrame->workGrid->Col-1]->FieldName;
      AnsiString editText=gridFrame->getSEditText();
      bool conditionChecked=gridFrame->getConditionChecked();
      gridFrame->initSearchSQL(formSearchSQL(fileID,fieldName,editText,conditionChecked));
    }
    gridFrame->loadSearchingDS();
    if( gridFrame->Showing) gridFrame->workGrid->SetFocus();
  }
}
//---------------------------------------------------------------------------
void __fastcall TTreeViewFrame::treeViewMouseUp(TObject *Sender,TMouseButton Button, TShiftState Shift, int X, int Y)
{
  if(treeView->IsEditing()==false && gridFrame!=NULL && gridFrame->Showing)
    gridFrame->workGrid->SetFocus();
}
//---------------------------------------------------------------------------
//**********
// private *
//**********
void __fastcall TTreeViewFrame::buildNode(TTreeNode* ParentNode, AnsiString ParentID)
{
AnsiString sql;
FBDataSet *fbds;
TpFIBDataSet *ds;

  if((fbds=newFBDataSet(this,"TRB_Tree_Frame::buildNode",fbDatabase->getDatabase(),true))==NULL) return;
  ds=fbds->getDataSet();
  if(ParentID.IsEmpty())
    sql="select "+keyFieldName+","+viewFieldName+" from "+rbTableName+" where FILE_ID is null and REC_TYPE=0 order by "+viewFieldName;
  else
    sql="select "+keyFieldName+","+viewFieldName+" from "+rbTableName+" where FILE_ID="+ParentID+" and REC_TYPE=0 order by "+viewFieldName;
  if(fbds->execSelectSQL("TTreeViewFrame::buildNode",sql)==false){ delete fbds; return;}

  ds->First();
  while(!ds->Eof)
  { TTreeNode *node;
    if(ParentNode==NULL) node=treeView->Items->Add(NULL,ds->FieldByName(viewFieldName)->AsString);
    else node=treeView->Items->AddChild(ParentNode,ds->FieldByName(viewFieldName)->AsString);
    node->ImageIndex=0;
    node->SelectedIndex=1;
    RBTreeData *rbTD=new RBTreeData();
    rbTD->ID=ds->FieldByName(keyFieldName)->AsString;
    node->Data=rbTD;
    buildNode(node,rbTD->ID);
    ds->Next();
  }
  if(ds!=NULL){ delete ds; ds=NULL; }
}
//---------------------------------------------------------------------------
void __fastcall TTreeViewFrame::buildTree(AnsiString rootID)
{
  treeView->Visible=false;
  //Удаляем текущее дерево
  removeTree();
  if(!rootID.IsEmpty())
  { //Ищем родительскую папку
    FBQuery *fbRQ=fbDatabase->getFBQ_T1();
    AnsiString parentID="";
    AnsiString sql;
    sql="select FILE_ID from "+rbTableName+" where "+keyFieldName+"="+rootID;
    if(fbRQ->execQuery("TTreeViewFrame::buildTree",sql)==false) return;
    if(!fbRQ->getQuery()->FieldByName("FILE_ID")->IsNull) parentID=fbRQ->getQuery()->FieldByName("FILE_ID")->AsString;
    fbRQ->getQuery()->Close();
    //Строим новое дерево
    buildNode(NULL,parentID);
    //Раскрываем все узлы
    treeView->FullExpand();
    //Встаем на текущий элемент
    if(currentFileID.IsEmpty())
    { treeView->Selected=treeView->Items->Item[0];
      currentFileID=static_cast<RBTreeData*>(treeView->Items->Item[0]->Data)->ID;
    }
    else treeView->Selected=findNodeInTreeView(currentFileID);
  }
  treeView->Visible=true;
  OnFilesBox->OnClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TTreeViewFrame::removeTree(void)
{
RBTreeData* rbTD;

  for(int i=0; i<treeView->Items->Count; i++)
  { rbTD=(static_cast<RBTreeData*>(treeView->Items->Item[i]->Data));
    if(rbTD != NULL) delete rbTD;
    treeView->Items->Item[i]->Data=NULL;
  }
  treeView->Items->Clear();
}
//---------------------------------------------------------------------------
//*********
// public *
//*********
__fastcall TTreeViewFrame::TTreeViewFrame(TComponent* Owner,AnsiString iniFN,\
AnsiString iniSN,FBDatabase *fbDB,AnsiString rbTI,AnsiString rootFID)
        : TFrame(Owner),iniSectionName(iniSN),iniFileName(iniFN),fbDatabase(fbDB)\
,rbTableInfo(rbTI)
{
  gridFrame=NULL;
//формат строки rbTableInfo:
// 1 - имя_таблицы,
// 2 - заголовок_таблицы,
// 3 - наименование_ключевого_поля (ID),
// 4 - наименование_поля_описания ,
// 5 - заголовок_корневой_папки,
// 6 - заголовок_папки_для_удаленных_записей
  rbTableName=getSubstring(rbTableInfo,',',1);
  keyFieldName=getSubstring(rbTableInfo,',',3);
  viewFieldName=getSubstring(rbTableInfo,',',4);

  rootFileID="";
  if(!rootFID.IsEmpty()) rootFileID=rootFID;
  else rootFileID=revisionRootFileRB(fbDatabase,rbTableInfo);
  if(rootFileID.IsEmpty()) buttonsPanel->Visible=false;
  delFileID=revisionDelFileRB(fbDatabase,rbTableInfo);
  formSelectSQL=NULL;
  formSearchSQL=NULL;
}
//---------------------------------------------------------------------------
__fastcall TTreeViewFrame::~TTreeViewFrame(void)
{
AnsiString s=iniSectionName;//+"_"+rbTableName;

  TIniFile *ini = new TIniFile(iniFileName);
  ini->WriteString(s,"currentFileID",currentFileID);
  delete ini;
  removeTree();
}
//---------------------------------------------------------------------------
void __fastcall TTreeViewFrame::initTreeViewFrame(void)
{
AnsiString s=iniSectionName;//+"_"+rbTableName;

  TIniFile *ini = new TIniFile(iniFileName);
  currentFileID=ini->ReadString(s,"currentFileID","");
  delete ini;

  buildTree(rootFileID);
}
//---------------------------------------------------------------------------
void __fastcall TTreeViewFrame::setPositionInTreeView(TTreeNode *node)
{
  if(node==NULL) return;
  treeView->Selected=node;
}
//---------------------------------------------------------------------------
TTreeNode * __fastcall TTreeViewFrame::findNodeInTreeView(AnsiString id)
{
int i;
RBTreeData* rbTD;

  if(id.IsEmpty()) return NULL;
  for(i=0; i < treeView->Items->Count; i++)
  { rbTD=static_cast<RBTreeData*>(treeView->Items->Item[i]->Data);
    if(rbTD->ID==id) return treeView->Items->Item[i];
  }
  return NULL;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TTreeViewFrame::getFileID(void)
{
  if(OnFilesBox->Checked) return currentFileID;
  else return "";
}
//---------------------------------------------------------------------------

