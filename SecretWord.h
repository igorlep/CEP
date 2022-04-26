//---------------------------------------------------------------------------

#ifndef SecretWordH
#define SecretWordH
//---------------------------------------------------------------------------
#include <Psock.hpp>
//---------------------------------------------------------------------------
//Класс "Секретное слово" - кодирования и декодирования паролей
//  Для работы с классом SecretWord необходимо, чтобы в файле с ресурсам программы
//  в разделе Bitmap был ресурс MP, загруженный из файла Bitmaps.bmp
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
