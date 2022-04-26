//---------------------------------------------------------------------------
#include <math.h>
#pragma hdrstop
#include "DBClasses.h"
//#include "DifferentFunctions.h"
//#include "SomeFunctions.h"
#include "GridF.h"
#include "ViewNMPBySDFunctions.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
//**************************************
//* ������������ ��� �� �������������� *
//**************************************
// - ������� ������������ select sql �������
AnsiString __fastcall formOrderingNMPBySDSelectSQL(bool groupOnPublishing,AnsiString id,TDateTime date1,TDateTime date2)
{
AnsiString sql="";

  if(groupOnPublishing==false)
  { sql="select s.sd_id,s.sd_name,f.fact_q";
    sql+=" from facts_table f,subdivisions_table s";
    sql+=" where s.sd_id=f.sd_id and f.e_id="+id;
  }
  else
  { sql="select s.sd_id,s.sd_name,sum(f.fact_q) as fact_q";
    sql+=" from facts_table f,subdivisions_table s,exits_table e";
    sql+=" where s.sd_id=f.sd_id and e.e_id=f.e_id";
    sql+=" and e.nmp_id="+id;
    sql+=" and e.e_date between '"+date1.DateString()+"' and '"+date2.DateString()+"'";
    sql+=" group by s.sd_id,s.sd_name";
  }
  return sql;
}
//---------------------------------------------------------------------------
// - ������� �������� ������
class TGridFrame * __fastcall createOrderingViewNMPBySDFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel,bool groupOnPublishing)
{
TGridFrame* gf=NULL;
AnsiString tableInfo;
AnsiString fieldsInfo;
AnsiString footerFieldsInfo;

  tableInfo="SUBDIVISIONS_TABLE,������������ ��� �� ��������������,SD_ID,SD_NAME,,";

  if(groupOnPublishing==false)
  { fieldsInfo ="s.SD_ID,SD_ID,���,Center,1,1,1,0,;";
    fieldsInfo+="s.SD_NAME,SD_NAME,�������������,Left,1,1,1,0,;";
    fieldsInfo+="f.FACT_Q,FACT_Q,�����������,Right,1,1,0,0,#0";
  }
  else
  { fieldsInfo ="s.SD_ID,SD_ID,���,Center,1,1,1,0,;";
    fieldsInfo+="s.SD_NAME,SD_NAME,�������������,Left,1,1,1,0,;";
    fieldsInfo+="sum(f.FACT_Q),FACT_Q,�����������,Right,1,1,0,0,#0";
  }
  footerFieldsInfo ="FACT_Q,fvtSum,#0,,Right";
  try
  {
    gf=new TGridFrame(Owner,iniFN,iniSN,fbDB,tableInfo,fieldsInfo,footerFieldsInfo);
  }
  catch (Exception &exc)
  { MessageDlg("createOrderingViewNMPBySDFrame - ������ �������� TGridFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
  if(gf!=NULL)
  { gf->initFrame();
    gf->Parent=workPanel;
  }

  return gf;
}
//---------------------------------------------------------------------------
//*********************************
//* ������� ��� �� �������������� *
//*********************************
AnsiString __fastcall formReturnNMPBySDSelectSQL(AnsiString eid)
{
AnsiString sql="";

  sql="select s.sd_id,s.sd_name,f.fact_q,f.return_q";
  sql+=" from facts_table f,subdivisions_table s,exits_table e";
  sql+=" where s.sd_id=f.sd_id and e.e_id=f.e_id";
  sql+=" and e.e_id="+eid;

  return sql;
}
//---------------------------------------------------------------------------
// - ������� �������� ������
class TGridFrame * __fastcall createViewReturnNMPBySDFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel)
{
TGridFrame* gf=NULL;
AnsiString tableInfo;
AnsiString fieldsInfo;
AnsiString footerFieldsInfo;

  tableInfo="SUBDIVISIONS_TABLE,������������ ��� �� ��������������,SD_ID,SD_NAME,,";

  fieldsInfo ="s.SD_ID,SD_ID,���,Center,1,1,1,0,;";
  fieldsInfo+="s.SD_NAME,SD_NAME,�������������,Left,1,1,1,0,;";
  fieldsInfo+="f.FACT_Q,FACT_Q,�����������,Right,1,1,0,0,#0;";
  fieldsInfo+="f.RETURN_Q,RETURN_Q,���������� �� �����,Right,1,1,0,0,#0";

  footerFieldsInfo ="FACT_Q,fvtSum,#0,,Right;";
  footerFieldsInfo+="RETURN_Q,fvtSum,#0,,Right";

  try
  {
    gf=new TGridFrame(Owner,iniFN,iniSN,fbDB,tableInfo,fieldsInfo,footerFieldsInfo);
  }
  catch (Exception &exc)
  { MessageDlg("createViewReturnNMPBySDFrame - ������ �������� TGridFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
  if(gf!=NULL)
  { gf->initFrame();
    gf->Parent=workPanel;
  }

  return gf;
}
//---------------------------------------------------------------------------

