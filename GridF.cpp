//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "DifferentFunctions.h"
#include "ColumnsConfig.h"
#include "GridF.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "DBGridEh"
#pragma link "JvComponent"
#pragma link "JvExControls"
#pragma link "JvNavigationPane"
#pragma link "FIBDataSet"
#pragma link "pFIBDataSet"
#pragma link "DBGridEh"
#pragma link "DBGridEh"
#pragma link "DBGridEh"
#pragma resource "*.dfm"
//TGridFrame *GridFrame;
//---------------------------------------------------------------------------
void FieldInfo::init(AnsiString fieldInfo)
{
  sqlFieldName=getSubstring(fieldInfo,',',1);
  gridFieldName=getSubstring(fieldInfo,',',2);
  gridFieldTitle=getSubstring(fieldInfo,',',3);
  align=getSubstring(fieldInfo,',',4);
  readOnly=Sysutils::StrToBool(getSubstring(fieldInfo,',',5));
  gridViewAble=Sysutils::StrToBool(getSubstring(fieldInfo,',',6));
  titleButton=Sysutils::StrToBool(getSubstring(fieldInfo,',',7));
  searchAble=Sysutils::StrToBool(getSubstring(fieldInfo,',',8));
  if(getSubstring(fieldInfo,',',9)!=NULL) formatString=getSubstring(fieldInfo,',',9);
}
//---------------------------------------------------------------------------
void FooterFieldInfo::init(AnsiString fieldInfo)
{
  fieldName=getSubstring(fieldInfo,',',1);
  valueType=getSubstring(fieldInfo,',',2);
  formatString=getSubstring(fieldInfo,',',3);
  value=getSubstring(fieldInfo,',',4);
  align=getSubstring(fieldInfo,',',5);
}
//---------------------------------------------------------------------------
static int GridFrameNumber=1;
//---------------------------------------------------------------------------
//*********
// public *
//*********
__fastcall TGridFrame::TGridFrame(TComponent* Owner,AnsiString iniFN,\
AnsiString iniSN,FBDatabase *fbDB,AnsiString tableInf,AnsiString fieldsInf,\
AnsiString footerFieldsInfo)
        : TFrame(Owner),owner(Owner),iniFileName(iniFN),iniSectionName(iniSN),fbDatabase(fbDB),
          tableInfo(tableInf),fieldsInfoStr(fieldsInf),footerFieldsInfoStr(footerFieldsInfo)
{
AnsiString name=this->Name+IntToStr(GridFrameNumber);
bool rez=Owner->FindComponent(name);

  if(rez==true) GridFrameNumber++;
  this->Name=this->Name+IntToStr(GridFrameNumber);

  tableName=getSubstring(tableInfo,',',1),
  tableTitle=getSubstring(tableInfo,',',2),
  keyFieldName=getSubstring(tableInfo,',',3),
  viewFieldName=getSubstring(tableInfo,',',4);

  TIniFile *ini = new TIniFile(iniFileName);
  bool loadingFLG=ini->ReadBool("EntryForm","loadingFLG",true);
  if(loadingFLG==true)
  { AnsiString fi=ini->ReadString(iniSectionName,"fieldsInfo","");
    if(!fi.IsEmpty()) fieldsInfoStr=fi;
  }
  delete ini;

  fieldsInfo=NULL;
  fbDS=NULL;

  gcHeader=NULL;
  SEdit=NULL;
  conditionSBox=NULL;
  fbSDS=NULL;

  statusBar=NULL;

  fieldsQuantity=0;
  editAbleFieldsQuantity=0;
  searchAbleFieldsQuantity=0;

  additionalAdjustmentsForGrid=NULL;

  workGrid->Columns->Clear();
  columnsConfigFieldEditInGridVisible=true;
  columnsConfigFieldViewInGridVisible=true;
  columnsConfigFieldTitleButtonVisible=true;
  columnsConfigFieldSearchAbleVisible=true;
}
//---------------------------------------------------------------------------
__fastcall TGridFrame::~TGridFrame(void)
{
TIniFile *ini = new TIniFile(iniFileName);

  ini->WriteString(iniSectionName,"fieldsInfo",fieldsInfoStr);
  selectedRecordID=getSelectedRecordID();
  ini->WriteString(iniSectionName,"selectedRecordID",selectedRecordID);
  ini->WriteString(iniSectionName,"orderByStr",formatSortStringDS(forWorkDS));
  ini->WriteInteger(iniSectionName,"Col",workGrid->Col);
  for(int i=0; i<workGrid->Columns->Count; i++)
  { AnsiString fieldName=workGrid->Columns->Items[i]->FieldName;
    int width=workGrid->Columns->Items[i]->Width;
    ini->WriteInteger(iniSectionName,fieldName+"_Width",width);
  }
  delete ini;

  if(fbDS!=NULL){ delete fbDS; fbDS=NULL;}
  if(fbSDS!=NULL){ delete fbSDS; fbSDS=NULL;}

  if(fieldsInfo!=NULL)
  { for(int i=0; i<fieldsQuantity; i++) if(fieldsInfo[i]!=NULL){ delete fieldsInfo[i]; fieldsInfo[i]=NULL; }
    delete []fieldsInfo; fieldsInfo=NULL;
  }

  if(footerFieldsInfo!=NULL)
  { for(int i=0; i<footerFieldsQuantity; i++) if(footerFieldsInfo[i]!=NULL){ delete footerFieldsInfo[i]; footerFieldsInfo[i]=NULL; }
    delete []footerFieldsInfo; footerFieldsInfo=NULL;
  }
}
//---------------------------------------------------------------------------
void __fastcall TGridFrame::initWorkGrid(AnsiString fieldsInf)
{
  //Удаляем из Grid-а колонки
  int cc=workGrid->Columns->Count;
//  AnsiString wgName=this->Name;
//  AnsiString fName;
  for(int i=0; i<cc; i++)
  { //if(i > 0) fName=workGrid->Columns->Items[0]->Field->FieldName;
    workGrid->Columns->Delete(0);
  }
  //Удаляем предыдущую версию fieldsInfo
  if(fieldsInfo!=NULL)
  { for(int i=0; i<fieldsQuantity; i++) if(fieldsInfo[i]!=NULL){ delete fieldsInfo[i]; fieldsInfo[i]=NULL; }
    delete []fieldsInfo;
    fieldsInfo=NULL;
  }
  fieldsInfoStr=fieldsInf;
  //Инициируем структуру fieldsInfo
  // подсчитываем количество полей
  fieldsQuantity=0;
  while(getSubstring(fieldsInfoStr,';',fieldsQuantity+1)!=NULL) fieldsQuantity++;
  // создаем массив указателей fieldsInfo
  try
  {
    fieldsInfo=new FieldInfo*[fieldsQuantity];
  }
  catch (Exception &exc)
  { MessageDlg("Ошибка выделения памяти под fieldsInfo\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    fieldsInfo=NULL;
    return;
  }

  TIniFile *ini = new TIniFile(iniFileName);
  selectedRecordID=ini->ReadString(iniSectionName,"selectedRecordID","");
  AnsiString orderBy=ini->ReadString(iniSectionName,"orderByStr","");
  int colum=ini->ReadInteger(iniSectionName,"Col",1);

  // создаем структуры FieldInfo
//  int j=0;,col=1;
  for(int i=0; i<fieldsQuantity; i++)
  { fieldsInfo[i]=NULL;
    try
    {
      fieldsInfo[i]=new FieldInfo;
    }
    catch (Exception &exc)
    { MessageDlg("Ошибка выделения памяти под fieldsInfo["+IntToStr(i)+"]\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
      fieldsInfo[i]=NULL;
      return;
    }
    fieldsInfo[i]->init(getSubstring(fieldsInfoStr,';',i+1));
//    if(fieldsInfo[i]->isGridViewAble()) j++;
//    if(fieldsInfo[i]->getGridFieldName()==viewFieldName) col=j;
  }
  //Инициируем workGrid
  editAbleFieldsQuantity=0;
  searchAbleFieldsQuantity=0;
  gridViewFieldsQuantity=0;
  for(int i=0; i<fieldsQuantity; i++)
  { if(fieldsInfo[i]->isGridViewAble()==true)
    { gridViewFieldsQuantity++;
      TColumnEh *column=workGrid->Columns->Add();
      column->FieldName=fieldsInfo[i]->getGridFieldName();
      column->Title->Caption=fieldsInfo[i]->getGridFieldTitle();
      if(fieldsInfo[i]->getFormatString()!=NULL)
        column->DisplayFormat=fieldsInfo[i]->getFormatString();
      AnsiString align=fieldsInfo[i]->getAlign();
      if(align=="Center") column->Alignment=Classes::taCenter;
      else if(align=="Left") column->Alignment=Classes::taLeftJustify;
      else column->Alignment=Classes::taRightJustify;
      column->Title->TitleButton=fieldsInfo[i]->isTitleButton();
      column->ReadOnly=fieldsInfo[i]->isReadOnly();
      if(column->ReadOnly==false)
      { editAbleFieldsQuantity++;
        column->Title->Font->Style=TFontStyles()<< fsBold;
//        column->Font->Style=TFontStyles()<< fsBold;
      }
      if(fieldsInfo[i]->isSearchAble()==true) searchAbleFieldsQuantity++;
      column->AutoFitColWidth=false;
      column->Width=ini->ReadInteger(iniSectionName,column->FieldName+"_Width",64);
      if(column->FieldName=="DEL_FLG")
      { column->ImageList=del_flg_ImageList;
        column->KeyList->Clear();
        column->KeyList->Add("0");
        column->KeyList->Add("1");
        column->NotInKeyListIndex=0;
      }
      else if(column->FieldName=="LOCK_DATA")
      { column->ImageList=lock_flg_ImageList;
        column->KeyList->Clear();
        column->KeyList->Add("0");
        column->KeyList->Add("1");
        column->NotInKeyListIndex=0;
      }
    }
  }
  delete ini;

  if(gridViewFieldsQuantity==0) return;
//  if(col > 0 && col <= workGrid->Columns->Count)
//  { //workGrid->Col=col;
//    if(getSearchAbleFieldsQuantity() > 0)
//      workGrid->Columns->Items[col-1]->Title->Font->Color=clTeal;
//  }

  if(additionalAdjustmentsForGrid!=NULL) additionalAdjustmentsForGrid(this);

  //Восстанавливаем порядок сортировки
  workGrid->SortMarkedColumns->Clear();
  if(orderBy!=NULL && !orderBy.IsEmpty())
  { AnsiString field,fields;
    int n=1,j=0;
    fields=orderBy.SubString(orderBy.Pos("order by ")+9,orderBy.Length());
    while((field=getSubstring(fields,',',n))!=NULL)
    { field=field.TrimLeft();
      field=field.TrimRight();
      j=0;
      for(int i=0; i<fieldsQuantity; i++)
      { if(fieldsInfo[i]->isGridViewAble())
        { AnsiString sqlName=fieldsInfo[i]->getSQLFieldName();
          if(field==sqlName)
          { int k=workGrid->SortMarkedColumns->Add(workGrid->Columns->Items[j]);
            workGrid->SortMarkedColumns->Items[k]->Title->SortMarker=smUpEh;
            break;
          }
          j++;
        }
      }
      n++;
    }
  }
  if(colum > 0 && colum <= workGrid->Columns->Count) workGrid->Col=colum;
  else{ workGrid->Col=1; colum=1; }
  if(fieldsInfo[colum-1]->isGridViewAble() && fieldsInfo[colum-1]->isSearchAble())
    workGrid->Columns->Items[colum-1]->Title->Font->Color=clTeal;
}
//---------------------------------------------------------------------------
void __fastcall TGridFrame::reLoadWorkGrid(AnsiString newFI)
{
  workGrid->Visible=false;
  initWorkGrid(newFI);
  initFooterStr(footerFieldsInfoStr);
  workGrid->Visible=true;
}
//---------------------------------------------------------------------------
void __fastcall TGridFrame::initFooterStr(AnsiString footerFieldsInf)
{
  //Удаляем предыдущую версию footerFieldsInfo
  if(footerFieldsInfo!=NULL)
  { for(int i=0; i<footerFieldsQuantity; i++) if(footerFieldsInfo[i]!=NULL){ delete footerFieldsInfo[i]; footerFieldsInfo[i]=NULL; }
    delete []footerFieldsInfo; footerFieldsInfo=NULL;
  }
  if(footerFieldsInf==NULL || footerFieldsInf.IsEmpty())
  { workGrid->SumList->Active=false;
    workGrid->FooterRowCount=0;
    return;
  }

  if(gridViewFieldsQuantity==0) return;
  footerFieldsInfoStr=footerFieldsInf;
  //Инициируем структуру footerFieldsInfo
  // подсчитываем количество полей
  footerFieldsQuantity=0;
  while(getSubstring(footerFieldsInfoStr,';',footerFieldsQuantity+1)!=NULL) footerFieldsQuantity++;
  // создаем массив указателей footerFieldsInfo
  try
  {
    footerFieldsInfo=new FooterFieldInfo*[footerFieldsQuantity];
  }
  catch (Exception &exc)
  { MessageDlg("Ошибка выделения памяти под footerFieldsInfo\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    footerFieldsInfo=NULL;
    return;
  }
  // создаем структуры FooterFieldInfo
  for(int i=0; i<footerFieldsQuantity; i++)
  { footerFieldsInfo[i]=NULL;
    try
    {
      footerFieldsInfo[i]=new FooterFieldInfo;
    }
    catch (Exception &exc)
    { MessageDlg("Ошибка выделения памяти под footerFieldsInfo["+IntToStr(i)+"]\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
      fieldsInfo[i]=NULL;
      return;
    }
    footerFieldsInfo[i]->init(getSubstring(footerFieldsInfoStr,';',i+1));
  }
  //Инициируем в Grid строку Footer
  if(footerFieldsQuantity > 0)
  { workGrid->SumList->Active=true;
    workGrid->FooterRowCount=1;
//    workGrid->FooterColor=Graphics::clMoneyGreen;
  }
  for(int i=0; i<footerFieldsQuantity; i++)
  { AnsiString fieldName=footerFieldsInfo[i]->getFieldName();
    int col=findPozitionColumnInGrid(fieldName);
    if(col > 0)
    { workGrid->Columns->Items[col-1]->Footer->FieldName=fieldName;
      AnsiString valueType=footerFieldsInfo[i]->getValueType();
      if(valueType=="fvtAvg") workGrid->Columns->Items[col-1]->Footer->ValueType=fvtAvg;
      else if(valueType=="fvtCount") workGrid->Columns->Items[col-1]->Footer->ValueType=fvtCount;
      else if(valueType=="fvtFieldValue") workGrid->Columns->Items[col-1]->Footer->ValueType=fvtFieldValue;
      else if(valueType=="fvtStaticText") workGrid->Columns->Items[col-1]->Footer->ValueType=fvtStaticText;
      else if(valueType=="fvtSum") workGrid->Columns->Items[col-1]->Footer->ValueType=fvtSum;
      workGrid->Columns->Items[col-1]->Footer->DisplayFormat=footerFieldsInfo[i]->getFormatString();
      workGrid->Columns->Items[col-1]->Footer->Value=footerFieldsInfo[i]->getValue();
      AnsiString align=footerFieldsInfo[i]->getAlign();
      if(align=="Center") workGrid->Columns->Items[col-1]->Footer->Alignment=Classes::taCenter;
      else if(align=="Left") workGrid->Columns->Items[col-1]->Footer->Alignment=Classes::taLeftJustify;
      else workGrid->Columns->Items[col-1]->Footer->Alignment=Classes::taRightJustify;
      workGrid->Columns->Items[col-1]->Footer->Font->Style << fsBold;
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TGridFrame::initFrame(void)
{
  initWorkGrid(fieldsInfoStr);
  initFooterStr(footerFieldsInfoStr);

  if(fbDS!=NULL){ delete fbDS; fbDS=NULL;}
  //Инициируем основной DataSet
  fbDS=newFBDataSet(this,"TGridFrame::TGridFrame:",fbDatabase->getDatabase(),true);
  if(fbDS==NULL)
  { workDS=NULL;
    return;
  }
  workDS=fbDS->getDataSet();
  workDS->Options << poFetchAll;
  workSource->DataSet=workDS;
  workDS->BeforeClose=dsBeforeClose;
  workDS->AfterOpen=dsAfterOpen;
  workDS->AfterInsert=dsAfterInsert;
  workDS->AfterPost=dsAfterPost;

  if(fbSDS!=NULL){ delete fbSDS; fbSDS=NULL;}
  //Инициируем DataSet поиcка по модели
  if((fbSDS=newFBDataSet(this,"TPublishers_NMPFrame::TPublishers_NMPFrame",fbDatabase->getDatabase(),true))==NULL) return;
  searchDS=fbSDS->getDataSet();

  initSQLs();
  selectSQL="";
  searchSQL="";
}
//---------------------------------------------------------------------------
void __fastcall TGridFrame::initSelectSQL(AnsiString sql)
{
  //Если sql запрос содержит строку order by, вырезаем ее, т.к. порядок сортировки определяется пользователем
  if(sql.Pos("order by") > 0) sql=sql.SubString(1,sql.Pos("order by")-1);
  selectSQL=sql;
}
//---------------------------------------------------------------------------
void __fastcall TGridFrame::loadDS(void)
{
  if(workDS->Active==true) workDS->Close();

  if(gridViewFieldsQuantity==0) return;

  if(selectSQL.IsEmpty()==true) return;

  workDS->SQLs->SelectSQL->Clear();
  AnsiString sql=selectSQL+formatSortStringDS(forWorkDS);
  workDS->SQLs->SelectSQL->Add(sql);

  typedef void __fastcall (__closure *pDSonDCh)(TObject *Sender,TField *Field);
  pDSonDCh onDCh=workSource->OnDataChange;
  workSource->OnDataChange=NULL;
  try
  {
    workDS->Open();
  }
  catch(const Exception &exc)
  { MessageDlg("Ошибка - "+selectSQL+"\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return;
  }
  workSource->OnDataChange=onDCh;

  if(statusBar!=NULL)
    statusBar->SimpleText="Всего записей: "+IntToStr(workDS->RecordCount);

  //Инициируем searchDS
//  loadSearchingDS();
}
//---------------------------------------------------------------------------
void __fastcall TGridFrame::initSearchSQL(AnsiString sql)
{
  //Если sql запрос содержит строку order by, вырезаем ее, т.к. порядок сортировки определяется пользователем
  if(sql.Pos("order by") > 0) sql=sql.SubString(1,sql.Pos("order by")-1);
  searchSQL=sql;
}
//---------------------------------------------------------------------------
void __fastcall TGridFrame::loadSearchingDS(void)
{
  if(workDS->Active==false) return;
  if(gridViewFieldsQuantity==0) return;
  if(searchSQL!=NULL && !searchSQL.IsEmpty())
  { searchDS->Close();
    searchDS->SQLs->SelectSQL->Clear();
    AnsiString sql=assemblySearchSQLString();
    if(sql.IsEmpty()) return;
    searchDS->SQLs->SelectSQL->Add(sql);
    try
    {
      searchDS->Open();
    }
    catch (Exception &exc)
    { MessageDlg("Ошибка - "+searchSQL+"\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
      return;
    }
    searchDS->Last();
    if(searchDS->RecordCount > 0)
    { TLocateOptions lo; lo.Clear();
      searchDS->First();
      workDS->Last();
      if(locateDS()==true) workGrid->SetFocus();
    }
  }
}
//---------------------------------------------------------------------------
//Формирование строки сортировки для SQL запроса
AnsiString __fastcall TGridFrame::formatSortStringDS(OrderString forWho)
{
AnsiString sortString="";
bool firstItem=true;

  for(int i=0; i<workGrid->SortMarkedColumns->Count; ++i)
  { for(int j=0; j<fieldsQuantity; j++)
    { if(fieldsInfo[j]->isGridViewAble() && fieldsInfo[j]->isTitleButton())
      { if(workGrid->SortMarkedColumns->Items[i]->FieldName.UpperCase()==fieldsInfo[j]->getGridFieldName().UpperCase())
        { if(firstItem==true){ sortString=" order by "; firstItem=false; }
          else sortString+=", ";
          if(forWho==forWorkDS) sortString+=fieldsInfo[j]->getSQLFieldName();
          else sortString+=fieldsInfo[j]->getGridFieldName();
          workGrid->SortMarkedColumns->Items[i]->Title->SortMarker=smUpEh;
          break;
        }
      }
    }
  }
  return sortString;
}
//---------------------------------------------------------------------------
void __fastcall TGridFrame::forwardSearch(void)
{
  if(searchDS->Active==true)
  { if(searchDS->RecNo < searchDS->RecordCount) searchDS->Next();
    else searchDS->First();
    locateDS();
    workGrid->SetFocus();
  }
}
//---------------------------------------------------------------------------
void __fastcall TGridFrame::backwardSearch(void)
{
  if(searchDS->Active==true)
  { if(searchDS->RecNo > 1) searchDS->Prior();
    else searchDS->Last();
    locateDS();
    workGrid->SetFocus();
  }
}
//---------------------------------------------------------------------------
//Ищет положение поля по его sql наименованию
int __fastcall TGridFrame::findPozitionFieldForSQLFieldName(AnsiString sqlFieldName)
{
  for(int p=0; p<fieldsQuantity; p++)
    if(fieldsInfo[p]->getSQLFieldName().UpperCase().Trim()==sqlFieldName.UpperCase().Trim()) return p+1;
  return 0;
}
//---------------------------------------------------------------------------
//ищет положение поля по его grid наименованию
int __fastcall TGridFrame::findPozitionFieldForGridFieldName(AnsiString gridFieldName)
{
  for(int p=0; p<fieldsQuantity; p++)
    if(fieldsInfo[p]->getGridFieldName().UpperCase().Trim()==gridFieldName.UpperCase().Trim()) return p+1;
  return 0;
}
//---------------------------------------------------------------------------
//ищет положение колонки в grid
int __fastcall TGridFrame::findPozitionColumnInGrid(AnsiString gridFieldName)
{
int p,poz=0;

  if(findPozitionFieldForGridFieldName(gridFieldName)==0) return 0;
  if(fieldsInfo[findPozitionFieldForGridFieldName(gridFieldName)-1]->isGridViewAble()==false) return 0;
  for(p=0; p<fieldsQuantity; p++)
  { if(fieldsInfo[p]->isGridViewAble()==true) poz++;
    if(fieldsInfo[p]->getGridFieldName().UpperCase().Trim()==gridFieldName.UpperCase().Trim()) break;
  }
  return poz;
}
//---------------------------------------------------------------------------
FieldInfo * TGridFrame::getFieldInfoByGridFieldName(AnsiString gridFieldName)
{
  for(int p=0; p<fieldsQuantity; p++)
    if(fieldsInfo[p]->getGridFieldName().UpperCase().Trim()==gridFieldName.UpperCase().Trim()) return fieldsInfo[p];
  return NULL;
}
//---------------------------------------------------------------------------
void __fastcall TGridFrame::setHeaderCaption(AnsiString caption)
{
  if(gcHeader!=NULL) gcHeader->Caption=caption;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TGridFrame::getFieldDataOfSelectedRecord(AnsiString fieldName)
{
bool act=fbDS->getDataSet()->Active;
int poz=findPozitionFieldForGridFieldName(fieldName);


  if(fbDS!=NULL && act && poz>0)
  { AnsiString s=fbDS->getDataSet()->FieldByName(fieldName)->AsString;
    return s;
  }
  else return "";
}
//---------------------------------------------------------------------------
AnsiString __fastcall TGridFrame::getFieldDataOfSelectedRecord(AnsiString refTableName,AnsiString keyFieldName,AnsiString refFieldName,AnsiString refFieldValue,AnsiString retFieldName)
{
AnsiString sql;
FBQuery *fbRQ=fbDatabase->getFBQ_T1();
AnsiString ret="";

  sql="select "+retFieldName+" from "+refTableName;
  sql+=" where "+keyFieldName+"='"+refFieldValue+"'";
  if(fbRQ->execQuery("TGridFrame::getFieldDataOfSelectedRecord",sql)==false) return ret;
  ret=fbRQ->getQuery()->FieldByName(retFieldName)->AsString;
  fbRQ->getQuery()->Close();
  return ret;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TGridFrame::getSelectedRecordID(void)
{
  if(workDS->Active==true) return workDS->FieldByName(keyFieldName)->AsString;
  else return "";
}
//---------------------------------------------------------------------------
AnsiString __fastcall TGridFrame::getSEditText(void)
{
  if(SEdit!=NULL) return SEdit->Text;
  else return "";
}
//---------------------------------------------------------------------------
bool __fastcall TGridFrame::getConditionChecked(void)
{
  if(conditionSBox!=NULL) return conditionSBox->Checked;
  else return false;
}
//---------------------------------------------------------------------------
bool __fastcall TGridFrame::isSearchDSActive(void)
{
  return searchDS->Active;
}
//---------------------------------------------------------------------------
bool __fastcall TGridFrame::isReadOnly(AnsiString fieldName)
{
FieldInfo *fi=getFieldInfoByGridFieldName(fieldName);

  if(fi!=NULL) return fi->isReadOnly();
  else return true;
}
//---------------------------------------------------------------------------
bool __fastcall TGridFrame::isGridViewAble(AnsiString fieldName)
{
FieldInfo *fi=getFieldInfoByGridFieldName(fieldName);

  if(fi!=NULL) return fi->isGridViewAble();
  else return false;
}
//---------------------------------------------------------------------------
bool __fastcall TGridFrame::isTitleButton(AnsiString fieldName)
{
FieldInfo *fi=getFieldInfoByGridFieldName(fieldName);

  if(fi!=NULL) return fi->isTitleButton();
  else return false;
}
//---------------------------------------------------------------------------
bool __fastcall TGridFrame::isSearchAble(AnsiString fieldName)
{
FieldInfo *fi=getFieldInfoByGridFieldName(fieldName);

  if(fi!=NULL) return fi->isSearchAble();
  else return false;
}
//---------------------------------------------------------------------------

//**********
// private *
//**********
void __fastcall TGridFrame::initSQLs(void)
{
AnsiString sql="";
bool firstFLG=true;

  workDS->SQLs->InsertSQL->Clear();
  workDS->SQLs->UpdateSQL->Clear();
  workDS->SQLs->DeleteSQL->Clear();
  workDS->SQLs->RefreshSQL->Clear();

  if(editAbleFieldsQuantity==0) return;

  //InsertSQL
  FieldInfo *fieldInfo=getFieldInfoByGridFieldName(keyFieldName);
  AnsiString sqlKeyFieldName="";
  if(fieldInfo!=NULL) sqlKeyFieldName=fieldInfo->getSQLFieldName();

  AnsiString prefix="";
  if(sqlKeyFieldName.Pos(".")>0) prefix=sqlKeyFieldName.SubString(1,sqlKeyFieldName.Pos("."));
  sql="insert into "+tableName+"(";
  for(int i=0; i<fieldsQuantity; i++)
  { if(prefix.IsEmpty() || fieldsInfo[i]->getSQLFieldName().Pos(prefix))
    { if(firstFLG==true) firstFLG=false;
      else sql+=",";
      sql+=fieldsInfo[i]->getGridFieldName();
    }
  }
  sql+=") values(";
  firstFLG=true;
  for(int i=0; i<fieldsQuantity; i++)
  { if(prefix.IsEmpty() || fieldsInfo[i]->getSQLFieldName().Pos(prefix))
    { if(firstFLG==true) firstFLG=false;
      else sql+=",";
      sql+="?";
      sql+=fieldsInfo[i]->getGridFieldName();
    }
  }
  sql+=")";
  workDS->SQLs->InsertSQL->Add(sql);

  //UpdateSQLs
  firstFLG=true;
  sql="update "+tableName;
  for(int i=0; i<fieldsQuantity; i++)
//  { if(fieldsInfo[i]->getGridFieldName()==keyFieldName || fieldsInfo[i]->getGridFieldName()=="FILE_ID" || fieldsInfo[i]->isReadOnly()==true || fieldsInfo[i]->isGridViewAble()==false) continue;
  { if(fieldsInfo[i]->getGridFieldName()==keyFieldName || fieldsInfo[i]->getGridFieldName()=="FILE_ID" || fieldsInfo[i]->isReadOnly()==true) continue;
    else
    { if(firstFLG==true){ sql+=" set "; firstFLG=false; }
      else sql+=",";
      sql+=fieldsInfo[i]->getGridFieldName()+"=?"+fieldsInfo[i]->getGridFieldName();
    }
  }
  sql+=" where "+keyFieldName+"=?OLD_"+keyFieldName;
  workDS->SQLs->UpdateSQL->Add(sql);

  //DeleteSQL
  sql="delete from "+tableName+" where "+keyFieldName+"=?OLD_"+keyFieldName;
  workDS->SQLs->DeleteSQL->Add(sql);

  //RefreshSQL
  sql="select * from "+tableName+" where "+keyFieldName+"=?OLD_"+keyFieldName;
  workDS->SQLs->RefreshSQL->Add(sql);
}
//---------------------------------------------------------------------------
bool __fastcall TGridFrame::locateDS(void)
{
AnsiString fieldNames="";
int fc=searchDS->Fields->Count;
Variant *fieldsData=new Variant[fc];
bool rez;

  for(int i=0; i<fc; i++)
  { if(i > 0) fieldNames+=";";
    AnsiString fieldName=searchDS->Fields->Fields[i]->FieldName;
    fieldNames+=fieldName;
    fieldsData[i]=searchDS->FieldByName(fieldName)->AsString;
  }
  rez=workDS->Locate(fieldNames,VarArrayOf(fieldsData,fc-1),TLocateOptions().Clear());
  delete []fieldsData;
  return rez;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TGridFrame::assemblySearchSQLString(void)
{
//Проблемы, решаемые этой функцией:
//  Окончательный вид строки отбора записей для searchDS поиска по по медели
//зависит от выбранного пользователем поля, по которому производится поиск, и от
//выбранных полей сортировки записей осноного DataSet-а.
//  Если, эти поля содержат агрегатные функции, то в строке отвора должны
//появитья предложения group by и having.
//  Другая проблема возникает, если поля отбора выбираются из разных таблиц.
//В этом случае строка order by должна формироваться из имен полей основного
//DataSet-а, а не из имен полей Grid-а

AnsiString sql="";;

  if(SEdit==NULL) return sql;
  if(searchSQL.IsEmpty()) return sql;

AnsiString searchFN;   //наименование поля, по которому производится поиск
AnsiString editText=SEdit->Text; //модель поиска
bool condition=false;  //без учета регистра
AnsiString orderBy=""; 
AnsiString groupBy="";
AnsiString having="";

  sql=searchSQL;
  if(conditionSBox!=NULL) condition=conditionSBox->Checked;

  //Проверяем, содержат ли наименования полей отбора разделитель '.', отделяющий
  //наименование таблицы от наименования поля. Если нет, то формируем строку
  //order by из наименований полей Grid-а, иначе - из наименований полей
  //основного DataSet-а
  bool workDSflg=false;
  AnsiString s=sql.SubString(1,sql.UpperCase().Pos("FROM")-1);
  if(s.Pos(".") == 0)
  { orderBy=formatSortStringDS(forSearchDS);
    searchFN=getFieldInfoByGridFieldName(workGrid->Columns->Items[workGrid->Col-1]->FieldName)->getGridFieldName();
  }
  else
  { orderBy=formatSortStringDS(forWorkDS);
    if(orderBy.Pos(".") == 0) orderBy="";
    searchFN=getFieldInfoByGridFieldName(workGrid->Columns->Items[workGrid->Col-1]->FieldName)->getSQLFieldName();
    workDSflg=true;
  }
  if(searchFN.IsEmpty()) return "";
  //Проверяем есть или поле searchFN в перечне полей предложения select
//  if(s.Pos(searchFN) == 0) return "";

  //Проверяем, содержит ли наименование поля поиска shearchFN или строка order by
  //агрегатные функции
  bool agregateShearchFNflg=false;
  AnsiString fn=searchFN.UpperCase();
  if(fn.Pos("COUNT(")!=0 || fn.Pos("SUM(")!=0 || fn.Pos("AVG(")!=0 || fn.Pos("MAX(")!=0 || fn.Pos("MIN(")!=0)
    agregateShearchFNflg=true;
  bool agregateOrderbyFLG=false;
  fn=orderBy.UpperCase();
  if(fn.Pos("COUNT(")!=0 || fn.Pos("SUM(")!=0 || fn.Pos("AVG(")!=0 || fn.Pos("MAX(")!=0 || fn.Pos("MIN(")!=0)
    agregateOrderbyFLG=true;

  //Формирование строки sql при наличии агрегатных функций
  if(agregateShearchFNflg==true || agregateOrderbyFLG==true)
  { //Формируем строку group by
    groupBy=" group by ";
    AnsiString fNames=sql.SubString(1,sql.UpperCase().Pos("FROM ")-1).UpperCase();
    bool first=true;
    for(int i=0; i<fieldsQuantity; i++)
    { if(workDSflg==false) fn=fieldsInfo[i]->getGridFieldName();
      else fn=fieldsInfo[i]->getSQLFieldName();
      if(fNames.Pos(fn.UpperCase()) > 0)
      { if(first==false) groupBy+=",";
        else first=false;
        groupBy+=fn;
      }
    }
    //Если searchFN не содержит агрегатных функций и отсутствует в groupBy,
    //добавляем ее в groupBy
    if(agregateShearchFNflg==false && groupBy.UpperCase().Pos(searchFN)==0)
      groupBy+=","+searchFN;
    //Добавляем в group by поля из order by, не являющиеся агрегатными функциями
    AnsiString obFields=orderBy.SubString(10,orderBy.Length()-9);
    int n=1;
    while((fn=getSubstring(obFields,',',n))!=NULL && !fn.IsEmpty())
    { fn=fn.UpperCase();
      if(fn.Pos("COUNT(")==0 && fn.Pos("SUM(")==0 && fn.Pos("AVG(")==0 && fn.Pos("MAX(")==0 && fn.Pos("MIN(")==0)
      { fn=fn.Trim();
        if(groupBy.UpperCase().Pos(fn.UpperCase())==0)
        { int n;
          if(workDSflg==true) n=findPozitionFieldForSQLFieldName(fn);
          else n=findPozitionFieldForGridFieldName(fn);
          if(n > 0)
          { FieldInfo *fi=fieldsInfo[n-1];
            if(workDSflg==false) groupBy+=","+fi->getGridFieldName();
            else groupBy+=","+fi->getSQLFieldName();
          }
        }
      }
      n++;
    }
    groupBy+=" ";

    if(agregateShearchFNflg==false)
    { if(condition==true)
        sql+=" and "+searchFN+" like '%"+editText+"%'";
      else
        sql+=" and UPPER("+searchFN+") like '%"+editText.UpperCase()+"%'";
      sql+=groupBy;
    }
    else
    { sql+=groupBy;
      //Добавляем строку having
      having+=" having ";
      if(condition==true)
        having+=searchFN+" like '%"+editText+"%'";
      else
        having+="UPPER("+searchFN+") like '%"+editText.UpperCase()+"%'";
    }
/*
    //Формируем строку having
    if(agregateOrderbyFLG==true)
    { bool first;
      if(having.IsEmpty())
      { having=" having ";
        first=true;
      }
      else first=false;
      AnsiString obFields=orderBy.SubString(10,orderBy.Length()-9);
      int n=1;
      while((fn=getSubstring(obFields,',',n))!=NULL)
      { fn=fn.UpperCase();
        if(fn.Pos("COUNT(")!=0 || fn.Pos("SUM(")!=0 || fn.Pos("AVG(")!=0 || fn.Pos("MAX(")!=0 || fn.Pos("MIN(")!=0)
        { fn=fn.Trim();
          if(first==false) having+=",";
          else first=false;
          having+=fn;
        }
        n++;
      }
    }
    */
    sql+=having;
    sql+=orderBy;
  }
  //Формирование строки sql при отсутствии агрегатных функций
  else
  { if(sql.UpperCase().Pos("WHERE") == 0) sql+=" where ";
    else sql+=" and ";
    if(condition==true)
      sql+=searchFN+" like '%"+editText+"%'";
    else
      sql+=" UPPER("+searchFN+") like '%"+editText.UpperCase()+"%'";
    sql+=orderBy;
  }
  return sql;
}
//---------------------------------------------------------------------------

//************
// published *
//************
void __fastcall TGridFrame::workGridSortMarkingChanged(TObject *Sender)
{
  loadDS();
  loadSearchingDS();
}
//---------------------------------------------------------------------------
void __fastcall TGridFrame::workGridKeyPress(TObject *Sender, char &Key)
{
  if(SEdit==NULL) return;

//  AnsiString name=this->ClassName();
//  AnsiString pname=this->Parent->Name;
//  AnsiString pcname=this->Parent->ClassName();

  int col=workGrid->Col-1;
  AnsiString fieldName=workGrid->Columns->Items[col]->FieldName;
  FieldInfo *fi=getFieldInfoByGridFieldName(fieldName);
  if(fi==NULL || fi->isSearchAble()==false) return;
  if(workDS->State==dsEdit || workDS->State==dsInsert) return;
  if(SEdit!=NULL && (IsCharAlphaA(Key) || IsCharAlphaNumericA(Key)))
  { SEdit->SetFocus();
    SEdit->SetTextBuf(AnsiString(Key).c_str());
    SEdit->SelStart=SEdit->GetTextLen();
    Key=0;
  }
}
//---------------------------------------------------------------------------
void __fastcall TGridFrame::workGridEnter(TObject *Sender)
{
  if(gcHeader==NULL) return;
  gcHeader->Font->Style=TFontStyles() << fsBold;
}
//---------------------------------------------------------------------------
void __fastcall TGridFrame::workGridExit(TObject *Sender)
{
  if(workDS->State==dsEdit || workDS->State==dsInsert) workDS->Post();
  if(gcHeader==NULL) return;
  gcHeader->Font->Style=TFontStyles();
}
//---------------------------------------------------------------------------
void __fastcall TGridFrame::workGridColEnter(TObject *Sender)
{
  if(getSearchAbleFieldsQuantity() == 0) return;

  int col=workGrid->Col-1;
  AnsiString fieldName=workGrid->Columns->Items[col]->FieldName;

  if(SEdit!=NULL) SEdit->Text="";
  for(int i=0; i<workGrid->Columns->Count; i++)
    workGrid->Columns->Items[i]->Title->Font->Color=clWindowText;
  workGrid->Repaint();
  FieldInfo *fi=getFieldInfoByGridFieldName(fieldName);
  if(fi==NULL || fi->isSearchAble()==false) return;
  workGrid->Columns->Items[col]->Title->Font->Color=clTeal;
  workGrid->Repaint();
}
//---------------------------------------------------------------------------
void __fastcall TGridFrame::dsBeforeClose(TDataSet *DataSet)
{
  selectedRecordID=DataSet->FieldByName(keyFieldName)->AsString;
}
//---------------------------------------------------------------------------
void __fastcall TGridFrame::dsAfterOpen(TDataSet *DataSet)
{
  if(!selectedRecordID.IsEmpty())
    DataSet->Locate(keyFieldName,selectedRecordID,TLocateOptions().Clear());
}
//---------------------------------------------------------------------------
void __fastcall TGridFrame::dsAfterInsert(TDataSet *DataSet)
{
AnsiString id=getNewKeyValue(fbDatabase,tableName);

  DataSet->FieldByName(keyFieldName)->AsString=id;
  if(findPozitionFieldForGridFieldName("DEL_FLG") > 0) DataSet->FieldByName("DEL_FLG")->AsString="0";
  if(findPozitionFieldForGridFieldName("REC_TYPE") > 0) DataSet->FieldByName("REC_TYPE")->AsString="1";
}
//---------------------------------------------------------------------------
void __fastcall TGridFrame::dsAfterPost(TDataSet *DataSet)
{
  if(workGrid->Columns->Items[workGrid->Col-1]->Title->SortMarker==smUpEh)
    workGrid->OnSortMarkingChanged(this);
}
//---------------------------------------------------------------------------
void __fastcall TGridFrame::columnsConfigClick(TObject *Sender)
{
TColumnsConfigForm *CCF;

  try
  {
    CCF=new TColumnsConfigForm(owner,iniFileName,iniSectionName,tableTitle,fieldsInfoStr);
  }
  catch(const Exception &exc)
  { MessageDlg("Ошибка создания объекта TColumnsConfigForm\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return;
  }
  CCF->grid->Columns->Items[2]->Visible=columnsConfigFieldViewInGridVisible;
  CCF->grid->Columns->Items[3]->Visible=columnsConfigFieldEditInGridVisible;
  CCF->grid->Columns->Items[4]->Visible=columnsConfigFieldTitleButtonVisible;
  CCF->grid->Columns->Items[5]->Visible=columnsConfigFieldSearchAbleVisible;
  if(CCF->ShowModal()==mrOk)
  { AnsiString newFI=CCF->getFieldsInfo();
    reLoadWorkGrid(newFI);
    loadDS();
    loadSearchingDS();
  }
  delete CCF;
}
//---------------------------------------------------------------------------

