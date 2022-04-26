//---------------------------------------------------------------------------
#ifndef UnloadDataForBookKeepingFunctionsH
#define UnloadDataForBookKeepingFunctionsH
//---------------------------------------------------------------------------
#include "DBClasses.h"
//---------------------------------------------------------------------------
// - ����� - �������� ������ ��� ����������� ����
AnsiString __fastcall formUnloadDataForBookSelectSQL(int type,TDateTime dt1,TDateTime dt2);
// - �������� ������ � ������������ �������
class TGridFrame * __fastcall cteateUnloadDataForBookPFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel,int type);
class TGridFrame * __fastcall cteateUnloadDataForBookSDFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel,int type);
//TGridFrame * __fastcall cteateUnloadDataForBookKeepingFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel);

//������������ ��������� � ��������� �� ��������
// - �������� ������� ������������ ��������� � �������
void createMessage(TComponent *owner,AnsiString iniFileName,int type);
// - ������� ������������ ����� � �������
AnsiString __fastcall createDBFFileForBookKeeping(TComponent *owner,int type,AnsiString dirName);
void  __fastcall lockOrderingData(class FBDatabase *fbDB,TDateTime date1,TDateTime date2);
#endif

/*������ ��� �������� ������ �� ������������

//������ �������� �������:
AnsiString tableInfo="�1,�2,�3,�4,�5,�6";
// ���:
// �1 - ������������_�������_��_������������,
// �2 - ���������_����������� (��������� � ��������� ���� �����������),
// �3 - ������������_���������_����,
// �4 - ������������_����_���������, (����, �� �������� ������������ ����� ������)
// �5 - ���������_��������_�����_�����������,
// �6 - ���������_�����_���_���������_�������

//������ � ��������� �����:
AnsiString fieldsInfo="��1;��2;...��N";
// ���: ��1;��2;��3;...��N - ��������� ���� 1, ��2 - ��������� ����2, ��N - ��������� ���� N
// ������ ��������� ����:
// ��=�1,�2,�3,�4,�5,�6,�7,�8
// ���:
// �1 - ������������_����_�_SQL_�������,
// �2 - ������������_����_�_DataSet-�,
// �3 - ���������_����_�_Gride-�,
// �4 - ������������_����_�_Gride-�, (Left,Center,Right)
// �5 - ����_"������_������",
// �6 - ����_"����������_�_Gride-�,
// �7 - ����_"���������_����_�_Grid-�_�������_�������"
// �8 - ����_"�������� ����� �� ������"
// �9 - ������ ������ � Grid-�

//������ � ��������� Footer �����
AnsiString footerFieldsInfo="��1,��2,...��N";
// ���: ��1;��2;��3;...��N - ��������� ���� 1, ��2 - ��������� ����2, ��N - ��������� ���� N
// ������ ��������� ����:
// ��=�1,�2,�3,�4,�5
// ���:
// �1 - ������������ ����, ��� �������� ����� �������� �������� � ������ Footer
// �2 - ��� �������� (fvtAvg,fvtCount,fvtFieldValue,fvtStaticText,fvtSum)
// �3 - ������ ������
// �4 - ��������
// �4 - ���������

//������ � ��������� �����, ������������ � ������� ���������� ������
AnsiString contentsInfo="��1;��2;...��N";
// ������ ��������� ����:
// ��=�1,�2
// ���:
// �1 - ������������_����,
// �2 - ���������_����

//������ ������� �������� ������ �����������
TReferenceBookFrame * __fastcall create???RB_Frame(TComponent* Owner,TPanel *workPanel,AnsiString iniFileName,FBDatabase *fbDB)
{
AnsiString tableInfo=",,,,,";
AnsiString fieldsInfo =",,,,,,;";
           fieldsInfo+=",,,,,,;";
           fieldsInfo+=",,,,,,;";
           fieldsInfo+=",,,,,,;";
           ...
           fieldsInfo+=",,,,,,";
AnsiString contentsInfo= ",;";
           contentsInfo+=",;";
           contentsInfo+=",;";
           ...
           contentsInfo+=",";
bool searchAble=;      //���� "���� ������ �� �������� ��������� ����"
bool directEdit=;      //���� "�������������� � Grid-�"
bool tvViewAble=;      //���� "���������� ������� ������ �����"
bool contentViewAble=; //���� "���������� ������� ����������� ������"

TRBFrame *RBFrame;

  try
  {
    RBFrame=new TReferenceBookFrame(TComponent* Owner,AnsiString OwnerN,\
FBDatabase *fb_DB,AnsiString iniFN,AnsiString tableInf,AnsiString fieldsInf,\
AnsiString contentsInf,bool TVViewAble,bool NeedsSearchingForModels);
  }
  catch (Exception &exc)
  {
    MessageDlg("������ �������� TRBFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
  return RBFrame;
}

*/

