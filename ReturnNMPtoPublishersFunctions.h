//---------------------------------------------------------------------------

#ifndef ReturnNMPtoPublishersFunctionsH
#define ReturnNMPtoPublishersFunctionsH
//---------------------------------------------------------------------------
#include "GridF.h"
//---------------------------------------------------------------------------
//����� ��� ������ ����������
// - ������� ������������ select sql �������
AnsiString __fastcall formPublishersSelectSQL(TDateTime date1,TDateTime date2,int typeNMP);
// - ������� �������� ������ ��� ������ ����������
TGridContentFrame * __fastcall createPublisherSelectFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel);
// - ������� �������� ������ � ���
TFrame * __fastcall createReturnNMPFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel,int typeNMP);

//������� ��� ������ � ��������� ����-12, ������ ����-2 � ����-16
// - ������ ���������
void __fastcall printTorg12(TComponent *owner,TpFIBDatabase *db,TpFIBDataSet *pDS,TpFIBDataSet *nmpDS,int kind);
// - ������ ���� ����-2
void __fastcall printTorg2(TComponent *owner,int kind);
// - ������ ���� ����-16
void __fastcall printTorg16(TComponent *owner,FBDatabase *fbDB);
// - �������� ������� ������������ ��������� � ��������� ����-12 ��� ����-2
void createMessage(TComponent *owner,AnsiString iniFileName,int type,int kind);
// type=1 - ����-12 �������,���������� ��������
// type=2 - ����-12 �������, ���������� ��������
// type=3 - ����-2
// kind=1 - ������ ���������, kind=2 - ���������� ���������
// - ������� ������������ ���������
bool __fastcall createMessagesForPublishers(TComponent *owner,int type,int kind,AnsiString subject,AnsiString content,bool replyFLG,bool delFilesFLG);
// - ������� ������������ ����� � ��������� ����-12
AnsiString __fastcall createTorg12File(TComponent *owner,int type,int kind,AnsiString dirName);
// - ������� ������ ������ � Excel ���� � ��������� ����-12
void putTorg12Str(int ns, class DirectWorkExcel *de, TDataSet *DS);
// - ������� ������������ ����� � ��������� ����-2
AnsiString __fastcall createTorg2File(TComponent *owner,int kind,AnsiString dirName);
// - ������� ������ ������ � Excel ���� � ����� ����-2
void putTorg2Str(int ns,int row,class DirectWorkExcel *de,TDataSet *DS);
double getPriceS_NDS(TDataSet *DS);
double getSumma1(TDataSet *DS);
double getSumma2(TDataSet *DS);
double getSumma3(TDataSet *DS);
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

