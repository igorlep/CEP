//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "FoldersTreeF.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Placemnt"
#pragma link "JvComCtrls"
#pragma link "JvDotNetControls"
#pragma link "JvExComCtrls"
#pragma resource "*.dfm"
//TFoldersTreeFrame *FoldersTreeFrame;
//---------------------------------------------------------------------------
//**********
//* public *
//**********
__fastcall TFoldersTreeFrame::TFoldersTreeFrame(TComponent* Owner,AnsiString iniFN,AnsiString sName,FBDatabase *fbdb,AnsiString tabName)
        : TFrame(Owner),iniFileName(iniFN),sectionName(sName),fbDB(fbdb),tableName(tabName)
{
  BuildTree();
  loadProperties();
}
//---------------------------------------------------------------------------
__fastcall TFoldersTreeFrame::~TFoldersTreeFrame(void)
{
  saveProperties();
  RemoveTree();
}
//---------------------------------------------------------------------------
//Возвращает полное наименование папки, включающее наименование всех вышестоящих папок (кроме корневой)
AnsiString __fastcall TFoldersTreeFrame::getFullFolderName(void)
{
AnsiString fullName="";
TTreeNode *current=TV->Selected,*parent=current->Parent;

  while(parent!=NULL)
  { fullName="\\"+current->Text+fullName;
    current=parent;
    parent=current->Parent;
  }
  return fullName;
}
//---------------------------------------------------------------------------

//***********
//* private *
//***********
void __fastcall TFoldersTreeFrame::loadProperties(void)
{
TIniFile *ini = new TIniFile(iniFileName);
TStringList* expandlist=new TStringList();
AnsiString selectedFolderID;

  ini->ReadSectionValues(sectionName,expandlist);
  for(int i=0; i<expandlist->Count; i++)
  { AnsiString s=expandlist->Strings[i];
    if(s.Pos("selectedFolderID")==0)
    { s=s.SubString(s.Pos("=")+1,s.Length());
      expandlist->Strings[i]=s;
    }
  }
  selectedFolderID=ini->ReadString(sectionName,"selectedFolderID",0);
  ExpandFolders(expandlist);
  FindPositionTree(selectedFolderID);
  delete expandlist;
  delete ini;
}
//---------------------------------------------------------------------------
void __fastcall TFoldersTreeFrame::saveProperties(void)
{
TIniFile *ini = new TIniFile(iniFileName);
TStringList* expandlist=new TStringList();

  ini->EraseSection(sectionName);
  SaveExpandedFolders(expandlist);
  for(int i=0; i<expandlist->Count; i++)
    ini->WriteString(sectionName,IntToStr(i+1),expandlist->Strings[i]);
  ini->WriteString(sectionName,"selectedFolderID",(static_cast<TStructItem*>(TV->Selected->Data))->id);
  delete expandlist;
  delete ini;
}
//---------------------------------------------------------------------------
//Создание списка открытых узлов дерева папок
void __fastcall TFoldersTreeFrame::SaveExpandedFolders(TStrings* list)
{
  list->Clear();
  for(int i=0; i < TV->Items->Count; i++)
  { if(TV->Items->Item[i]->Expanded)
    { TStructItem* si=static_cast<TStructItem*>(TV->Items->Item[i]->Data);
      list->Add(si->id);
    }
  }
}
//---------------------------------------------------------------------------
//Открытие узлов в дереве папок из списка list
void __fastcall TFoldersTreeFrame::ExpandFolders(TStrings* list)
{
  if(list->Count!=0)
  { for(int i=0; i < list->Count; i++)
    { for(int j=0; j < TV->Items->Count; j++)
      { TStructItem* si=static_cast<TStructItem*>(TV->Items->Item[j]->Data);
        if(list->Strings[i]==si->id)
        { TV->Items->Item[j]->Expand(false);
          break;
        }
      }
    }
  }
}
//---------------------------------------------------------------------------
//Позиционирование в дереве папок на папку с кодом id
void __fastcall TFoldersTreeFrame::FindPositionTree(AnsiString id)
{
bool finded=false;
TStructItem* si;

  for(int i=0; i < TV->Items->Count; i++)
  { si=static_cast<TStructItem*>(TV->Items->Item[i]->Data);
    if(si->id==id)
    { TV->Selected=TV->Items->Item[i];
      finded=true;
      break;
    }
  }
  if(!finded) TV->Selected=TV->Items->Item[0];
}
//---------------------------------------------------------------------------
//Построение узла дерева папок
void __fastcall TFoldersTreeFrame::BuildNode(TTreeNode* ParentNode, AnsiString ParentID)
{
AnsiString sql;
FBDataSet *fbDS=NULL;
TpFIBDataSet *tds;

  try
  {
    fbDS=newFBDataSet(this,"TFoldersTreeFrame::BuildNode",fbDB->getDatabase(),true);
  }
  catch(const Exception &exc)
  { MessageDlg("Ошибка создания объекта newFBDataSet\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return;
  }

  if(!ParentID.IsEmpty())
  { sql="select * from "+tableName+" where PARENT_ID=";
    sql+="'"+ParentID+"' order by FOLDER_NUMBER";
  }
  else
    sql="select * from "+tableName+" where PARENT_ID is NULL";
  if(fbDS->execSelectSQL("",sql)==false)
  { delete fbDS;
    return;
  }
  tds=fbDS->getDataSet();
  tds->First();
  while(!tds->Eof)
  { TTreeNode *tn;
    if(!ParentID.IsEmpty()) tn=TV->Items->AddChild(ParentNode,tds->FieldByName("FOLDER_NAME")->AsString);
    else tn=TV->Items->Add(NULL,tds->FieldByName("FOLDER_NAME")->AsString);
    TStructItem *si=new TStructItem();
    si->AbsoluteIndex=tn->AbsoluteIndex;
    si->id=tds->FieldByName("FOLDER_ID")->AsString;
    si->folder_number=tds->FieldByName("FOLDER_NUMBER")->AsString;
    tn->Data=si;
    BuildNode(tn, si->id);
    tds->Next();
  }
  delete fbDS;
}
//---------------------------------------------------------------------------
//Построение дерева папок
void __fastcall TFoldersTreeFrame::BuildTree(void)
{
  //Запоминаем ID выбранной папки
  AnsiString SelectedFolderID;
  if(TV->Items->Count!=0 && TV->Selected->Level!=0)
    SelectedFolderID=(static_cast<TStructItem*>(TV->Selected->Data))->id;
  //Сохраняем состояние дерева
  TStringList* expandlist=new TStringList();
  SaveExpandedFolders(expandlist);
  //Удаляем текущее дерево
  RemoveTree();
  //Строим новое дерево
  BuildNode(NULL,"");
  //Восстанавливаем состояние дерева
  ExpandFolders(expandlist);
  delete expandlist;
  if(!SelectedFolderID.IsEmpty()) FindPositionTree(SelectedFolderID);
  else TV->Selected=TV->Items->Item[0];
}
//---------------------------------------------------------------------------
//Удаление дерева папок
void __fastcall TFoldersTreeFrame::RemoveTree(void)
{
TStructItem* si;

  for(int i=0; i<TV->Items->Count; i++)
  { si=(static_cast<TStructItem*>(TV->Items->Item[i]->Data));
    delete si;
    TV->Items->Item[i]->Data=NULL;
  }
  TV->Items->Clear();
}
//---------------------------------------------------------------------------

