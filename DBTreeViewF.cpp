//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <inifiles.hpp>

#include "DifferentFunctions.h"
#include "DBTreeViewF.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ChTreeView"
#pragma link "JvComponent"
#pragma link "JvExControls"
#pragma link "JvNavigationPane"
#pragma resource "*.dfm"
//TDBTreeViewFrame *DBTreeViewFrame;
//---------------------------------------------------------------------------
//**********
//* public *
//**********
__fastcall TDBTreeViewFrame::TDBTreeViewFrame(TComponent* Owner,TWinControl* parent,AnsiString iniFN,FBDatabase *fbdb,AnsiString structTInfo)
        : TFrame(Owner),iniFileName(iniFN),fbDB(fbdb),structureTableInfo(structTInfo)
{
  ownerName=Owner->Name;
  Parent=parent;
  tableName=getSubstring(structureTableInfo,',',1);
  tableTitle=getSubstring(structureTableInfo,',',2);
  keyFieldName=getSubstring(structureTableInfo,',',3);
  viewFieldName=getSubstring(structureTableInfo,',',4);
  levelExponent=StrToInt(getSubstring(structureTableInfo,',',5));
  levelTemplate="";
  for(int i=0; i<levelExponent; i++) levelTemplate+="_";
  DBTreeViewHeader->Caption=tableTitle;
  changeFLG=false;
}
//---------------------------------------------------------------------------
__fastcall TDBTreeViewFrame::~TDBTreeViewFrame(void)
{
AnsiString s=ownerName+"_DBTreeViewFrame_"+tableName;

  TIniFile *ini = new TIniFile(iniFileName);
  ini->WriteString(s,"currentItemID",currentItemID);
  //Сохраняем коды распахнутых узлов
  AnsiString expandedItems="";
  for(int i=0; i<dbTreeView->Items->Count; i++)
  { if(dbTreeView->Items->Item[i]->Expanded)
    { AnsiString expItemCod=(static_cast<DBTreeData*>(dbTreeView->Items->Item[i]->Data))->ID;
      if(!expandedItems.IsEmpty()) expandedItems+=",";
      expandedItems+=expItemCod;
    }
  }
  ini->WriteString(s,"expandedItems",expandedItems);
  delete ini;
  removeTree();
}
//---------------------------------------------------------------------------
void __fastcall TDBTreeViewFrame::initTreeViewFrame(void)
{
AnsiString s=ownerName+"_DBTreeViewFrame_"+tableName;

  TIniFile *ini = new TIniFile(iniFileName);
  currentItemID=ini->ReadString(s,"currentItemID","");
  AnsiString expandedItems=ini->ReadString(s,"expandedItems","");
  delete ini;
  buildTree();
  //Восстанавливаем картину распахнутых узлов
  if(!expandedItems.IsEmpty())
  { int i=1;
    AnsiString itemCod;
    while(!(itemCod=getSubstring(expandedItems,',',i)).IsEmpty() && itemCod!="0")
    { TTreeNode *tn=findNodeInTreeView(itemCod);
      if(tn!=NULL) tn->Expand(false);
      i++;
    }
  }
}
//---------------------------------------------------------------------------
TTreeNode * __fastcall TDBTreeViewFrame::findNodeInTreeView(AnsiString id)
{
DBTreeData* dbTD;

  if(id.IsEmpty()) return NULL;
  for(int i=0; i < dbTreeView->Items->Count; i++)
  { dbTD=static_cast<DBTreeData*>(dbTreeView->Items->Item[i]->Data);
    if(dbTD->ID==id) return dbTreeView->Items->Item[i];
  }
  return NULL;
}
//---------------------------------------------------------------------------
void __fastcall TDBTreeViewFrame::setPositionInTreeView(TTreeNode *node)
{
  if(node==NULL) return;
  dbTreeView->Selected=node;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TDBTreeViewFrame::getCheckedItems(void)
{
AnsiString checkedItems="";

  for(int i=0; i<dbTreeView->Items->Count; i++)
  { CheckStyle chStyle=dbTreeView->getCheckStyleNode(dbTreeView->Items->Item[i]);
    if(chStyle==csChecked || chStyle==csCheckedgray)
    { AnsiString itemCod=static_cast<DBTreeData*>(dbTreeView->Items->Item[i]->Data)->ID;
      if(!checkedItems.IsEmpty()) checkedItems+=";";
      checkedItems+=itemCod+","+IntToStr(int(chStyle));

    }
  }
  return checkedItems;
}
//---------------------------------------------------------------------------
void __fastcall TDBTreeViewFrame::setCheckForItems(AnsiString checkedItems)
{
  if(checkedItems.IsEmpty()) return;
  dbTreeView->Visible=false;
  int i=1;
  AnsiString subStr,checkItem;
  while(!(subStr=getSubstring(checkedItems,';',i)).IsEmpty() && subStr!="0")
  { checkItem=getSubstring(subStr,',',1);
    CheckStyle chStyle=CheckStyle(StrToInt(getSubstring(subStr,',',2)));
    TTreeNode *tn=findNodeInTreeView(checkItem);
    dbTreeView->setCheckStyleNode(tn,chStyle);
    i++;
  }
  dbTreeView->Visible=true;
}
//---------------------------------------------------------------------------

//***********
//* private *
//***********
void __fastcall TDBTreeViewFrame::buildNode(TTreeNode* ParentNode, AnsiString ParentID)
{
AnsiString sql;
FBDataSet *fbds;
TpFIBDataSet *ds;

  if((fbds=newFBDataSet(this,"TDBTreeViewFrame::buildNode",fbDB->getDatabase(),true))==NULL) return;
  ds=fbds->getDataSet();
  if(!ParentID.IsEmpty())
    sql="select "+keyFieldName+","+viewFieldName+" from "+tableName+" where "+keyFieldName+" like '"+ParentID+"."+levelTemplate+"'";
  else
    sql="select "+keyFieldName+","+viewFieldName+" from "+tableName+" where "+keyFieldName+" like '"+levelTemplate+"'";
  sql+=" order by "+keyFieldName;
  if(fbds->execSelectSQL("TDBTreeViewFrame::buildNode",sql)==false){ delete fbds; return;}

  ds->Last();
  int nc=ds->RecordCount;
  ds->First();
  if(nc > 0)
  { while(!ds->Eof)
    { TTreeNode *node=NULL;
      if(ParentNode==NULL)
      { AnsiString s=ds->FieldByName(viewFieldName)->AsString;
        try
        {
          node=dbTreeView->Items->Add(NULL,s);
        }
        catch(Exception &exc)
        {
          MessageDlg("Ошибка создания TTreeNode root\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
        }
      }
      else
      { try
        {
          node=dbTreeView->Items->AddChild(ParentNode,ds->FieldByName(viewFieldName)->AsString);
        }
        catch(Exception &exc)
        {
          MessageDlg("Ошибка создания TTreeNode child\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
        }
      }
      if(node!=NULL)
      { node->ImageIndex=0;
        node->SelectedIndex=1;
        DBTreeData *dbTD=NULL;
        try
        {
          dbTD=new DBTreeData();
        }
        catch(Exception &exc)
        {
          MessageDlg("Ошибка создания DBTreeData\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
        }
        if(dbTD!=NULL)
        { dbTD->ID=ds->FieldByName(keyFieldName)->AsString;
          node->Data=dbTD;
          buildNode(node,dbTD->ID);
        }
      }
      ds->Next();
    }
  }
  if(ds!=NULL)
  { if(ds->Active==true) ds->Close();
    delete ds; ds=NULL;
  }
}
//---------------------------------------------------------------------------
void __fastcall TDBTreeViewFrame::buildTree(void)
{
  dbTreeView->Visible=false;
  //Удаляем текущее дерево
  removeTree();
  //Строим новое дерево
  buildNode(NULL,"");
  //Встаем на текущий элемент
  if(currentItemID.IsEmpty())
  { setPositionInTreeView(dbTreeView->Items->Item[0]);
    currentItemID=static_cast<DBTreeData*>(dbTreeView->Items->Item[0]->Data)->ID;
  }
  else setPositionInTreeView(findNodeInTreeView(currentItemID));
  dbTreeView->Visible=true;
}
//---------------------------------------------------------------------------
void __fastcall TDBTreeViewFrame::removeTree(void)
{
DBTreeData* dbTD;

  for(int i=0; i<dbTreeView->Items->Count; i++)
  { dbTD=(static_cast<DBTreeData*>(dbTreeView->Items->Item[i]->Data));
    if(dbTD != NULL) delete dbTD;
    dbTreeView->Items->Item[i]->Data=NULL;
  }
  dbTreeView->Items->Clear();
}
//---------------------------------------------------------------------------

//*************
//* published *
//*************
void __fastcall TDBTreeViewFrame::dbTreeViewChange(TObject *Sender,
      TTreeNode *Node)
{
  if(dbTreeView->Visible==false) return;
  if(dbTreeView->Selected->Data!=NULL)
    currentItemID=(static_cast<DBTreeData*>(dbTreeView->Selected->Data))->ID;
}
//---------------------------------------------------------------------------
void __fastcall TDBTreeViewFrame::selectAllClick(TObject *Sender)
{
  dbTreeView->Visible=false;
  for(int i=0; i < dbTreeView->Items->Count; i++)
    dbTreeView->setCheckStyleNode(dbTreeView->Items->Item[i], csChecked);
  dbTreeView->Visible=true;
  changeFLG=true;
}
//---------------------------------------------------------------------------
void __fastcall TDBTreeViewFrame::unselectAllClick(TObject *Sender)
{
  dbTreeView->Visible=false;
  for(int i=0; i < dbTreeView->Items->Count; i++)
    dbTreeView->setCheckStyleNode(dbTreeView->Items->Item[i], csUnchecked);
  dbTreeView->Visible=true;
  changeFLG=true;
}
//---------------------------------------------------------------------------
void __fastcall TDBTreeViewFrame::expandClick(TObject *Sender)
{
  dbTreeView->FullExpand();
  setPositionInTreeView(findNodeInTreeView(currentItemID));
}
//---------------------------------------------------------------------------
void __fastcall TDBTreeViewFrame::collapseClick(TObject *Sender)
{
  dbTreeView->FullCollapse();
}
//---------------------------------------------------------------------------
void __fastcall TDBTreeViewFrame::dbTreeViewEnter(TObject *Sender)
{
  DBTreeViewHeader->Font->Style=TFontStyles() << fsBold;
}
//---------------------------------------------------------------------------
void __fastcall TDBTreeViewFrame::dbTreeViewExit(TObject *Sender)
{
  DBTreeViewHeader->Font->Style=TFontStyles();
}
//---------------------------------------------------------------------------
void __fastcall TDBTreeViewFrame::tvPopupMenuPopup(TObject *Sender)
{
  if(saveItem->OnClick!=NULL)
  {
    saveItem->Enabled=changeFLG;
  }
  else saveItem->Visible=false;
}
//---------------------------------------------------------------------------
void __fastcall TDBTreeViewFrame::dbTreeViewSelectNode(TObject *Sender,TTreeNode *Node)
{
  changeFLG=true;
}
//---------------------------------------------------------------------------
void __fastcall TDBTreeViewFrame::dbTreeViewDeselectNode(TObject *Sender,TTreeNode *Node)
{
  changeFLG=true;
}
//---------------------------------------------------------------------------

