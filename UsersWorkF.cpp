//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "PLACEMNT.HPP"
#include "DifferentFunctions.h"
#include "SomeFunctions.h"
#include "DBClasses.h"
#include "RBF.h"
#include "DBTreeViewF.h"
#include "RBWorksF.h"
#include "UsersWorkFunctions.h"
#include "UsersWorkF.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Placemnt"
#pragma link "FIBDataSet"
#pragma link "pFIBDataSet"
#pragma resource "*.dfm"
//TUsersWorkFrame *UsersWorkFrame;
//---------------------------------------------------------------------------
//**********
//* public *
//**********
__fastcall TUsersWorkFrame::TUsersWorkFrame(TComponent* owner,AnsiString iniFN,FBDatabase *fbdb)
        : TFrame(owner),Owner(owner),iniFileName(iniFN),fbDB(fbdb)
{
  iniSectionName="UsersWorkFrame_"+getUser()->getURole();
  usersFrame=NULL;
  rightsTV=NULL;
}
//---------------------------------------------------------------------------
__fastcall TUsersWorkFrame::~TUsersWorkFrame(void)
{
  getUser()->getDataFromUsersTable(getUser()->getUID());
  saveProperties();
  if(usersFrame!=NULL){ delete usersFrame; usersFrame=NULL; }
  if(rightsTV!=NULL){ delete rightsTV; rightsTV=NULL; }
}
//---------------------------------------------------------------------------
void __fastcall TUsersWorkFrame::initFrame(void)
{
  loadProperties();
  usersFrame=createUsersFrame(this,iniFileName,"UsersFrame_"+getUser()->getURole(),fbDB,leftPanel);
  if(usersFrame!=NULL)
  { usersFrame->getRB_GC()->getGridFrame()->workGrid->OnMouseDown=uGridMouseDown;
    usersFrame->getRB_GC()->getGridFrame()->workSource->OnDataChange=uSourceDataChange;
    usersFrame->loadDS();
    usersFrame->getRB_GC()->getGridFrame()->getWorkDataSet()->Locate("U_ID",userID,TLocateOptions().Clear());
  }
  else return;
  AnsiString role=getUser()->getURole().UpperCase();
  if(role=="АДМИНИСТРАТОР")
  { rightsTV=createUsersTVFrame(this,iniFileName,fbDB,rightPanel);
    if(rightsTV!=NULL)
    { rightsTV->initTreeViewFrame();
      rightsTV->dbTreeView->OnMouseDown=tvMouseDown;
      rightsTV->saveItem->OnClick=saveUserRightsClick;
      loadRightsKitUser();
    }
    else return;
  }
  else
  { rightPanel->Visible=false;
    splitter->Visible=false;
    leftPanel->Align=alClient;
  }
}
//---------------------------------------------------------------------------
void __fastcall TUsersWorkFrame::setFocus(void)
{
  if(activeFrame==1)
  {
    if(usersFrame!=NULL) usersFrame->setFocus();
  }
  else
  {
    if(rightsTV!=NULL) rightsTV->dbTreeView->SetFocus();
  }
}
//---------------------------------------------------------------------------
//Загрузка прав текущего пользователя
void __fastcall TUsersWorkFrame::loadRightsKitUser(void)
{
  if(usersFrame==NULL || userID.IsEmpty()) return;
  userID=usersFrame->getRB_GC()->getGridFrame()->getFieldDataOfSelectedRecord("U_ID");

  if(rightsTV==NULL) return;
  rightsTV->unselectAll->Click();
  fillTV(userID);
}
//---------------------------------------------------------------------------

//***********
//* private *
//***********
void __fastcall TUsersWorkFrame::loadProperties(void)
{
TIniFile *ini = new TIniFile(iniFileName);

  leftPanel->Width=ini->ReadInteger(iniSectionName,"leftPanel_Width",200);
  activeFrame=ini->ReadInteger(iniSectionName,"activeFrame",1);
  userID=ini->ReadString(iniSectionName,"userID",1);
  delete ini;
}
//---------------------------------------------------------------------------
void __fastcall TUsersWorkFrame::saveProperties(void)
{
TIniFile *ini = new TIniFile(iniFileName);

  ini->WriteInteger(iniSectionName,"leftPanel_Width",leftPanel->Width);
  ini->WriteInteger(iniSectionName,"activeFrame",activeFrame);
  ini->WriteString(iniSectionName,"userID",userID);
  delete ini;
}
//---------------------------------------------------------------------------
void __fastcall TUsersWorkFrame::saveRightsKitUser(void)
{
FBQuery *fbWQ=fbDB->getFBQ_T2();
FBQuery *fbRQ=fbDB->getFBQ_T1();
AnsiString sql;
AnsiString checkedItems;
//AnsiString uID;

  if(usersFrame==NULL || rightsTV==NULL) return;
//  uID=usersFrame->getRB_GC()->getGridFrame()->getFieldDataOfSelectedRecord("U_ID");

  //Стартуем транзакцию
  fbWQ->getTransaction()->StartTransaction();
  //Удаляем права текущего пользователя из таблица USERS_RIGHTS_TABLE
  sql="delete from USERS_RIGHTS_TABLE where USER_ID="+userID;
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
          sql+=userID+",";
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
  rightsTV->setChangeFLG(false);
  MessageDlg("Операция успешно завершена",mtInformation,TMsgDlgButtons() << mbOK,0);
}
//---------------------------------------------------------------------------
//Заполнение дерева пометками
void __fastcall TUsersWorkFrame::fillTV(AnsiString id)
{
FBDataSet *fbDS=fbDB->getFBDS_T1_T2();
TpFIBDataSet *DS=fbDS->getDataSet();
AnsiString sql;
AnsiString checkedItems="";

  sql="select R_ID,CHECKED_STYLE from  USERS_RIGHTS_TABLE where USER_ID="+id;
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
  rightsTV->setChangeFLG(false);
}
//---------------------------------------------------------------------------

//*************
//* published *
//*************
void __fastcall TUsersWorkFrame::uGridMouseDown(TObject *Sender,TMouseButton Button, TShiftState Shift, int X, int Y)
{
  activeFrame=1;
}
//---------------------------------------------------------------------------
void __fastcall TUsersWorkFrame::tvMouseDown(TObject *Sender,TMouseButton Button,TShiftState Shift,int X,int Y)
{
  activeFrame=2;
}
//---------------------------------------------------------------------------
void __fastcall TUsersWorkFrame::uSourceDataChange(TObject *Sender,TField *Field)
{
  if(usersFrame==NULL) return;
  if(rightsTV!=NULL)
  { if(rightsTV->getChangeFLG()==true)
    { if(MessageDlg("Внимание! Последние изменения прав пользователя не сохранены.\nСохранить изменения?",mtInformation,TMsgDlgButtons() << mbOK<<mbCancel,0)==mrOk)
        saveRightsKitUser();
      rightsTV->setChangeFLG(false);
    }
    userID=usersFrame->getRB_GC()->getGridFrame()->getFieldDataOfSelectedRecord("U_ID");
    loadRightsKitUser();
    rightsTV->setHeaderCaption("Права пользователя \""+usersFrame->getRB_GC()->getGridFrame()->getFieldDataOfSelectedRecord("U_NAME")+"\"");
  }
}
//---------------------------------------------------------------------------
//Сохранение прав пользователя
void __fastcall TUsersWorkFrame::saveUserRightsClick(TObject *Sender)
{
  saveRightsKitUser();
  (static_cast<TRBWorksFrame*>(Owner))->initSelectBar();
  (static_cast<TRBWorksFrame*>(Owner))->RBSelectBar->Repaint();
}
//---------------------------------------------------------------------------

