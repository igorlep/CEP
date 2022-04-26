//---------------------------------------------------------------------------
//���� - DBClasses.cpp
//******************************************************************
// ������� � ������ ��� ������ � ����� ������ Firebird (Interbase) *
//******************************************************************

#include "SecretWord.h"
#pragma hdrstop
#include "DBClasses.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

//**********
// ������� *
//**********
//������� �������� ����������
TpFIBTransaction * newTransaction(TComponent *Owner, AnsiString func,TpFIBDatabase *db)
{
TpFIBTransaction *t;

  try
  {
    t=new TpFIBTransaction(Owner);
  }
  catch (Exception &exc)
  { MessageDlg(func+": ������ �������� ������� Transaction"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
  t->DefaultDatabase=db;
  return t;
}
//---------------------------------------------------------------------------
//������� �������� TpFIBQuery
TpFIBQuery * newQuery(TComponent *Owner, AnsiString func,TpFIBDatabase *db,TpFIBTransaction *t,bool forR)
{
TpFIBQuery *q;

  try
  {
    q=new TpFIBQuery(Owner);
  }
  catch (Exception &exc)
  { MessageDlg(func+": ������ �������� ������� TpFIBQuery"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
  q->Database=db;
  q->Transaction=t;
  if(forR==true) q->Options << qoStartTransaction << qoAutoCommit;
  return q;
}
//---------------------------------------------------------------------------
//������� ���������� ������� Query
bool execQuery(AnsiString func,AnsiString sql,TpFIBQuery *query)
{
bool commitFLG=false;

  query->Close();
  query->SQL->Clear();
  query->SQL->Add(sql);

  if(!query->Options.Contains(qoStartTransaction) && query->Transaction->InTransaction==false)
  { commitFLG=true;
    query->Transaction->StartTransaction();
  }
  try
  {
    query->ExecQuery();
  }
  catch (Exception &exc)
  { MessageDlg(func+": ������ - "+sql+"\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    if(query->Transaction->InTransaction && commitFLG) query->Transaction->Rollback();
    return false;
  }
  if(query->Transaction->InTransaction && commitFLG) query->Transaction->Commit();
  return true;
}
//---------------------------------------------------------------------------
//������� �������� TpFIBDataSet
TpFIBDataSet * newDataSet(TComponent *Owner, AnsiString func,TpFIBDatabase *db,TpFIBTransaction *rt,TpFIBTransaction *ut)
{
TpFIBDataSet *DS;

  try
  {
    DS=new TpFIBDataSet(Owner);
  }
  catch (Exception &exc)
  { MessageDlg(func+": ������ �������� ������� TpFIBDataSet\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
  DS->Database=db;
  DS->Transaction=rt;
  DS->UpdateTransaction=ut;
  DS->AutoCommit=true;
  return DS;
}
//---------------------------------------------------------------------------
TpFIBDataSet * newDataSet(TComponent *Owner,AnsiString func,TpFIBDatabase *db)
{
TpFIBDataSet *DS=NULL;
TpFIBTransaction *t=NULL;

  if((t=newTransaction(Owner,"newDataSet",db))!=NULL)
    if((DS=newDataSet(Owner,"newDataSet",db,t,t))==NULL){ delete t; t=NULL;}
  return DS;
}
//---------------------------------------------------------------------------
//������� ���������� SelectSQL ������� DataSet
bool __fastcall execSelectSQLDataSet(AnsiString func,AnsiString sql,TpFIBDataSet *ds)
{
bool commitFLG=false;

  ds->Close();
  ds->SQLs->SelectSQL->Clear();
  ds->SQLs->SelectSQL->Add(sql);

  if(ds->AutoCommit==false && ds->Transaction->InTransaction==false)
  { commitFLG=true;
    ds->Transaction->StartTransaction();
  }
  try
  {
    ds->Open();
  }
  catch (Exception &exc)
  { MessageDlg(func+": ������ - "+sql+"\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    if(ds->Transaction->InTransaction && commitFLG) ds->Transaction->Rollback();
    return false;
  }
  if(ds->Transaction->InTransaction && commitFLG) ds->Transaction->Commit();
  return true;
}
//---------------------------------------------------------------------------

//****************
// ����� FBQuery *
//****************
//������������ ������
__fastcall FBQuery::FBQuery(TComponent *Owner, TpFIBDatabase *db, bool forR)
{
  T=NULL;
  Q=NULL;
  forRead=forR;
  if((T=newTransaction(Owner,"FBQuery::FBQuery",db))==NULL) return;
  if((Q=newQuery(Owner,"FBQuery::FBQuery",db,T,forRead))==NULL) return;
  if(forRead==true) Q->Options << qoStartTransaction << qoAutoCommit;
}
//---------------------------------------------------------------------------
__fastcall FBQuery::FBQuery(TComponent *Owner,TpFIBDatabase *db,TpFIBTransaction *t,bool forR)
{
  T=NULL;
  Q=NULL;
  forRead=forR;
  if((Q=newQuery(Owner,"FBQuery::FBQuery",db,t,forRead))==NULL) return;
  if(forRead==true) Q->Options << qoStartTransaction << qoAutoCommit;
}
//---------------------------------------------------------------------------
//����������
__fastcall FBQuery::~FBQuery(void)
{
  if(T!=NULL)
  { if(T->InTransaction) T->Rollback();
    delete T; T=NULL;
  }
  if(Q != NULL)
  { Q->Close();
    delete Q; Q=NULL;
  }
}
//---------------------------------------------------------------------------
TpFIBTransaction * __fastcall FBQuery::getTransaction(void)
{
  return (static_cast<TpFIBTransaction*>(Q->Transaction));
}
//---------------------------------------------------------------------------
bool __fastcall FBQuery::execQuery(AnsiString func,AnsiString sql)
{
  return ::execQuery(func,sql,Q);
}
//---------------------------------------------------------------------------
//������� �������� ������� FBQuery
FBQuery * newFBQuery(TComponent *Owner, AnsiString func,TpFIBDatabase *db,bool forRead)
{
  try
  {
    return new FBQuery(Owner, db, forRead);
  }
  catch (Exception &exc)
  { MessageDlg(func+": ������ �������� ������� FBQuery\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
}
//---------------------------------------------------------------------------
FBQuery * newFBQuery(TComponent *Owner, AnsiString func,TpFIBDatabase *db,TpFIBTransaction *t,bool forRead)
{
  try
  {
    return new FBQuery(Owner,db,t,forRead);
  }
  catch (Exception &exc)
  { MessageDlg(func+": ������ �������� ������� FBQuery\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
}
//---------------------------------------------------------------------------

//******************
// ����� FBDataSet *
//******************
//������������ ������
__fastcall FBDataSet::FBDataSet(TComponent *Owner,TpFIBDatabase *db,bool autoCommit)
{
  RT=NULL;
  WT=NULL;
  DataSet=NULL;
  if((RT=newTransaction(Owner,"FBDataSet::FBDataSet",db))==NULL) return;
  if((WT=newTransaction(Owner,"FBDataSet::FBDataSet",db))==NULL) return;
  if((DataSet=newDataSet(Owner, "FBDataSet::FBDataSet",db,RT,WT))==NULL) return;
  DataSet->AutoCommit=autoCommit;
  DataSet->SQLs->SelectSQL->Clear();
}
//---------------------------------------------------------------------------
 __fastcall FBDataSet::FBDataSet(TComponent *Owner,TpFIBDatabase *db,TpFIBTransaction *rt,TpFIBTransaction *wt,bool autoCommit)
{
  RT=NULL;
  WT=NULL;
  DataSet=NULL;
  if((DataSet=newDataSet(Owner, "FBDataSet::FBDataSet",db,rt,wt))==NULL) return;
  DataSet->AutoCommit=autoCommit;
  DataSet->SQLs->SelectSQL->Clear();
}
//---------------------------------------------------------------------------
//����������
__fastcall FBDataSet::~FBDataSet(void)
{
  if(WT!=NULL)
  { if(WT->InTransaction) WT->Rollback();
    delete WT;
    WT=NULL;
  }
  if(RT!=NULL)
  { if(RT->InTransaction) RT->Rollback();
    delete RT;
    RT=NULL;
  }
  if(DataSet!=NULL)
  { if(DataSet->Active==true) DataSet->Close();
    delete DataSet;
    DataSet=NULL;
  }
}
//---------------------------------------------------------------------------
bool __fastcall FBDataSet::execSelectSQL(AnsiString func,AnsiString sql)
{
  return execSelectSQLDataSet(func,sql,DataSet);
}
//---------------------------------------------------------------------------
//������� �������� ������� FBDataSet
FBDataSet * newFBDataSet(TComponent *Owner,AnsiString func,TpFIBDatabase *db,bool autoCommit)
{
  try
  {
    return new FBDataSet(Owner,db,autoCommit);
  }
  catch (Exception &exc)
  { MessageDlg(func+": ������ �������� ������� FBDataSet\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
}
//---------------------------------------------------------------------------
FBDataSet * newFBDataSet(TComponent *Owner,AnsiString func,TpFIBDatabase *db,TpFIBTransaction *rt,TpFIBTransaction *wt,bool autoCommit)
{
  try
  {
    return new FBDataSet(Owner,db,rt,wt,autoCommit);
  }
  catch (Exception &exc)
  { MessageDlg(func+": ������ �������� ������� FBDataSet\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
}
//---------------------------------------------------------------------------

//********************
// ����� ���� ������ *
//********************
__fastcall FBDatabase::FBDatabase(TComponent* owner)
{
  dataBase=NULL;
  T1=NULL;         //���������� �� ���������
  T2=NULL;         //���������� �� ��������� ��� ���������� ������

  fbQ_T1=NULL;     //��� ������ � �������� ��������� T1 (qoStartTransaction, qoAutoCommit)
  Q_T1=NULL;
  fbDS_T1_T2=NULL; //������ � �������� ���������� T1, ������ � �������� ���������� T2 (AutoCommit)
  DS_T1_T2=NULL;
  fbQ_T2=NULL;     //��� ������/������ � �������� ��������� T2
  Q_T2=NULL;
  fbDS_T2=NULL;    //������ � ������ � �������� ���������� T2
  DS_T2=NULL;

  Owner=owner;
  initString="";
  alias="";
  server="";
  dbFullName="";
  dbPath="";
  baseName="";

  try
  {
    dataBase=new TpFIBDatabase(Owner);
  }
  catch (Exception &exc)
  { MessageDlg("������ �������� ������� dataBase"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    dataBase=NULL;
    return;
  }

  if((T1=newTransaction(Owner,"FBDatabase::FBDatabase",dataBase))==NULL) return;
  dataBase->DefaultTransaction=T1;
  if((T2=newTransaction(Owner,"FBDatabase::FBDatabase",dataBase))==NULL) return;
  dataBase->DefaultUpdateTransaction=T2;

  if((fbQ_T1=newFBQuery(Owner,"FBDatabase::FBDatabase",dataBase,T1,true))==NULL) return;
  Q_T1=fbQ_T1->getQuery();
  if((fbQ_T2=newFBQuery(Owner,"FBDatabase::FBDatabase",dataBase,T2,false))==NULL) return;
  Q_T2=fbQ_T2->getQuery();
  if((fbDS_T1_T2=newFBDataSet(Owner,"FBDatabase::FBDatabase",dataBase,T1,T2,true))==NULL) return;
  DS_T1_T2=fbDS_T1_T2->getDataSet();
  if((fbDS_T2=newFBDataSet(Owner,"FBDatabase::FBDatabase",dataBase,T2,T2,false))==NULL) return;
  DS_T2=fbDS_T2->getDataSet();

  try
  {
    sw=new SecretWord();
  }
  catch (Exception &exc)
  { MessageDlg("������ �������� ������� SecretWord"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    sw=NULL;
    return;
  }
}
//---------------------------------------------------------------------------
//������������� ������� FBDatabase
// initStr - ������ �������������
// ������ ������:
// �������� ���� ������=������:������ ��� ���� ������ -user ����� -pas ������
// ���: �������� ���� ������ - ������������ ������
//      ������ - ��� ��� IP ����� ����������, �� ������� ������� SQL ������ firebird � ��������� ���� ������
//      ������ ��� ���� ������ - ��������� ��� ���� ������ �� �������
//      -user - ���� ��� ������� ������ ������������, � ������� �������� ���������� ����������� � ����
//      ����� - ����� ������������ (������ ���� ������ � ���� ������). �����, ����������� ���������� � ����� ����, SYSDBA
//      -pas - ����� ��� ������� ������
//      ������ - ������ ������������ (������ ������������ � ������� SYSDBA - masterkey)
void __fastcall FBDatabase::init(AnsiString initStr)
{
int len;

  initString=initStr;
  if(initString.IsEmpty()) return;
  //��������� ����
  if(initString.Pos("=") > 0)
    alias=initString.SubString(0,initString.Pos("=")-1);
  else alias="";
  //������
  if(initString.Pos(":") > 0)
    server=initString.SubString(initString.Pos("=")+1,initString.Pos(":")-initString.Pos("="));
  else server="localhost";
  //������ ��� ����� � �����
  if(initString.Pos(" -user ") > 0)
  { if(initString.Pos(":") > 0) len=initString.Pos(" -user ")-initString.Pos(":")-1;
    else len=initString.Pos(" -user ")-1;
  }
  else len=initString.Length()-initString.Pos(":");
  dbFullName=initString.SubString(initString.Pos(":")+1,len);
  //���� � ����
  dbPath=ExtractFilePath(dbFullName);
  //��� ����� � �����
  baseName=ExtractFileName(dbFullName);
  //��� ������������
  userName="";
  if(initString.Pos(" -user ") > 0 && initString.Pos(" -pas ") > 0)
  { len=initString.Pos(" -pas ")-initString.Pos(" -user ")-7;
    userName=initString.SubString(initString.Pos(" -user ")+7,len);
  }
  //������
  userPassword="";
  if(initString.Pos(" -pas ") > 0)
  { len=initString.Length()-initString.Pos(" -pas ")-5;
    userPassword=initString.SubString(initString.Pos(" -pas ")+6,len);
    if(len > 50) userPassword=sw->decodeWord(userPassword);
  }

  if(dataBase!=NULL)
  { dataBase->ConnectParams->UserName=userName;
    dataBase->ConnectParams->Password=userPassword;
    dataBase->ConnectParams->CharSet="WIN1251";
    dataBase->SQLDialect=3;
    dataBase->LibraryName="gds32.dll";
    dataBase->AliasName=alias;
    dataBase->DBName=server+dbFullName;
  }
}
//---------------------------------------------------------------------------
bool __fastcall FBDatabase::connect(void)
{
  if(isConnected()==true) return true;
  if(dataBase==NULL) return false;
  try
  {
    dataBase->Connected=true;
  }
  catch(const Exception &exc)
  { MessageDlg("������ ����������� � ���� - "+dataBase->DBName+"\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return false;
  }
  return true;
}
//---------------------------------------------------------------------------
void __fastcall FBDatabase::disconnect(void)
{
  if(dataBase==NULL) return;
  if(isConnected()==true)
  { if(T1!=NULL && T1->InTransaction) T1->Rollback();
    if(T2!=NULL && T2->InTransaction) T2->Rollback();
    if(Q_T1!=NULL && Q_T1->Open) Q_T1->Close();
    if(Q_T2!=NULL && Q_T2->Open) Q_T2->Close();
    if(DS_T1_T2!=NULL && DS_T1_T2->Active) DS_T1_T2->Close();
    if(DS_T2!=NULL && DS_T2->Active) DS_T2->Close();
    dataBase->Close();
  }
}
//---------------------------------------------------------------------------
bool __fastcall FBDatabase::isConnected(void)
{
  if(dataBase==NULL) return false;
  else return dataBase->Connected;
}
//---------------------------------------------------------------------------
__fastcall FBDatabase::~FBDatabase(void)
{
  if(isConnected()==true) disconnect();
  if(fbDS_T1_T2!=NULL){ delete fbDS_T1_T2; fbDS_T1_T2=NULL; DS_T1_T2=NULL;}
  if(fbDS_T2!=NULL){ delete fbDS_T2; fbDS_T2=NULL; DS_T2=NULL;}
  if(fbQ_T1!=NULL){ delete fbQ_T1; fbQ_T1=NULL; Q_T1=NULL;}
  if(fbQ_T2!=NULL){ delete fbQ_T2; fbQ_T2=NULL; Q_T2=NULL;}
  if(T1!=NULL){ delete T1; T1=NULL;}
  if(T2!=NULL){ delete T2; T2=NULL;}
  if(dataBase!=NULL){ delete dataBase; dataBase=NULL;}
  if(sw!=NULL){ delete sw; sw=NULL;}
}
//---------------------------------------------------------------------------

