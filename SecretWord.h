//---------------------------------------------------------------------------

#ifndef SecretWordH
#define SecretWordH
//---------------------------------------------------------------------------
#include <Psock.hpp>
//---------------------------------------------------------------------------
//����� "��������� �����" - ����������� � ������������� �������
//  ��� ������ � ������� SecretWord ����������, ����� � ����� � �������� ���������
//  � ������� Bitmap ��� ������ MP, ����������� �� ����� Bitmaps.bmp
class SecretWord
{
private:
  char *symbols;
  Graphics::TBitmap *p;
  AnsiString __fastcall selectMap(void);
  AnsiString __fastcall selectWay(void);
  AnsiString __fastcall generateSymbol(void);
public:
  __fastcall SecretWord(void);
  __fastcall ~SecretWord(void);
  AnsiString __fastcall encodeWord(AnsiString str);
  AnsiString __fastcall decodeWord(AnsiString str);
};
//---------------------------------------------------------------------------
#endif
