//---------------------------------------------------------------------------
//Файл - DBClasses.cpp
//******************************************************************
// Функции и классы для работы с базой данных Firebird (Interbase) *
//******************************************************************

#include "SecretWord.h"
#pragma hdrstop
#include "DBClasses.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

//**********
// Функции *
//**********
//Функция создания транзакции
TpFIBTransaction * newTransaction(TComponent *Owner, AnsiString func,TpFIBDatabase *db)
{
TpFIBTransaction *t;

  try
  {
    t=new TpFIBTransaction(Owner);
  }
  catch (Exception &exc)
  { MessageDlg(func+": Ошибка создания объекта Transaction"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
  t->DefaultDatabase=db;
  return t;
}
//---------------------------------------------------------------------------
//Функция создания TpFIBQuery
TpFIBQuery * newQuery(TComponent *Owner, AnsiString func,TpFIBDatabase *db,TpFIBTransaction *t,bool forR)
{
TpFIBQuery *q;

  try
  {
    q=new TpFIBQuery(Owner);
  }
  catch (Exception &exc)
  { MessageDlg(func+": Ошибка создания объекта TpFIBQuery"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
  q->Database=db;
  q->Transaction=t;
  if(forR==true) q->Options << qoStartTransaction << qoAutoCommit;
  return q;
}
//---------------------------------------------------------------------------
//Функция выполнения запроса Query
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
  { MessageDlg(func+": Ошибка - "+sql+"\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    if(query->Transaction->InTransaction && commitFLG) query->Transaction->Rollback();
    return false;
  }
  if(query->Transaction->InTransaction && commitFLG) query->Transaction->Commit();
  return true;
}
//---------------------------------------------------------------------------
//Функция создания TpFIBDataSet
TpFIBDataSet * newDataSet(TComponent *Owner, AnsiString func,TpFIBDatabase *db,TpFIBTransaction *rt,TpFIBTransaction *ut)
{
TpFIBDataSet *DS;

  try
  {
    DS=new TpFIBDataSet(Owner);
  }
  catch (Exception &exc)
  { MessageDlg(func+": Ошибка создания объекта TpFIBDataSet\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
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
//Функция выполнения SelectSQL запроса DataSet
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
  { MessageDlg(func+": Ошибка - "+sql+"\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    if(ds->Transaction->InTransaction && commitFLG) ds->Transaction->Rollback();
    return false;
  }
  if(ds->Transaction->InTransaction && commitFLG) ds->Transaction->Commit();
  return true;
}
//---------------------------------------------------------------------------

//****************
// Класс FBQuery *
//****************
//Конструкторы класса
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
//Деструктор
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
//Функции создания объекта FBQuery
FBQuery * newFBQuery(TComponent *Owner, AnsiString func,TpFIBDatabase *db,bool forRead)
{
  try
  {
    return new FBQuery(Owner, db, forRead);
  }
  catch (Exception &exc)
  { MessageDlg(func+": Ошибка создания объекта FBQuery\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
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
  { MessageDlg(func+": Ошибка создания объекта FBQuery\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
}
//---------------------------------------------------------------------------

//******************
// Класс FBDataSet *
//******************
//Конструкторы класса
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
//Деструктор
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
//Функции создания объекта FBDataSet
FBDataSet * newFBDataSet(TComponent *Owner,AnsiString func,TpFIBDatabase *db,bool autoCommit)
{
  try
  {
    return new FBDataSet(Owner,db,autoCommit);
  }
  catch (Exception &exc)
  { MessageDlg(func+": Ошибка создания объекта FBDataSet\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
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
  { MessageDlg(func+": Ошибка создания объекта FBDataSet\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
}
//---------------------------------------------------------------------------

//********************
// Класс база данных *
//********************
__fastcall FBDatabase::FBDatabase(TComponent* owner)
{
  dataBase=NULL;
  T1=NULL;         //транзакция по умолчанию
  T2=NULL;         //транзакция по умолчанию для обновления данных

  fbQ_T1=NULL;     //для чтения в границах танзакции T1 (qoStartTransaction, qoAutoCommit)
  Q_T1=NULL;
  fbDS_T1_T2=NULL; //чтение в границах транзакции T1, запись в границах транзакции T2 (AutoCommit)
  DS_T1_T2=NULL;
  fbQ_T2=NULL;     //для чтения/записи в границах танзакции T2
  Q_T2=NULL;
  fbDS_T2=NULL;    //чтение и запись в границах транзакции T2
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
  { MessageDlg("Ошибка создания объекта dataBase"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
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
  { MessageDlg("Ошибка создания объекта SecretWord"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    sw=NULL;
    return;
  }
}
//---------------------------------------------------------------------------
//Инициализация объекта FBDatabase
// initStr - строка инициализации
// Формат строки:
// Описание базы данных=сервер:полное имя базы данных -user логин -pas пароль
// где: Описание базы данных - произвольная строка
//      сервер - имя или IP адрес компьютера, на котором запущен SQL сервер firebird и находится база данных
//      полное имя базы данных - локальное имя базы данных на сервере
//      -user - ключ для задания логина пользователя, с правами которого происходит подключение к базе
//      логин - логин пользователя (должен быть создан в базе данных). Логин, прописанный изначально в любой базе, SYSDBA
//      -pas - ключь для задания пароля
//      пароль - пароль пользователя (пароль пользователя с логином SYSDBA - masterkey)
void __fastcall FBDatabase::init(AnsiString initStr)
{
int len;

  initString=initStr;
  if(initString.IsEmpty()) return;
  //Псевдоним базы
  if(initString.Pos("=") > 0)
    alias=initString.SubString(0,initString.Pos("=")-1);
  else alias="";
  //Сервер
  if(initString.Pos(":") > 0)
    server=initString.SubString(initString.Pos("=")+1,initString.Pos(":")-initString.Pos("="));
  else server="localhost";
  //Полное имя файла с базой
  if(initString.Pos(" -user ") > 0)
  { if(initString.Pos(":") > 0) len=initString.Pos(" -user ")-initString.Pos(":")-1;
    else len=initString.Pos(" -user ")-1;
  }
  else len=initString.Length()-initString.Pos(":");
  dbFullName=initString.SubString(initString.Pos(":")+1,len);
  //Путь к базе
  dbPath=ExtractFilePath(dbFullName);
  //Имя файла с базой
  baseName=ExtractFileName(dbFullName);
  //Имя пользователя
  userName="";
  if(initString.Pos(" -user ") > 0 && initString.Pos(" -pas ") > 0)
  { len=initString.Pos(" -pas ")-initString.Pos(" -user ")-7;
    userName=initString.SubString(initString.Pos(" -user ")+7,len);
  }
  //Пароль
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
  { MessageDlg("Ошибка подключения к базе - "+dataBase->DBName+"\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
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

