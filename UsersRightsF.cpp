//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "PLACEMNT.HPP"
#include "DifferentFunctions.h"
#include "SomeFunctions.h"
#include "DBClasses.h"
#include "GridF.h"
#include "DBTreeViewF.h"
#include "UsersRightsFunctions.h"
#include "UsersRightsF.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "JvComponent"
#pragma link "JvExControls"
#pragma link "JvNavigationPane"
#pragma link "Placemnt"
#pragma resource "*.dfm"
//TUsersRightsFrame *UsersRightsFrame;
//---------------------------------------------------------------------------
//**********
//* public *
//**********
__fastcall TUsersRightsFrame::TUsersRightsFrame(TComponent* Owner,AnsiString iniFN,FBDatabase *fbdb)
        : TFrame(Owner),iniFileName(iniFN),fbDB(fbdb)
{
  iniSectionName="UsersRightsFrame";
  usersFrame=NULL;
  rightsTV=NULL;
}
//---------------------------------------------------------------------------
__fastcall TUsersRightsFrame::~TUsersRightsFrame(void)
{
  saveProperties();
  if(rightsTV!=NULL){ delete rightsTV; rightsTV=NULL; }
  if(usersFrame!=NULL){ delete usersFrame; usersFrame=NULL; }
}
//---------------------------------------------------------------------------
void __fastcall TUsersRightsFrame::initFrame(void)
{
  loadProperties();
  usersFrame=createUsersRFrame(this,iniFileName,iniSectionName,fbDB,rPanel);
  if(usersFrame!=NULL)
  { usersFrame->workGrid->PopupMenu=uMenu;
    usersFrame->workSource->OnDataChange=uSourceDataChange;
    usersFrame->setHeader(uHeader);
    usersFrame->setHeaderCaption("Пользователи программы");
    usersFrame->setStatusBar(uStatusBar);
    usersFrame->workGrid->OnMouseDown=uGridMouseDown;
    AnsiString sql=formUsersRSelectSQL();
    usersFrame->initSelectSQL(sql);
    usersFrame->loadDS();
  }
  else return;
  rightsTV=createTVFrame(this,iniFileName,fbDB,lPanel);
  if(rightsTV!=NULL)
  { rightsTV->initTreeViewFrame();
    rightsTV->dbTreeView->OnMouseDown=tvMouseDown;
  }
  else return;
  usersFrame->getWorkDataSet()->Locate("U_ID",userID,TLocateOptions().Clear());
  setFocus();
}
//---------------------------------------------------------------------------

//***********
//* private *
//***********
void __fastcall TUsersRightsFrame::loadProperties(void)
{
TIniFile *ini = new TIniFile(iniFileName);

  rPanel->Width=ini->ReadInteger(iniSectionName,"rPanel_Width",200);
  activeFrame=ini->ReadInteger(iniSectionName,"activeFrame",1);
  userID=ini->ReadInteger(iniSectionName,"userID",1);
  delete ini;
}
//---------------------------------------------------------------------------
void __fastcall TUsersRightsFrame::saveProperties(void)
{
TIniFile *ini = new TIniFile(iniFileName);

  ini->WriteInteger(iniSectionName,"rPanel_Width",rPanel->Width);
  ini->WriteInteger(iniSectionName,"activeFrame",activeFrame);
  ini->WriteInteger(iniSectionName,"userID",StrToInt(usersFrame->getFieldDataOfSelectedRecord("U_ID")));
  delete ini;
}
//---------------------------------------------------------------------------
void __fastcall TUsersRightsFrame::setFocus(void)
{
  if(activeFrame==1)
  {
    if(usersFrame!=NULL) usersFrame->workGrid->SetFocus();
  }
  else
  {
    if(rightsTV!=NULL) rightsTV->dbTreeView->SetFocus();
  }
}
//---------------------------------------------------------------------------
void __fastcall TUsersRightsFrame::saveRightsKitUser(void)
{
FBQuery *fbWQ=fbDB->getFBQ_T2();
FBQuery *fbRQ=fbDB->getFBQ_T1();
AnsiString sql;
AnsiString checkedItems;
AnsiString uID;

  if(usersFrame==NULL || rightsTV==NULL) return;
  uID=usersFrame->getFieldDataOfSelectedRecord("U_ID");

  //Стартуем транзакцию
  fbWQ->getTransaction()->StartTransaction();
  //Удаляем права текущего пользователя из таблица USERS_RIGHTS_TABLE
  sql="delete from USERS_RIGHTS_TABLE where USER_ID="+uID;
  if(fbWQ->execQuery("TUsersRightsFrame::saveRightsKitUser",sql)==false)
  { fbWQ->getTransaction()->Rollback();
    return;
  }
  checkedItems=rightsTV->getCheckedItems();
  if(!checkedItems.IsEmpty())
  { AnsiString subStr;
    int i=1;
    while(!(subStr=getSubstring(checkedItems,';',i)).IsEmpty() && subStr!="0")
    { AnsiString item=getSubstring(subStr,',',1);
      AnsiString cs=getSubstring(subStr,',',2);
      sql="select MI_NAME from MENU_ITEMS_TABLE where MI_ID='"+item+"'";
      if(fbRQ->execQuery("TUsersRightsFrame::saveRightsKit",sql)==true)
      { AnsiString miName=fbRQ->getQuery()->FieldByName("MI_NAME")->AsString;
        if(!miName.IsEmpty())
        { sql="insert into USERS_RIGHTS_TABLE(USER_ID,R_ID,MENU_NAME,CHECKED_STYLE) values(";
          sql+=uID+",";
          sql+="'"+item+"',";
          sql+="'"+miName+"',";
          sql+=cs+")";
          if(fbWQ->execQuery("TUsersRightsFrame::saveRightsKitUser",sql)==false)
          { fbWQ->getTransaction()->Rollback();
            return;
          }
        }
      }
      i++;
    }
  }
  fbWQ->getTransaction()->Commit();
  MessageDlg("Операция успешно завершена",mtInformation,TMsgDlgButtons() << mbOK,0);
}
//---------------------------------------------------------------------------
void __fastcall TUsersRightsFrame::loadRightsKitUser(void)
{
FBDataSet *fbDS=fbDB->getFBDS_T1_T2();
TpFIBDataSet *DS=fbDS->getDataSet();
AnsiString sql;
AnsiString checkedItems="";
AnsiString uID;

  if(usersFrame==NULL || rightsTV==NULL) return;
  rightsTV->unselectAllClick(rightsTV);

  uID=usersFrame->getFieldDataOfSelectedRecord("U_ID");

  sql="select R_ID,CHECKED_STYLE from  USERS_RIGHTS_TABLE where USER_ID="+uID;
  if(fbDS->execSelectSQL("",sql)==false) return;
  DS->First();
  int i=0;
  while(!DS->Eof)
  { AnsiString rID=DS->FieldByName("R_ID")->AsString;
    AnsiString cs=DS->FieldByName("CHECKED_STYLE")->AsString;
    if(i > 0) checkedItems+=";";
    checkedItems+=rID+","+cs;
    DS->Next();
    i++;
  }
  rightsTV->setCheckForItems(checkedItems);
}
//---------------------------------------------------------------------------

//*************
//* published *
//*************
void __fastcall TUsersRightsFrame::uGridMouseDown(TObject *Sender,TMouseButton Button, TShiftState Shift, int X, int Y)
{
  activeFrame=1;
}
//---------------------------------------------------------------------------
void __fastcall TUsersRightsFrame::uSourceDataChange(TObject *Sender,TField *Field)
{
  if(usersFrame==NULL) return;
  if(rightsTV!=NULL)
  { loadRightsKitUser();
    rightsTV->setHeaderCaption("Права пользователя \""+usersFrame->getFieldDataOfSelectedRecord("U_NAME")+"\"");
  }
}
//---------------------------------------------------------------------------
void __fastcall TUsersRightsFrame::tvMouseDown(TObject *Sender,TMouseButton Button,TShiftState Shift,int X,int Y)
{
  activeFrame=2;
}
//---------------------------------------------------------------------------
//Сохранение набора прав пользователя
void __fastcall TUsersRightsFrame::saveRightsKitUserItemClick(TObject *Sender)
{
  saveRightsKitUser();
}
//---------------------------------------------------------------------------

