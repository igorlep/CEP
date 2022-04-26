//---------------------------------------------------------------------------
//����� "�����" - ��� ������ � �������� FOLDERS_TABLE

#ifndef FolderH
#define FolderH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "DBClasses.h"
//---------------------------------------------------------------------------
class Folder
{
private:
  FBQuery *fbRQ;
  TpFIBQuery *RQ;
  FBQuery *fbWQ;
  TpFIBTransaction *WT;
  FBDataSet *fbDS;
  TpFIBDataSet *DS;
  AnsiString tableName;
public:
  __fastcall Folder(TComponent* owner,TpFIBDatabase *db,AnsiString tName);
  __fastcall ~Folder(void);
  //������� ���������� ������������� �������� �����
  AnsiString __fastcall getRootFolderID(void);
  //�������, ������������ �������� ���� FOLDER_NUMBER
  AnsiString __fastcall getFolderNumber(AnsiString folderID);
  //������� ��������� ������� ������ �� ���� �������� � ������������ �����
  //���� ������ �����������, ��, ���� �������� createRecord == true, ������� ������
  //���������� ��� ������
  AnsiString __fastcall checkRecordInFoldersTable(AnsiString parentID, AnsiString folderName,bool createRecord);
  //������� �������������� �����
  bool __fastcall renameFolder(AnsiString folderID,AnsiString newFolderName);
  //������� �������� ������ �� ������� tableName
  bool __fastcall removeFolder(AnsiString folderID);
  //������� ���������� ������ ��� �����, ������� � ��������
  AnsiString getFolderFullName(AnsiString folderID);
  //������� ���������� ��� ����� �� �� ������� �����
  AnsiString getFolderID(AnsiString fullFolderName);
};
//---------------------------------------------------------------------------
//������� �������� ���������� Folder
Folder *newFolder(TComponent* owner,TpFIBDatabase *db,AnsiString tName);
//---------------------------------------------------------------------------

#endif
