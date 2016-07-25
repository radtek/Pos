{*************************************************************************}
{ Rave Reports version 5.1                                                }
{ Copyright (c), 1995-2002, Nevrona Designs, all rights reserved          }
{*************************************************************************}
unit RpRenderPDF;

interface

{$I RpVer.inc}
{$IFDEF LEVEL6}{$WARN SYMBOL_DEPRECATED OFF}{$ENDIF}

uses
  Windows,
  Graphics, JPEG,
  Classes, SysUtils, RpRender, RpDefine;

const
  NUM_CHARS = 256;

type
  TFontWidthTable = array [0..NUM_CHARS-1] of smallint;
  TPDFFontStyles = (psNormal, psBold, psItalic, psBoldItalic);
  TPDFFontEncoding = (feMacRomanEncoding, feMacExpertEncoding, feWinAnsiEncoding,
                      fePDFDocEncoding, feStandardEncoding);
  TEllipsePts = record
    XA,XB,XC,XD,XE,
    YA,YB,YC,YD,YE: double
  end;
  TSmallArcRecord = record
    X1, X2, X3, X4: double;
    Y1, Y2, Y3, Y4: double;
  end;
  TSmallArcBuffer = array[1..4] of TSmallArcRecord;

const
  PDFVersion: string = '%PDF-1.3';
  DPI_MULTIPLIER = 72;
  FontNames: array[0..13] of string = (
    'Courier','CourierBold','CourierItalic','CourierBoldItalic',
    'Helvetica','HelveticaBold','HelveticaItalic','HelveticaBoldItalic',
    'TimesRoman','TimesRomanBold','TimesRomanItalic','TimesRomanBoldItalic',
    'Symbol','ZapfDingbats'
  );

  FontEncodingNames: array[feMacRomanEncoding..feStandardEncoding] of string = (
   'MacRomanEncoding','MacExpertEncoding', 'WinAnsiEncoding',
   'PDFDocEncoding', 'StandardEncoding');

  FontWidthsCourier: TFontWidthTable = (
    {   0 }  600,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    {  10 }    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    {  20 }    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    {  30 }    0,    0,  600,  600,  600,  600,  600,  600,  600,  600,
    {  40 }  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,
    {  50 }  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,
    {  60 }  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,
    {  70 }  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,
    {  80 }  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,
    {  90 }  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,
    { 100 }  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,
    { 110 }  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,
    { 120 }  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,
    { 130 }  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,
    { 140 }  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,
    { 150 }  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,
    { 160 }  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,
    { 170 }  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,
    { 180 }  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,
    { 190 }  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,
    { 200 }  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,
    { 210 }  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,
    { 220 }  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,
    { 230 }  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,
    { 240 }  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,
    { 250 }  600,  600,  600,  600,  600,  600);

  FontWidthsHelvetica: TFontWidthTable = (
    {   0 }  278,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    {  10 }    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    {  20 }    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    {  30 }    0,    0,  278,  278,  355,  556,  556,  889,  667,  191,
    {  40 }  333,  333,  389,  584,  278,  333,  278,  278,  556,  556,
    {  50 }  556,  556,  556,  556,  556,  556,  556,  556,  278,  278,
    {  60 }  584,  584,  584,  556, 1015,  667,  667,  722,  722,  667,
    {  70 }  611,  778,  722,  278,  500,  667,  556,  833,  722,  778,
    {  80 }  667,  778,  722,  667,  611,  722,  667,  944,  667,  667,
    {  90 }  611,  278,  278,  278,  469,  556,  333,  556,  556,  500,
    { 100 }  556,  556,  278,  556,  556,  222,  222,  500,  222,  833,
    { 110 }  556,  556,  556,  556,  333,  500,  278,  556,  500,  722,
    { 120 }  500,  500,  500,  334,  260,  334,  584,  350,  558,  350,
    { 130 }  222,  556,  333, 1000,  556,  556,  333, 1000,  667,  333,
    { 140 } 1000,  350,  611,  350,  350,  222,  222,  333,  333,  350,
    { 150 }  556, 1000,  333, 1000,  500,  333,  944,  350,  500,  667,
    { 160 }  278,  333,  556,  556,  556,  556,  260,  556,  333,  737,
    { 170 }  370,  556,  584,  333,  737,  333,  333,  584,  333,  333,
    { 180 }  333,  556,  537,  278,  333,  333,  365,  556,  834,  834,
    { 190 }  834,  611,  667,  667,  667,  667,  667,  667, 1000,  722,
    { 200 }  667,  667,  667,  667,  278,  278,  278,  278,  722,  722,
    { 210 }  778,  778,  778,  778,  778,  584,  778,  722,  722,  722,
    { 220 }  722,  667,  667,  611,  556,  556,  556,  556,  556,  556,
    { 230 }  889,  500,  556,  556,  556,  556,  278,  278,  278,  278,
    { 240 }  556,  556,  556,  556,  556,  556,  556,  584,  611,  556,
    { 250 }  556,  556,  556,  500,  556,  500);

  FontWidthsHelveticaBold: TFontWidthTable = (
    {   0 }  278,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    {  10 }    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    {  20 }    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    {  30 }    0,    0,  278,  333,  474,  556,  556,  889,  722,  238,
    {  40 }  333,  333,  389,  584,  278,  333,  278,  278,  556,  556,
    {  50 }  556,  556,  556,  556,  556,  556,  556,  556,  333,  333,
    {  60 }  584,  584,  584,  611,  975,  722,  722,  722,  722,  667,
    {  70 }  611,  778,  722,  278,  556,  722,  611,  833,  722,  778,
    {  80 }  667,  778,  722,  667,  611,  722,  667,  944,  667,  667,
    {  90 }  611,  333,  278,  333,  584,  556,  333,  556,  611,  556,
    { 100 }  611,  556,  333,  611,  611,  278,  278,  556,  278,  889,
    { 110 }  611,  611,  611,  611,  389,  556,  333,  611,  556,  778,
    { 120 }  556,  556,  500,  389,  280,  389,  584,  350,  558,  350,
    { 130 }  278,  556,  500, 1000,  556,  556,  333, 1000,  667,  333,
    { 140 } 1000,  350,  611,  350,  350,  278,  278,  500,  500,  350,
    { 150 }  556, 1000,  333, 1000,  556,  333,  944,  350,  500,  667,
    { 160 }  278,  333,  556,  556,  556,  556,  280,  556,  333,  737,
    { 170 }  370,  556,  584,  333,  737,  333,  333,  584,  333,  333,
    { 180 }  333,  611,  556,  278,  333,  333,  365,  556,  834,  834,
    { 190 }  834,  611,  722,  722,  722,  722,  722,  722, 1000,  722,
    { 200 }  667,  667,  667,  667,  278,  278,  278,  278,  722,  722,
    { 210 }  778,  778,  778,  778,  778,  584,  778,  722,  722,  722,
    { 220 }  722,  667,  667,  611,  556,  556,  556,  556,  556,  556,
    { 230 }  889,  556,  556,  556,  556,  556,  278,  278,  278,  278,
    { 240 }  611,  611,  611,  611,  611,  611,  611,  584,  611,  611,
    { 250 }  611,  611,  611,  556,  611,  556);

  FontWidthsHelveticaOblique: TFontWidthTable = (
    {   0 }  278,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    {  10 }    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    {  20 }    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    {  30 }    0,    0,  278,  278,  355,  556,  556,  889,  667,  191,
    {  40 }  333,  333,  389,  584,  278,  333,  278,  278,  556,  556,
    {  50 }  556,  556,  556,  556,  556,  556,  556,  556,  278,  278,
    {  60 }  584,  584,  584,  556, 1015,  667,  667,  722,  722,  667,
    {  70 }  611,  778,  722,  278,  500,  667,  556,  833,  722,  778,
    {  80 }  667,  778,  722,  667,  611,  722,  667,  944,  667,  667,
    {  90 }  611,  278,  278,  278,  469,  556,  333,  556,  556,  500,
    { 100 }  556,  556,  278,  556,  556,  222,  222,  500,  222,  833,
    { 110 }  556,  556,  556,  556,  333,  500,  278,  556,  500,  722,
    { 120 }  500,  500,  500,  334,  260,  334,  584,  350,  558,  350,
    { 130 }  222,  556,  333, 1000,  556,  556,  333, 1000,  667,  333,
    { 140 } 1000,  350,  611,  350,  350,  222,  222,  333,  333,  350,
    { 150 }  556, 1000,  333, 1000,  500,  333,  944,  350,  500,  667,
    { 160 }  278,  333,  556,  556,  556,  556,  260,  556,  333,  737,
    { 170 }  370,  556,  584,  333,  737,  333,  333,  584,  333,  333,
    { 180 }  333,  556,  537,  278,  333,  333,  365,  556,  834,  834,
    { 190 }  834,  611,  667,  667,  667,  667,  667,  667, 1000,  722,
    { 200 }  667,  667,  667,  667,  278,  278,  278,  278,  722,  722,
    { 210 }  778,  778,  778,  778,  778,  584,  778,  722,  722,  722,
    { 220 }  722,  667,  667,  611,  556,  556,  556,  556,  556,  556,
    { 230 }  889,  500,  556,  556,  556,  556,  278,  278,  278,  278,
    { 240 }  556,  556,  556,  556,  556,  556,  556,  584,  611,  556,
    { 250 }  556,  556,  556,  500,  556,  500);

  FontWidthsHelveticaBoldOblique: TFontWidthTable = (
    {   0 }  278,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    {  10 }    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    {  20 }    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    {  30 }    0,    0,  278,  333,  474,  556,  556,  889,  722,  238,
    {  40 }  333,  333,  389,  584,  278,  333,  278,  278,  556,  556,
    {  50 }  556,  556,  556,  556,  556,  556,  556,  556,  333,  333,
    {  60 }  584,  584,  584,  611,  975,  722,  722,  722,  722,  667,
    {  70 }  611,  778,  722,  278,  556,  722,  611,  833,  722,  778,
    {  80 }  667,  778,  722,  667,  611,  722,  667,  944,  667,  667,
    {  90 }  611,  333,  278,  333,  584,  556,  333,  556,  611,  556,
    { 100 }  611,  556,  333,  611,  611,  278,  278,  556,  278,  889,
    { 110 }  611,  611,  611,  611,  389,  556,  333,  611,  556,  778,
    { 120 }  556,  556,  500,  389,  280,  389,  584,  350,  558,  350,
    { 130 }  278,  556,  500, 1000,  556,  556,  333, 1000,  667,  333,
    { 140 } 1000,  350,  611,  350,  350,  278,  278,  500,  500,  350,
    { 150 }  556, 1000,  333, 1000,  556,  333,  944,  350,  500,  667,
    { 160 }  278,  333,  556,  556,  556,  556,  280,  556,  333,  737,
    { 170 }  370,  556,  584,  333,  737,  333,  333,  584,  333,  333,
    { 180 }  333,  611,  556,  278,  333,  333,  365,  556,  834,  834,
    { 190 }  834,  611,  722,  722,  722,  722,  722,  722, 1000,  722,
    { 200 }  667,  667,  667,  667,  278,  278,  278,  278,  722,  722,
    { 210 }  778,  778,  778,  778,  778,  584,  778,  722,  722,  722,
    { 220 }  722,  667,  667,  611,  556,  556,  556,  556,  556,  556,
    { 230 }  889,  556,  556,  556,  556,  556,  278,  278,  278,  278,
    { 240 }  611,  611,  611,  611,  611,  611,  611,  584,  611,  611,
    { 250 }  611,  611,  611,  556,  611,  556);

  FontWidthsTimesRoman: TFontWidthTable = (
    {   0 }  250,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    {  10 }    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    {  20 }    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    {  30 }    0,    0,  250,  333,  408,  500,  500,  833,  778,  180,
    {  40 }  333,  333,  500,  564,  250,  333,  250,  278,  500,  500,
    {  50 }  500,  500,  500,  500,  500,  500,  500,  500,  278,  278,
    {  60 }  564,  564,  564,  444,  921,  722,  667,  667,  722,  611,
    {  70 }  556,  722,  722,  333,  389,  722,  611,  889,  722,  722,
    {  80 }  556,  722,  667,  556,  611,  722,  722,  944,  722,  722,
    {  90 }  611,  333,  278,  333,  469,  500,  333,  444,  500,  444,
    { 100 }  500,  444,  333,  500,  500,  278,  278,  500,  278,  778,
    { 110 }  500,  500,  500,  500,  333,  389,  278,  500,  500,  722,
    { 120 }  500,  500,  444,  480,  200,  480,  541,  350,  500,  350,
    { 130 }  333,  500,  444, 1000,  500,  500,  333, 1000,  556,  333,
    { 140 }  889,  350,  611,  350,  350,  333,  333,  444,  444,  350,
    { 150 }  500, 1000,  333,  980,  389,  333,  722,  350,  444,  722,
    { 160 }  250,  333,  500,  500,  500,  500,  200,  500,  333,  760,
    { 170 }  276,  500,  564,  333,  760,  333,  333,  564,  300,  300,
    { 180 }  333,  500,  453,  250,  333,  300,  310,  500,  750,  750,
    { 190 }  750,  444,  722,  722,  722,  722,  722,  722,  889,  667,
    { 200 }  611,  611,  611,  611,  333,  333,  333,  333,  722,  722,
    { 210 }  722,  722,  722,  722,  722,  564,  722,  722,  722,  722,
    { 220 }  722,  722,  556,  500,  444,  444,  444,  444,  444,  444,
    { 230 }  667,  444,  444,  444,  444,  444,  278,  278,  278,  278,
    { 240 }  500,  500,  500,  500,  500,  500,  500,  564,  500,  500,
    { 250 }  500,  500,  500,  500,  500,  500);

  FontWidthsTimesBold: TFontWidthTable = (
    {   0 }  250,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    {  10 }    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    {  20 }    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    {  30 }    0,    0,  250,  333,  555,  500,  500, 1000,  833,  278,
    {  40 }  333,  333,  500,  570,  250,  333,  250,  278,  500,  500,
    {  50 }  500,  500,  500,  500,  500,  500,  500,  500,  333,  333,
    {  60 }  570,  570,  570,  500,  930,  722,  667,  722,  722,  667,
    {  70 }  611,  778,  778,  389,  500,  778,  667,  944,  722,  778,
    {  80 }  611,  778,  722,  556,  667,  722,  722, 1000,  722,  722,
    {  90 }  667,  333,  278,  333,  581,  500,  333,  500,  556,  444,
    { 100 }  556,  444,  333,  500,  556,  278,  333,  556,  278,  833,
    { 110 }  556,  500,  556,  556,  444,  389,  333,  556,  500,  722,
    { 120 }  500,  500,  444,  394,  220,  394,  520,  350,  500,  350,
    { 130 }  333,  500,  500, 1000,  500,  500,  333, 1000,  556,  333,
    { 140 } 1000,  350,  667,  350,  350,  333,  333,  500,  500,  350,
    { 150 }  500, 1000,  333, 1000,  389,  333,  722,  350,  444,  722,
    { 160 }  250,  333,  500,  500,  500,  500,  220,  500,  333,  747,
    { 170 }  300,  500,  570,  333,  747,  333,  333,  570,  300,  300,
    { 180 }  333,  556,  540,  250,  333,  300,  330,  500,  750,  750,
    { 190 }  750,  500,  722,  722,  722,  722,  722,  722, 1000,  722,
    { 200 }  667,  667,  667,  667,  389,  389,  389,  389,  722,  722,
    { 210 }  778,  778,  778,  778,  778,  570,  778,  722,  722,  722,
    { 220 }  722,  722,  611,  556,  500,  500,  500,  500,  500,  500,
    { 230 }  722,  444,  444,  444,  444,  444,  278,  278,  278,  278,
    { 240 }  500,  556,  500,  500,  500,  500,  500,  570,  500,  556,
    { 250 }  556,  556,  556,  500,  556,  500);

  FontWidthsTimesItalic: TFontWidthTable = (
    {   0 }  250,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    {  10 }    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    {  20 }    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    {  30 }    0,    0,  250,  333,  420,  500,  500,  833,  778,  214,
    {  40 }  333,  333,  500,  675,  250,  333,  250,  278,  500,  500,
    {  50 }  500,  500,  500,  500,  500,  500,  500,  500,  333,  333,
    {  60 }  675,  675,  675,  500,  920,  611,  611,  667,  722,  611,
    {  70 }  611,  722,  722,  333,  444,  667,  556,  833,  667,  722,
    {  80 }  611,  722,  611,  500,  556,  722,  611,  833,  611,  556,
    {  90 }  556,  389,  278,  389,  422,  500,  333,  500,  500,  444,
    { 100 }  500,  444,  278,  500,  500,  278,  278,  444,  278,  722,
    { 110 }  500,  500,  500,  500,  389,  389,  278,  500,  444,  667,
    { 120 }  444,  444,  389,  400,  275,  400,  541,  350,  500,  350,
    { 130 }  333,  500,  556,  889,  500,  500,  333, 1000,  500,  333,
    { 140 }  944,  350,  556,  350,  350,  333,  333,  556,  556,  350,
    { 150 }  500,  889,  333,  980,  389,  333,  667,  350,  389,  556,
    { 160 }  250,  389,  500,  500,  500,  500,  275,  500,  333,  760,
    { 170 }  276,  500,  675,  333,  760,  333,  333,  675,  300,  300,
    { 180 }  333,  500,  523,  250,  333,  300,  310,  500,  750,  750,
    { 190 }  750,  500,  611,  611,  611,  611,  611,  611,  889,  667,
    { 200 }  611,  611,  611,  611,  333,  333,  333,  333,  722,  667,
    { 210 }  722,  722,  722,  722,  722,  675,  722,  722,  722,  722,
    { 220 }  722,  556,  611,  500,  500,  500,  500,  500,  500,  500,
    { 230 }  667,  444,  444,  444,  444,  444,  278,  278,  278,  278,
    { 240 }  500,  500,  500,  500,  500,  500,  500,  675,  500,  500,
    { 250 }  500,  500,  500,  444,  500,  444);

  FontWidthsTimesBoldItalic: TFontWidthTable = (
    {   0 }  250,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    {  10 }    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    {  20 }    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    {  30 }    0,    0,  250,  389,  555,  500,  500,  833,  778,  278,
    {  40 }  333,  333,  500,  570,  250,  333,  250,  278,  500,  500,
    {  50 }  500,  500,  500,  500,  500,  500,  500,  500,  333,  333,
    {  60 }  570,  570,  570,  500,  832,  667,  667,  667,  722,  667,
    {  70 }  667,  722,  778,  389,  500,  667,  611,  889,  722,  722,
    {  80 }  611,  722,  667,  556,  611,  722,  667,  889,  667,  611,
    {  90 }  611,  333,  278,  333,  570,  500,  333,  500,  500,  444,
    { 100 }  500,  444,  333,  500,  556,  278,  278,  500,  278,  778,
    { 110 }  556,  500,  500,  500,  389,  389,  278,  556,  444,  667,
    { 120 }  500,  444,  389,  348,  220,  348,  570,  350,  500,  350,
    { 130 }  333,  500,  500, 1000,  500,  500,  333, 1000,  556,  333,
    { 140 }  944,  350,  611,  350,  350,  333,  333,  500,  500,  350,
    { 150 }  500, 1000,  333, 1000,  389,  333,  722,  350,  389,  611,
    { 160 }  250,  389,  500,  500,  500,  500,  220,  500,  333,  747,
    { 170 }  266,  500,  606,  333,  747,  333,  333,  570,  300,  300,
    { 180 }  333,  576,  500,  250,  333,  300,  300,  500,  750,  750,
    { 190 }  750,  500,  667,  667,  667,  667,  667,  667,  944,  667,
    { 200 }  667,  667,  667,  667,  389,  389,  389,  389,  722,  722,
    { 210 }  722,  722,  722,  722,  722,  570,  722,  722,  722,  722,
    { 220 }  722,  611,  611,  500,  500,  500,  500,  500,  500,  500,
    { 230 }  722,  444,  444,  444,  444,  444,  278,  278,  278,  278,
    { 240 }  500,  556,  500,  500,  500,  500,  500,  570,  500,  556,
    { 250 }  556,  556,  556,  444,  500,  444);

  FontWidthsSymbol: TFontWidthTable = (
    // 12 Symbol --- This is in StandardEncoding
    {   0 }   250,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    {  10 }     0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    {  20 }     0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    {  30 }     0,    0,  250,  333,  713,  500,  549,  833,  778,  439,
    {  40 }   333,  333,  500,  549,  250,  549,  250,  278,  500,  500,
    {  50 }   500,  500,  500,  500,  500,  500,  500,  500,  278,  278,
    {  60 }   549,  549,  549,  444,  549,  722,  667,  722,  612,  611,
    {  70 }   763,  603,  722,  333,  631,  722,  686,  889,  722,  722,
    {  80 }   768,  741,  556,  592,  611,  690,  439,  768,  645,  795,
    {  90 }   611,  333,  863,  333,  658,  500,  500,  631,  549,  549,
    { 100 }   494,  439,  521,  411,  603,  329,  603,  549,  549,  576,
    { 110 }   521,  549,  549,  521,  549,  603,  439,  576,  713,  686,
    { 120 }   493,  686,  494,  480,  200,  480,  549,    0,    0,    0,
    { 130 }     0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    { 140 }     0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    { 150 }     0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    { 160 }     0,  620,  247,  549,  167,  713,  500,  753,  753,  753,
    { 170 }   753, 1042,  987,  603,  987,  603,  400,  549,  411,  549,
    { 180 }   549,  713,  494,  460,  549,  549,  549,  549, 1000,  603,
    { 190 }  1000,  658,  823,  686,  795,  987,  768,  768,  823,  768,
    { 200 }   768,  713,  713,  713,  713,  713,  713,  713,  768,  713,
    { 210 }   790,  790,  890,  823,  549,  250,  713,  603,  603, 1042,
    { 220 }   987,  603,  987,  603,  494,  329,  790,  790,  786,  713,
    { 230 }   384,  384,  384,  384,  384,  384,  494,  494,  494,  494,
    { 240 }     0,  329,  274,  686,  686,  686,  384,  384,  384,  384,
    { 250 }   384,  384,  494,  494,  494,    0);

  FontWidthsZapfDingbats: TFontWidthTable = (
    // 13 ZapfDingbats --- This is in StandardEncoding
    {   0 }   250,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    {  10 }     0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    {  20 }     0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    {  30 }     0,    0,  278,  974,  961,  974,  980,  719,  789,  790,
    {  40 }   791,  690,  960,  939,  549,  855,  911,  933,  911,  945,
    {  50 }   974,  755,  846,  762,  761,  571,  677,  763,  760,  759,
    {  60 }   754,  494,  552,  537,  577,  692,  786,  788,  788,  790,
    {  70 }   793,  794,  816,  823,  789,  841,  823,  833,  816,  831,
    {  80 }   923,  744,  723,  749,  790,  792,  695,  776,  768,  792,
    {  90 }   759,  707,  708,  682,  701,  826,  815,  789,  789,  707,
    { 100 }   687,  696,  689,  786,  787,  713,  791,  785,  791,  873,
    { 110 }   761,  762,  762,  759,  759,  892,  892,  788,  784,  438,
    { 120 }   138,  277,  415,  392,  392,  668,  668,    0,    0,    0,
    { 130 }     0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    { 140 }     0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    { 150 }     0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    { 160 }     0,  732,  544,  544,  910,  667,  760,  760,  776,  595,
    { 170 }   694,  626,  788,  788,  788,  788,  788,  788,  788,  788,
    { 180 }   788,  788,  788,  788,  788,  788,  788,  788,  788,  788,
    { 190 }   788,  788,  788,  788,  788,  788,  788,  788,  788,  788,
    { 200 }   788,  788,  788,  788,  788,  788,  788,  788,  788,  788,
    { 210 }   788,  788,  894,  838, 1016,  458,  748,  924,  748,  918,
    { 220 }   927,  928,  928,  834,  873,  828,  924,  924,  917,  930,
    { 230 }   931,  463,  883,  836,  836,  867,  867,  696,  696,  874,
    { 240 }     0,  874,  760,  946,  771,  865,  771,  888,  967,  888,
    { 250 }   831,  873,  927,  970,  918,    0);

type
  TRPPDFDocInfo = class(TPersistent)
  protected
    FTitle: string;
    FAuthor: string;
    FSubject: string;
    FKeyWords: string;
    FCreator: string;
    FProducer: string;

  public
    constructor Create; virtual;

  published
    property Title: string read FTitle write FTitle;
    property Author: string read FAuthor write FAuthor;
    property Subject: string read FSubject write FSubject;
    property KeyWords: string read FKeywords write FKeyWords;
    property Creator: string read FCreator write FCreator;
    property Producer: string read FProducer write FProducer;
  end;

  TRvRenderPDF = class;

  TRPPDFObject = class(TObject)
  protected
    FRvRenderPDF: TRvRenderPDF;
    FID: integer;
    FDataStream: TMemoryStream;

    procedure SetID(AValue: integer);
    function GetID: integer;

    procedure InitData; virtual; abstract;

  public
    constructor Create(AOwner: TObject); virtual;
    destructor Destroy; override;

    property DataStream: TMemoryStream read FDataStream;
    property RvRenderPDF: TRvRenderPDF read FRvRenderPDF write FRvRenderPDF;

  end;

  TRPPDFCatalog = class(TRPPDFObject)
  protected
    FShowOutlines: boolean;

  public
    procedure InitData; override;

    property ShowOutlines: boolean read FShowOutlines write FShowOutlines;
  end;

  TRPPDFPages = class(TRPPDFObject)
  protected
    FOpenPage: integer;
  public
    procedure InitData; override;
    function GetPageReferences: string;

    property OpenPage: integer read FOpenPage write FOpenPage;
  end;

  TRPPDFPageContent = class(TRPPDFObject)
  protected
    FPageStream: TMemoryStream;

  public
    constructor Create(AOwner: TObject); override;
    destructor Destroy; override;
    procedure InitData; override;

    property PageStream: TMemoryStream read FPageStream;
  end;

  TRPPDFPage = class(TRPPDFObject)
  protected
    FPageContent: TRPPDFPageContent;
    FPageWidth: double;
    FPageHeight: double;
    FTextColor: TColor;
    FLastTextColor: TColor;
    FLastTextColorDefined: boolean;
    FPenColor: TColor;
    FLastPenColor: TColor;
    FLastPenColorDefined: boolean;
    FLastPenWidth: double;
    FLastPenWidthDefined: boolean;
    FLastPenStyle: TPenStyle;
    FLastPenStyleDefined: boolean;
    FBrushColor: TColor;
    FLastBrushColor: TColor;
    FLastBrushColorDefined: boolean;
    FLastBrushStyle: TBrushStyle;
    FLastBrushStyleDefined: boolean;

    property LastTextColorDefined: boolean read FLastTextColorDefined write FLastTextColorDefined;
    property LastPenColorDefined: boolean read FLastPenColorDefined write FLastPenColorDefined;
    property LastPenWidthDefined: boolean read FLastPenWidthDefined write FLastPenWidthDefined;
    property LastPenStyleDefined: boolean read FLastPenStyleDefined write FLastPenStyleDefined;
    property LastBrushStyleDefined: boolean read FLastBrushStyleDefined write FLastBrushStyleDefined;
    property LastBrushColorDefined: boolean read FLastBrushColorDefined write FLastBrushColorDefined;
    function InchesToUnitsX(const AValue: double): double;
    function InchesToUnitsY(const AValue: double): double;
    function FormatRPPDFText(const AValue: string): string;
    procedure ClosePath;
    // Rotate (x, y) by angle in degrees, and return output in (xrot, yrot).
    procedure RotateXYCoordinate(AX1, AY1, AAngle: double; var xRot, yRot: double);
    function DegreesToRadians(ADegrees: double): double;
    procedure SmallArc(AX1, AY1, AR1, MidTheta, HalfAngle: double; ccwcw: double; AIndex: integer);
    procedure RadiusArc(const AX1, AY1, AR1, sAngle, eAngle: double);
    procedure AdjustRotationOffset(const ATextWidth: double;
                                   var AX, AY: double);

  public
    constructor Create(AOwner: TObject); override;
    destructor Destroy; override;
    procedure InitData; override;

    function GetRPPDFColor(Color: TColor; bAExpanded: boolean): string;
    function GetEllipsePts(const AX1, AY1, AX2, AY2: double): TEllipsePts;

    procedure PrintCharSpacing;
    procedure PrintWordSpacing;
    procedure PrintTextColor;
    procedure PrintPenColor;
    procedure PrintBrushColor;
    procedure PrintLeft(const AX, AY: double; AText: string);
    procedure PrintCenter(const AX, AY: double; AText: string);
    procedure PrintRight(const AX, AY: double; AText: string);
    procedure TextRect(Rect: TRect; X1,Y1: double; S1: string);
    procedure SetPenAndBrush;
    procedure MoveTo(const AX, AY: double);
    procedure LineTo(const AX, AY: double; DoClosePath: boolean);
    procedure Arc(const pfX1, pfY1, pfX2, pfY2, pfX3, pfY3, pfX4, pfY4: double; DoClosePath, AllowFill: boolean);
    procedure Rectangle(const AX1, AY1, AX2, AY2: double; const IsClipping: boolean);
    procedure RoundRect(const AX1, AY1, AX2, AY2, AX3, AY3: double);
    procedure CurveTo(const AX1, AY1, AX2, AY2, AX3, AY3: double);
    procedure Ellipse(const AX1, AY1, AX2, AY2: double);
    procedure PrintJPG(const AX1, AY1, AX2, AY2: double; AName: string);
    procedure Pie(const pfX1, pfY1, pfX2, pfY2, pfX3, pfY3, pfX4, pfY4: double);
    procedure PolyLine(const PolyLineArr: array of TFloatPoint);
    procedure Polygon(const PointArr: array of TFloatPoint);

    property PageContent: TRPPDFPageContent read FPageContent;
    property PageWidth: double read FPageWidth write FPageWidth;
    property PageHeight: double read FPageHeight write FPageHeight;
    property TextColor: TColor read FTextColor write FTextColor;
    property LastTextColor: TColor read FLastTextColor write FLastTextColor;
    property PenColor: TColor read FPenColor write FPenColor;
    property LastPenColor: TColor read FLastPenColor write FLastPenColor;
    property LastPenWidth: double read FLastPenWidth write FLastPenWidth;
    property LastPenStyle: TPenStyle read FLastPenStyle write FLastPenStyle;
    property LastBrushStyle: TBrushStyle read FLastBrushStyle write FLastBrushStyle;
    property BrushColor: TColor read FBrushColor write FBrushColor;
    property LastBrushColor: TColor read FLastBrushColor write FLastBrushColor;
  end;

  TRPPDFOutlines = class(TRPPDFObject)
  protected

  public
    procedure InitData; override;
  end;

  TRPPDFProcSet = class(TRPPDFObject)
  protected

  public
    procedure InitData; override;
  end;

  TRPPDFPatternResource = class (TRPPDFObject)
  protected
    FResourceStream: string;

    property ResourceStream: string read FResourceStream write FResourceStream;
  public
    procedure InitData; override;
  end;

  TRPPDFPattern = class (TRPPDFObject)
  protected
    FObjectName: string;
    FPatternStream: string;
    FPatternResource: TRPPDFPatternResource;

    property ObjectName: string read FObjectName write FObjectName;
    property PatternStream: string read FPatternStream write FPatternStream;
    property PatternResource: TRPPDFPatternResource read FPatternResource write FPatternResource;
  public
    constructor Create(AOwner: TObject); override;
    destructor Destroy; override;

    procedure InitData; override;
    procedure SetPattern(AStyle: TBrushStyle; AFillColor: TColor);

  end;

  TRPPDFFont = class(TRPPDFObject)
  protected
    FFontType: string;
    FObjectName: string;
    FFontName: string;
    FontWidths: TFontWidthTable;
    FPDFFontName: string;
    FFontStyles: TFontStyles;
    FFontCharset: TFontCharset;
    FFirstChar: integer;
    FLastChar: integer;

    procedure SetCharSet(AFontCharset: TFontCharset);

    property PDFFontName: string read FPDFFontName write FPDFFontName;
    property FontStyles: TFontStyles read FFontStyles write FFontStyles;
    property FontType: string read FFontType write FFontType;
    property ObjectName: string read FObjectName write FObjectName;
    property FontName: string read FFontName write FFontName;
    property FirstChar: integer read FFirstChar write FFirstChar;
    property LastChar: integer read FLastChar write FLastChar;
    property FontCharset: TFontCharset read FFontCharset write SetCharset;

  public

    constructor Create(AOwner: TObject); override;

    function GetTextWidth(AFontSize: double; AText: string): double;
    procedure InitData; override;

  end;


  TRPPDFFontDescriptor = class(TRPPDFObject)
  protected
    FPDFFontName: string;
    FFlags: integer;
    FFontBBox: TRect;
    FMissingWidth: integer;
    FStemV: integer;
    FStemH: integer;
    FItalicAngle: integer;
    FCapHeight: integer;
    FXHeight: integer;
    FAscent: integer;
    FDescent: integer;
    FLeading: integer;
    FMaxWidth: integer;
    FAvgWidth: integer;
    FIsEmbeddingAllowed: boolean;
    FPanose: TPanose;

    function SetBit(AValue: integer; ABit: integer): integer;
    property Panose: TPanose read FPanose write FPanose;


  public
    procedure InitData; override;
    procedure SetValues(OutlineTextMetric: POutlineTextMetric; AMissingWidth: integer); virtual;

    property PDFFontName: string read FPDFFontName write FPDFFontName;
    property Flags: integer read FFlags write FFlags;
    property FontBBox: TRect read FFontBBox write FFontBBox;
    property MissingWidth: integer read FMissingWidth write FMissingWidth;
    property StemV: integer read FStemV write FStemV;
    property StemH: integer read FStemH write FStemH;
    property ItalicAngle: integer read FItalicAngle write FItalicAngle;
    property CapHeight: integer read FCapHeight write FCapHeight;
    property XHeight: integer read FXHeight write FXHeight;
    property Ascent: integer read FAscent write FAscent;
    property Descent: integer read FDescent write FDescent;
    property Leading: integer read FLeading write FLeading;
    property MaxWidth: integer read FMaxWidth write FMaxWidth;
    property AvgWidth: integer read FAvgWidth write FAvgWidth;
    property IsEmbeddingAllowed: boolean read FIsEmbeddingAllowed write FIsEmbeddingAllowed;
  end;

  TRPPDFFontDescriptorComposite = class;

  TRPPDFFontDescendant = class (TRPPDFFont)
  protected
    FPDFFontName: string;
    FDescriptor: TRPPDFFontDescriptorComposite;
    FMaxCharWidth: integer;
    FAveCharWidth: integer;
    FRegistry, FOrdering: string;
    FSupplement: integer;
    FCharset: TFontCharSet;

  public
    procedure InitData; override;

    property Descriptor: TRPPDFFontDescriptorComposite read FDescriptor write FDescriptor;
    property PDFFontName: string read FPDFFontName write FPDFFontName;
    property Registry: string read FRegistry write FRegistry;
    property Ordering: string read FOrdering write FOrdering;
    property Supplement: integer read FSupplement write FSupplement;
    property MaxCharWidth: integer read FMaxCharWidth write FMaxCharWidth;
    property AveCharWidth: integer read FAveCharWidth write FAveCharWidth;
    property Charset: TFontCharset read FCharset write FCharset;
  end;

  TRPPDFFontDescriptorComposite = class(TRPPDFFontDescriptor)
  protected
    FDescendantFont: TRPPDFFontDescendant;
    FDescriptor: TRPPDFFontDescriptorComposite;
    FStyle: string;

  public
    property Descriptor: TRPPDFFontDescriptorComposite read FDescriptor write FDescriptor;
    procedure InitData; override;
    procedure SetValues(OutlineTextMetric: POutlineTextMetric; AMissingWidth: integer); override;

    property Style: string read FStyle write FStyle;
  end;

  TRPPDFFontFile = class(TRPPDFObject)
  protected
    FFontStream: TMemoryStream;
    FLength1: integer;

  public
    constructor Create(AOwner: TObject); override;
    destructor Destroy; override;

    procedure InitData; override;
    procedure LoadFontFile(Handle: THandle);

    property FontStream: TMemoryStream read FFontStream;
  end; { TRPPDFFontFile }


  TRPPDFFontTrueType = class(TRPPDFFont)
  protected
    FDescriptor: TRPPDFFontDescriptor;
    FFontFile: TRPPDFFontFile;
    FIsEmbeddingAllowed: boolean;

    procedure SetFontData;

  public
    constructor Create(AOwner: TObject); override;
    destructor Destroy; override;

    procedure InitData; override;

    property Descriptor: TRPPDFFontDescriptor read FDescriptor write FDescriptor;
    property FontFile: TRPPDFFontFile read FFontFile write FFontFile;
    property IsEmbeddingAllowed: boolean read FIsEmbeddingAllowed write FIsEmbeddingAllowed;
  end;  { TRPPDFFontTrueType }

  TRPPDFFontComposite = class(TRPPDFFont)
  protected
    FDescriptor: TRPPDFFontDescriptorComposite;
    FDescendant: TRPPDFFontDescendant;
    FCompositeFontEncoding: string;
    FFirstChar: integer;
    FLastChar: integer;

    procedure SetFontData;

  public
    constructor Create(AOwner: TObject); override;
    destructor Destroy; override;

    procedure InitData; override;

    property Descriptor: TRPPDFFontDescriptorComposite read FDescriptor write FDescriptor;
    property Descendant: TRPPDFFontDescendant read FDescendant write FDescendant;
    property CompositeFontEncoding: string read FCompositeFontEncoding write FCompositeFontEncoding;
    property FirstChar: integer read FFirstChar write FFirstChar;
    property LastChar: integer read FLastChar write FLastChar;
  end;  { TRPPDFFontComposite }

  TRPPDFXObject = class(TRPPDFObject)
  private
    function GetImageHeight: integer;
    function GetImageWidth: integer;
  protected
    FImageStream: TStream;
    FObjectName: string;
    FImage: TJPEGImage;
    property ImageStream: TStream read FImageStream;

  public
    constructor Create(AOwner: TObject); override;
    destructor Destroy; override;
    procedure InitData; override;

    property ObjectName: string read FObjectName write FObjectName;
    property Width: integer read GetImageWidth;
    property Height: integer read GetImageHeight;
    property Image: TJPEGImage read FImage write FImage;
  end;

  TCompressEvent = procedure(    InStream: TStream;
                                 OutStream: TStream;
                             var CompressMethod: string) of object;

  TImageQualityRange = 1..100;

  TRvRenderPDF = class(TRPRenderStream)
  protected
    FDataStream: TMemoryStream;
    FOutlineList: TStringList;
    FPageList: TStringList;
    FXObjectList: TStringList;
    FFontList: TStringList;
    FPatternList: TStringList;
    FXRefList: TStringList;

    FCatalogObject: TRPPDFCatalog;
    FPagesObject: TRPPDFPages;
    FOutlinesObject: TRPPDFOutlines;
    FCurrentPageObject: TRPPDFPage;
    FCurrentFontObject: TRPPDFFont;
    FProcSetObject: TRPPDFProcSet;

    XRefPos: longint;
    FOpenMode: string;
    FFontEncoding: TPDFFontEncoding;
    FFontSize: integer;
    FFontRotation: double;
    FUnderlineFont: boolean;
    FStrikeoutFont: boolean;
    FFrameMode : TFrameMode;
    FUseCompression: boolean;
    FEmbedFonts: boolean;
    FEmbedBaseFonts: boolean;
    FirstNewPage: boolean;
    SmallArcData: TSmallArcBuffer;
    //
    FOnCompress: TCompressEvent;
    FImageQuality: TImageQualityRange;
    FMetafileDPI: integer;
    //
    FDocInfo: TRPPDFDocInfo;
    FFontBitmap: Graphics.TBitmap;
    FFontChanged: boolean;

    function GetPageHeight: double;
    function GetPageWidth: double;
    procedure SetPageHeight(const AValue: double);
    procedure SetPageWidth(const AValue: double);
    function GetPenColor: TColor;
    procedure SetPenColor(const AValue: TColor);
    function GetBrushColor: TColor;
    procedure SetBrushColor(const AValue: TColor);
    function GetTextColor: TColor;
    function IsCJKFont: boolean;
    function CreatePattern(AStyle: TBrushStyle; AFillColor: TColor): string;
    function CreatePDFFont(AFontName: string; AFontStyles: TFontStyles;
     AFontCharset: TFontCharset): string;
    function GetPDFFontName(AFontName: string; AFontStyles: TFontStyles;
     AFontCharset: TFontCharset): string;
    function IsTrueType(AFontName: string; AFontStyles: TFontStyles;
     AFontCharset: TFontCharset): boolean;
    function GetFont(AFontName: string; AFontStyles: TFontStyles;
     AFontCharset: TFontCharset): TRPPDFFont;
    function GetPatternName(AStyle: TBrushStyle; AFillColor: TColor): string;
    function GetPattern(APatternName: string): TRPPDFPattern;
    function GetDoFill: boolean;
    function GetDoStroke: boolean;
    procedure UpdateFont;
    function GetCurrentFontObject: TRPPDFFont;
    function GetUnderlineFont: boolean;
    function GetStrikeoutFont: boolean;
    function GetFontSize: integer;
    function GetFontRotation: double;

    procedure NewPage;

    function DateTimeToPDF(const Value: TDateTime): string;
    procedure SetTextColor(AColor: TColor);
    procedure SetGraphicColor(AStrokeColor: TColor; ABrushColor: TColor);
    function FormatEx(const AData: string; const AArgs: array of const): string;
    procedure PrintF(const AData: string; const AArgs: array of const); override;
    procedure PrintLnF(const AData: string; const AArgs: array of const); override;
    procedure PrintRightWidth(var X1,Y1: double; Text: string; Width: double); override;

    function StrToHex(const AValue: string): string;

    property PageWidth: double read GetPageWidth write SetPageWidth;
    property PageHeight: double read GetPageHeight write SetPageHeight;
    property FontRotation: double read GetFontRotation;
    property PenColor: TColor read GetPenColor write SetPenColor;
    property BrushColor: TColor read GetBrushColor write SetBrushColor;
    property DataStream: TMemoryStream read FDataStream;
    property TextColor: TColor read GetTextColor write SetTextColor;
    property CurrentFont: TRPPDFFont read GetCurrentFontObject write FCurrentFontObject;
    property UnderlineFont: boolean read GetUnderlineFont;
    property StrikeoutFont: boolean read GetStrikeoutFont;
    property FontSize: integer read GetFontSize;
    property CurrentPage: TRPPDFPage read FCurrentPageObject write FCurrentPageObject;
    property PageList: TStringList read FPageList;
    property XObjectList: TStringList read FXObjectList;
    property FontList: TStringList read FFontList;
    property PatternList: TStringList read FPatternList;
    property CatalogObject: TRPPDFCatalog read FCatalogObject;
    property OutlinesObject: TRPPDFOutlines read FOutlinesObject;
    property PagesObject: TRPPDFPages read FPagesObject;
    property ProcSetObject: TRPPDFProcSet read FProcSetObject;
    property XRefList: TStringList read FXRefList;
    property OutlineList: TStringList read FOutlineList;
    property FontBitmap: TBitmap read FFontBitmap write FFontBitmap;

  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;

    procedure PrintRender(NDRStream: TStream;
                          OutputFileName: TFileName); override;
    procedure CreatePDFObjects;
    procedure FreePDFObjects;

    procedure PrintLeft(const AX, AY: double; AText: string);
    procedure PrintCenter(const AX, AY: double; AText: string);
    procedure PrintRight(const AX, AY: double; AText: string);

    procedure InitData;
    procedure FreeData;

    procedure SetIDs;
    procedure WriteHeader;
    procedure WriteDocInfo;
    procedure WriteCatalog;
    procedure WriteOutlinesObject;
    procedure WritePagesObject;
    procedure WritePages;
    procedure WriteProcSet;
    procedure WriteFonts;
    procedure WritePatterns;
    procedure WriteXObjects;
    procedure WriteXRef;
    procedure WriteTrailer;
    procedure WriteStartXRef;
    procedure WriteEof;

    procedure RegisterObject;
    procedure GenerateBook;
    //
    procedure FontChanged(Sender: TObject); override;
    procedure LeftText(const psText: string; const pfX, pfY: double); override;
    procedure CenterText(const psText: string; const pfX, pfY: double); override;
    procedure RightText(const psText: string; const pfX, pfY: double); override;
    procedure TextRect(Rect: TRect; X1,Y1: double;S1: string); override;
    procedure PrintSpaces(const AText: string; const AX, AY, AWidth: double); override;
    procedure Arc(const AX1, AY1, AX2, AY2, AX3, AY3, AX4, AY4: double); override;
    procedure PDFArc(const AX1, AY1, AX2, AY2, AX3, AY3, AX4, AY4: double; ADoClosePath, AAllowFill: boolean);
    procedure Chord(const AX1, AY1, AX2, AY2, AX3, AY3, AX4, AY4: double); override;
    procedure Rectangle(const pfX1, pfY1, pfX2, pfY2: double); override;
    procedure PDFRectangle(const AX1, AY1, AX2, AY2: double);
    procedure FillRect(const pRect: TRect); override;
    procedure RoundRect(const pfX1, pfY1, pfX2, pfY2, pfX3, pfY3: double); override;
    procedure PDFRoundRect(const AX1, AY1, AX2, AY2, AX3, AY3: double);
    procedure Ellipse(const pfX1, pfY1, pfX2, pfY2: double); override;
    procedure PDFEllipse(const AX1, AY1, AX2, AY2: double);
    procedure MoveTo(const pfX1, pfY1: double); override;
    procedure PDFMoveTo(const AX, AY: double);
    procedure LineTo(const pfX1, pfY1: double); override;
    procedure PDFLineTo(const AX, AY: double; ADoClosePath: boolean);
    procedure PrintBitmapRect(const X1, Y1, X2, Y2: double; AGraphic: Graphics.TBitmap); override;
    procedure PrintBitmap(const X1,Y1,ScaleX,ScaleY: double; AGraphic: Graphics.TBitmap); override;
    procedure StretchDraw(const Rect: TRect; AGraphic: TGraphic); override;
    procedure Draw(const pfX1, pfY1: double; AGraphic: TGraphic); override;
    function AddJPG(const AImage: TJPEGImage; AReuseJPG: boolean): string;
    procedure PrintJPG(const AX1, AY1, AX2, AY2: double; AName: string);
    procedure Pie(const AX1, AY1, AX2, AY2, AX3, AY3, AX4, AY4: double); override;
    procedure PDFPie(const AX1, AY1, AX2, AY2, AX3, AY3, AX4, AY4: double; ADoClosePath, AAllowFill: boolean);
    procedure PolyLine(const PolyLineArr: array of TFloatPoint); override;
    procedure Polygon(const PointArr: array of TFloatPoint); override;

    //
    procedure DocBegin; override;
    procedure DocEnd; override;
    procedure PageBegin; override;
    procedure PageEnd; override;
    //
    property DoStroke: boolean read GetDoStroke;
    property DoFill: boolean read GetDoFill;
    property EmbedBaseFonts: boolean read FEmbedBaseFonts write FEmbedBaseFonts;
  published
    property OnDecodeImage;
    property UseCompression: boolean read fUseCompression write fUseCompression default false;
    property EmbedFonts: boolean read FEmbedFonts write FEmbedFonts;
    property OnCompress: TCompressEvent read FOnCompress write FOnCompress;
    property ImageQuality: TImageQualityRange read FImageQuality write FImageQuality;
    property MetafileDPI: integer read FMetafileDPI write FMetafileDPI;
    property FontEncoding: TPDFFontEncoding read FFontEncoding write FFontEncoding;
    property DocInfo: TRPPDFDocInfo read FDocInfo write FDocInfo;
  end;
  TRpRenderPDF = TRvRenderPDF; // Rave 4.0 compatibility


implementation

{ TRPPDFDocInfo }
constructor TRPPDFDocInfo.Create;
begin
  inherited;
  FCreator := 'Rave (http://www.nevrona.com/rave)';
  FProducer := 'Nevrona Designs';
end;

{ TRvRenderPDF }

constructor TRvRenderPDF.Create(AOwner: TComponent);
begin
  inherited;
  FFontEncoding := feWinAnsiEncoding;
  DisplayName := 'Adobe Acrobat (PDF)';
  FileExtension := '*.pdf';
  FImageQuality := 90; { JPGImage Quality default }
  FMetaFileDPI := 300;
  FDocInfo := TRPPDFDocInfo.Create;
  FFontBitmap := Graphics.TBitmap.Create;
  FFontBitmap.Canvas.Font.PixelsPerInch := 6000;
  FFontBitmap.Canvas.Font.Size := 12;
end;

destructor TRvRenderPDF.Destroy;
begin
  inherited;
  FreeAndNil(FDocInfo);
  FreeAndNil(FFontBitmap);
end;

procedure TRvRenderPDF.PageBegin;
begin
  if FirstNewPage then begin
    FirstNewPage := false;
  end else begin
    NewPage;
  end;
  PageWidth := PaperWidth;
  PageHeight := PaperHeight;
end;

procedure TRvRenderPDF.PageEnd;
begin
end;

procedure TRvRenderPDF.LeftText(const psText: string; const pfX, pfY: double);
begin
  PrintLeft(pfX, pfY, psText);
end;

procedure TRvRenderPDF.CenterText(const psText: string; const pfX, pfY: double);
begin
  PrintCenter(pfX, pfY, psText);
end;

procedure TRvRenderPDF.RightText(const psText: string; const pfX, pfY: double);
begin
  PrintRight(pfX, pfY, psText);
end;

procedure TRvRenderPDF.PrintSpaces(const AText: string; const AX, AY, AWidth: double);
var
  CurrWord: string;
  Spaces: integer;
  SpaceWidth: double;
  SpacesStr: string;
  I1: integer;
  LText: string;
  XPos, YPos: double;

begin
{ Count the number of spaces in Text }
  LText := AText;
  Spaces := CountSpaces(LText);
  if Spaces = 0 then begin { Process each letter as a word }
  { Insert spaces in between each letter pair }
    I1 := 2;
    while I1 <= Length(LText) do begin
      if (LText[I1 - 1] <> ' ') and (LText[I1] <> ' ') then begin { Insert Space }
        Insert(' ',LText,I1);
        Inc(Spaces);
        Inc(I1);
      end; { if }
      Inc(I1);
    end; { while }
  end; { if }
  if Spaces = 0 then begin
    Spaces := 1;
  end; { if }

{ Figure out the width for each internal space }
  SpaceWidth := CurrentFont.GetTextWidth(FontSize, ' ');
  SpaceWidth := SpaceWidth + ((AWidth - CurrentFont.GetTextWidth(FontSize, LText)) / Spaces);

{ Print first word left aligned, middle and last words right aligned }
  CurrWord := GetFirstWord(LText);
  XPos := AX;
  YPos := AY;
  PrintLeft(XPos,YPos, CurrWord);
  AdjustXY(XPos,YPos,CurrentFont.GetTextWidth(FontSize, CurrWord));
  while LText <> '' do begin
    SpacesStr := GetSpaces(LText);
    Spaces := Length(SpacesStr);
    CurrWord := GetFirstWord(LText);
    // Need to print SpacesStr so underlining works
    PrintRightWidth(XPos,YPos,SpacesStr + CurrWord,
     CurrentFont.GetTextWidth(FontSize, CurrWord) + (Spaces * SpaceWidth));
  end; { while }
end;

procedure TRvRenderPDF.TextRect(Rect: TRect; X1,Y1: double; S1: string);
begin
  CurrentPage.TextRect(Rect, X1, Y1, S1);
end;


procedure TRvRenderPDF.Rectangle(const pfX1, pfY1, pfX2, pfY2: double);

begin
  PenColor := Converter.Pen.Color;
  BrushColor := Converter.Brush.Color;
  PDFRectangle(pfX1, pfY1, pfX2, pfY2);
end;

procedure TRvRenderPDF.MoveTo(const pfX1, pfY1: double);
begin
  PDFMoveTo(pfX1, pfY1);
end;

procedure TRvRenderPDF.LineTo(const pfX1, pfY1: double);
var
  BrushStyle: TBrushStyle;
  BrushColor: TColor;
begin
  PenColor := Converter.Pen.Color;
  BrushColor := Converter.Brush.Color;
  BrushStyle := Converter.Brush.Style;
  Converter.Brush.Style := bsClear;
  PDFLineTo(pfX1, pfY1, true);
  Converter.Brush.Color := BrushColor;
  Converter.Brush.Style := BrushStyle;
end;

procedure TRvRenderPDF.DocEnd;
begin { DocEnd }
  GenerateBook;
  DataStream.Position := 0;
  if FOutputStream = nil then begin
    FOutputStream := TFileStream.Create(OutputFileName,fmCreate or fmShareDenyWrite);
    FOutputStream.CopyFrom(DataStream, DataStream.Size);
    FreeAndNil(FOutputStream);
  end else begin
    FOutputStream.CopyFrom(DataStream, DataStream.Size);
  end; { else }
  FreePDFObjects;
  FreeFileStream;
end;  { DocEnd }

procedure TRvRenderPDF.DocBegin;
begin
  FFontChanged := true;
  CreatePDFObjects;
end; { DocBegin }

function TRvRenderPDF.DateTimeToPDF(const Value: TDateTime): string;
var
  sOrigShortDateFormat: string;
  sOrigLongTimeFormat: string;
  s1: string;
begin
  sOrigShortDateFormat := ShortDateFormat;
  sOrigLongTimeFormat := LongTimeFormat;

  ShortDateFormat := 'yyyy/mm/dd';
  LongTimeFormat := 'hh:mm:ss';

  s1 := DateTimeToStr(Value);

  Result := Copy(s1,1,4) + Copy(s1,6,2) + Copy(s1,9,2);
  Result := Result + Copy(s1,12,2) + Copy(s1,15,2) + Copy(s1, 18,2);

  LongTimeFormat := sOrigLongTimeFormat;
  ShortDateFormat := sOrigShortDateFormat;
end;

procedure TRvRenderPDF.SetTextColor(AColor: TColor);
begin
  CurrentPage.TextColor := AColor;
end;

procedure TRvRenderPDF.SetGraphicColor(AStrokeColor: TColor;
                                      ABrushColor: TColor );
begin
  PenColor := AStrokeColor;
  BrushColor := ABrushColor;
end;

procedure TRvRenderPDF.Ellipse(const pfX1, pfY1, pfX2, pfY2: double);
begin
  PenColor := Converter.Pen.Color;
  BrushColor := Converter.Brush.Color;
  PDFEllipse(pfX1, pfY1, pfX2, pfY2);
end;

procedure TRvRenderPDF.PrintBitmapRect(const X1, Y1, X2, Y2: double;
  AGraphic: Graphics.TBitmap);
var
  JPGImage: TJPEGImage;
  ImageName: string;
begin
  JPGImage := TJPEGImage.Create;
  try
    JPGImage.CompressionQuality := 100;
    JPGImage.Assign(AGraphic);
    ImageName := AddJPG(JPGImage,Converter.ReuseGraphic);
  finally
    JPGImage.Free;
  end; { tryf }
  PrintJPG(X1, Y1, X2, Y2, ImageName);
end;

procedure TRvRenderPDF.PrintBitmap(const X1,Y1,ScaleX,ScaleY: double;
  AGraphic: Graphics.TBitmap);
var
  JPGImage: TJPEGImage;
  ImageName: string;
  X2, Y2: double;
begin
  JPGImage := TJPEGImage.Create;
  try
    JPGImage.CompressionQuality := 100;
    JPGImage.Assign(AGraphic);
    ImageName := AddJPG(JPGImage,Converter.ReuseGraphic);
  finally
    JPGImage.Free;
  end; { tryf }
  X2 := X1 + ((AGraphic.Width / 72) * ScaleX);
  Y2 := Y1 + ((AGraphic.Height / 72) * ScaleY);
  PrintJPG(X1, Y1, X2, Y2, ImageName);
end;

procedure TRvRenderPDF.UpdateFont;
begin
  if FFontChanged then begin
    FFontChanged := false;
    CurrentFont := GetFont(GetPDFFontName(Converter.Font.Name,Converter.Font.Style,Converter.Font.Charset),
     Converter.Font.Style,Converter.Font.Charset);
    if fsUnderline in Converter.Font.Style then begin
      FUnderlineFont := true;
    end else begin
      FUnderlineFont := false;
    end; { else }
    if fsStrikeout in Converter.Font.Style then begin
      FStrikeoutFont := true;
    end else begin
      FStrikeoutFont := false;
    end; { else }
    FFontSize := Converter.Font.Size;
    TextColor := Converter.Font.Color;
    FFontRotation := Converter.FontData.Rotation;
  end; { if }
end;

function TRvRenderPDF.GetFontRotation: double;
begin
  UpdateFont;
  Result := FFontRotation;
end;

function TRvRenderPDF.GetFontSize: integer;
begin
  UpdateFont;
  Result := FFontSize;
end;

function TRvRenderPDF.GetUnderlineFont: boolean;
begin
  UpdateFont;
  Result := FUnderlineFont;
end;

function TRvRenderPDF.GetStrikeoutFont: boolean;
begin
  UpdateFont;
  Result := FStrikeoutFont;
end;

function TRvRenderPDF.GetCurrentFontObject: TRPPDFFont;
begin
  UpdateFont;
  Result := FCurrentFontObject;
end;

procedure TRvRenderPDF.FontChanged(Sender: TObject);
begin
  FFontChanged := true;
end;

procedure TRvRenderPDF.NewPage;
var
  TextColor: TColor;
  PenColor: TColor;
  BrushColor: TColor;
begin
  TextColor := CurrentPage.TextColor;
  PenColor := CurrentPage.PenColor;
  BrushColor := CurrentPage.BrushColor;
  CurrentPage := TRPPDFPage.Create(self);
  CurrentPage.TextColor := TextColor;
  CurrentPage.PenColor := PenColor;
  CurrentPage.BrushColor := BrushColor;
  PageList.AddObject(FormatEx('P%d',[PageList.Count + 1]), CurrentPage);
end;

function TRvRenderPDF.GetPageHeight: double;
begin
  Result := CurrentPage.PageHeight;
end;

function TRvRenderPDF.GetPageWidth: double;
begin
  Result := CurrentPage.PageWidth;
end;

procedure TRvRenderPDF.SetPageHeight(const AValue: double);
begin
  CurrentPage.PageHeight := AValue;
end;

procedure TRvRenderPDF.SetPageWidth(const AValue: double);
begin
  CurrentPage.PageWidth := AValue;
end;

procedure TRvRenderPDF.PrintLeft(const AX, AY: double; AText: string);
begin
  CurrentPage.PrintLeft(AX, AY, AText);
end;

procedure TRvRenderPDF.PrintCenter(const AX, AY: double; AText: string);
begin
  CurrentPage.PrintCenter(AX, AY, AText);
end;

procedure TRvRenderPDF.PrintRight(const AX, AY: double; AText: string);
begin
  CurrentPage.PrintRight(AX, AY, AText);
end;

function TRvRenderPDF.GetPenColor: TColor;
begin
  Result := CurrentPage.PenColor;
end;

procedure TRvRenderPDF.SetPenColor(const AValue: TColor);
begin
  CurrentPage.PenColor := AValue;
end;

function TRvRenderPDF.GetBrushColor: TColor;
begin
  Result := CurrentPage.BrushColor;
end;

procedure TRvRenderPDF.SetBrushColor(const AValue: TColor);
begin
  CurrentPage.BrushColor := AValue;
end;

procedure TRvRenderPDF.PDFRectangle(const AX1, AY1, AX2, AY2: double);
begin
  CurrentPage.Rectangle(AX1, AY1, AX2, AY2, false);
end;

procedure TRvRenderPDF.PDFMoveTo(const AX, AY: double);
begin
  CurrentPage.MoveTo(AX, AY);
end;

procedure TRvRenderPDF.PDFLineTo(const AX, AY: double; ADoClosePath: boolean);
begin
  CurrentPage.LineTo(AX, AY, ADoClosePath);
end;

procedure TRvRenderPDF.GenerateBook;
begin
  SetIDs;
  WriteHeader;
  WriteCatalog;
  WriteOutlinesObject;
  WritePagesObject;
  WritePages;
  WriteProcSet;
  WriteFonts;
  WritePatterns;
  WriteXObjects;
  WriteDocInfo;
  WriteXRef;
  WriteTrailer;
  WriteStartXRef;
  WriteEof;
end;

function TRvRenderPDF.GetTextColor: TColor;
begin
  UpdateFont;
  Result := CurrentPage.TextColor;
end;

procedure TRvRenderPDF.PDFEllipse(const AX1, AY1, AX2, AY2: double);
begin
  CurrentPage.Ellipse(AX1, AY1, AX2, AY2);
end;

function TRvRenderPDF.AddJPG(const AImage: TJPEGImage;
  AReuseJPG: boolean): string;
var
  XObject: TRPPDFXObject;
begin
  if AReuseJPG then begin
  end else begin
    XObject := TRPPDFXObject.Create(self);
    XObject.Image.CompressionQuality := 100;
    XObject.Image.Assign(AImage);
    Result := FormatEx('G%d',[XObjectList.Count + 1]);
    XObject.ObjectName := Result;
    XObjectList.AddObject(Result, XObject);
  end; { else }
end;

procedure TRvRenderPDF.PrintJPG(const AX1, AY1, AX2, AY2: double;
  AName: string);
begin
  CurrentPage.PrintJPG(AX1, AY1, AX2, AY2, AName);
end;

function TRvRenderPDF.IsCJKFont: boolean;
begin
  case ConvertCharset((Converter.Font.Charset)) of
    Windows.SHIFTJIS_CHARSET,
    Windows.HANGEUL_CHARSET,
    Windows.JOHAB_CHARSET: begin
      Result := true;
    end else begin
      Result := false;
    end
  end; { case }
end;

function TRvRenderPDF.CreatePDFFont(AFontName: string; AFontStyles: TFontStyles;
  AFontCharset: TFontCharset): string;

begin { CreatePDFFont }
  Result := '';

  if IsCJKFont then begin
    CurrentFont := TRPPDFFontComposite.Create(self);
    CurrentFont.FontName := Converter.Font.Name;
    CurrentFont.PDFFontName := GetPDFFontName(Converter.Font.Name, AFontStyles, AFontCharset);
    CurrentFont.FontStyles := AFontStyles;
    CurrentFont.FontCharset := AFontCharset;
    TRPPDFFontComposite(CurrentFont).SetFontData;
    FontList.AddObject(CurrentFont.PDFFontName,CurrentFont);
  end else if CompareText(AFontName,'Courier-BoldOblique') = 0 then begin
    CurrentFont := TRPPDFFont.Create(self);
    CurrentFont.FontName := Converter.Font.Name;
    CurrentFont.PDFFontName := GetPDFFontName(Converter.Font.Name, AFontStyles, AFontCharset);
    CurrentFont.FontStyles := AFontStyles;
    CurrentFont.FontCharset := AFontCharset;
    CurrentFont.FontWidths := FontWidthsCourier;
    FontList.AddObject(CurrentFont.PDFFontName,CurrentFont);
  end else if CompareText(AFontName,'Courier-Bold') = 0 then begin
    CurrentFont := TRPPDFFont.Create(self);
    CurrentFont.FontName := Converter.Font.Name;
    CurrentFont.PDFFontName := GetPDFFontName(Converter.Font.Name, AFontStyles, AFontCharset);
    CurrentFont.FontStyles := AFontStyles;
    CurrentFont.FontCharset := AFontCharset;
    CurrentFont.FontWidths := FontWidthsCourier;
    FontList.AddObject(CurrentFont.PDFFontName,CurrentFont);
  end else if CompareText(AFontName,'Courier-Oblique') = 0 then begin
    CurrentFont := TRPPDFFont.Create(self);
    CurrentFont.FontName := Converter.Font.Name;
    CurrentFont.PDFFontName := GetPDFFontName(Converter.Font.Name, AFontStyles, AFontCharset);
    CurrentFont.FontStyles := AFontStyles;
    CurrentFont.FontCharset := AFontCharset;
    CurrentFont.FontWidths := FontWidthsCourier;
    FontList.AddObject(CurrentFont.PDFFontName,CurrentFont);
  end else if (CompareText(AFontName,'Courier') = 0) then begin
    CurrentFont := TRPPDFFont.Create(self);
    CurrentFont.FontName := Converter.Font.Name;
    CurrentFont.PDFFontName := GetPDFFontName(Converter.Font.Name, AFontStyles, AFontCharset);
    CurrentFont.FontStyles := AFontStyles;
    CurrentFont.FontCharset := AFontCharset;
    CurrentFont.FontWidths := FontWidthsCourier;
    FontList.AddObject(CurrentFont.PDFFontName,CurrentFont);
  end else if CompareText(AFontName,'Times-BoldItalic') = 0 then begin
    CurrentFont := TRPPDFFont.Create(self);
    CurrentFont.FontName := Converter.Font.Name;
    CurrentFont.PDFFontName := GetPDFFontName(Converter.Font.Name, AFontStyles, AFontCharset);
    CurrentFont.FontStyles := AFontStyles;
    CurrentFont.FontCharset := AFontCharset;
    CurrentFont.FontWidths := FontWidthsTimesBoldItalic;
    FontList.AddObject(CurrentFont.PDFFontName,CurrentFont);
  end else if CompareText(AFontName,'Times-Bold') = 0  then begin
    CurrentFont := TRPPDFFont.Create(self);
    CurrentFont.FontName := Converter.Font.Name;
    CurrentFont.PDFFontName := GetPDFFontName(Converter.Font.Name, AFontStyles, AFontCharset);
    CurrentFont.FontStyles := AFontStyles;
    CurrentFont.FontCharset := AFontCharset;
    CurrentFont.FontWidths := FontWidthsTimesBold;
    FontList.AddObject(CurrentFont.PDFFontName,CurrentFont);
  end else if CompareText(AFontName,'Times-Italic') = 0 then begin
    CurrentFont := TRPPDFFont.Create(self);
    CurrentFont.FontName := Converter.Font.Name;
    CurrentFont.PDFFontName := GetPDFFontName(Converter.Font.Name, AFontStyles, AFontCharset);
    CurrentFont.FontStyles := AFontStyles;
    CurrentFont.FontCharset := AFontCharset;
    CurrentFont.FontWidths := FontWidthsTimesItalic;
    FontList.AddObject(CurrentFont.PDFFontName,CurrentFont);
  end else if CompareText(AFontName,'Times-Roman') = 0 then begin
    CurrentFont := TRPPDFFont.Create(self);
    CurrentFont.FontName := Converter.Font.Name;
    CurrentFont.PDFFontName := GetPDFFontName(Converter.Font.Name, AFontStyles, AFontCharset);
    CurrentFont.FontStyles := AFontStyles;
    CurrentFont.FontCharset := AFontCharset;
    CurrentFont.FontWidths := FontWidthsTimesRoman;
    FontList.AddObject(CurrentFont.PDFFontName,CurrentFont);
  end else if CompareText(AFontName,'Helvetica-BoldOblique') = 0 then begin
    CurrentFont := TRPPDFFont.Create(self);
    CurrentFont.FontName := Converter.Font.Name;
    CurrentFont.PDFFontName := GetPDFFontName(Converter.Font.Name, AFontStyles, AFontCharset);
    CurrentFont.FontStyles := AFontStyles;
    CurrentFont.FontCharset := AFontCharset;
    CurrentFont.FontWidths := FontWidthsHelveticaBoldOblique;
    FontList.AddObject(CurrentFont.PDFFontName,CurrentFont);
  end else if CompareText(AFontName,'Helvetica-Bold') = 0 then begin
    CurrentFont := TRPPDFFont.Create(self);
    CurrentFont.FontName := Converter.Font.Name;
    CurrentFont.PDFFontName := GetPDFFontName(Converter.Font.Name, AFontStyles, AFontCharset);
    CurrentFont.FontStyles := AFontStyles;
    CurrentFont.FontCharset := AFontCharset;
    CurrentFont.FontWidths := FontWidthsHelveticaBold;
    FontList.AddObject(CurrentFont.PDFFontName,CurrentFont);
  end else if CompareText(AFontName,'Helvetica-Oblique') = 0 then begin
    CurrentFont := TRPPDFFont.Create(self);
    CurrentFont.FontName := Converter.Font.Name;
    CurrentFont.PDFFontName := GetPDFFontName(Converter.Font.Name, AFontStyles, AFontCharset);
    CurrentFont.FontStyles := AFontStyles;
    CurrentFont.FontCharset := AFontCharset;
    CurrentFont.FontWidths := FontWidthsHelveticaOblique;
    FontList.AddObject(CurrentFont.PDFFontName,CurrentFont);
  end else if CompareText(AFontName,'Helvetica') = 0 then begin
    CurrentFont := TRPPDFFont.Create(self);
    CurrentFont.FontName := Converter.Font.Name;
    CurrentFont.PDFFontName := GetPDFFontName(Converter.Font.Name, AFontStyles, AFontCharset);
    CurrentFont.FontStyles := AFontStyles;
    CurrentFont.FontCharset := AFontCharset;
    CurrentFont.FontWidths := FontWidthsHelvetica;
    FontList.AddObject(CurrentFont.PDFFontName,CurrentFont);
  end else if (CompareText(AFontName,'Symbol') = 0) and
              not IsTrueType(AFontName, AFontStyles, AFontCharset) then begin
    CurrentFont := TRPPDFFont.Create(self);
    CurrentFont.FontName := Converter.Font.Name;
    CurrentFont.PDFFontName := GetPDFFontName(Converter.Font.Name, AFontStyles, AFontCharset);
    CurrentFont.FontStyles := AFontStyles;
    CurrentFont.FontCharset := AFontCharset;
    CurrentFont.FontWidths := FontWidthsSymbol;
    FontList.AddObject(CurrentFont.PDFFontName,CurrentFont);
  end else if CompareText(AFontName,'ZapfDingbats') = 0 then begin
    CurrentFont := TRPPDFFont.Create(self);
    CurrentFont.FontName := Converter.Font.Name;
    CurrentFont.PDFFontName := GetPDFFontName(Converter.Font.Name, AFontStyles, AFontCharset);
    CurrentFont.FontStyles := AFontStyles;
    CurrentFont.FontCharset := AFontCharset;
    CurrentFont.FontWidths := FontWidthsZapfDingbats;
    FontList.AddObject(CurrentFont.PDFFontName,CurrentFont);
  end else if IsTrueType(AFontName, AFontStyles, AFontCharset) then begin
    CurrentFont := TRPPDFFontTrueType.Create(self);
    CurrentFont.FontName := Converter.Font.Name;
    CurrentFont.PDFFontName := GetPDFFontName(Converter.Font.Name, AFontStyles, AFontCharset);
    CurrentFont.FontStyles := AFontStyles;
    CurrentFont.FontCharset := AFontCharset;
    TRPPDFFontTrueType(CurrentFont).SetFontData;
    FontList.AddObject(CurrentFont.PDFFontName,CurrentFont);
  end else begin
    if FontList.IndexOf(GetPDFFontName('Helvetica',AFontStyles, AFontCharset)) < 0 then begin
      Result := CreatePDFFont(GetPDFFontName('Helvetica',AFontStyles, AFontCharset), AFontStyles, AFontCharset);
    end; { if }
  end; { else }
  Result := CurrentFont.PDFFontName;
end;  { CreatePDFFont }

function TRvRenderPDF.GetPDFFontName(AFontName: string;
AFontStyles: TFontStyles; AFontCharset: TFontCharset): string;

  function AddFontStyle(AName: string): string;
  begin
    Result := AName;
    if (fsBold in AFontStyles) and (fsItalic in AFontStyles) then begin
      Result := Result + ',BoldItalic';
    end else if fsBold in AFontStyles then begin
      Result := Result + ',Bold';
    end else if fsItalic in AFontStyles then begin
      Result := Result + ',Italic';
    end;
  end;

  function TrueTypeName: string;
  var i1: integer;
  begin
    Result := AFontName;
    for i1 := Length(Result) downto 1 do begin
      if Result[i1] = ' ' then begin
        Delete(Result, i1, 1);
      end; { if }
    end; { for }
    Result := AddFontStyle(Result);
  end;

  function MBCSFontName: string;
  var
    I1: integer;
    S1: string;
  begin
    Result := '';
    S1 := TrueTypeName;
    for I1 := 1 to Length(S1) do begin
      Result := Result + '#' + StrToHex(S1[I1]);
    end; { for }
  end;

begin { GetPDFFontName }
  if IsCJKFont then begin
    Result := MBCSFontName;
  end else if EmbedBaseFonts and IsTrueType(AFontName, AFontStyles, AFontCharset) then begin
    Result := TrueTypeName;
  end else begin
    if (CompareText(AFontName, 'Courier') = 0) or
       (CompareText(AFontName, 'Courier New') = 0) then begin
      Result := 'Courier';
    end else if (CompareText(AFontName, 'Times New Roman') = 0) or
                (CompareText(AFontName, 'TimesRoman') = 0) then begin
      Result := 'Times-Roman';
      if (fsBold in AFontStyles) and (fsItalic in AFontStyles) then begin
        Result := 'Times-BoldItalic';
      end else if fsBold in AFontStyles then begin
        Result := 'Times-Bold';
      end else if fsItalic in AFontStyles then begin
        Result := 'Times-Italic';
      end else begin
        Result := 'Times-Roman';
      end; { else }
      Exit;
    end else if CompareText(AFontName, 'Symbol') = 0 then begin
      Result := 'Symbol';
      Exit;
    end else if CompareText(AFontName, 'Arial') = 0 then begin
      Result := 'Helvetica';
    end else if CompareText(AFontName, 'ZapfDingbats') = 0 then begin
      Result := 'ZapfDingbats';
      Exit;
    end else if IsTrueType(AFontName, AFontStyles, AFontCharset) then begin
      Result := TrueTypeName;
      Exit;
    end else begin
      Result := 'Helvetica';
    end; { else }
    if (fsBold in AFontStyles) and (fsItalic in AFontStyles) then begin
      Result := Result + '-BoldOblique';
    end else if fsBold in AFontStyles then begin
      Result := Result + '-Bold';
    end else if fsItalic in AFontStyles then begin
      Result := Result + '-Oblique';
    end; { else }
  end; { else }
end;  { GetPDFFontName }

function TRvRenderPDF.IsTrueType(AFontName: string; AFontStyles: TFontStyles;
 AFontCharset: TFontCharset): boolean;
var
  TextMetric: TTextMetric;
begin
  FontBitmap.Canvas.Font.Name := AFontName;
  FontBitmap.Canvas.Font.Style := AFontStyles;
  FontBitmap.Canvas.Font.Charset := AFontCharset;
  GetTextMetrics((FontBitmap.Canvas.Handle),TextMetric);
  Result := (TextMetric.tmPitchAndFamily and TMPF_TRUETYPE) <> 0;
end;

function TRvRenderPDF.CreatePattern(AStyle: TBrushStyle; AFillColor: TColor): string;
var
  Pattern: TRPPDFPattern;
begin
  Result := GetPatternName(AStyle, AFillColor);
  Pattern := TRPPDFPattern.Create(Self);
  Pattern.SetPattern(AStyle, AFillColor);
  PatternList.AddObject(Result, Pattern)
end;

function TRvRenderPDF.GetPatternName(AStyle: TBrushStyle; AFillColor: TColor): string;
begin
  case AStyle of
    bsClear: Result := 'bsClear';
    bsSolid: Result := 'bsSolid';
    bsBDiagonal: Result := 'bsBDiagonal';
    bsFDiagonal: Result := 'bsFDiagonal';
    bsDiagCross: Result := 'bsDiagCross';
    bsCross: Result := 'bsCross';
    bsHorizontal: Result := 'bsHorizontal';
    bsVertical: Result := 'bsVertical';
    else begin
      Result := 'bsClear';
    end;
  end;
  if AFillColor >= 0 then begin
    Result := Result + IntToHex(AFillColor, 8);
  end else begin
    Result := Result + IntToHex(clBlack, 8);
  end; { else }
end;

function TRvRenderPDF.GetPattern(APatternName: string): TRPPDFPattern;
var
  PatternName: string;

begin
  PatternName := GetPatternName(Converter.Brush.Style, Converter.Brush.Color);
  if PatternList.IndexOf(PatternName) < 0 then begin
    CreatePattern(Converter.Brush.Style, Converter.Brush.Color);
  end; { if }
  Result := TRPPDFPattern(PatternList.Objects[PatternList.IndexOf(PatternName)]);
end;  { GetPattern}

function TRvRenderPDF.GetFont(AFontName: string;
  AFontStyles: TFontStyles; AFontCharset: TFontCharset): TRPPDFFont;
var
  PDFFontName: string;
begin
  PDFFontName := GetPDFFontName(Converter.Font.Name, AFontStyles, AFontCharset);
  if FontList.IndexOf(PDFFontName) < 0 then begin
    CreatePDFFont(PDFFontName, AFontStyles, AFontCharset);
  end; { if }
  Result := TRPPDFFont(FontList.Objects[FontList.IndexOf(PDFFontName)]);
end;

procedure TRvRenderPDF.SetIDs;
var
  CurrentID: integer;
  i1: integer;
  PDFPage: TRPPDFPage;
  PDFFont: TRPPDFFont;
  PDFPattern: TRPPDFPattern;
  PDFXObject: TRPPDFXObject;
begin { SetIDs }
  CurrentID := 1;
  CatalogObject.SetID(CurrentID);
  Inc(CurrentID);
  OutlinesObject.SetID(CurrentID);
  Inc(CurrentID);
  PagesObject.SetID(CurrentID);
  for i1 := 0 to PageList.Count - 1 do begin
   PDFPage := TRPPDFPage(PageList.Objects[i1]);
   Inc(CurrentID);
   PDFPage.SetID(CurrentID);
   Inc(CurrentID);
   PDFPage.PageContent.SetID(CurrentID);
  end; { for }
  Inc(CurrentID);
  ProcSetObject.SetID(CurrentID);
  for i1 := 0 to FontList.Count - 1 do begin
   PDFFont := TRPPDFFont(FontList.Objects[i1]);
   Inc(CurrentID);
   PDFFont.SetID(CurrentID);
    if PDFFont is TRPPDFFontTrueType then begin
      Inc(CurrentID);
      TRPPDFFontTrueType(PDFFont).Descriptor.SetID(CurrentID);
      if EmbedFonts and TRPPDFFontTrueType(PDFFont).IsEmbeddingAllowed then begin
        Inc(CurrentID);
        TRPPDFFontTrueType(PDFFont).FontFile.SetID(CurrentID);
      end; { if }
    end else if PDFFont is TRPPDFFontComposite then begin
      Inc(CurrentID);
      TRPPDFFontComposite(PDFFont).Descendant.SetID(CurrentID);
      Inc(CurrentID);
      TRPPDFFontComposite(PDFFont).Descriptor.SetID(CurrentID);
    end; { else }
  end; { for }
  for i1 := 0 to PatternList.Count - 1 do begin
    PDFPattern := TRPPDFPattern(PatternList.Objects[i1]);
    Inc(CurrentID);
    PDFPattern.SetID(CurrentID);
    Inc(CurrentID);
    PDFPattern.PatternResource.SetID(CurrentID);
  end; { for }
  for i1 := 0 to XObjectList.Count - 1 do begin
   PDFXObject := TRPPDFXObject(XObjectList.Objects[i1]);
   Inc(CurrentID);
   PDFXObject.SetID(CurrentID);
  end; { for }

end;  { SetIDs }

procedure TRvRenderPDF.WriteHeader;
begin
  ActiveStream := DataStream;
  PrintLn(PDFVersion);
  PrintLn('%âãÏÓ');
  PrintLn;
end;

procedure TRvRenderPDF.WriteDocInfo;
begin
  ActiveStream := DataStream;
  RegisterObject;
  PrintLnF('%d 0 obj',[FXRefList.Count]);
  PrintLn('<<');
  with CurrentPage do begin
    if DocInfo.Title <> '' then begin
      PrintLnF('/Title (%s)',[FormatRPPDFText(DocInfo.Title)]);
    end; { if }
    if DocInfo.Author <> '' then begin
      PrintLnF('/Author (%s)',[FormatRPPDFText(DocInfo.Author)]);
    end; { if }
    if DocInfo.Subject <> '' then begin
      PrintLnF('/Subject (%s)',[FormatRPPDFText(DocInfo.Subject)]);
    end; { if }
    if DocInfo.Keywords <> '' then begin
      PrintLnF('/Keywords (%s)',[FormatRPPDFText(DocInfo.Keywords)]);
    end; { if }
    if DocInfo.Creator <> '' then begin
      PrintLnF('/Creator (%s)',[FormatRPPDFText(DocInfo.Creator)]);
    end; { if }
    if DocInfo.Producer <> '' then begin
      PrintLnF('/Producer (%s)',[FormatRPPDFText(DocInfo.Producer)]);
    end; { if }
    PrintLnF('/CreationDate (D:%s)', [DateTimeToPDF(Now)]);
  end; { with }
  PrintLn('>>');
  PrintLn('endobj');
  PrintLn;
end;

procedure TRvRenderPDF.WriteCatalog;
begin
  RegisterObject;
  CatalogObject.InitData;
  DataStream.CopyFrom(CatalogObject.DataStream, CatalogObject.DataStream.Size);
end;

procedure TRvRenderPDF.WriteOutlinesObject;
begin
  RegisterObject;
  OutlinesObject.InitData;
  DataStream.CopyFrom(OutlinesObject.DataStream, OutlinesObject.DataStream.Size);
end;

procedure TRvRenderPDF.WritePagesObject;
begin
  RegisterObject;
  PagesObject.InitData;
  DataStream.CopyFrom(PagesObject.DataStream, PagesObject.DataStream.Size);
end;

procedure TRvRenderPDF.WritePages;
var
  i1: integer;
  PageObject: TRPPDFPage;
  ContentObject: TRPPDFPageContent;
begin
  for i1 := 0 to PageList.Count - 1 do begin
    RegisterObject;
    PageObject := TRPPDFPage(PageList.Objects[i1]);
    PageObject.InitData;
    DataStream.CopyFrom(PageObject.DataStream, PageObject.DataStream.Size);
    RegisterObject;
    ContentObject := PageObject.PageContent;
    ContentObject.InitData;
    DataStream.CopyFrom(ContentObject.DataStream, ContentObject.DataStream.Size);
  end; { for }
end;

procedure TRvRenderPDF.WriteProcSet;
begin
  RegisterObject;
  ProcSetObject.InitData;
  DataStream.CopyFrom(ProcSetObject.DataStream, ProcSetObject.DataStream.Size);
end;

procedure TRvRenderPDF.WriteFonts;
var
  i1: integer;
  FontObject: TRPPDFFont;
  TrueTypeObject: TRPPDFFontTrueType;
  CompositeObject: TRPPDFFontComposite;
begin { WriteFonts }
  for i1 := 0 to FontList.Count - 1 do begin
    RegisterObject;
    FontObject := TRPPDFFont(FontList.Objects[i1]);
    FontObject.InitData;
    DataStream.CopyFrom(FontObject.DataStream, FontObject.DataStream.Size);
    if FontObject is TRPPDFFontTrueType then begin
      RegisterObject;
      TrueTypeObject := TRPPDFFontTrueType(FontObject);
      TrueTypeObject.Descriptor.InitData;
      DataStream.CopyFrom(TrueTypeObject.Descriptor.DataStream, TrueTypeObject.Descriptor.DataStream.Size);
      if EmbedFonts and TrueTypeObject.IsEmbeddingAllowed then begin
        RegisterObject;
        TrueTypeObject.FontFile.InitData;
        DataStream.CopyFrom(TrueTypeObject.FontFile.DataStream, TrueTypeObject.FontFile.DataStream.Size);
      end; { if }
    end; { if }
    if FontObject is TRPPDFFontComposite then begin
      RegisterObject;
      CompositeObject := TRPPDFFontComposite(FontObject);
      CompositeObject.Descendant.InitData;
      DataStream.CopyFrom(CompositeObject.Descendant.DataStream, CompositeObject.Descendant.DataStream.Size);

      RegisterObject;
      CompositeObject.Descriptor.InitData;
      DataStream.CopyFrom(CompositeObject.Descriptor.DataStream, CompositeObject.Descriptor.DataStream.Size);
    end; { if }
  end; { for }
end;  { WriteFonts }

procedure TRvRenderPDF.WritePatterns;
var
  i1: integer;
  PatternObject: TRPPDFPattern;
begin { WritePatterns }
  for i1 := 0 to PatternList.Count - 1 do begin
    RegisterObject;
    PatternObject := TRPPDFPattern(PatternList.Objects[i1]);
    PatternObject.InitData;
    DataStream.CopyFrom(PatternObject.DataStream, PatternObject.DataStream.Size);
    RegisterObject;
    PatternObject.PatternResource.InitData;
    DataStream.CopyFrom(PatternObject.PatternResource.DataStream, PatternObject.PatternResource.DataStream.Size);
  end; { for }
end;  { WritePatterns }

procedure TRvRenderPDF.WriteXObjects;
var
  i1: integer;
  XObject: TRPPDFXObject;
begin
  for i1 := 0 to XObjectList.Count - 1 do begin
    RegisterObject;
    XObject := TRPPDFXObject(XObjectList.Objects[i1]);
    XObject.InitData;
    DataStream.CopyFrom(XObject.DataStream, XObject.DataStream.Size);
  end; { for }
end;

procedure TRvRenderPDF.WriteXRef;
var
  i1: integer;
  XRefStr: string;

  function FormatXRef(AValue: longint): string;

  begin
    Result := FormatEx('%10.0d',[AValue]);

    while Pos(' ',Result) > 0 do begin
      Result[Pos(' ',Result)] := '0';
    end; { while }

  end;

begin { WriteXRef }
  XRefPos := DataStream.Position;

  ActiveStream := DataStream;
  PrintLn('xref');
  PrintLnF('0 %d',[XRefList.Count + 1]);
  PrintLn('0000000000 65535 f');
  for i1 := 1 to XRefList.Count do begin
    XRefStr := FormatXRef(integer(XRefList.Objects[i1 - 1]));
    PrintLnF('%s 00000 n',[XRefStr]);
  end; { for }
  PrintLn;
end;  { WriteXRef }

procedure TRvRenderPDF.WriteTrailer;
begin
  ActiveStream := DataStream;
  PrintLn('trailer');
  PrintLn('<<');
  PrintLnF('/Size %d',[XRefList.Count + 1]);
  PrintLnF('/Root %d 0 R', [CatalogObject.GetID]);
  PrintLnF('/Info %d 0 R',[XRefList.Count]);
  PrintLn('>>');
  PrintLn;
end;

procedure TRvRenderPDF.WriteStartXRef;
begin
  ActiveStream := DataStream;
  PrintLn('startxref');
  PrintLnF('%d',[XRefPos]);
end;

procedure TRvRenderPDF.WriteEof;
begin
  ActiveStream := DataStream;
  PrintLn('%%EOF');
end;

procedure TRvRenderPDF.RegisterObject;
begin
  XRefList.AddObject(IntToStr(XRefList.Count + 1), TObject(DataStream.Position));
end;

function TRvRenderPDF.FormatEx(const AData: string;
                                  const AArgs: array of const): string;
var
  OrigSeparator: char;
begin
  OrigSeparator := DecimalSeparator;
  DecimalSeparator := '.';
  Result := Format(AData, AArgs);
  DecimalSeparator := OrigSeparator;
end;

procedure TRvRenderPDF.PrintF(const AData: string;
                                 const AArgs: array of const);
begin
  Print(FormatEx(AData, AArgs));
end;

procedure TRvRenderPDF.PrintLnF(const AData: string;
                                   const AArgs: array of const);
begin
  PrintLn(FormatEx(AData, AArgs));
end;

procedure TRvRenderPDF.PrintRightWidth(var X1, Y1: double; Text: string; Width: double);
var
  SpaceWidth: double;
begin
{ Print a block of spaces to fill the front space width }
  SpaceWidth := Width - CurrentFont.GetTextWidth(FontSize, Text);

{ Print Text right aligned }
  AdjustXY(X1,Y1,SpaceWidth);
  AdjustXY(X1,Y1,CurrentFont.GetTextWidth(FontSize, Text));
  PrintRight(X1,Y1,Text);
end;

procedure TRvRenderPDF.PDFRoundRect(const AX1, AY1, AX2, AY2, AX3,
  AY3: double);
begin
  CurrentPage.RoundRect(AX1, AY1, AX2, AY2, AX3, AY3);
end;

procedure TRvRenderPDF.RoundRect(const pfX1, pfY1, pfX2, pfY2, pfX3,
  pfY3: double);

begin
  PenColor := Converter.Pen.Color;
  BrushColor := Converter.Brush.Color;
  PDFRoundRect(pfX1, pfY1, pfX2, pfY2, pfX3, pfY3);
end;

procedure TRvRenderPDF.Arc(const AX1, AY1, AX2, AY2, AX3, AY3, AX4,
  AY4: double);
var
  BrushStyle: TBrushStyle;
begin
  PenColor := Converter.Pen.Color;
  BrushColor := Converter.Brush.Color;
  BrushStyle := Converter.Brush.Style;
  Converter.Brush.Style := bsClear;
  PDFArc(AX1, AY1, AX2, AY2, AX3, AY3, AX4, AY4, true, false);
  Converter.Brush.Style := BrushStyle;
end;

procedure TRvRenderPDF.PDFArc(const AX1, AY1, AX2, AY2, AX3, AY3, AX4,
  AY4: double; ADoClosePath, AAllowFill: boolean);
begin
  CurrentPage.Arc(AX1, AY1, AX2, AY2, AX3, AY3, AX4, AY4, ADoClosePath, AAllowFill);
end;

procedure TRvRenderPDF.Chord(const AX1, AY1, AX2, AY2, AX3, AY3, AX4,
  AY4: double);
begin
  PenColor := Converter.Pen.Color;
  BrushColor := Converter.Brush.Color;
  PDFArc(AX1, AY1, AX2, AY2, AX3, AY3, AX4, AY4, true, true);
end;

function TRvRenderPDF.GetDoFill: boolean;
begin
  Result := bsClear <> Converter.Brush.Style;
end;

function TRvRenderPDF.GetDoStroke: boolean;
begin
  Result := psClear <> Converter.Pen.Style;
end;

procedure TRvRenderPDF.PDFPie(const AX1, AY1, AX2, AY2, AX3, AY3, AX4,
  AY4: double; ADoClosePath, AAllowFill: boolean);
begin
  CurrentPage.Pie(AX1, AY1, AX2, AY2, AX3, AY3, AX4, AY4);
end;

procedure TRvRenderPDF.Pie(const AX1, AY1, AX2, AY2, AX3, AY3, AX4,
  AY4: double);
begin
  PenColor := Converter.Pen.Color;
  BrushColor := Converter.Brush.Color;
  PDFPie(AX1, AY1, AX2, AY2, AX3, AY3, AX4, AY4, true, false);
end;

procedure TRvRenderPDF.PolyLine(const PolyLineArr: array of TFloatPoint);
var
  BrushStyle: TBrushStyle;
  BrushColor: TColor;
begin
  PenColor := Converter.Pen.Color;
  BrushColor := Converter.Brush.Color;
  BrushStyle := Converter.Brush.Style;
  Converter.Brush.Style := bsClear;
  CurrentPage.PolyLine(PolyLineArr);
  Converter.Brush.Color := BrushColor;
  Converter.Brush.Style := BrushStyle;
end;

procedure TRvRenderPDF.Polygon(const PointArr: array of TFloatPoint);
begin
  PenColor := Converter.Pen.Color;
  BrushColor := Converter.Brush.Color;
  CurrentPage.Polygon(PointArr);
end;

procedure TRvRenderPDF.FreeData;
var
  i1: integer;
begin
  FreeAndNil(FOwnedStream);

  FreeAndNil(FCatalogObject);
  FreeAndNil(FOutlinesObject);
  FreeAndNil(FProcSetObject);
  CurrentFont := nil;
  for i1 := 0 to OutlineList.Count - 1 do begin
    OutlineList.Objects[i1].Free;
  end; { for }
  OutlineList.Clear;
  for i1 := 0 to PageList.Count - 1 do begin
    PageList.Objects[i1].Free;
  end; { for }
  PageList.Clear;
  for i1 := 0 to XObjectList.Count - 1 do begin
    XObjectList.Objects[i1].Free;
  end; { for }
  XObjectList.Clear;
  for i1 := 0 to FontList.Count - 1 do begin
    FontList.Objects[i1].Free;
  end; { for }
  FontList.Clear;
  for i1 := 0 to PatternList.Count - 1 do begin
    PatternList.Objects[i1].Free;
  end; { for }
  PatternList.Clear;
  FreeAndNil(FPagesObject);
  // XRefList objects do not need to be freed
  XRefList.Clear;
end;

procedure TRvRenderPDF.InitData;
begin
  XRefPos := 0;

  FCatalogObject := TRPPDFCatalog.Create(self);
  FPagesObject := TRPPDFPages.Create(self);
  FCurrentPageObject := TRPPDFPage.Create(self);
  FPageList.AddObject('P1', FCurrentPageObject);

  FOutlinesObject := TRPPDFOutlines.Create(self);
  FProcSetObject := TRPPDFProcSet.Create(self);

  FOpenMode := 'Fit';
  FFontSize := 12;
  FirstNewPage := true;
end;

procedure TRvRenderPDF.FillRect(const pRect: TRect);
var
  PenStyle: TPenStyle;
begin
  PenColor := Converter.Pen.Color;
  PenStyle := Converter.Pen.Style;
  Converter.Pen.Style := psClear;
  BrushColor := Converter.Brush.Color;
  PDFRectangle(pRect.Left / 72,
               pRect.Top / 72,
               pRect.Right / 72,
               pRect.Bottom / 72);
  Converter.Pen.Style := PenStyle;
end;

procedure TRvRenderPDF.StretchDraw(const Rect: TRect; AGraphic: TGraphic);
var
  JPGImage: TJPEGImage;
  ImageName: string;
  Bitmap: Graphics.TBitmap;
  Rect1: TRect;
begin
  JPGImage := TJPEGImage.Create;
  JPGImage.CompressionQuality := ImageQuality;
  Bitmap := Graphics.TBitmap.Create;
  try
    Rect1.Left := 0;
    Rect1.Top := 0;
    Rect1.Right := Rect.Right - Rect.Left;
    Rect1.Right := Round(Rect1.Right / DPI_MULTIPLIER * MetafileDPI);
    Rect1.Bottom := Rect.Bottom - Rect.Top;
    Rect1.Bottom := Round(Rect1.Bottom / DPI_MULTIPLIER * MetafileDPI);
    Bitmap.Width := Rect1.Right;
    Bitmap.Height := Rect1.Bottom;
    Bitmap.Canvas.StretchDraw(Rect1, AGraphic);
    JPGImage.Assign(Bitmap);
    JPGImage.Compress;
    ImageName := AddJPG(JPGImage,Converter.ReuseGraphic);

  finally
    JPGImage.Free;
    Bitmap.Free;
  end; { tryf }
  PrintJPG(Rect.Left / DPI_MULTIPLIER,
           Rect.Top / DPI_MULTIPLIER,
           Rect.Right / DPI_MULTIPLIER,
           Rect.Bottom / DPI_MULTIPLIER,
           ImageName);
end; { StretchDraw }

procedure TRvRenderPDF.Draw(const pfX1, pfY1: double; AGraphic: TGraphic);
var
  JPGImage: TJPEGImage;
  ImageName: string;
  Bitmap: Graphics.TBitmap;
  X2, Y2: double;
  Rect1: TRect;
begin
  JPGImage := TJPEGImage.Create;
  JPGImage.CompressionQuality := ImageQuality;
  Bitmap := Graphics.TBitmap.Create;
  try
    Bitmap.Width := AGraphic.Width;
    Bitmap.Height := AGraphic.Height;
    Rect1.Left := 0;
    Rect1.Top := 0;
    Rect1.Right := Bitmap.Width;
    Rect1.Bottom := Bitmap.Height;
    Bitmap.Canvas.StretchDraw(Rect1, AGraphic);
    JPGImage.Assign(Bitmap);
    ImageName := AddJPG(JPGImage,Converter.ReuseGraphic);
  finally
    JPGImage.Free;
    Bitmap.Free;
  end; { tryf }
  X2 := pfX1 + (AGraphic.Width / 72);
  Y2 := pfY1 + (AGraphic.Height / 72);
  PrintJPG(pfX1, pfY1, X2, Y2, ImageName);
end; { Draw }

procedure TRvRenderPDF.CreatePDFObjects;
begin
  FDataStream := TMemoryStream.Create;
  FOutlineList := TStringList.Create;
  FPageList:= TStringList.Create;
  FXObjectList := TStringList.Create;
  FFontList := TStringList.Create;
  FPatternList := TStringList.Create;
  FXRefList := TStringList.Create;

  InitData;
end;

procedure TRvRenderPDF.FreePDFObjects;
begin
  FreeData;
  FreeAndNil(FDataStream);
  FreeAndNil(FOutLineList);
  FreeAndNil(FPageList);
  FreeAndNil(FXObjectList);
  FreeAndNil(FFontList);
  FreeAndNil(FPatternList);
  FreeAndNil(FXRefList);
end;

function TRvRenderPDF.StrToHex(const AValue: string): string;
var
  I1: integer;
begin
  Result := '';
  for I1 := 1 to Length(AValue) do begin
    Result := Result + IntToHex(ord(AValue[I1]), 2);
  end; { for }
end; { StrToHex }

{$IFDEF LEVEL6}{$WARN SYMBOL_DEPRECATED OFF}{$ENDIF}
procedure TRvRenderPDF.PrintRender(NDRStream: TStream;
                                   OutputFileName: TFileName);
var
  LExtension: string;
begin
  if Pos('.',OutputFileName) < 1 then begin
    LExtension := Copy(FileExtension, Pos('.',FileExtension),Length(FileExtension));
    if Pos(';',LExtension) > 0 then begin
      LExtension := Copy(LExtension, 0, Pos(';',LExtension) - 1);
    end; { if }

    OutputFileName := OutputFileName + LExtension;
  end; { if }
  InitFileStream(OutputFileName);
  with TRPConverter.Create(NDRStream, self) do try
    Generate;
  finally
    Free;
  end; { with }
end;
{$IFDEF LEVEL6}{$WARN SYMBOL_DEPRECATED ON}{$ENDIF}

{ TRPPDFObject }

constructor TRPPDFObject.Create(AOwner: TObject);
begin
  FDataStream := TMemoryStream.Create;
  FRvRenderPDF := AOwner as TRvRenderPDF;
end;

destructor TRPPDFObject.Destroy;
begin
  inherited;
  FreeAndNil(FDataStream);
end;

function TRPPDFObject.GetID: integer;
begin
  Result := FID;
end;

procedure TRPPDFObject.SetID(AValue: integer);
begin
  FID := AValue;
end;

{ TRPPDFCatalog }

procedure TRPPDFCatalog.InitData;
var
  OutlinesObject: TRPPDFObject;
  PagesObject: TRPPDFObject;
begin
  OutlinesObject := RvRenderPDF.OutlinesObject;
  PagesObject := RvRenderPDF.PagesObject;

  RvRenderPDF.ActiveStream := DataStream;
  RvRenderPDF.PrintLnF('%d 0 obj',[GetID]);
  RvRenderPDF.PrintLn('<<');
  RvRenderPDF.PrintLn('/Type /Catalog');
  RvRenderPDF.PrintLnF('/Outlines %d 0 R',[OutlinesObject.GetID]);
  RvRenderPDF.PrintLnF('/Pages %d 0 R',[PagesObject.GetID]);
  if (ShowOutlines) then begin
    RvRenderPDF.PrintLn('/PageMode /UseOutlines');
  end; { if }
//  if (RvRenderPDF.OpenMode = 'Fit') then begin
//    RvRenderPDF.PrintLn('/OpenAction [ %d 0 R /%s ] ',
//                       [$page->_getID(), RvRenderPDF.OpenMode]);
//  end else if (OpenMode = '') then begin
//    $result .= sprintf("/OpenAction [ %d 0 R /%s 0] \r\n",
//                       $page->_getID(), $openMode);
//  end; { else }
  RvRenderPDF.PrintLn('>>');
  RvRenderPDF.PrintLn('endobj');
  RvRenderPDF.PrintLn;
  RvRenderPDF.ActiveStream.Position := 0;
end;

{ TRPPDFPages }

function TRPPDFPages.GetPageReferences: string;
var
  i1: integer;
  PDFObject: TRPPDFObject;
begin
  for i1 := 0 to RvRenderPDF.PageList.Count - 1 do begin
    PDFObject := TRPPDFObject(RvRenderPDF.PageList.Objects[i1]);
    Result := Result + RvRenderPDF.FormatEx('%d 0 R ',[PDFObject.GetID]);
  end; { for }
  Result := Copy(Result, 1, Length(Result) - 1);
end;

procedure TRPPDFPages.InitData;
begin
  RvRenderPDF.ActiveStream := DataStream;
  RvRenderPDF.PrintLnF('%d 0 obj',[GetID]);
  RvRenderPDF.PrintLn('<<');
  RvRenderPDF.PrintLn('/Type /Pages');
  RvRenderPDF.PrintLnF('/Count %d',[RvRenderPDF.PageList.Count]);
  RvRenderPDF.PrintLnF('/Kids [ %s ] ', [GetPageReferences]);
  RvRenderPDF.PrintLn('>>');
  RvRenderPDF.PrintLn('endobj');
  RvRenderPDF.PrintLn;
  RvRenderPDF.ActiveStream.Position := 0;
end;

{ TRPPDFPageContent }

constructor TRPPDFPageContent.Create(AOwner: TObject);
begin
  inherited Create(AOwner);
  FPageStream := TMemoryStream.Create;
  RvRenderPDF := AOwner as TRvRenderPDF;
  RvRenderPDF.ActiveStream := PageStream;
  RvRenderPDF.PrintLn('2 J');
end;

destructor TRPPDFPageContent.Destroy;
begin
  inherited;
  FreeAndNil(FPageStream);
end;

procedure TRPPDFPageContent.InitData;
var
  CompressMethod: string;
  Stream: TStream;
begin
  RvRenderPDF.ActiveStream := DataStream;
  with RvRenderPDF do begin
    PrintLnF('%d 0 obj',[GetID]);

    if UseCompression and Assigned(FOnCompress) then begin
      Stream := TMemoryStream.Create;
      try
        CompressMethod := 'FlateDecode';
        PageStream.Position := 0;
        FOnCompress(PageStream,Stream,CompressMethod);
(* Typical OnCompress method contents
  With TCompressionStream.Create(clMax, OutStream) do try
    CopyFrom(InStream, InStream.Size);
  finally
    Free;
  end; { with }
*)
        PrintLn('<<');
        PrintLnF('/Length %d', [Stream.Size]);
        PrintLn('/Filter [/' + CompressMethod + ']');
        PrintLn('>>');
        PrintLn('stream');
        Stream.Position := 0;
        ActiveStream.CopyFrom(Stream, Stream.Size);
      finally
        Stream.Free;
      end;
    end else begin
      PrintLnF('<< /Length %d >>',[PageStream.Size]);
      PrintLn('stream');
      PageStream.Position := 0;
      ActiveStream.CopyFrom(PageStream, PageStream.Size);
    end; { else }
(*
    UseCompression := RPRender.UseCompression;
    If UseCompression then begin
      Stream := TMemoryStream.Create;
      try
        CompressionStream := TCompressionStream.Create(clMax, Stream);
        try
          PageStream.Position := 0;
          CompressionStream.CopyFrom(PageStream, PageStream.Size);
        finally
          CompressionStream.Free;
        end;
        PrintLn('<<');
        PrintLn('/Length %d', [Stream.Size]);
        PrintLn('/Filter [/FlateDecode]');
        PrintLn('>>');
        PrintLn('stream');
        Stream.Position := 0;
        CopyFrom(Stream, Stream.Size);
      finally
        Stream.Free;
      end;
    end else begin
      PrintLnF('<< /Length %d >>',[PageStream.Size]);
      PrintLn('stream');
      PageStream.Position := 0;
      ActiveStream.CopyFrom(PageStream, PageStream.Size);
(*    end; { else } *)
    PrintLn('endstream');
    PrintLn('endobj');
    PrintLn;
    ActiveStream.Position := 0;
  end; { with }
end;

{ TRPPDFPage }

procedure TRPPDFPage.Arc(const pfX1, pfY1, pfX2, pfY2, pfX3, pfY3, pfX4,
  pfY4: double; DoClosePath, AllowFill: boolean);
var
  X1, Y1: double;
  XC, YC: double;
  A1, A2: double;
  R1: double;

  function VectorAngle(XC,YC: double;
                       XT,YT: double): double;

  begin { VectorAngle }
    if XT > XC then begin
      Result := 360.0 - (ArcTan((YT - YC) / (XT - XC)) * 180.0 / Pi);
      if Result > 360.0 then begin
        Result := Result - 360.0;
      end; { if }
    end else if XT < XC then begin
      Result := 180.0 - (ArcTan((YT - YC) / (XT - XC)) * 180.0 / Pi);
    end else begin
      if YT < YC then begin
        Result := 90.0;
      end else begin
        Result := 270.0;
      end; { else }
    end; { else }
  end;  { VectorAngle }

begin
  RvRenderPDF.ActiveStream := PageContent.PageStream;
  with RvRenderPDF do begin
    SetPenAndBrush;
    PrintLn('% arc code');
    X1 := (pfX1 + pfX2)/ 2;
    Y1 := (pfY1 + pfY2) / 2;
    R1 := (pfX2 - pfX1) / 2;
    XC := (pfX1 + pfX2) / 2;
    YC := (pfY1 + pfY2) / 2;
    A1 := VectorAngle(XC,YC,pfX3,pfY3);
    A2 := VectorAngle(XC,YC,pfX4,pfY4);
    RadiusArc(X1, Y1, R1, A1, A2);
    if DoClosePath then begin
      if AllowFill then begin
        ClosePath;
      end else begin
        PrintLn('S');
      end; { else }
    end; { if }
  end; { with }
end;

procedure TRPPDFPage.ClosePath;
begin
  with RvRenderPDF do begin
    if DoStroke and DoFill then begin
      PrintLn('b');
    end else if DoFill then begin
      PrintLn('f');
    end else if DoStroke then begin
      PrintLn('S');
    end; { else }
  end; { with }
end;

constructor TRPPDFPage.Create(AOwner: TObject);
begin
  inherited Create(AOwner);

  PageWidth := 8.5;
  PageHeight := 11.0;
  FPageContent := TRPPDFPageContent.Create(AOwner);
  FLastPenColorDefined := false;
  FPenColor := clBlack;
  FLastBrushColorDefined := false;
  FBrushColor := clWhite;
end;

procedure TRPPDFPage.CurveTo(const AX1, AY1, AX2, AY2, AX3, AY3: double);
begin
  RvRenderPDF.ActiveStream := PageContent.PageStream;
  with RvRenderPDF do begin
    PrintLnF('%.4f %.4f %.4f %.4f %.4f %.4f c',[
            InchesToUnitsX(AX1), InchesToUnitsY(AY1),
            InchesToUnitsX(AX2), InchesToUnitsY(AY2),
            InchesToUnitsX(AX3), InchesToUnitsY(AY3)]);
  end; { with }
end;

function TRPPDFPage.DegreesToRadians(ADegrees: double): double;
begin
  Result := ADegrees * System.Pi / 180;
end;

destructor TRPPDFPage.Destroy;
begin
  inherited;
  FreeAndNil(FPageContent);
end;

procedure TRPPDFPage.Ellipse(const AX1, AY1, AX2, AY2: double);
var
  EllipsePts: TEllipsePts;
begin
  RvRenderPDF.ActiveStream := PageContent.PageStream;
  with RvRenderPDF do begin
    SetPenAndBrush;

    EllipsePts := GetEllipsePts(AX1, AY1, AX2, AY2);

    with EllipsePts do begin
      MoveTo(XC,YA);
      CurveTo(XD,YA,XE,YB,XE,YC);
      CurveTo(XE,YD,XD,YE,XC,YE);
      CurveTo(XB,YE,XA,YD,XA,YC);
      CurveTo(XA,YB,XB,YA,XC,YA);
    end; { with }
    ClosePath;
  end; { with }
end; { Ellipse }

function TRPPDFPage.FormatRPPDFText(const AValue: string): string;
var
  s1: string;
  i1: integer;
begin
  s1 := AValue;
  for i1 := Length(s1) downto 1 do begin
    if (s1[i1] = '(') or (s1[i1] = ')') or (s1[i1] = '\') then begin
      Insert('\', s1, i1);
    end; { if }
  end; { for }
  Result := s1;
end;

function TRPPDFPage.GetEllipsePts(const AX1, AY1, AX2, AY2: double): TEllipsePts;
var
  Factor: double;
  Offset: double;
begin
  with Result do begin
    case RvRenderPDF.FrameMode of
      fmInside: Offset := RvRenderPDF.PenWidth / 2;
      fmSplit: Offset := 0;
      fmOutside: Offset := -(RvRenderPDF.PenWidth / 2);
      else Offset := 0;
    end; { case }
    Factor := (4.0 / 3.0) * (Sqrt(2) - 1.0);
    //Factor := (Sqrt(2) - 1.0) / 3 * 4;
    if (AX1 + Offset) < (AX2 - Offset) then begin
      XA := AX1 + Offset;
    end else begin
      XA := AX2 - Offset;
    end; { else }
    XC := ((AX1 + Offset) + (AX2 - Offset)) / 2.0;
    if (AX1 + Offset) > (AX2 - Offset) then begin
      XE := AX1 + Offset;
    end else begin
      XE := AX2 - Offset;
    end; { else }
    XB := XC - Factor * (XC - XA);
    XD := XC + Factor * (XC - XA);

    if (AY1 + Offset) < (AY2 - Offset) then begin
      YA := AY1 + Offset;
    end else begin
      YA := AY2 - Offset;
    end; { else }
    YC := ((AY1 + Offset) + (AY2 - Offset)) / 2.0;
    if (AY1 + Offset) > (AY2 - Offset) then begin
      YE := AY1 + Offset;
    end else begin
      YE := AY2 - Offset;
    end; { else }
    YB := YC - Factor * (YC - YA);
    YD := YC + Factor * (YC - YA);
  end; { with }
end;

function TRPPDFPage.GetRPPDFColor(Color: TColor; bAExpanded: boolean): string;
var
  sColor: string;
  dRed: double;
  dGreen: double;
  dBlue: double;
  dMode: double;
begin
  if Color >= 0 then begin
    sColor := IntToHex(Color, 8);
  end else begin
    sColor := IntToHex(clBlack, 8);
  end; { else }
  dMode := StrToInt('$' + Copy(sColor, 1,2)) / 255;
  dBlue := StrToInt('$' + Copy(sColor, 3,2)) / 255;
  dGreen := StrToInt('$' + Copy(sColor, 5,2)) / 255;
  dRed := StrToInt('$' + Copy(sColor, 7, 2)) / 255;
  sColor := RvRenderPDF.FormatEx('%.3g %.3g %.3g', [dRed, dGreen, dBlue]);
  if bAExpanded then begin
    sColor := RvRenderPDF.FormatEx('%s %.3g', [sColor, dMode]);
  end; { if }
  Result := sColor;
end;

function TRPPDFPage.InchesToUnitsX(const AValue: double): double;
begin
  Result := AValue * DPI_MULTIPLIER;
end;

function TRPPDFPage.InchesToUnitsY(const AValue: double): double;
begin
  Result := (PageHeight - AValue) * DPI_MULTIPLIER;
end;

procedure TRPPDFPage.InitData;
var
  i1: integer;
  id: integer;
  PDFObject: TRPPDFObject;
begin
  RvRenderPDF.ActiveStream := DataStream;
  id := GetID;
  RvRenderPDF.PrintLnF('%d 0 obj',[id]);
  RvRenderPDF.PrintLn('<<');
  RvRenderPDF.PrintLn('/Type /Page');
  RvRenderPDF.PrintLnF('/Parent %d 0 R', [RvRenderPDF.PagesObject.GetID]);
  RvRenderPDF.PrintLn('/Resources <<');

  if RvRenderPDF.FontList.Count > 0 then begin
    RvRenderPDF.PrintLn('/Font <<');
    for i1 := 0 to RvRenderPDF.FontList.Count - 1 do begin
      PDFObject := TRPPDFObject(RvRenderPDF.FontList.Objects[i1]);
      RvRenderPDF.PrintLnF('/%s %d 0 R ',[TRPPDFFont(PDFObject).ObjectName, PDFObject.GetID]);
    end; { for }
    RvRenderPDF.PrintLn('>>');
  end; { if }

  if RvRenderPDF.PatternList.Count > 0 then begin
    RvRenderPDF.PrintLn('/Pattern <<');
    for i1 := 0 to RvRenderPDF.PatternList.Count - 1 do begin
      PDFObject := TRPPDFObject(RvRenderPDF.PatternList.Objects[i1]);
      RvRenderPDF.PrintLnF('/%s %d 0 R ',[TRPPDFPattern(PDFObject).ObjectName, PDFObject.GetID]);
    end; { for }
    RvRenderPDF.PrintLn('>>');
  end; { if }

  if RvRenderPDF.XObjectList.Count > 0 then begin
    RvRenderPDF.PrintLn('/XObject <<');
    for i1 := 0 to RvRenderPDF.XObjectList.Count - 1 do begin
      PDFObject := TRPPDFObject(RvRenderPDF.XObjectList.Objects[i1]);
      RvRenderPDF.PrintLnF('/%s %d 0 R',[TRPPDFXObject(PDFObject).ObjectName, PDFObject.GetID]);
    end; { for }
    RvRenderPDF.PrintLn('>>');
  end; { if }
  RvRenderPDF.PrintLnF('/ProcSet %d 0 R', [RvRenderPDF.ProcSetObject.GetID]);

  RvRenderPDF.PrintLn('>>');
  RvRenderPDF.PrintLnF('/MediaBox [0 0 %.4f %.4f]',[PageWidth * DPI_MULTIPLIER,PageHeight * DPI_MULTIPLIER]);
  RvRenderPDF.PrintLnF('/Contents %d 0 R',[id + 1]);
  RvRenderPDF.PrintLn('>>');
  RvRenderPDF.PrintLn('endobj');
  RvRenderPDF.PrintLn;
  RvRenderPDF.ActiveStream.Position := 0;

end;

procedure TRPPDFPage.LineTo(const AX, AY: double; DoClosePath: boolean);
begin
  RvRenderPDF.ActiveStream := PageContent.PageStream;
  with RvRenderPDF do begin
    SetPenAndBrush;
    PrintF('%.4f %.4f l ',[InchesToUnitsX(AX),InchesToUnitsY(AY)]);
    if DoClosePath then begin
      ClosePath;
    end; { if }
  end; { with }
end; { LineTo }

procedure TRPPDFPage.SetPenAndBrush;
var
  Pattern: TRPPDFPattern;
begin
  with RvRenderPDF do begin
    if not LastPenStyleDefined or (Converter.Pen.Style <> LastPenStyle) then begin
      case Converter.Pen.Style of
        psDash: PrintLn('[13 6] 0 d');
        psDot: PrintLn('[2] 0 d');
        psDashDot: PrintLn('[8 4 1 4] 0 d');
        psDashDotDot: PrintLn('[10 3 2 2 2 3] 0 d');
        else begin
          PrintLn('[] 0 d');
        end;
      end; { case }
      LastPenStyle := Converter.Pen.Style;
      LastPenStyleDefined := true;
    end; { if }
    if not LastPenColorDefined or (PenColor <> LastPenColor) then begin
      PrintPenColor;
      LastPenColor := PenColor;
      LastPenColorDefined := true;
      LastTextColorDefined := false;
    end; { if }
    if not LastBrushColorDefined or (BrushColor <> LastBrushColor) then begin
      PrintBrushColor;
      LastBrushColor := BrushColor;
      LastBrushColorDefined := true;
      LastTextColorDefined := false;
    end; { if }
    if not LastBrushStyleDefined or (Converter.Brush.Style <> LastBrushStyle) then begin
      if (Converter.Brush.Style <> bsClear) and (Converter.Brush.Style <> bsSolid) then begin
        Pattern := GetPattern(GetPatternName(Converter.Brush.Style, Converter.Brush.Color));
        PrintLn('/Pattern cs');
        PrintLnF('/%s scn',[Pattern.ObjectName]);
      end else begin
        if (LastBrushStyle <> bsClear) and (LastBrushStyle <> bsSolid) then begin
          PrintLn('/DeviceRGB cs');
          PrintPenColor;
          PrintBrushColor;
        end;
      end; { else }
      LastBrushStyle := Converter.Brush.Style;
      LastBrushStyleDefined := true;
    end; { if }
    if not LastPenWidthDefined or (PenWidth <> LastPenWidth) then begin
      PrintLnF('%.4f w ', [PenWidth * DPI_MULTIPLIER]); // %Set linewidth
      LastPenWidth := PenWidth;
      LastPenWidthDefined := true;
    end;
  end; { with }
end;

procedure TRPPDFPage.MoveTo(const AX, AY: double);
begin
  RvRenderPDF.ActiveStream := PageContent.PageStream;
  with RvRenderPDF do begin
    SetPenAndBrush;
    PrintLnF('%.4f %.4f m',[InchesToUnitsX(AX),InchesToUnitsY(AY)]);
  end; { with }
end;

procedure TRPPDFPage.Pie(const pfX1, pfY1, pfX2, pfY2, pfX3, pfY3, pfX4,
  pfY4: double);
begin
  Arc(pfX1, pfY1, pfX2, pfY2, pfX3, pfY3, pfX4,pfY4, false, true);
  LineTo((pfX1 + pfX2) / 2.0, (pfY1 + pfY2) / 2.0, true);
end;

procedure TRPPDFPage.PolyLine(const PolyLineArr: array of TFloatPoint);
var
  I1: integer;
  Cnt: integer;
begin
  Cnt := Length(PolyLineArr);
  for I1 := 0 to Cnt - 1 do begin
    LineTo(PolyLineArr[I1].X,PolyLineArr[I1].Y,false);
    if I1 mod 4 = 3 then begin
      RvRenderPDF.PrintLn;
    end;
  end; { for }
  ClosePath;
end;

procedure TRPPDFPage.Polygon(const PointArr: array of TFloatPoint);
var
  I1: integer;
  Cnt: integer;
begin
  Cnt := Length(PointArr);
  for I1 := 0 to Cnt - 1 do begin
    LineTo(PointArr[I1].X,PointArr[I1].Y,false);
  end; { for }
  ClosePath;
end;

procedure TRPPDFPage.PrintBrushColor;
begin
  RvRenderPDF.ActiveStream := PageContent.PageStream;
  with RvRenderPDF do begin
    PrintLnF('%s rg',[GetRPPDFColor(BrushColor, false)]);
  end; { with }
end;

procedure TRPPDFPage.AdjustRotationOffset(const ATextWidth: double;
                                            var AX, AY: double);
var
  Rotation: double;
begin
  Rotation := RvRenderPDF.FontRotation;

  if (Rotation > 0) and (Rotation <= 90) then begin
    AX := AX - (ATextWidth) * cos(Rotation * pi / 180);
    AY := AY + (ATextWidth) * sin(Rotation * pi / 180);
  end else if (Rotation > 90) and (Rotation <= 180) then begin
    AX := AX + (ATextWidth) * cos((180 - Rotation) * pi / 180);
    AY := AY + (ATextWidth) * sin((180 - Rotation) * pi / 180);
  end else if (Rotation > 180) and (Rotation <= 270) then begin
    AX := AX + (ATextWidth) * cos((Rotation - 180) * pi / 180);
    AY := AY - (ATextWidth) * sin((Rotation - 180) * pi / 180);
  end else if (Rotation > 270) and (Rotation < 360) then begin
    AX := AX - (ATextWidth) * cos((360 - Rotation) * pi / 180);
    AY := AY - (ATextWidth) * sin((360 - Rotation) * pi / 180);
  end else begin
    AX := AX - (ATextWidth);
    AY := AY;
  end; { else }

end;


procedure TRPPDFPage.PrintCenter(const AX, AY: double; AText: string);
var
  TextWidth : double;
  CurrentFont: TRPPDFFont;
  X2,Y2: double;
begin
  CurrentFont := RvRenderPDF.CurrentFont;
  TextWidth := CurrentFont.GetTextWidth(RvRenderPDF.FontSize, AText);
  X2 := AX;
  Y2 := AY;

  AdjustRotationOffset(TextWidth/2, X2, Y2);
  PrintLeft(X2, Y2, AText);
end;

procedure TRPPDFPage.TextRect(Rect: TRect; X1,Y1: double; S1: string);
begin
  RvRenderPDF.ActiveStream := PageContent.PageStream;
  RvRenderPDF.PrintLn(' q ');
  with Rect do begin
    RvRenderPDF.PenWidth := 0;
    Rectangle(Left / DPI_MULTIPLIER, Top / DPI_MULTIPLIER,
              Right / DPI_MULTIPLIER, Bottom / DPI_MULTIPLIER, true);
  end; { with }

  PrintLeft(X1, Y1, S1);
  RvRenderPDF.PrintLn(' Q ');
end;

procedure TRPPDFPage.PrintJPG(const AX1, AY1, AX2, AY2: double;
  AName: string);
begin
  RvRenderPDF.ActiveStream := PageContent.PageStream;
  with RvRenderPDF do begin
    PrintLn('q');
    PrintLnF('%.4f 0 0 %.4f %.4f %.4f cm',[
                              (AX2 - AX1) * DPI_MULTIPLIER,
                              (AY2 - AY1) * DPI_MULTIPLIER,
                              AX1 * DPI_MULTIPLIER,
                              (PageHeight - AY2) * DPI_MULTIPLIER]);
    PrintLnF('/%s Do',[AName]);
    PrintLn('Q');
  end; { with }
end;

procedure TRPPDFPage.PrintLeft(const AX, AY: double; AText: string);
var
  FontObjectName: string;
  Radians: double;
  CurrentFont: TRPPDFFont;
  Offset: double;
  LineWidth: double;
  SaveLineWidth: double;
  x1: double;
  y1: double;
begin
  RvRenderPDF.ActiveStream := PageContent.PageStream;
  CurrentFont := RvRenderPDF.CurrentFont;
  with RvRenderPDF do begin
    PrintLn('BT');
    if not LastTextColorDefined or (TextColor <> LastTextColor) then begin
      PrintTextColor;
      LastTextColor := TextColor;
      LastTextColorDefined := true;
      LastPenColorDefined := false;
      LastBrushColorDefined := false;
    end; { if }
    FontObjectName := CurrentFont.ObjectName;
//    PrintCharSpacing;
//    PrintWordSpacing;
    PrintLnF('/%s %.4d Tf',[FontObjectName,RvRenderPDF.FontSize]);

    if ((Pos('-',CurrentFont.PDFFontName) > 0) and
        (CompareText(Copy(CurrentFont.PDFFontName,1,Pos('-',CurrentFont.PDFFontName) - 1),'Courier') = 0)) or
       (CompareText(CurrentFont.PDFFontName,'Courier') = 0) then begin
      Offset := (RvRenderPDF.FontSize / DPI_MULTIPLIER) * 0.65;
    end else begin
      Offset := (RvRenderPDF.FontSize * 0.275) / DPI_MULTIPLIER;
    end; { else }
    PrintLnF('%.4f %.4f Td',[InchesToUnitsX(AX - Offset), InchesToUnitsY(AY)]);

    if FontRotation <> 0 then begin
      Radians := FontRotation * pi / 180;
      PrintLnF('%.4f %.4f %.4f %.4f %.4f %.4f Tm',[
              Cos(Radians),Sin(Radians),-Sin(Radians), Cos(Radians),
              InchesToUnitsX(AX),InchesToUnitsY(AY)]);
    end; { if }

    RvRenderPDF.PrintLnF('(%s) Tj',[FormatRPPDFText(AText)]);

    PrintLn('ET');

  end; { with }

  if (RvRenderPDF.FontRotation = 0)
      and (RvRenderPDF.UnderlineFont
      or RvRenderPDF.StrikeoutFont) then begin
    SaveLineWidth := RvRenderPDF.PenWidth;
    x1 := AX + CurrentFont.GetTextWidth(RvRenderPDF.FontSize, AText);
    if RvRenderPDF.UnderlineFont then begin
      LineWidth := (RvRenderPDF.FontSize + 1) * 0.0014;
      RvRenderPDF.PenWidth := LineWidth;
      y1 := AY + (LineWidth * 1.5);
      MoveTo(AX, y1);
      LineTo(x1, y1, true);
    end; { if }
    if RvRenderPDF.StrikeoutFont then begin
      LineWidth := (RvRenderPDF.FontSize + 1) * 0.0006;
      RvRenderPDF.PenWidth := LineWidth;
      y1 := AY - (RvRenderPDF.FontSize / DPI_MULTIPLIER) * 0.22;
      MoveTo(AX, y1);
      LineTo(x1, y1, true);
    end; { if }
    RvRenderPDF.PenWidth := SaveLineWidth;
  end; { if }
end;

procedure TRPPDFPage.PrintCharSpacing;
begin
  RvRenderPDF.ActiveStream := PageContent.PageStream;
  with RvRenderPDF do begin
    PrintLn('0 TL');
    PrintLnF('%.2f Tc',[0.0]);
  end; { with }
end;

procedure TRPPDFPage.PrintWordSpacing;
begin
  RvRenderPDF.ActiveStream := PageContent.PageStream;
  with RvRenderPDF do begin
    PrintLnF('%d Tw',[0]);
  end; { with }
end;

procedure TRPPDFPage.PrintPenColor;
begin
  RvRenderPDF.ActiveStream := PageContent.PageStream;
  with RvRenderPDF do begin
    PrintLnF('%s RG',[GetRPPDFColor(PenColor, false)]);
  end; { with }
end;

procedure TRPPDFPage.PrintRight(const AX, AY: double; AText: string);
var
  TextWidth : double;
  CurrentFont: TRPPDFFont;
  X2,Y2: double;
begin
  CurrentFont := RvRenderPDF.CurrentFont;
  TextWidth := CurrentFont.GetTextWidth(RvRenderPDF.FontSize, AText);
  X2 := AX;
  Y2 := AY;

  AdjustRotationOffset(TextWidth, X2, Y2);
  PrintLeft(X2, Y2, AText);
end;

procedure TRPPDFPage.PrintTextColor;
begin
  RvRenderPDF.ActiveStream := PageContent.PageStream;
  with RvRenderPDF do begin
    PrintLnF('%s rg',[GetRPPDFColor(TextColor, false)]);
  end; { with }
end;

procedure TRPPDFPage.RadiusArc(const AX1, AY1, AR1, sAngle, eAngle: double);
var
  i1: integer;
  numArcs: integer;
  ccwcw: double;
  angleBump: double;
  halfBump: double;
  aSpan: double;
  curAngle: double;
begin
  if sAngle <> eAngle then begin
    numArcs := 1;
    ccwcw := 1.0;

    aSpan := eAngle - sAngle;
    if aSpan < 0 then begin
      aSpan := 360 + aSpan;
    end;

    if eAngle < sAngle then begin
      ccwcw := -1.0;
    end; { if }
    while Abs(aSpan) / numArcs > 90.0 do begin
      Inc(numArcs);
    end; { while }
    angleBump := aSpan / numArcs;
    halfBump := 0.5 * angleBump;
    curAngle := sAngle + halfBump;
    for i1 := 1 to numArcs do begin
      SmallArc(AX1,AY1,AR1,curAngle,halfBump, ccwcw, i1);
      curAngle := curAngle + angleBump;
    end;
    with RvRenderPDF do begin
      if (eAngle - sAngle) < 0 then begin
        MoveTo(SmallArcData[numArcs].X1, SmallArcData[numArcs].Y1);
        for i1 := numArcs downto 1 do begin
          with SmallArcData[i1] do begin
            CurveTo(X2, Y2, X3, Y3, X4, Y4);
          end; { with }
        end; { for }
      end else begin
        MoveTo(SmallArcData[1].X1, SmallArcData[1].Y1);
        for i1 := 1 to numArcs do begin
          with SmallArcData[i1] do begin
            CurveTo(X2, Y2, X3, Y3, X4, Y4);
          end; { with }
        end; { for }
      end;
    end; { with }
  end;
end;

procedure TRPPDFPage.Rectangle(const AX1, AY1, AX2, AY2: double;
  const IsClipping: boolean);
begin
  RvRenderPDF.ActiveStream := PageContent.PageStream;
  with RvRenderPDF do begin
    if not DoStroke and not DoFill then Exit;
    SetPenAndBrush;
    PrintF('%.4f %.4f %.4f %.4f re ',[
                             InchesToUnitsX(AX1),
                             InchesToUnitsY(AY1),
                             InchesToUnitsX(AX2 - AX1),
                             InchesToUnitsY(AY2) -
                             InchesToUnitsY(AY1)
                             ]);
    if IsClipping then begin
      PrintLn('W n ');
    end; { if }
    ClosePath;
  end; { with }
end;

// Rotate (x, y) by angle in degrees, and return output in (xrot, yrot).
procedure TRPPDFPage.RotateXYCoordinate(AX1, AY1, AAngle: double; var xRot,
  yRot: double);
var
  Theta, rCos, rSin: double; //cos, sine of theta
begin
  Theta := DegreesToRadians(AAngle);
  rCos := Cos(Theta);
  rSin := Sin(Theta);
  xRot := (rCos * AX1) - (rSin * AY1);
  yRot := (rSin * AX1) + (rCos * AY1);
end;

procedure TRPPDFPage.RoundRect(const AX1, AY1, AX2, AY2, AX3, AY3: double);
var
  EllipsePts: TEllipsePts;
  RectLineWidth: double;
  RectLineHeight: double;
begin
  RvRenderPDF.ActiveStream := PageContent.PageStream;
  with RvRenderPDF do begin
    SetPenAndBrush;
    EllipsePts := GetEllipsePts(AX1, AY1, AX1 + AX3, AY1 + AY3);
    RectLineWidth := AX2 - AX1 - AX3;
    RectLineHeight := AY2 - AY1 - AY3;

    with EllipsePts do begin
      MoveTo(XC,YA);
      PDFLineTo(XC + RectLineWidth, YA, false);
      CurveTo(XD + RectLineWidth,YA,XE + RectLineWidth,YB,XE + RectLineWidth,YC);
      PDFLineTo(XE + RectLineWidth,YC + RectLineHeight, false);
      CurveTo(
        XE + RectLineWidth,
        YD + RectLineHeight,
        XD + RectLineWidth,
        YE + RectLineHeight,
        XC + RectLineWidth,
        YE + RectLineHeight
      );
      PDFLineTo(XC, YE + RectLineHeight, false);
      CurveTo(
        XB,
        YE + RectLineHeight,
        XA,
        YD + RectLineHeight,
        XA,
        YC + RectLineHeight
      );
      PDFLineTo(XA, YC, false);
      CurveTo(XA,YB,XB,YA,XC,YA);
    end; { with }
    ClosePath;
  end; { with }
end;

procedure TRPPDFPage.SmallArc(AX1, AY1, AR1, MidTheta, HalfAngle,
ccwcw: double; AIndex: integer);
var
  vCos, vSin: double;
  x0, y0, x1, y1, x2, y2, x3, y3: double;
  HalfTheta: double;
begin
  HalfTheta := DegreesToRadians(Abs(halfAngle));
  vCos := Cos(HalfTheta);
  vSin := Sin(HalfTheta);

  x0 := AR1 * vCos;
  y0 := -ccwcw * AR1 * vSin;
  x1 := AR1 * (4.0 - vCos) / 3.0;
  x2 := x1;
  y1 := AR1 * ccwcw * (1.0 - vCos) * (vCos - 3.0) / (3.0 * vSin);
  y2 := -y1;
  x3 := AR1 * vCos;
  y3 := ccwcw * AR1 * vSin;

  RotateXYCoordinate(x0,y0,MidTheta,x0,y0);
  RotateXYCoordinate(x1,y1,MidTheta,x1,y1);
  RotateXYCoordinate(x2,y2,MidTheta,x2,y2);
  RotateXYCoordinate(x3,y3,MidTheta,x3,y3);
  with RvRenderPDF do begin
    SmallArcData[AIndex].X1 := AX1 + x0;
    SmallArcData[AIndex].Y1 := AY1 - y0;
    SmallArcData[AIndex].X2 := AX1 + x1;
    SmallArcData[AIndex].Y2 := AY1 - y1;
    SmallArcData[AIndex].X3 := AX1 + x2;
    SmallArcData[AIndex].Y3 := AY1 - y2;
    SmallArcData[AIndex].X4 := AX1 + x3;
    SmallArcData[AIndex].Y4 := AY1 - y3;
  end; { with }
//MoveTo(AX1+x0,AY1-y0);
//CurveTo(AX1+x1,AY1-y1,AX1+x2,AY1-y2,AX1+x3,AY1-y3);
end;

{ TRPPDFOutlines }

procedure TRPPDFOutlines.InitData;
var
  OutlineList: TStringList;
begin
  RvRenderPDF.ActiveStream := DataStream;
  OutlineList := RvRenderPDF.OutlineList;
  RvRenderPDF.PrintLnF('%d 0 obj',[GetID]);
  RvRenderPDF.PrintLn('<<');
  RvRenderPDF.PrintLn('/Type /Outlines');
  RvRenderPDF.PrintLnF('/Count %d',[OutlineList.Count]);
  RvRenderPDF.PrintLn('>>');
  RvRenderPDF.PrintLn('endobj');
  RvRenderPDF.PrintLn;
  RvRenderPDF.ActiveStream.Position := 0;
end;

{ TRPPDFProcSet }

procedure TRPPDFProcSet.InitData;
begin
  RvRenderPDF.ActiveStream := DataStream;
  RvRenderPDF.PrintLnF('%d 0 obj',[GetID]);

  if RvRenderPDF.XObjectList.Count > 0 then begin
    RvRenderPDF.PrintLn('[/PDF /Text /ImageB /ImageC /ImageI]');
  end else begin
    RvRenderPDF.PrintLn('[/PDF /Text]');
  end; { else }

  RvRenderPDF.PrintLn('endobj');
  RvRenderPDF.PrintLn;
  RvRenderPDF.ActiveStream.Position := 0;
end;

{ TRPPDFFontDescriptor }

procedure TRPPDFFontDescriptor.SetValues(OutlineTextMetric: POutlineTextMetric;
                                         AMissingWidth: integer);
begin { SetValues }
  MissingWidth := AMissingWidth;


  Panose := OutlineTextMetric.otmPanoseNumber;
  with OutlineTextMetric.otmTextMetrics do begin
    with Panose do begin
      Flags := 0;
      Flags := SetBit(Flags,6);
      if bProportion = PAN_PROP_MONOSPACED then begin
        Flags := SetBit(Flags,1);
      end; { if }
      if bFamilyType = PAN_FAMILY_SCRIPT then begin
        Flags := SetBit(Flags,4);
      end; { if }
      if ItalicAngle <> 0 then begin
        Flags := SetBit(Flags,7);
      end; { if }
    end; { with }

    FontBBox := OutlineTextMetric.otmrcFontBox;
    StemV := 80;
    StemH := 80;
    ItalicAngle := OutlineTextMetric.otmItalicAngle;
    CapHeight := tmAscent;
    XHeight := Round(tmAscent / 2);
    Ascent := tmAscent;
    Descent := -tmDescent;
    Leading := tmInternalLeading + tmExternalLeading;
    MaxWidth := tmMaxCharWidth;
    AvgWidth := tmAveCharWidth;
  end; { with }
end;  { SetValues }

function TRPPDFFontDescriptor.SetBit(AValue: integer; ABit: integer): integer;
var i1: integer;
begin { SetBit }
  i1 := 1 shl (ABit - 1);
  Result := AValue or i1;
end;  { SetBit }

procedure TRPPDFFontDescriptor.InitData;
begin { InitData }
  RvRenderPDF.ActiveStream := DataStream;
  RvRenderPDF.PrintLnF('%d 0 obj',[GetID]);
  RvRenderPDF.PrintLn('<<');
  RvRenderPDF.PrintLn('/Type /FontDescriptor');
  RvRenderPDF.PrintLnF('/FontName /%s',[PDFFontName]);
  RvRenderPDF.PrintLnF('/Flags %d',[Flags]);
  RvRenderPDF.PrintLnF('/FontBBox [%d %d %d %d]',
                       [FontBBox.Left,
                       FontBBox.Bottom,
                       FontBBox.Right,
                       FontBBox.Top]);
  RvRenderPDF.PrintLnF('/MissingWidth %d',[MissingWidth]);
  RvRenderPDF.PrintLnF('/StemV %d',[StemV]);
  RvRenderPDF.PrintLnF('/StemH %d',[StemH]);
  RvRenderPDF.PrintLnF('/ItalicAngle %d',[ItalicAngle]);
  RvRenderPDF.PrintLnF('/CapHeight %d',[CapHeight]);
  RvRenderPDF.PrintLnF('/XHeight %d',[XHeight]);
  RvRenderPDF.PrintLnF('/Ascent %d',[Ascent]);
  RvRenderPDF.PrintLnF('/Descent %d',[Descent]);
  RvRenderPDF.PrintLnF('/Leading %d',[Leading]);
  RvRenderPDF.PrintLnF('/MaxWidth %d',[MaxWidth]);
  RvRenderPDF.PrintLnF('/AvgWidth %d',[AvgWidth]);
  if RvRenderPDF.EmbedFonts and IsEmbeddingAllowed then begin
    RvRenderPDF.PrintLnF('/FontFile2 %d 0 R',[GetID + 1]);
  end; { if }
  RvRenderPDF.PrintLn('>>');
  RvRenderPDF.PrintLn('endobj');
  RvRenderPDF.PrintLn;
  RvRenderPDF.ActiveStream.Position := 0;
end;  { InitData }

{ TRPPDFFontDescendant }

procedure TRPPDFFontDescendant.InitData;
//!!!CJK
  procedure PrintWidths;
  var
    i1: integer;
  begin
    RvRenderPDF.PrintLn('/W [');

    if Descriptor.Panose.bProportion = PAN_PROP_MONOSPACED then begin
      if CharSet = SHIFTJIS_CHARSET {Japanese} then begin
        RvRenderPDF.PrintLnF('%d [', [231]);
      end else begin
        RvRenderPDF.PrintLnF('%d [', [8094]);
      end;
    end else begin
      RvRenderPDF.PrintLnF('%d [', [1]);
    end; { else }

    i1 := 32;
    while i1 <= 126 do begin
      RvRenderPDF.Print(IntToStr(FontWidths[i1]) + ' ');
      if i1 mod 10 = 9 then begin
        RvRenderPDF.PrintLn;
      end;
      Inc(i1);
    end; // while
    RvRenderPDF.PrintLn(']');

    RvRenderPDF.PrintLn(']');

  end;

begin { InitData }
  RvRenderPDF.ActiveStream := DataStream;
  RvRenderPDF.PrintLnF('%d 0 obj',[GetID]);
  RvRenderPDF.PrintLn('<<');
  RvRenderPDF.PrintLn('/Type /Font');
  RvRenderPDF.PrintLn('/Subtype /CIDFontType2');
  RvRenderPDF.PrintLnF('/BaseFont /%s',[PDFFontName]);
  RvRenderPDF.PrintLnF('/FontDescriptor %d 0 R',[Descriptor.GetID]);
  RvRenderPDF.PrintLnF('/WinCharSet /%d',[CharSet]);//!!!CJK
  RvRenderPDF.PrintLnF('/CIDSystemInfo << /Registry (%s)/Ordering (%s)/Supplement %d >>', [Registry, Ordering, Supplement]);
  RvRenderPDF.PrintLnF('/DW %d',[MaxCharWidth]);

  PrintWidths;

  RvRenderPDF.PrintLn('>>');
  RvRenderPDF.PrintLn('endobj');
  RvRenderPDF.PrintLn;
  RvRenderPDF.ActiveStream.Position := 0;
end;  { InitData }


procedure TRPPDFFontDescriptorComposite.SetValues(OutlineTextMetric: POutlineTextMetric;
                                         AMissingWidth: integer);

  function SetStyle: string;
  begin
    Result := '<< /Panose <';

    with Panose do begin
      Result := Result + IntToHex(ord(bLetterForm), 2);
      Result := Result + IntToHex(ord(bArmStyle), 2);
      Result := Result + IntToHex(ord(bFamilyType), 2);
      Result := Result + IntToHex(ord(bSerifStyle), 2);
      Result := Result + IntToHex(ord(bWeight), 2);
    end; { with }

    Result := Result + '00000000000000>>>';
  end;

begin { SetValues }
  MissingWidth := AMissingWidth;


  Panose := OutlineTextMetric.otmPanoseNumber;
  with OutlineTextMetric.otmTextMetrics do begin
    with Panose do begin
      Flags := 0;
      Flags := SetBit(Flags,6);
      if bProportion = PAN_PROP_MONOSPACED then begin
        Flags := SetBit(Flags,1);
      end; { if }
      if bFamilyType = PAN_FAMILY_SCRIPT then begin
        Flags := SetBit(Flags,4);
      end; { if }
      if ItalicAngle <> 0 then begin
        Flags := SetBit(Flags,7);
      end; { if }
      Style := SetStyle;
    end; { with }

    FontBBox := OutlineTextMetric.otmrcFontBox;
    StemV := 80;
    StemH := 80;
    ItalicAngle := OutlineTextMetric.otmItalicAngle;
    CapHeight := tmAscent;
    XHeight := Round(tmAscent / 2);
    Ascent := tmAscent;
    Descent := -tmDescent;
    Leading := tmInternalLeading + tmExternalLeading;
    MaxWidth := tmMaxCharWidth;
    AvgWidth := tmAveCharWidth;
  end; { with }
end;  { SetValues }

procedure TRPPDFFontDescriptorComposite.InitData;
begin { InitData }
  RvRenderPDF.ActiveStream := DataStream;
  RvRenderPDF.PrintLnF('%d 0 obj',[GetID]);
  RvRenderPDF.PrintLn('<<');
  RvRenderPDF.PrintLn('/Type /FontDescriptor');
  RvRenderPDF.PrintLnF('/FontName /%s',[PDFFontName]);
  RvRenderPDF.PrintLnF('/Ascent %d',[Ascent]);
  RvRenderPDF.PrintLnF('/CapHeight %d',[CapHeight]);
  RvRenderPDF.PrintLnF('/Descent %d',[Descent]);
  RvRenderPDF.PrintLnF('/Flags %d',[Flags]);
  RvRenderPDF.PrintLnF('/FontBBox [%d %d %d %d]',
                       [FontBBox.Left,
                       FontBBox.Bottom,
                       FontBBox.Right,
                       FontBBox.Top]);
  RvRenderPDF.PrintLnF('/ItalicAngle %d',[ItalicAngle]);
  RvRenderPDF.PrintLnF('/StemV %d',[StemV]);
  RvRenderPDF.PrintLnF('/XHeight %d',[XHeight]);
  RvRenderPDF.PrintLnF('/StemH %d',[StemH]);
  RvRenderPDF.PrintLn('/Style ' + Style);
  RvRenderPDF.PrintLn('>>');
  RvRenderPDF.PrintLn('endobj');
  RvRenderPDF.PrintLn;
  RvRenderPDF.ActiveStream.Position := 0;
end;  { InitData }

{ TRPPDFFontFile }

constructor TRPPDFFontFile.Create(AOwner: TObject);
begin
  inherited Create(AOwner);
  FFontStream := TMemoryStream.Create;
end;

destructor TRPPDFFontFile.Destroy;
begin
  FreeAndNil(FFontStream);
end;

procedure TRPPDFFontFile.LoadFontFile(Handle: THandle);
var
  lpvBuffer: pointer;
  FileSize: cardinal;
begin { LoadFontFile }
  FileSize := GetFontData(Handle, 0, 0, nil, 0);

  GetMem(lpvBuffer,FileSize);
  try
    FileSize := GetFontData(Handle, 0, 0, lpvBuffer, FileSize);
    FontStream.Position := 0;
    FontStream.WriteBuffer(lpvBuffer^, FileSize);
  finally
    FreeMem(lpvBuffer);
  end; { tryf }
end;  { LoadFontFile }

procedure TRPPDFFontFile.InitData;
var
  Stream: TMemoryStream;
  CompressMethod: string;
begin
  RvRenderPDF.ActiveStream := DataStream;
  with RvRenderPDF do begin
    PrintLnF('%d 0 obj',[GetID]);

    if UseCompression and Assigned(FOnCompress) then begin
      Stream := TMemoryStream.Create;
      try
        CompressMethod := 'FlateDecode';
        FontStream.Position := 0;
        FOnCompress(FontStream,Stream,CompressMethod);
(* Typical OnCompress method contents
  With TCompressionStream.Create(clMax, OutStream) do try
    CopyFrom(InStream, InStream.Size);
  finally
    Free;
  end; { with }
*)
        PrintLn('<<');
        PrintLnF('/Length %d ',[FontStream.Size]);
        PrintLnF('/Length1 %d',[FontStream.Size]);
        PrintLn('/Filter [/' + CompressMethod + ']');
        PrintLn('>>');
        PrintLn('stream');
        Stream.Position := 0;
        ActiveStream.CopyFrom(Stream, Stream.Size);
      finally
        Stream.Free;
      end;
    end else begin
      PrintLnF('<< /Length %d ',[FontStream.Size]);
      PrintLnF('/Length1 %d >>',[FontStream.Size]);
      PrintLn('stream');
      FontStream.Position := 0;
      ActiveStream.CopyFrom(FontStream, FontStream.Size);
    end; { else }
    PrintLn('endstream');
    PrintLn('endobj');
    PrintLn;
    ActiveStream.Position := 0;
  end; { with }
end;

{ TRPPDFFontTrueType }

constructor TRPPDFFontTrueType.Create(AOwner: TObject);
begin
  inherited Create(AOwner);

  FontType := 'TrueType';
  FDescriptor := TRPPDFFontDescriptor.Create(AOwner);
end;

destructor TRPPDFFontTrueType.Destroy;
begin
  FreeAndNil(FDescriptor);
  if Assigned(FFontFile) then begin
    FreeAndNil(FFontFile);
  end; { if }
  inherited;
end;


procedure TRPPDFFontTrueType.InitData;
var
  i1: integer;
begin
  RvRenderPDF.ActiveStream := DataStream;
  with RvRenderPDF do begin
    PrintLnF('%d 0 obj',[GetID]);
    PrintLn('<<');
    PrintLn('/Type /Font');
    PrintLnF('/Subtype /%s',[FontType]);
    PrintLnF('/Name /%s',[ObjectName]);
    PrintLnF('/BaseFont /%s',[PDFFontName]);
    PrintLn('/Encoding /' + FontEncodingNames[FontEncoding]);
    RvRenderPDF.PrintLnF('/FirstChar %d',[FirstChar]);
    RvRenderPDF.PrintLnF('/LastChar %d',[LastChar]);
    RvRenderPDF.Print('/Widths [');
    i1 := FirstChar;
    while i1 <= LastChar do begin
      Print(IntToStr(FontWidths[i1]) + ' ');
      if i1 mod 10 = 9 then begin
        PrintLn;
      end;
      Inc(i1);
    end; // while
    RvRenderPDF.PrintLn(']');
    RvRenderPDF.PrintLnF('/FontDescriptor %d 0 R',[Descriptor.GetID]);
    PrintLn('>>');
    PrintLn('endobj');
    PrintLn;
    ActiveStream.Position := 0;
  end; { with }
end;

procedure TRPPDFFontTrueType.SetFontData;
type
  TABCArray = array[0..255] of TABC;

const
  TRUETYPEALLOWED = 1;
var
  OutlineTextMetric: POutlineTextMetric;
  nSize: longint;
  lpABC: TABCArray;

  procedure SetWidths;

  var
    i1: integer;

  begin
    i1 := 0;
    while i1 < LastChar - FirstChar + 1 do begin
      FontWidths[i1 + FirstChar] := lpABC[i1].abcA + word(lpABC[i1].abcB) + lpABC[i1].abcC;
      Inc(i1);
    end; // while
  end;

begin
  RvRenderPDF.FontBitmap.Canvas.Font.Name := FontName;
  RvRenderPDF.FontBitmap.Canvas.Font.Style := FontStyles;
  RvRenderPDF.FontBitmap.Canvas.Font.Charset := FontCharset;


  nSize := GetOutlineTextMetrics((RvRenderPDF.FontBitmap.Canvas.Handle),
                      sizeof(OutlineTextMetric),
                      nil);
  GetMem(OutlineTextMetric, nSize);
  try
    if GetOutlineTextMetrics((RvRenderPDF.FontBitmap.Canvas.Handle),
                        nSize,
                        OutlineTextMetric) > 0 then begin
      FirstChar := cardinal(OutlineTextMetric.otmTextMetrics.tmFirstChar);
      LastChar := cardinal(OutlineTextMetric.otmTextMetrics.tmLastChar);
      if GetCharABCWidths((RvRenderPDF.FontBitmap.Canvas.Handle),
                          FirstChar,
                          LastChar,
                          lpABC) then begin
        SetWidths;

      end; { if }
    end; { if }
    FDescriptor.PDFFontName := PDFFontName;
    FDescriptor.SetValues(OutlineTextMetric, FontWidths[32]);
    // If the bit is set truetype is not allowed
    IsEmbeddingAllowed := (OutlineTextMetric.otmfsType and TRUETYPEALLOWED) <> TRUETYPEALLOWED;
    FDescriptor.IsEmbeddingAllowed := IsEmbeddingAllowed;
    if RvRenderPDF.EmbedFonts and IsEmbeddingAllowed then begin
      FontFile := TRPPDFFontFile.Create(RvRenderPDF);
      FontFile.LoadFontFile((RvRenderPDF.FontBitmap.Canvas.Handle));
    end; { if }
  finally
    FreeMem(OutlineTextMetric, nSize);
  end; { tryf }
end;

{ TRPPDFFontComposite }

constructor TRPPDFFontComposite.Create(AOwner: TObject);
begin
  inherited Create(AOwner);

  FontType := 'Type0';
  FDescendant := TRPPDFFontDescendant.Create(AOwner);
  FDescriptor := TRPPDFFontDescriptorComposite.Create(AOwner);
end;

destructor TRPPDFFontComposite.Destroy;
begin
  FreeAndNil(FDescendant);
  FreeAndNil(FDescriptor);
end;

procedure TRPPDFFontComposite.InitData;
begin
  RvRenderPDF.ActiveStream := DataStream;
  with RvRenderPDF do begin
    PrintLnF('%d 0 obj',[GetID]);
    PrintLn('<<');
    PrintLn('/Type /Font');
    PrintLnF('/Subtype /%s',[FontType]);
    PrintLnF('/Name /%s',[ObjectName]);
    PrintLnF('/BaseFont /%s',[PDFFontName]);
    PrintLn('/Encoding /' + CompositeFontEncoding);
    RvRenderPDF.PrintLnF('/DescendantFonts [ %d 0 R ]',[Descendant.GetID]);
    PrintLn('>>');
    PrintLn('endobj');
    PrintLn;
    ActiveStream.Position := 0;
  end; { with }
end;

procedure TRPPDFFontComposite.SetFontData;
type
  TABCArray = array[0..255] of TABC;

var
  OutlineTextMetric: POutlineTextMetric;
  Panose: TPanose;
  nSize: longint;
  lpABC: TABCArray;


  procedure SetWidths;

  var
    i1: integer;

  begin
    i1 := 0;
    while i1 < LastChar - FirstChar + 1 do begin
      FontWidths[i1 + FirstChar] := lpABC[i1].abcA + word(lpABC[i1].abcB) + lpABC[i1].abcC;
      Inc(i1);
    end; // while
  end;

begin
  if RvRenderPDF.IsCJKFont then begin
    with RvRenderPDF.FontBitmap.Canvas do begin
      Font.Name := FontName;
      Font.Style := FontStyles;
      Font.Charset := FontCharset;
    end; { with }

    nSize := GetOutlineTextMetrics((RvRenderPDF.FontBitmap.Canvas.Handle),
                        sizeof(OutlineTextMetric),
                        nil);
    GetMem(OutlineTextMetric, nSize);
    try
      if GetOutlineTextMetrics((RvRenderPDF.FontBitmap.Canvas.Handle),
                          nSize,
                          OutlineTextMetric) > 0 then begin
        Panose := OutlineTextMetric.otmPanoseNumber;
        if Panose.bProportion = PAN_PROP_MONOSPACED then begin
          if FontCharSet = SHIFTJIS_CHARSET then begin
            CompositeFontEncoding := '90ms-RKSJ-H';
          end else begin
            CompositeFontEncoding := 'KSCms-UHC-HW-H';
          end; { else }
        end else begin
          if FontCharSet = SHIFTJIS_CHARSET then begin
            CompositeFontEncoding := '90msp-RKSJ-H';
          end else begin
            CompositeFontEncoding := 'KSCms-UHC-H';
          end; { else }
        end; { else }
        Descriptor.PDFFontName := PDFFontName;
        Descriptor.SetValues(OutlineTextMetric, FontWidths[32]);
        Descendant.PDFFontName := PDFFontName;
        Descendant.Descriptor := Descriptor;
        Descendant.Registry := 'Adobe';
        if FontCharSet = SHIFTJIS_CHARSET then begin
          Descendant.Ordering := 'Japan1';
        end else begin
          Descendant.Ordering := 'Korea1';
        end; { else }
        if FontCharSet = SHIFTJIS_CHARSET then begin
          Descendant.Supplement := 2;
        end else begin
          Descendant.Supplement := 1;
        end; { else }
        Descendant.MaxCharWidth := OutlineTextMetric.otmTextMetrics.tmMaxCharWidth;
        Descendant.AveCharWidth := OutlineTextMetric.otmTextMetrics.tmAveCharWidth;
        Descendant.Charset := RvRenderPDF.FontBitmap.Canvas.Font.Charset;

        FirstChar := cardinal(OutlineTextMetric.otmTextMetrics.tmFirstChar);
        LastChar := cardinal(OutlineTextMetric.otmTextMetrics.tmLastChar);
        if GetCharABCWidths((RvRenderPDF.FontBitmap.Canvas.Handle),
                            FirstChar,
                            LastChar,
                            lpABC) then begin
          SetWidths;
          Descendant.FirstChar := FirstChar;
          Descendant.LastChar := LastChar;
          Descendant.FontWidths := FontWidths;
        end; { if }
      end; { if }
    finally
      FreeMem(OutlineTextMetric, nSize);
    end; { tryf }
  end;
end;

{ TRPPDFPatternResource }
procedure TRPPDFPatternResource.InitData;
begin
  RvRenderPDF.ActiveStream := DataStream;
  with RvRenderPDF do begin
    PrintLnF('%d 0 obj',[GetID]);
    PrintLn('<<');
    PrintLn('>>');
    PrintLn('endobj');
    PrintLn;
    ActiveStream.Position := 0;
  end; { with }
end;

{ TRPPDFPattern }

constructor TRPPDFPattern.Create(AOwner: TObject);
begin
  inherited;
  ObjectName := RvRenderPDF.FormatEx('P%d',[RvRenderPDF.PatternList.Count + 1]);
  FPatternResource := TRPPDFPatternResource.Create(AOwner);
end;

destructor TRPPDFPattern.Destroy;
begin
  inherited;
  FPatternResource.Destroy;
end;

procedure TRPPDFPattern.InitData;
begin
  RvRenderPDF.ActiveStream := DataStream;
  with RvRenderPDF do begin
    PrintLnF('%d 0 obj',[GetID]);
    PrintLn('<<');
    PrintLn('/Type /Pattern');
    PrintLn('/PatternType 1');
    PrintLn('/PaintType 1');
    PrintLn('/TilingType 1');
    PrintLn('/BBox [0 0 8 8]');
    PrintLn('/XStep 8');
    PrintLn('/YStep 8');
    PrintLnF('/Resources %d 0 R',[Self.PatternResource.GetID]);
    PrintLnF('/Length %d',[Length(PatternStream)]);
    PrintLn('>>');
    PrintLn('stream');
    PrintLnF('%s',[PatternStream]);
    PrintLn('endsteam');
    PrintLn('endobj');
    PrintLn;
    ActiveStream.Position := 0;
  end; { with }
end;

procedure TRPPDFPattern.SetPattern(AStyle: TBrushStyle; AFillColor: TColor);
const
  CrLf: string = #13#10;
begin
  PatternStream := '';
  case AStyle of
    bsBDiagonal: begin
      PatternStream := 'q' + CrLf;
      PatternStream := PatternStream + RvRenderPDF.CurrentPage.GetRPPDFColor(AFillColor, false) + ' RG';
      PatternStream := PatternStream + CrLf + '1 i';
      PatternStream := PatternStream + CrLf + '0 0 8 8 re';
      PatternStream := PatternStream + CrLf + 'W n';
      PatternStream := PatternStream + CrLf + '1 J 0 j 0.5 w 10 M []0 d';
      PatternStream := PatternStream + CrLf + '0 0 m';
      PatternStream := PatternStream + CrLf + '8 8 l';
      PatternStream := PatternStream + CrLf + '0 -8 m';
      PatternStream := PatternStream + CrLf + '8 0 l';
      PatternStream := PatternStream + CrLf + '0 8 m';
      PatternStream := PatternStream + CrLf + '8 16 l';
      PatternStream := PatternStream + CrLf + 'S Q';
    end;
    bsFDiagonal: begin
      PatternStream := 'q' + CrLf;
      PatternStream := PatternStream + RvRenderPDF.CurrentPage.GetRPPDFColor(AFillColor, false) + ' RG';
      PatternStream := PatternStream + CrLf + '1 i';
      PatternStream := PatternStream + CrLf + '0 0 8 8 re';
      PatternStream := PatternStream + CrLf + 'W n';
      PatternStream := PatternStream + CrLf + '1 J 0 j 0.5 w 10 M []0 d';
      PatternStream := PatternStream + CrLf + '0 0 m';
      PatternStream := PatternStream + CrLf + '8 -8 l';
      PatternStream := PatternStream + CrLf + '0 8 m';
      PatternStream := PatternStream + CrLf + '8 0 l';
      PatternStream := PatternStream + CrLf + '8 8 m';
      PatternStream := PatternStream + CrLf + '16 0 l';
      PatternStream := PatternStream + CrLf + 'S Q';
    end;
    bsDiagCross: begin
      PatternStream := 'q' + CrLf;
      PatternStream := PatternStream + RvRenderPDF.CurrentPage.GetRPPDFColor(AFillColor, false) + ' RG';
      PatternStream := PatternStream + CrLf + '1 i';
      PatternStream := PatternStream + CrLf + '0 0 8 8 re';
      PatternStream := PatternStream + CrLf + 'W n';
      PatternStream := PatternStream + CrLf + '1 J 0 j 0.5 w 10 M []0 d';
      PatternStream := PatternStream + CrLf + '0 0 m';
      PatternStream := PatternStream + CrLf + '8 -8 l';
      PatternStream := PatternStream + CrLf + '0 8 m';
      PatternStream := PatternStream + CrLf + '8 0 l';
      PatternStream := PatternStream + CrLf + '8 8 m';
      PatternStream := PatternStream + CrLf + '16 0 l';
      PatternStream := PatternStream + CrLf + '0 0 m';
      PatternStream := PatternStream + CrLf + '8 8 l';
      PatternStream := PatternStream + CrLf + '0 -8 m';
      PatternStream := PatternStream + CrLf + '8 0 l';
      PatternStream := PatternStream + CrLf + '0 8 m';
      PatternStream := PatternStream + CrLf + '8 16 l';
      PatternStream := PatternStream + CrLf + 'S Q';
    end;
    bsCross: begin
      PatternStream := 'q' + CrLf;
      PatternStream := PatternStream + RvRenderPDF.CurrentPage.GetRPPDFColor(AFillColor, false) + ' RG';
      PatternStream := PatternStream + CrLf + '1 i';
      PatternStream := PatternStream + CrLf + '0 0 8 8 re';
      PatternStream := PatternStream + CrLf + 'W n';
      PatternStream := PatternStream + CrLf + '1 J 0 j 0.5 w 10 M []0 d';
      PatternStream := PatternStream + CrLf + '0 4 m';
      PatternStream := PatternStream + CrLf + '8 4 l';
      PatternStream := PatternStream + CrLf + '4 0 m';
      PatternStream := PatternStream + CrLf + '4 8 l';
      PatternStream := PatternStream + CrLf + 'S Q';
    end;
    bsHorizontal: begin
      PatternStream := 'q' + CrLf;
      PatternStream := PatternStream + RvRenderPDF.CurrentPage.GetRPPDFColor(AFillColor, false) + ' RG';
      PatternStream := PatternStream + CrLf + '1 i';
      PatternStream := PatternStream + CrLf + '0 0 8 8 re';
      PatternStream := PatternStream + CrLf + 'W n';
      PatternStream := PatternStream + CrLf + '1 J 0 j 0.5 w 10 M []0 d';
      PatternStream := PatternStream + CrLf + '0 4 m';
      PatternStream := PatternStream + CrLf + '8 4 l';
      PatternStream := PatternStream + CrLf + 'S Q';
    end;
    bsVertical: begin
      PatternStream := 'q' + CrLf;
      PatternStream := PatternStream + RvRenderPDF.CurrentPage.GetRPPDFColor(AFillColor, false) + ' RG';
      PatternStream := PatternStream + CrLf + '1 i';
      PatternStream := PatternStream + CrLf + '0 0 8 8 re';
      PatternStream := PatternStream + CrLf + 'W n';
      PatternStream := PatternStream + CrLf + '1 J 0 j 0.5 w 10 M []0 d';
      PatternStream := PatternStream + CrLf + '4 0 m';
      PatternStream := PatternStream + CrLf + '4 8 l';
      PatternStream := PatternStream + CrLf + 'S Q';
    end;
  end;
end;  { SetPattern }

{ TRPPDFFont }

constructor TRPPDFFont.Create(AOwner: TObject);
begin
  inherited;
  ObjectName := RvRenderPDF.FormatEx('F%d',[RvRenderPDF.FontList.Count + 1]);
  FontType := 'Type1';
end;


procedure TRPPDFFont.SetCharSet(AFontCharset: TFontCharset);
begin
  FFontCharSet := ConvertCharset(AFontCharset);
end;

function TRPPDFFont.GetTextWidth(AFontSize: double; AText: string): double;
var
  TextExtent: TSize;
begin
  with RvRenderPDF.FontBitmap.Canvas do begin
    Font.Name := FontName;
    Font.Style := FontStyles;
    Font.Charset := FontCharset;
  end;
  GetTextExtentPoint32((RvRenderPDF.FontBitmap.Canvas.Handle),
	 PChar(AText),Length(AText),TextExtent);
  if TextExtent.cX = 0 then begin
    TextExtent.cX := 1;
  end; { if }
  Result := TextExtent.cX;
  Result := (Result / 1000) * AFontSize / DPI_MULTIPLIER;
end;

procedure TRPPDFFont.InitData;
begin
  RvRenderPDF.ActiveStream := DataStream;
  with RvRenderPDF do begin
    PrintLnF('%d 0 obj',[GetID]);
    PrintLn('<<');
    PrintLn('/Type /Font');
    PrintLnF('/Subtype /%s',[FontType]);
    PrintLnF('/Name /%s',[ObjectName]);
    PrintLnF('/BaseFont /%s',[PDFFontName]);
    PrintLn('/Encoding /' + FontEncodingNames[FontEncoding]);
    PrintLn('>>');
    PrintLn('endobj');
    PrintLn;
    ActiveStream.Position := 0;
  end; { with }
end;

{ TRPPDFXObject }

constructor TRPPDFXObject.Create(AOwner: TObject);
begin
  inherited;
  FImageStream := TMemoryStream.Create;
  FImage := TJPEGImage.Create;
  FImage.CompressionQuality := 100;
end;

destructor TRPPDFXObject.Destroy;
begin
  inherited;
  FreeAndNil(FImageStream);
  FreeAndNil(FImage);
end;

function TRPPDFXObject.GetImageHeight: integer;
begin
  Result := FImage.Height;
end;

function TRPPDFXObject.GetImageWidth: integer;
begin
  Result := FImage.Width;
end;

procedure TRPPDFXObject.InitData;
begin
  RvRenderPDF.ActiveStream := DataStream;
  with RvRenderPDF do begin
    Image.CompressionQuality := RvRenderPDF.ImageQuality;
    Image.SaveToStream(ImageStream);
    ImageStream.Position := 0;
    PrintLnF('%d 0 obj',[GetID]);
    PrintLn('<<');
    PrintLn('/Type /XObject');
    PrintLn('/Subtype /Image');
    PrintLnF('/Name /%s',[ObjectName]);
    PrintLnF('/Width %d /Height %d ',[Width,Height]);
    PrintLn('/BitsPerComponent 8');
    PrintLnF('/ColorSpace /DeviceRGB /Length %d /Filter [/DCTDecode ]',[ImageStream.Size]);
    PrintLn('>>');
    PrintLn('stream');
    ActiveStream.CopyFrom(ImageStream, ImageStream.Size);
    PrintLn('endstream');
    PrintLn('endobj');
    PrintLn;
    ActiveStream.Position := 0;
  end; { with }
end;

end.