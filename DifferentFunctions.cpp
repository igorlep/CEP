//---------------------------------------------------------------------------

#include "DBClasses.h"
#include <math.h>
#pragma hdrstop
#include "GUID.h"
#include "Propis.h"
#include "DifferentFunctions.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "DBGridEh"
//---------------------------------------------------------------------------
//������� ����������� ���������� ��� ������
unsigned short __fastcall lastDayOfMonth(unsigned short y, unsigned short m)
{
unsigned short ld=31;

  if(m==4 || m==6 || m==9 || m==11) ld=30;
  else if(m==2)
  { if(y%4) ld=28;
    else ld=29;
  }
  return ld;
}
//---------------------------------------------------------------------------
//������ ������
TDateTime __fastcall beginOfMonth(TDateTime date)
{
unsigned short y,m,d;

  date.DecodeDate(&y,&m,&d);
  return TDateTime(y,m,1);
}
//---------------------------------------------------------------------------
//����� ������
TDateTime __fastcall endOfMonth(TDateTime date)
{
unsigned short y,m,d;

  date.DecodeDate(&y,&m,&d);
  return TDateTime(y,m,lastDayOfMonth(y,m));
}
//---------------------------------------------------------------------------
//��������� ������� ��� ������ � �����
AnsiString __fastcall getMonthYearString(TDateTime date)
{
unsigned short y,m,d;
char *mes[12]={"������","�������","����","������","���","����","����","������","��������","�������","������","�������"};

  if(int(date)==0) return "";
  date.DecodeDate(&y,&m,&d);
  return AnsiString(mes[m-1])+" "+IntToStr(y)+" ����";
}
//---------------------------------------------------------------------------
TDateTime __fastcall getDateFromMonthYearString(AnsiString month_yearStr)
{
unsigned short y,m,d;
char *mes[12]={"������","�������","����","������","���","����","����","������","��������","�������","������","�������"};
int p;

  if(month_yearStr==NULL || month_yearStr.IsEmpty()) return 0;
  for(p=0; p<12; ++p) if(month_yearStr.Pos(mes[p]) != 0) break;
  if(p < 12)
  { m=p+1;
    int poz=month_yearStr.Pos(" ")+1;
    AnsiString year=month_yearStr.SubString(poz,month_yearStr.Length()-poz);
    year=year.SubString(1,year.Pos(" ")-1);
    y=StrToInt(year);
    return TDateTime(y,m,1);
  }
  else return 0;
}
//---------------------------------------------------------------------------

//������� ���������� ��������� �� ������ � �������������
AnsiString __fastcall getSubstring(AnsiString str, char r, int n)
//str - ������ � �������������
//r - ������-����������� ��������
//n - ����� ����������� ���������
{
int i=1, j=1;

  if(str.IsEmpty()) return "";
  while(j<=str.Length() && i<n)
  { if(str[j]==r) i++;
    j++;
  }
  AnsiString s=str.SubString(j,str.Length());
  if(s.IsEmpty()) return NULL;
  if(s.Pos(AnsiString(r))==0) return s;
  return s.SubString(1,s.Pos(AnsiString(r))-1);
}
//---------------------------------------------------------------------------

//********************************
//* ������� ��� ������ � ������� *
//********************************
// - ��������� ������ �������� ��������� ���� ��� tableName
AnsiString __fastcall getNewKeyValue(FBDatabase * fb_db, AnsiString tableName)
{
AnsiString sql;
FBQuery * fbWQ=fb_db->getFBQ_T2();

  sql="select GEN_ID(GEN_"+tableName+"_ID,1) as ID from RDB$DATABASE";
  if(fbWQ->execQuery("getNewRB_KeyValue:",sql)==false) return "";
  return fbWQ->getQuery()->FieldByName("ID")->AsString;
}
//---------------------------------------------------------------------------
AnsiString __fastcall getNewKeyValue(FBQuery * fbWQ, AnsiString tableName)
{
AnsiString sql;

  sql="select GEN_ID(GEN_"+tableName+"_ID,1) as ID from RDB$DATABASE";
  if(fbWQ->execQuery("getNewRB_KeyValue:",sql)==false) return "";
  return fbWQ->getQuery()->FieldByName("ID")->AsString;
}
//---------------------------------------------------------------------------
void __fastcall setGenerator(FBQuery *fbWQ,AnsiString tableName,AnsiString genName,AnsiString keyName)
{
AnsiString sql;

  //�������� ����������
  sql="select max("+keyName+") MAX_ID from "+tableName;
  if(fbWQ->execQuery("DifferentFunctions::setGenerator",sql)==true)
  { int id=fbWQ->getQuery()->FieldByName("MAX_ID")->AsInteger;
    sql="set generator "+genName+" to "+IntToStr(id);
    fbWQ->execQuery("DifferentFunctions::setGenerator",sql);
  }
}
//---------------------------------------------------------------------------

// - ��������� �������� ���� fieldName ������� tableName �� �������� keyData ��������� ���� keyFieldName
/*AnsiString __fastcall getDataForFieldName(FBDatabase * fb_db,AnsiString tableName,AnsiString keyFieldName,AnsiString keiData, AnsiString fieldName)
{
AnsiString fieldData="";
FBQuery *fbRQ=fb_db->getFBQ_T1();
AnsiString sql;

  sql="select "+FieldName+" from "+tableName+" where "+keyFieldName+"='"+keyData+"'";
  if(fbRQ->execQuery("",sql)==false) return fieldData;
  fieldData=fbRQ->getQuery()->FieldByName(fieldName)->AsString;
  fbRQ->getQuery()->Close();
  return fieldData;
}
//---------------------------------------------------------------------------
*/

//*****************************************
//* ������ ��� ������ � �������� �������� *
//*****************************************
//������ ������������ ���������
AnsiString __fastcall getConstName(TComponent* owner,FBDatabase * fb_db,AnsiString identifier)
{
AnsiString sql;
FBQuery * fbRQ=newFBQuery(owner,"getConstName",fb_db->getDatabase(),true);
AnsiString constName="";

  if(fbRQ!=NULL)
  { sql="select CONST_NAME from CONSTANTS_TABLE where IDENTIFIER='"+identifier+"'";
    if(fbRQ->execQuery("getConstTitle",sql)==true && fbRQ->getQuery()->RecordCount > 0)
      constName=fbRQ->getQuery()->FieldByName("CONST_NAME")->AsString;
    delete fbRQ;
  }
  return constName;
}
//---------------------------------------------------------------------------
//������ ��������� ���������
AnsiString __fastcall getConstTitle(TComponent* owner,FBDatabase * fb_db,AnsiString identifier)
{
AnsiString sql;
FBQuery * fbRQ=newFBQuery(owner,"getConstTitle",fb_db->getDatabase(),true);
AnsiString constTitle="";

  if(fbRQ!=NULL)
  { sql="select CONST_TITLE from CONSTANTS_TABLE where IDENTIFIER='"+identifier+"'";
    if(fbRQ->execQuery("getConstTitle",sql)==true && fbRQ->getQuery()->RecordCount > 0)
      constTitle=fbRQ->getQuery()->FieldByName("CONST_TITLE")->AsString;
    delete fbRQ;
  }
  return constTitle;
}
//---------------------------------------------------------------------------
//������ �������� ���������
AnsiString __fastcall getConstValue(TComponent* owner,FBDatabase * fb_db,AnsiString identifier,TDateTime date)
{
AnsiString sql;
FBQuery * fbRQ=newFBQuery(owner,"getConstValue",fb_db->getDatabase(),true);
AnsiString constValue="";
TDateTime constDT=date;

  if(fbRQ!=NULL)
  { //�������� ��� ���������
    sql="select * from CONSTANTS_TABLE where IDENTIFIER='"+identifier+"'";
    if(fbRQ->execQuery("getConstTitle",sql)==true && fbRQ->getQuery()->RecordCount > 0)
    { AnsiString id=fbRQ->getQuery()->FieldByName("ID")->AsString;
      int constType=fbRQ->getQuery()->FieldByName("CONST_TYPE")->AsInteger;
      if(int(constDT)==0) constDT=DATE();
      sql="select CONST_VALUE from GET_SIMPLE_CONST_VALUE(";
      sql+="'"+identifier+"',";
      sql+="'"+constDT.DateString()+"')";
      if(fbRQ->execQuery("getConstValue",sql)==true &&  fbRQ->getQuery()->RecordCount > 0)
      { constValue=fbRQ->getQuery()->FieldByName("CONST_VALUE")->AsString;
        if(constType==1) //���������-������
        { AnsiString refTableName=fbRQ->getQuery()->FieldByName("REF_TABLE_NAME")->AsString;
          AnsiString refKeyName=fbRQ->getQuery()->FieldByName("REF_KEY_FIELD_NAME")->AsString;
          AnsiString retFieldName=fbRQ->getQuery()->FieldByName("RET_FIELD_NAME")->AsString;
          //������ �������� ���������
          sql="select "+retFieldName+" from "+refTableName+" where "+refKeyName+"='"+constValue+"'";
          if(fbRQ->execQuery("getConstValue",sql)==true &&  fbRQ->getQuery()->RecordCount > 0)
            constValue=fbRQ->getQuery()->FieldByName("CONST_VALUE")->AsString;
        }
      }
    }
    delete fbRQ;
  }
  return constValue;
}
//---------------------------------------------------------------------------
//������ ��� ���������
//0 - ������� ��������� (CONST_VALUES - �������� �������� ���� ���������),
//1 - ���������-������ (CONST_VALUES - �������� �������� ��������� � ������ �� ������ �� ������ ������� � ���� ���������),
//2 - CONST_VALUES - �������� �������� ��������� � ������ �� ������ �� ������� CONST_VALUES, � ������� �������� ���������.
int __fastcall getConstType(TComponent* owner,FBDatabase * fb_db,AnsiString identifier)
{
AnsiString sql;
FBQuery * fbRQ=newFBQuery(owner,"getConstValue",fb_db->getDatabase(),true);
int const_type;

  if(fbRQ!=NULL)
  { sql="select CONST_TYPE from CONSTANTS_TABLE where IDENTIFIER='"+identifier+"'";
    if(fbRQ->execQuery("getConstType",sql)==true && fbRQ->getQuery()->RecordCount > 0)
      const_type=fbRQ->getQuery()->FieldByName("CONST_TYPE")->AsInteger;
    else const_type=-1;
  }
  delete fbRQ;
  return const_type;
}
//---------------------------------------------------------------------------
//������ ��� ���������
//0 - ���������������, 1 - �������������
int __fastcall getConstKind(TComponent* owner,FBDatabase * fb_db,AnsiString identifier)
{
AnsiString sql;
FBQuery * fbRQ=newFBQuery(owner,"getConstValue",fb_db->getDatabase(),true);
int const_kind;

  if(fbRQ!=NULL)
  { sql="select CONST_KIND from CONSTANTS_TABLE where IDENTIFIER='"+identifier+"'";
    if(fbRQ->execQuery("getConstType",sql)==true && fbRQ->getQuery()->RecordCount > 0)
      const_kind=fbRQ->getQuery()->FieldByName("CONST_KIND")->AsInteger;
    else const_kind=-1;
  }
  delete fbRQ;
  return const_kind;
}
//---------------------------------------------------------------------------

//������������� �������� ������� ���������
void  __fastcall setConstValue(FBDatabase * fb_db,AnsiString identifer, AnsiString constValue)
{
AnsiString sql;
FBQuery * fbWQ=fb_db->getFBQ_T2();

  sql="update CONST_VALUES set CONST_VALUE='"+constValue+"'";
  sql+=" where CONST_ID=(select ID from CONSTANTS_TABLE where IDENTIFIER='"+identifer+"')";
  fbWQ->execQuery("setConstValue",sql);
}
//---------------------------------------------------------------------------
//������������� �������� ������������ ���������
bool  __fastcall setPeriodicalConstValue(FBDatabase * fb_db,AnsiString const_id,int &value_id,AnsiString constValue,TDateTime constDate)
{
AnsiString sql;
FBQuery * fbWQ=fb_db->getFBQ_T2();
bool ret=false;

  if(constValue.Length()>240) constValue.SetLength(240);
  fbWQ->getTransaction()->StartTransaction();
  if(value_id==0)  //����� �������� ���������
  { AnsiString id=getNewKeyValue(fbWQ,"PERIODICAL_CONST_VALUES");
    if(id.IsEmpty())
    { MessageDlg("������ ������������� ��������� ���� ������� PERIODICAL_CONST_VALUES",mtError,TMsgDlgButtons() << mbOK,0);
      fbWQ->getTransaction()->Rollback();
      return ret;
    }
    sql="insert into PERIODICAL_CONST_VALUES(ID,CONST_ID,CONST_VALUE,CONST_DATE) values(";
    sql+=id+",";
    sql+=const_id+",";
    sql+="'"+constValue+"',";
    sql+="'"+constDate.DateString()+"')";
    if(fbWQ->execQuery("setPeriodicalConstValue",sql)==false)
    { fbWQ->getTransaction()->Rollback();
      return ret;
    }
    fbWQ->getTransaction()->Commit();
    value_id=StrToInt(id);
    ret=true;
  }
  else
  { sql="update PERIODICAL_CONST_VALUES set CONST_VALUE='"+constValue+"',";
    sql+="CONST_DATE='"+constDate.DateString()+"'";
    sql+=" where ID="+IntToStr(value_id);
    if(fbWQ->execQuery("TPeriodicalConstantEditForm::SaveButtonClick",sql)==false)
    { fbWQ->getTransaction()->Rollback();
      return ret;
    }
    fbWQ->getTransaction()->Commit();
    ret=true;
  }
  return ret;
}
//---------------------------------------------------------------------------
//�������� �������� �������������� ���������
void  __fastcall delPeriodicalConstValue(FBDatabase * fb_db,AnsiString value_id)
{
AnsiString sql;
FBQuery * fbWQ=fb_db->getFBQ_T2();

  sql="delete from PERIODICAL_CONST_VALUES where ID="+value_id;
  fbWQ->execQuery("delPeriodicalConstValue(",sql);
}
//---------------------------------------------------------------------------
//�������� �������� �������������� ���������
AnsiString __fastcall getPeriodicalParamValue(FBDatabase * fb_db,AnsiString tableInfo,AnsiString keyID,TDateTime forDate)
{
AnsiString sql;
FBQuery * fbRQ=fb_db->getFBQ_T1();
AnsiString tableName=getSubstring(tableInfo,',',1);
AnsiString paramIDName=getSubstring(tableInfo,',',2);
AnsiString keyName=getSubstring(tableInfo,',',3);
AnsiString dateName=getSubstring(tableInfo,',',4);
AnsiString paramValueName=getSubstring(tableInfo,',',5);

  sql="select "+paramValueName+" from "+tableName+" where "+keyName+"='"+keyID+"'";
  sql+=" and "+dateName+"<='"+forDate.DateString()+"'";
  sql+=" order by "+dateName+" desc";
  if(fbRQ->execQuery("getPeriodicalParamValue",sql)==false) return "";
  return fbRQ->getQuery()->FieldByName(paramValueName)->AsString;
}
//---------------------------------------------------------------------------

//������� �������������� ����� � ������ ��������
// - ������� �������������� ������������� ����� � ������������ �������� ���. � ���.
AnsiString f_to_str(double d)
{
  return Propis(d).DoublePropis();
}
//---------------------------------------------------------------------------
// - ������� �������������� ������ � ������������ ��������
AnsiString i_to_str(int i)
{
  return Propis(double(i)).IntPropis();
}
//---------------------------------------------------------------------------

//�������, ������������ ���������� ������������� - GUID ������ 32 �����
AnsiString __fastcall genGUID(void)
{
  return guid().genGUID();
}
//---------------------------------------------------------------------------

//������� ������������ ������ ���������
AnsiString SetHeader(AnsiString str, TDateTime date1, TDateTime date2)
{
unsigned short y1,m1,d1,y2,m2,d2;

  str+=" �� ";
  if(date1==date2) str+=date1.DateString();
  else
  { date1.DecodeDate(&y1,&m1,&d1);
    date2.DecodeDate(&y2,&m2,&d2);
    if(y1==y2)
    { if(m1==m2)
      { if(d1==1 && d2==lastDayOfMonth(y2,m2))
        { char *mn[12]={"������","�������","����","������","���","����","����","������","��������","�������","������","�������"};
          str+=AnsiString(mn[m1-1])+" "+IntToStr(y1)+" ����";
        }
        else str+="������ � "+date1.DateString()+" �� "+date2.DateString();
      }
      else if(m1==1 && m2==3)
      { if(d1==1 && d2==lastDayOfMonth(y2,m2)) str+="������ ������� "+IntToStr(y1)+" ����";
        else str+="������ � "+date1.DateString()+" �� "+date2.DateString();
      }
      else if(m1==1 && m2==6)
      { if(d1==1 && d2==lastDayOfMonth(y2,m2)) str+="������ ��������� "+IntToStr(y1)+" ����";
        else str+="������ � "+date1.DateString()+" �� "+date2.DateString();
      }
      else if(m1==4 && m2==6)
      { if(d1==1 && d2==lastDayOfMonth(y2,m2)) str+="������ ������� "+IntToStr(y1)+" ����";
        else str+="������ � "+date1.DateString()+" �� "+date2.DateString();
      }
      else if(m1==7 && m2==9)
      { if(d1==1 && d2==lastDayOfMonth(y2,m2)) str+="������ ������� "+IntToStr(y1)+" ����";
        else str+="������ � "+date1.DateString()+" �� "+date2.DateString();
      }
      else if(m1==10 && m2==12)
      { if(d1==1 && d2==lastDayOfMonth(y2,m2)) str+="��������� ������� "+IntToStr(y1)+" ����";
        else str+="������ � "+date1.DateString()+" �� "+date2.DateString();
      }
      else if(m1==7 && m2==12)
      { if(d1==1 && d2==lastDayOfMonth(y2,m2)) str+="������ ��������� "+IntToStr(y1)+" ����";
        else str+="������ � "+date1.DateString()+" �� "+date2.DateString();
      }
      else if(m1==1 && m2==12)
      { if(d1==1 && d2==lastDayOfMonth(y2,m2)) str+=IntToStr(y1)+" ���";
        else str+="������ � "+date1.DateString()+" �� "+date2.DateString();
      }
      else str+="������ � "+date1.DateString()+" �� "+date2.DateString();
    }
    else str+="������ � "+date1.DateString()+" �� "+date2.DateString();
  }
  return str;
}
//---------------------------------------------------------------------------

