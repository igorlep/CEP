//Класс для работы с Excel файлами

#include <vcl.h>
#include "Math.hpp"
#pragma hdrstop
#include "DirectWorkExcel.h"
//---------------------------------------------------------------------------
__fastcall DirectWorkExcel::DirectWorkExcel(AnsiString fN, unsigned short sQ)
{
  fileName=fN;
  sheetsQuantity=sQ;
  if(sheetsQuantity==0) sheetsQuantity=1;
  sheetNumber=1;
  opened=false;
  readOnly=true;
}
//---------------------------------------------------------------------------
__fastcall DirectWorkExcel::~DirectWorkExcel(void)
{
  closeFile();
}
//---------------------------------------------------------------------------
bool __fastcall DirectWorkExcel::openFile(bool rO)
{
  //Если файл открыт, закрываем его
  if(opened) closeFile();
  if(fileName.IsEmpty())
  { MessageDlg("Не указано имя Excel файл",mtWarning,TMsgDlgButtons() << mbOK,0);
    return opened;
  }
  //Если Excel не запущен, запускаем его
  try
  { VarApp=Variant::CreateObject("Excel.Application");
    VarBooks=VarApp.OlePropertyGet("Workbooks");
  }
  catch(const Exception &exc)
  { MessageDlg("Не могу запустить Excel. Возможно Excel не установлен на Вашем компьютере\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return opened;
  }
  //Теперь открываем файл
  try
  { VarBooks.OleProcedure("Open", WideString(fileName));
    VarSheet=VarApp.OlePropertyGet("WorkSheets",sheetNumber);
    opened=true;
  }
  catch(const Exception &exc)
  {
    MessageDlg("Произошла ошибка при открытии файла\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
  }
  readOnly=rO;
  return opened;
}
//---------------------------------------------------------------------------
void  __fastcall DirectWorkExcel::setSheetNumber(unsigned short sn)
{
  sheetNumber=sn;
  if(opened)
    VarSheet=VarApp.OlePropertyGet("WorkSheets",sheetNumber);
}
//---------------------------------------------------------------------------
bool __fastcall DirectWorkExcel::createFile(void)
{
  //Если файл открыт, закрываем его
  if(opened) closeFile();
  if(fileName.IsEmpty())
  { MessageDlg("Не указано имя Excel файл",mtWarning,TMsgDlgButtons() << mbOK,0);
    return opened;
  }
  //Если Excel не запущен, запускаем его
  try
  { VarApp=Variant::CreateObject("Excel.Application");
    VarBooks=VarApp.OlePropertyGet("Workbooks");
  }
  catch(const Exception &exc)
  { MessageDlg("Не могу запустить Excel. Возможно Excel не установлен на Вашем компьютере\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return opened;
  }
  //Теперь открываем файл
  try
  { VarBooks.OleProcedure("Add");
    VarApp.OlePropertySet("DisplayAlerts", false);
    VarApp.OlePropertyGet("WorkSheets",sheetsQuantity).OleProcedure("SaveAs",WideString(fileName));
    VarSheet=VarApp.OlePropertyGet("WorkSheets",sheetNumber);
    opened=true;
  }
  catch(const Exception &exc)
  {
    MessageDlg("Произошла ошибка при открытии файла\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return opened;
  }
  return opened;
}
//---------------------------------------------------------------------------
bool __fastcall DirectWorkExcel::saveFile()
{
  if(readOnly==true) return true;
  if(opened==true)
  { try
    { VarApp.OlePropertySet("DisplayAlerts", false);
      VarApp.OlePropertyGet("WorkSheets",1).OleProcedure("SaveAs",WideString(fileName));
    }
    catch(const Exception &exc)
    { MessageDlg("Ошибка записи данных в файл\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
      return false;
    }
    return true;
  }
  return false;
}
//---------------------------------------------------------------------------
bool __fastcall DirectWorkExcel::closeFile(void)
{
  if(opened==true)
  { try
    { if(saveFile()==true)
      { VarApp.OlePropertyGet("WorkBooks",1).OleProcedure("Close");
        VarApp.OleProcedure("Quit");
        opened=false;
      }
    }
    catch(const Exception &exc)
    {
      MessageDlg("Не могу закрыть Excel приложение\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    }
  }
  return !opened;
}
//---------------------------------------------------------------------------
//Преоразует номер колонки в буквенный формат
AnsiString __fastcall DirectWorkExcel::ColumnConvertC1_To_A(int col)
{
AnsiString cn;

  if(col > 26)
  { cn=char('@'+col/26);
    while(col > 26) col-=26;
  }
  if(col > 0) cn+=char('@'+col);
  return cn;
}
//---------------------------------------------------------------------------
//Преобразует буквенный форма колонки в номер (число)
int __fastcall DirectWorkExcel::ColumnConvertA_ToC1(AnsiString col)
{
int cn=0;
char *str=col.c_str();

  for(int i=col.Length()-1,s=0; i>=0; i--,s++)
  { char c=str[i];
    int n=int(c)-int('A')+1;
    int p=Power(26,s);
    cn+=n*p;
  }
  return cn;
}
//---------------------------------------------------------------------------
//Формирует ссылку на ячейку в формате A1 по номеру строки и колонки
AnsiString __fastcall DirectWorkExcel::CellConvertR1C1_To_A1(int row, int col)
{
AnsiString cn=ColumnConvertC1_To_A(col);

  cn+=IntToStr(row);
  return cn;
}
//---------------------------------------------------------------------------
AnsiString DirectWorkExcel::getCellValue(int row, int col)
{
  if(opened==true)
  { Variant result,cur;
    cur = VarSheet.OlePropertyGet("Cells",row,col);
    result = cur.OlePropertyGet("Value");
    return static_cast<AnsiString>(result);
  }
  else return "";
}
//---------------------------------------------------------------------------
AnsiString DirectWorkExcel::getCellValue(int row, AnsiString col)
{
  if(opened==true)
  { Variant result,cur;
    cur = VarSheet.OlePropertyGet("Cells",row,ColumnConvertA_ToC1(col));
    result = cur.OlePropertyGet("Value");
    return static_cast<AnsiString>(result);
  }
  else return "";
}
//---------------------------------------------------------------------------
void __fastcall DirectWorkExcel::setCellValue(int row, AnsiString col, AnsiString Value)
{
  if(opened==true && readOnly==false)
  { Variant cur;
    cur = VarSheet.OlePropertyGet("Cells",row,ColumnConvertA_ToC1(col));
    cur.OlePropertySet("Value", Value.c_str());
  }
}
//---------------------------------------------------------------------------
void __fastcall DirectWorkExcel::setCellValue(AnsiString cellName, AnsiString Value)
{
  if(opened==true && readOnly==false)
  { Variant cur;
    cur = VarSheet.OlePropertyGet("Range",cellName.c_str());
    cur=cur.OlePropertyGet("Cells",1,1);
    cur.OlePropertySet("Value", Value.c_str());
  }
}
//---------------------------------------------------------------------------
void __fastcall DirectWorkExcel::setBorderCells(AnsiString lt, AnsiString rb, int ALine, int AWeight)
{
AnsiString range=lt;
Variant cur;

  if(opened && readOnly==false)
  { if(lt!=rb) range+=":"+rb;
    cur=VarSheet.OlePropertyGet("Range",range.c_str());
    for(int i=1; i<=4; i++)
    { try
      { cur.OlePropertyGet("Borders").OlePropertyGet("Item", i).OlePropertySet("Weight", 2);
        if(AWeight>0)
          cur.OlePropertyGet("Borders").OlePropertyGet("Item", i).OlePropertySet("Weight", AWeight);
      }
      catch(...)
      {
        MessageDlg("Не могу установить рамку",mtError,TMsgDlgButtons() << mbOK,0);
      }
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall DirectWorkExcel::setColorCells(AnsiString lt, AnsiString rb, int color)
{
AnsiString range=lt;
Variant cur;

  if(opened && readOnly==false)
  { if(lt!=rb) range+=":"+rb;
    cur=VarSheet.OlePropertyGet("Range",range.c_str());
    try
    {
      cur.OlePropertyGet("Interior").OlePropertySet("ColorIndex",color);
    }
    catch(...)
    {
      MessageDlg("Не могу установить цвет",mtError,TMsgDlgButtons() << mbOK,0);
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall DirectWorkExcel::setSheetLocked(AnsiString password, bool ASheetLocked)
{
  if(opened && readOnly==false)
  { if(ASheetLocked)
    {
      VarApp.OlePropertyGet("WorkSheets",sheetNumber).OleProcedure("Protect",WideString(password));
    }
    else
    {
      VarApp.OlePropertyGet("WorkSheets",sheetNumber).OleProcedure("Unprotect",WideString(password));
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall DirectWorkExcel::setMergeCells(AnsiString lt, AnsiString rb)
{
AnsiString range=lt;
  Variant cur;

  if(lt!=rb) range+=":"+rb;
  try
  {
    cur = VarSheet.OlePropertyGet("Range",range.c_str());
    cur.OleProcedure("Merge");
  }
  catch(...)
  {
    MessageDlg("Не могу объединить ячейки",mtError,TMsgDlgButtons() << mbOK,0);
  }
}
//---------------------------------------------------------------------------
void __fastcall DirectWorkExcel::setAlignment(AnsiString lt, AnsiString rb, AnsiString ha, AnsiString va, int orient)
{
Variant cur;
AnsiString range=lt;

  int h_alignment=0;
  if(ha.UpperCase()=="LEFT") h_alignment=-4131;
  else if(ha.UpperCase()=="CENTER") h_alignment=-4108;
  else if(ha.UpperCase()=="RIGHT") h_alignment=-4152;
  int v_alignment=0;
  if(va.UpperCase()=="TOP") v_alignment=-4160;
  else if(va.UpperCase()=="CENTER") v_alignment=-4108;
  else if(va.UpperCase()=="BOTTOM") v_alignment=-4107;

  try
  { if(lt!=rb) range+=":"+rb;
    cur = VarSheet.OlePropertyGet("Range",range.c_str());
    cur.OlePropertySet("HorizontalAlignment", h_alignment);
    cur.OlePropertySet("VerticalAlignment", v_alignment);
    cur.OlePropertySet("Orientation", orient);
  }
  catch(...)
  {
    MessageDlg("Ошибка установки Alignment: "+range,mtError,TMsgDlgButtons() << mbOK,0);
  }
}
//---------------------------------------------------------------------------
void __fastcall DirectWorkExcel::getFontCells(AnsiString lt, AnsiString rb, TFont *AFont)
{
AnsiString range=lt;
Variant cur;

  if(lt!=rb) range+=":"+rb;
  try
  { cur = VarSheet.OlePropertyGet("Range",range.c_str());
    AFont->Name=cur.OlePropertyGet("Font").OlePropertyGet("Name");
    AFont->Size=cur.OlePropertyGet("Font").OlePropertyGet("Size");
    AFont->Color=TColor(int(cur.OlePropertyGet("Font").OlePropertyGet("Color")));
//    if(cur.OlePropertyGet("Font").OlePropertyGet("Bold"))
//      AFont->Style.operator +=(TFontStyles()<<fsBold);
//    if(cur.OlePropertyGet("Font").OlePropertyGet("Italic"))
//      AFont->Style.operator +=(TFontStyles()<<fsItalic);
//    if(cur.OlePropertyGet("Font").OlePropertyGet("Underline"))
//      AFont->Style.operator +=(TFontStyles()<<fsUnderline);
//   if(cur.OlePropertyGet("Font").OlePropertyGet("Strikethrough"))
//      AFont->Style.operator +=(TFontStyles()<<fsStrikeOut);
  }
  catch(...)
  {
    MessageDlg("Не могу определить фонт ячейки",mtError,TMsgDlgButtons() << mbOK,0);
  }
}
//---------------------------------------------------------------------------
void __fastcall DirectWorkExcel::setFontCells(AnsiString lt, AnsiString rb, TFont *AFont)
{
AnsiString range=lt;
  Variant cur;

  if(lt!=rb) range+=":"+rb;
  try
  { cur = VarSheet.OlePropertyGet("Range",range.c_str());
    cur.OlePropertyGet("Font").OlePropertySet("Name", AFont->Name.c_str());
    cur.OlePropertyGet("Font").OlePropertySet("Size", AFont->Size);
    cur.OlePropertyGet("Font").OlePropertySet("Color", AFont->Color);
    cur.OlePropertyGet("Font").OlePropertySet("Bold", AFont->Style.Contains(fsBold));
    cur.OlePropertyGet("Font").OlePropertySet("Italic", AFont->Style.Contains(fsItalic));
    cur.OlePropertyGet("Font").OlePropertySet("Underline", AFont->Style.Contains(fsUnderline));
    cur.OlePropertyGet("Font").OlePropertySet("Strikethrough", AFont->Style.Contains(fsStrikeOut));
  }
  catch(...)
  {
    MessageDlg("Не могу установить фонт ячейки",mtError,TMsgDlgButtons() << mbOK,0);
  }
}
//---------------------------------------------------------------------------
void __fastcall DirectWorkExcel::setWrapText(AnsiString lt, AnsiString rb, bool wt)
{
Variant cur;
AnsiString range=lt;

  try
  { if(lt!=rb) range+=":"+rb;
    cur = VarSheet.OlePropertyGet("Range",range.c_str());
    cur.OlePropertySet("WrapText", wt);
  }
  catch(...)
  {
    MessageDlg("Ошибка установки WrapText: "+range,mtError,TMsgDlgButtons() << mbOK,0);
  }
}
//---------------------------------------------------------------------------
void __fastcall DirectWorkExcel::insertRow(int ARow)
{
Variant cur;

  try
  {
    AnsiString range=IntToStr(ARow)+":"+IntToStr(ARow);
    int xlDown=-4121;
    cur = VarSheet.OlePropertyGet("Rows",range.c_str());
    cur.OleFunction("Insert", xlDown);  //xlDown=-4121
  }
  catch(...)
  {
    MessageDlg("Не могу вставить строку в Excel таблицу",mtError,TMsgDlgButtons() << mbOK,0);
  }
}
//---------------------------------------------------------------------------
void __fastcall DirectWorkExcel::deleteRow(int ARow)
{
Variant cur;

  try
  {
    AnsiString range=IntToStr(ARow)+":"+IntToStr(ARow);
    int xlUp=-4162;
    cur = VarSheet.OlePropertyGet("Rows",range.c_str());
    cur.OleFunction("Delete", xlUp);  //xlUp=-4162
  }
  catch(...)
  {
    MessageDlg("Ошибка при удалении строки в Excel таблице",mtError,TMsgDlgButtons() << mbOK,0);
  }
}
//---------------------------------------------------------------------------
void __fastcall DirectWorkExcel::copyRange(AnsiString lt, AnsiString rb)
{
Variant cur;
AnsiString range=lt;

  try
  { if(lt!=rb) range+=":"+rb;
    cur = VarSheet.OlePropertyGet("Range",range.c_str());
    cur.OleProcedure("Select");
    cur.OleProcedure("Copy");
  }
  catch(...)
  {
    MessageDlg("Ошибка копирования области Excel таблицы: "+range,mtError,TMsgDlgButtons() << mbOK,0);
  }
}
//---------------------------------------------------------------------------
void __fastcall DirectWorkExcel::pasteRange(AnsiString lt)
{
Variant cur;
AnsiString range=lt;

  try
  { cur = VarSheet.OlePropertyGet("Range",range.c_str());
    cur.OleProcedure("Select");
    VarSheet.OleProcedure("Paste");
  }
  catch(...)
  {
    MessageDlg("Ошибка вставки области Excel таблицы: "+range,mtError,TMsgDlgButtons() << mbOK,0);
  }
}
//---------------------------------------------------------------------------
