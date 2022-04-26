//---------------------------------------------------------------------------

#pragma hdrstop
#include "GridF.h"
#include "DBTreeViewF.h"
#include "UsersRightsFunctions.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "DBGridEh"
//---------------------------------------------------------------------------
//**********************************
// - ����� � ������� ������������� *
//**********************************
// -- ����� �������������
AnsiString __fastcall formUsersRSelectSQL(void)
{
AnsiString sql="";

  sql="select u.U_ID,u.U_NAME,u.U_LOGIN,r.ROLE_NAME";
  sql+=" from USERS_TABLE u";
  sql+=" left outer join ROLES_TABLE r on r.ID=u.U_ROLE_ID";
  sql+=" where (u.DEL_FLG is NULL or u.DEL_FLG=0)";

  return sql;
}
//---------------------------------------------------------------------------
TGridFrame * __fastcall createUsersRFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel)
{
TGridFrame *u_f=NULL;
AnsiString tableInfo;
AnsiString fieldsInfo;

  tableInfo="USERS_TABLE,������� �������������,U_ID,U_NAME,,";

  fieldsInfo= "u.U_ID,U_ID,���,Center,1,1,1,0,;";
  fieldsInfo+="u.U_NAME,U_NAME,���,Left,1,1,1,0,;";
  fieldsInfo+="u.U_LOGIN,U_LOGIN,�����,Left,1,1,1,0,;";
  fieldsInfo+="r.ROLE_NAME,ROLE_NAME,����,Left,1,1,1,0,";

  try
  {
    u_f=new TGridFrame(Owner,iniFN,iniSN,fbDB,tableInfo,fieldsInfo);
  }
  catch (Exception &exc)
  { MessageDlg("createUsersRFrame - ������ �������� TGridFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
  if(u_f!=NULL)
  { u_f->initFrame();
    u_f->Parent=workPanel;
    u_f->setColumnsConfigFieldEditInGridVisible(false);
  }
  return u_f;
}
//---------------------------------------------------------------------------
// -- ����� � ������� ���� �������������
TDBTreeViewFrame * __fastcall createTVFrame(TComponent* Owner,AnsiString iniFN,FBDatabase *fbDB,TPanel *workPanel)
{
TDBTreeViewFrame *urTV;
AnsiString structTableInfo="MENU_ITEMS_TABLE,����� �������������,MI_ID,MI_TITLE,2";

  try
  {
    urTV=new TDBTreeViewFrame(Owner,workPanel,iniFN,fbDB,structTableInfo);
  }
  catch (Exception &exc)
  { MessageDlg("������ �������� ������� TDBTreeViewFrame"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
  urTV->DBTreeViewHeader->ColorFrom=clWhite;
  urTV->DBTreeViewHeader->ColorTo=clOlive;
  urTV->DBTreeViewHeader->Font->Color=clTeal;
  return urTV;
}
//---------------------------------------------------------------------------
