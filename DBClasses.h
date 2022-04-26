//---------------------------------------------------------------------------

#ifndef DBClassesH
#define DBClassesH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "FIBDatabase.hpp"
#include "pFIBDatabase.hpp"
#include "pFIBDataSet.hpp"
//---------------------------------------------------------------------------
//������� �������� TpFIBTransction
TpFIBTransaction * newTransaction(TComponent* Owner, AnsiString func,TpFIBDatabase *db);
//������� �������� TpFIBQuery
TpFIBQuery * newQuery(TComponent* owner,AnsiString func,TpFIBDatabase *db,TpFIBTransaction *t,bool forRead);
//������� ���������� ������� Query
bool execQuery(AnsiString func,AnsiString sql,TpFIBQuery *query);
//������� �������� TpFIBDataSet
TpFIBDataSet * newDataSet(TComponent* owner,AnsiString func,TpFIBDatabase *db,TpFIBTransaction *rt,TpFIBTransaction *ut);
TpFIBDataSet * newDataSet(TComponent* owner,AnsiString func,TpFIBDatabase *db);
//������� ���������� SelectSQL ������� DataSet
bool __fastcall execSelectSQLDataSet(AnsiString func,AnsiString sql,TpFIBDataSet *ds);
//---------------------------------------------------------------------------
//������ ��� ������� � ���� ������ FireBird
//Query
class FBQuery
{
private:
  TpFIBTransaction *T;
  TpFIBQuery *Q;
  bool forRead;
public:
  __fastcall FBQuery(TComponent* owner,TpFIBDatabase *db,bool forRead);
  __fastcall FBQuery(TComponent* owner,TpFIBDatabase *db,TpFIBTransaction *t,bool forRead);
  __fastcall ~FBQuery(void);
  bool __fastcall execQuery(AnsiString func,AnsiString sql);
  TpFIBTransaction * __fastcall getTransaction(void);
  TpFIBQuery * __fastcall getQuery(void){ return Q; }
  bool __fastcall isForRead(void){ return forRead; }
};
//---------------------------------------------------------------------------
//������� �������� ������� FBQuery
FBQuery * newFBQuery(TComponent* owner, AnsiString func,TpFIBDatabase *db,bool forRead);
FBQuery * newFBQuery(TComponent* owner, AnsiString func,TpFIBDatabase *db,TpFIBTransaction *t,bool forRead);
//---------------------------------------------------------------------------
//DataSet
class FBDataSet
{
private:
  TpFIBTransaction *RT;
  TpFIBTransaction *WT;
  TpFIBDataSet *DataSet;
public:
  __fastcall FBDataSet(TComponent* owner,TpFIBDatabase *db,bool autoCommit);
  __fastcall FBDataSet(TComponent* owner,TpFIBDatabase *db,TpFIBTransaction *rt,TpFIBTransaction *wt,bool autoCommit);
  __fastcall ~FBDataSet(void);
  bool __fastcall execSelectSQL(AnsiString func,AnsiString sql);
  TpFIBDataSet * __fastcall getDataSet(void){ return DataSet; }
};
//������� �������� ������� FBDataSet
FBDataSet * newFBDataSet(TComponent *owner,AnsiString func,TpFIBDatabase *db,bool autoCommit);
FBDataSet * newFBDataSet(TComponent *owner,AnsiString func,TpFIBDatabase *db,TpFIBTransaction *rt,TpFIBTransaction *wt,bool autoCommit);
//---------------------------------------------------------------------------

//����� ���� ������
class FBDatabase
{
private:
  TpFIBDatabase *dataBase;
  TpFIBTransaction *T1;   //���������� �� ���������
  TpFIBTransaction *T2;   //���������� �� ��������� ��� ���������� ������
  FBQuery *fbQ_T1;        //��� ������ � �������� ��������� T1 (qoStartTransaction, qoAutoCommit)
  TpFIBQuery *Q_T1;
  FBQuery *fbQ_T2;        //��� ������/������ � �������� ��������� T2
  TpFIBQuery *Q_T2;
  FBDataSet *fbDS_T1_T2;  //������ � �������� ���������� T1, ������ � �������� ���������� T2 (AutoCommit)
  TpFIBDataSet *DS_T1_T2;
  FBDataSet *fbDS_T2;     //������ � ������ � �������� ���������� T2
  TpFIBDataSet *DS_T2;
  TComponent* Owner;
  AnsiString initString, alias, server, dbPath, dbFullName, baseName;
  AnsiString userName, userPassword;
  class SecretWord *sw;
public:
  __fastcall FBDatabase(TComponent* owner);
  __fastcall ~FBDatabase(void);
  void __fastcall init(AnsiString initStr);
  bool __fastcall connect(void);
  void __fastcall disconnect(void);
  bool __fastcall isConnected(void);
  TpFIBDatabase * __fastcall getDatabase(void){ return dataBase; }
  AnsiString __fastcall getInitString(void){ return initString; }
  AnsiString __fastcall getDBAlias(void){ return alias; }
  AnsiString __fastcall getServer(void){ return server; }
  AnsiString __fastcall getDBPath(void){ return dbPath; }
  AnsiString __fastcall getDBFullName(void){ return dbFullName; }
  AnsiString __fastcall getBaseName(void){ return baseName; }
  AnsiString __fastcall getUserName(void){ return userName; }
  AnsiString __fastcall getUserPassword(void){ return userPassword; }
  TpFIBTransaction * __fastcall getT1(void){ return T1; }
  TpFIBTransaction * __fastcall getT2(void){ return T2; }
  FBQuery * __fastcall getFBQ_T1(void){ return fbQ_T1; }
  TpFIBQuery * __fastcall getQ_T1(void){ return Q_T1; }
  FBQuery * __fastcall getFBQ_T2(void){ return fbQ_T2; }
  TpFIBQuery * __fastcall getQ_T2(void){ return Q_T2; }
  FBDataSet * __fastcall getFBDS_T1_T2(void){ return fbDS_T1_T2; }
  TpFIBDataSet * __fastcall getDS_T1_T2(void){ return DS_T1_T2; }
  FBDataSet * __fastcall getFBDS_T2(void){ return fbDS_T2; }
  TpFIBDataSet * __fastcall getDS_T2(void){ return DS_T2; }
};

//---------------------------------------------------------------------------
#endif
