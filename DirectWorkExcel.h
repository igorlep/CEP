//Класс для работы с Excel файлами

#ifndef DirectWorkExcelH
#define DirectWorkExcelH
//-----------------------------------------------------------------------------
#include <Classes.hpp>
#include <NMsmtp.hpp>
//-----------------------------------------------------------------------------
class DirectWorkExcel
{
private:
  AnsiString fileName;            //полное имя файла
  unsigned short sheetsQuantity;  //количестов листов
  unsigned short sheetNumber;     //номер рабочего листа
  Variant VarApp, VarBooks, VarBook, VarSheet; //интерфейс для работы с Excel
  bool opened;                    //флаг - файл открыт/закрыт
  bool readOnly;                  //флаг - только чтение файла
public:
  __fastcall DirectWorkExcel(AnsiString fileName, unsigned short sheetsQuantity);
  __fastcall ~DirectWorkExcel(void);
  bool __fastcall openFile(bool readOnly);
  bool __fastcall createFile(void);
  bool __fastcall saveFile(void);
  bool __fastcall closeFile(void);
  void  __fastcall setSheetNumber(unsigned short sn);
  unsigned short  __fastcall getSheetNumber(void){ return sheetNumber; }
  //
  AnsiString __fastcall ColumnConvertC1_To_A(int col);
  int __fastcall ColumnConvertA_ToC1(AnsiString col);
  AnsiString __fastcall CellConvertR1C1_To_A1(int row, int col);
  AnsiString __fastcall Cell_A1(int row, AnsiString col){ return CellConvertR1C1_To_A1(row,ColumnConvertA_ToC1(col));}
  //
  AnsiString getCellValue(int row, int col);
  AnsiString getCellValue(int row, AnsiString col);
  void __fastcall setCellValue(int row, AnsiString col, AnsiString Value);
  void __fastcall setCellValue(AnsiString cellName, AnsiString Value);
  void __fastcall setBorderCells(AnsiString lt, AnsiString rb, int ALine=1, int AWeight=2);
  void __fastcall setColorCells(AnsiString lt, AnsiString rb, int color);
  void __fastcall setMergeCells(AnsiString lt, AnsiString rb);
  void __fastcall setSheetLocked(AnsiString password, bool ASheetLocked);
  void __fastcall setAlignment(AnsiString lt, AnsiString rb, AnsiString ha, AnsiString va, int orient);
  void __fastcall getFontCells(AnsiString lt, AnsiString rb, TFont *AFont);
  void __fastcall setFontCells(AnsiString lt, AnsiString rb, TFont *AFont);
  void __fastcall setWrapText(AnsiString lt, AnsiString rb, bool wt);
  void __fastcall insertRow(int ARow);
  void __fastcall deleteRow(int ARow);
  void __fastcall copyRange(AnsiString lt, AnsiString rb);
  void __fastcall pasteRange(AnsiString lt);
};
//-----------------------------------------------------------------------------

#endif
