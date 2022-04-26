//---------------------------------------------------------------------------

#pragma hdrstop
#include "SomeFunctions.h"
#include "RBF.h"
#include "ReferenceBook.h"
#include "DBTreeViewF.h"
#include "UsersWorkFunctions.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "DBGridEh"
//---------------------------------------------------------------------------
//***************************************************
// - ����� ��� ������ � �������������� � �� ������� *
//***************************************************
// -- ����� �������������
bool __fastcall CanDelUser(TComponent* owner,FBDatabase *fbDB,AnsiString id,AnsiString &str)
{
FBQuery * fbRQ=fbDB->getFBQ_T1();
AnsiString sql;

  str=="";
  if(fbRQ==NULL) return false;
  sql="select r.ROLE_NAME from USERS_TABLE u";
  sql+=" left outer join ROLES_TABLE r on r.ID=u.U_ROLE_ID";
  sql+=" where u.U_ID="+id;
  if(fbRQ->execQuery("CanDelUser",sql)==false) return false;
  if(fbRQ->getQuery()->FieldByName("ROLE_NAME")->AsString == "�������������")
    str="������ ������� ������������ \"�������������\"";
  else str="������ ������� ������ � ����� \""+id+"\"";
  return false;
}
//---------------------------------------------------------------------------
AnsiString __fastcall formUsersSelectSQL(bool viewDelRecords)
{
AnsiString sql;

  sql="select * from USERS_TABLE";
  if(getUser()->getURole()!="�������������")
    sql+=" where U_ID="+getUser()->getUID();
  else if(viewDelRecords==false) sql+=" where (DEL_FLG is NULL or DEL_FLG=0)";
  return sql;
}
//---------------------------------------------------------------------------
TRBFrame * __fastcall createUsersFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel)
{
AnsiString tableInfo="USERS_TABLE,������������,U_ID,U_LOGIN,������������,";
AnsiString fieldsInfo;
int role_poz;

  if(getUser()->getURole()=="�������������")
  { fieldsInfo="DEL_FLG,DEL_FLG,,Center,1,1,1,0,;";
    fieldsInfo+="U_ID,U_ID,���,Center,1,1,1,0,;";
    fieldsInfo+="U_NAME,U_NAME,������� �.�.,Left,0,1,1,0,;";
    fieldsInfo+="U_LOGIN,U_LOGIN,���������,Left,0,1,1,0,;";
    fieldsInfo+="U_PASSWORD,U_PASSWORD,������,Left,1,0,0,0,;";
    fieldsInfo+="U_ROLE_ID,U_ROLE_ID,���� ������������,Center,0,1,1,0,;";
    fieldsInfo+="U_MAIL,U_MAIL,����� ����������� �����,Left,0,1,1,0,";
    role_poz=4;
  }
  else
  { fieldsInfo+="U_ID,U_ID,���,Center,1,1,1,0,;";
    fieldsInfo+="U_NAME,U_NAME,������� �.�.,Left,0,1,1,0,;";
    fieldsInfo+="U_LOGIN,U_LOGIN,���������,Left,1,1,1,0,;";
    fieldsInfo+="U_PASSWORD,U_PASSWORD,������,Left,1,0,0,0,;";
    fieldsInfo+="U_ROLE_ID,U_ROLE_ID,���� ������������,Center,1,1,1,0,;";
    fieldsInfo+="U_MAIL,U_MAIL,����� ����������� �����,Left,0,1,1,0,";
    role_poz=3;
  }
AnsiString contentsInfo="";
bool tvViewAble=false,
     needsSearchingForModels=false;

TRBFrame *urbf;

  try
  {
    urbf=new TRBFrame(Owner,iniFN,iniSN,fbDB,tableInfo,fieldsInfo,contentsInfo,tvViewAble,needsSearchingForModels);
  }
  catch (Exception &exc)
  {
    MessageDlg("createUsersFrame - ������ �������� TRBFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
  urbf->Parent=workPanel;
  urbf->gcHeader->Caption="���������� �������������";
  urbf->getRB_GC()->canDelRecord=CanDelUser;
  urbf->formSelectSQL=formUsersSelectSQL;

  //������� ������ �������� ����
  urbf->rbMoveRecord->Visible=false;
  urbf->rbSelectRecord->Visible=false;
  //��������� ������ �������� ����
  urbf->rbSeparator3->Visible=true;
  urbf->rbInputPassword->Visible=true;
  urbf->rbViewPassword->Visible=true;

  if(getUser()->getURole()!="�������������")
  { urbf->rbAddRecord->Visible=false;
    urbf->rbDelRecord->Visible=false;
    urbf->rbUpdate->Visible=false;
    urbf->rbViewDelRecords->Visible=false;
    urbf->rbSeparator2->Visible=false;
    urbf->rbViewPassword->Visible=false;
  }
  //����������� �������� ������ ���� ������������
  AnsiString sql;
  sql="select * from ROLES_TABLE";
  FBDataSet *fbDS=fbDB->getFBDS_T1_T2();
  TpFIBDataSet *DS=fbDS->getDataSet();
  if(fbDS->execSelectSQL("createUsersFrame",sql)==true)
  { urbf->getRB_GC()->getGridFrame()->workGrid->Columns->Items[role_poz]->KeyList->Clear();
    urbf->getRB_GC()->getGridFrame()->workGrid->Columns->Items[role_poz]->PickList->Clear();
    DS->First();
    while(!DS->Eof)
    { urbf->getRB_GC()->getGridFrame()->workGrid->Columns->Items[role_poz]->KeyList->Add(DS->FieldByName("ID")->AsString);
      urbf->getRB_GC()->getGridFrame()->workGrid->Columns->Items[role_poz]->PickList->Add(DS->FieldByName("ROLE_NAME")->AsString);
      DS->Next();
    }
    DS->Close();
  }
  return urbf;
}
//---------------------------------------------------------------------------
// -- ����� � ������� ���� �������������
TDBTreeViewFrame * __fastcall createUsersTVFrame(TComponent* Owner,AnsiString iniFN,FBDatabase *fbDB,TPanel *wPanel)
{
TDBTreeViewFrame *urTV;
AnsiString structTableInfo="MENU_ITEMS_TABLE,����� �������������,MI_ID,MI_TITLE,2";

  try
  {
    urTV=new TDBTreeViewFrame(Owner,wPanel,iniFN,fbDB,structTableInfo);
  }
  catch (Exception &exc)
  { MessageDlg("������ �������� ������� TDBTreeViewFrame"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
//  urTV->DBTreeViewHeader->ColorFrom=clInactiveCaption;
//  urTV->DBTreeViewHeader->ColorTo=clWhite;
//  urTV->DBTreeViewHeader->Font->Color=clNavy;
  return urTV;
}
//---------------------------------------------------------------------------
