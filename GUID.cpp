//---------------------------------------------------------------------------
//Класс GUID - уникальный идентификатор записи

#include "GUID.h"
//---------------------------------------------------------------------------
AnsiString __fastcall guid::genGUID(void)
{
  CreateGUID(g);
  p=Sysutils::GUIDToString(g);
  return (AnsiString(p).SubString(2,8))+(AnsiString(p).SubString(11,4))+(AnsiString(p).SubString(16,4))+(AnsiString(p).SubString(21,4))+(AnsiString(p).SubString(26,12));
}
//---------------------------------------------------------------------------
