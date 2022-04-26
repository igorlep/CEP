//---------------------------------------------------------------------------

#ifndef SomeFunctionsH
#define SomeFunctionsH
//---------------------------------------------------------------------------
#include "ChTreeView.h"
#include "MainCAP_2010.h"
#include "DBClasses.h"
//---------------------------------------------------------------------------
//********************************************
//* ������� ��� ������ ������� ����� ������� *
//********************************************
FBDatabase * __fastcall getFBDataBase(void);
PCUser * __fastcall getUser(void);
TMainMenu * __fastcall getMainMenu(void);
TMainCAP_2010Form * __fastcall getMainForm(void);
TStatusBar * __fastcall getMainStatusBar(void);
TFrame * __fastcall getMainWorkFrame(void);
AnsiString __fastcall getAppDir(void);
AnsiString __fastcall getUserWorkDir(void);
void __fastcall closeBase();
void __fastcall openBase();

//���� �������� ���������������� ������� �� �����
TDateTime __fastcall getReturnDate(TComponent* owner,FBDatabase *fbDB,TDateTime e_date,AnsiString NMP_ID);
//���� �������� ���������������� ������� ����������
TDateTime __fastcall getDateReturnToPublisher(TDateTime r_date);
//TDateTime __fastcall getWriteOffDate(TComponent* owner,FBDatabase *fbDB,TDateTime fact_date,AnsiString e_id);

//���������� 3-� ��������� ������ ������� ����
TStatusPanel * __fastcall getStatusPanelMainWindow(void);
//������� ����� � 3-� StatusPanel MainStatusBar
void __fastcall set3MainStatusBar(AnsiString text);
//�������, ��������������� ���� �������� ������, ������������ ���� ������� �������������� ������������ ���
//void __fastcall setCreateLDOThread(bool flg);
//�������, ��������������� ���� �������� ������, ���������� �����, �������� ��� ���������
//void __fastcall setCreateTPFThread(bool flg);

//������� ��� ������ � ��������� foldersTableName � messagesTableName
// - ������� �������� ������ �� ������� foldersTableName
bool __fastcall removeFolder(TComponent *Owner,TpFIBDatabase *db,AnsiString foldersTableName,AnsiString messagesTableName,AnsiString folderID);
//������� ��������� ������� ������ �� ������� � ������������ �����
//���� ������ �����������, ��, ���� �������� createRecord == true, ������� ������
//���������� ��� ������
AnsiString __fastcall checkFolderInMessagesTable(TComponent *owner,TpFIBDatabase *db,AnsiString foldersTableName,AnsiString foolFolderName,bool createRecord);
//������� ��������� � ���� ���, �� �������, ����� 3-�� � 4-�� ������ � ������� ���� � ������
AnsiString checkYearAndMonthFolders(TComponent *owner,TpFIBDatabase *db,AnsiString foldersTableName,AnsiString folderName_1,AnsiString folderName_2,int year,int month);
//������� ��������� � ���� ���, �� �������, ����� 3-�� � 4-�� ������ � ������� ���� � ������
AnsiString checkYearAndMonthFolders(TComponent *owner,TpFIBDatabase *db,AnsiString foldersTableName,AnsiString fullFolderName,int year,int month);
//������ ���������� ������ ���� �����
AnsiString getFullNameFilder(TComponent *owner,TpFIBDatabase *db,AnsiString foldersTableName,AnsiString folderID);

// ������� �������� ���������
// - ������� �������� ������ � ��������� � ������� MESSAGES_TABLE
AnsiString __fastcall addMessageToBase(TComponent *owner,FBDatabase *fbDB,AnsiString folderID,AnsiString rName,AnsiString rMail,AnsiString subject,AnsiString content,TStringList *attachedFiles,bool reply,bool delFiles);
// - ������� �������� ������ � ��������� � ������� RECEIVED_MESSAGES_TABLE
//AnsiString __fastcall addRMessageToBase(TComponent *owner,FBDatabase *fbDB,AnsiString folderID,AnsiString sMail,AnsiString subject,AnsiString content,TStringList *attachedFiles);

//������� ����������� �������
bool __fastcall templateCopy(AnsiString fin, AnsiString fout);

//������� ��� ������ � ��������� ����-13
// - ������ ���������
void __fastcall printTorg13(TComponent *owner,TpFIBDatabase *db,TpFIBDataSet *lDS,TpFIBDataSet *rDS,int type,int kind);
// type=1 - ��������� �� ��������, type=2 - ��������� �� �������
// kind=1 - ������ ���������, kind=2 - ���������� ���������
// - �������� ������� ������������ ��������� � ��������� ����-13
void createTorg13Message(TComponent *owner,AnsiString iniFileName,int type,int kind);
// type=1 - ��������� �� ��������, type=2 - ��������� �� �������
// kind=1 - ������ ���������, kind=2 - ���������� ���������
// - ������� ������������ ���������
bool __fastcall createTorg13MessagesForSubdivisions(TComponent *owner,int type,int kind,AnsiString subject,AnsiString content,bool replyFLG,bool delFilesFLG);
// - ������� ������������ ����� � ��������� ����-13
AnsiString __fastcall createTorg13File(TComponent *owner,int type,int kind,AnsiString dirName);
// - ������� ������ ������ � Excel ���� � ��������� ����-13
void __fastcall putStr(int type,int ns, class DirectWorkExcel *de, TDataSet *DS);

//������� ��� ������ � ������� ���� ������������
// - �������� ����� ����������� �� ����� ���� �� ���� ����
CheckStyle __fastcall checkUserRightForMenuItem(TComponent *owner,FBDatabase *fbDB,AnsiString iserID,AnsiString miID);
// - �������� ����� ����������� �� ����� ���� �� ����� ����
CheckStyle __fastcall checkUserRightForMenuName(TComponent *owner,FBDatabase *fbDB,AnsiString iserID,AnsiString miName);
#endif

