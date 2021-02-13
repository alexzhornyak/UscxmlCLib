object FormCalculator: TFormCalculator
  Left = 0
  Top = 0
  Caption = 'Calculator'
  ClientHeight = 282
  ClientWidth = 336
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -16
  Font.Name = 'Arial'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 18
  object Panel1: TPanel
    AlignWithMargins = True
    Left = 3
    Top = 3
    Width = 330
    Height = 41
    Align = alTop
    BevelInner = bvLowered
    TabOrder = 0
    object LabelDisplay: TLabel
      AlignWithMargins = True
      Left = 5
      Top = 5
      Width = 313
      Height = 31
      Margins.Right = 10
      Align = alClient
      Alignment = taRightJustify
      Caption = '0'
      Layout = tlCenter
      ExplicitLeft = 309
      ExplicitWidth = 9
      ExplicitHeight = 18
    end
  end
  object Panel2: TPanel
    AlignWithMargins = True
    Left = 3
    Top = 50
    Width = 330
    Height = 41
    Align = alTop
    BevelInner = bvLowered
    TabOrder = 1
    object Button1: TButton
      AlignWithMargins = True
      Left = 248
      Top = 5
      Width = 75
      Height = 31
      Hint = 'OPER.MINUS'
      Align = alLeft
      Caption = '-'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 0
      OnClick = ButtonClick
    end
    object Button2: TButton
      AlignWithMargins = True
      Left = 167
      Top = 5
      Width = 75
      Height = 31
      Hint = 'OPER.PLUS'
      Align = alLeft
      Caption = '+'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 1
      OnClick = ButtonClick
    end
    object Button3: TButton
      AlignWithMargins = True
      Left = 86
      Top = 5
      Width = 75
      Height = 31
      Hint = 'OPER.STAR'
      Align = alLeft
      Caption = #215
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 2
      OnClick = ButtonClick
    end
    object Button4: TButton
      AlignWithMargins = True
      Left = 5
      Top = 5
      Width = 75
      Height = 31
      Hint = 'OPER.DIV'
      Align = alLeft
      Caption = #247
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 3
      OnClick = ButtonClick
    end
  end
  object Panel3: TPanel
    AlignWithMargins = True
    Left = 3
    Top = 238
    Width = 330
    Height = 41
    Align = alTop
    BevelInner = bvLowered
    TabOrder = 2
    object Button14: TButton
      AlignWithMargins = True
      Left = 5
      Top = 5
      Width = 75
      Height = 31
      Align = alLeft
      Caption = '0'
      TabOrder = 0
      OnClick = ButtonClick
    end
    object Button15: TButton
      AlignWithMargins = True
      Left = 86
      Top = 5
      Width = 75
      Height = 31
      Hint = 'POINT'
      Align = alLeft
      Caption = '.'
      TabOrder = 1
      OnClick = ButtonClick
    end
    object Button16: TButton
      AlignWithMargins = True
      Left = 248
      Top = 5
      Width = 75
      Height = 31
      Hint = 'EQUALS'
      Align = alLeft
      Caption = '='
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 2
      OnClick = ButtonClick
    end
    object Button17: TButton
      AlignWithMargins = True
      Left = 167
      Top = 5
      Width = 75
      Height = 31
      Hint = 'C'
      Align = alLeft
      Caption = 'C'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      TabOrder = 3
      OnClick = ButtonClick
    end
  end
  object Panel4: TPanel
    AlignWithMargins = True
    Left = 3
    Top = 191
    Width = 330
    Height = 41
    Align = alTop
    BevelInner = bvLowered
    TabOrder = 3
    object Button11: TButton
      AlignWithMargins = True
      Left = 5
      Top = 5
      Width = 75
      Height = 31
      Align = alLeft
      Caption = '1'
      TabOrder = 0
      OnClick = ButtonClick
    end
    object Button12: TButton
      AlignWithMargins = True
      Left = 86
      Top = 5
      Width = 75
      Height = 31
      Align = alLeft
      Caption = '2'
      TabOrder = 1
      OnClick = ButtonClick
    end
    object Button13: TButton
      AlignWithMargins = True
      Left = 167
      Top = 5
      Width = 75
      Height = 31
      Align = alLeft
      Caption = '3'
      TabOrder = 2
      OnClick = ButtonClick
    end
  end
  object Panel5: TPanel
    AlignWithMargins = True
    Left = 3
    Top = 144
    Width = 330
    Height = 41
    Align = alTop
    BevelInner = bvLowered
    TabOrder = 4
    object Button8: TButton
      AlignWithMargins = True
      Left = 5
      Top = 5
      Width = 75
      Height = 31
      Align = alLeft
      Caption = '4'
      TabOrder = 0
      OnClick = ButtonClick
    end
    object Button9: TButton
      AlignWithMargins = True
      Left = 86
      Top = 5
      Width = 75
      Height = 31
      Align = alLeft
      Caption = '5'
      TabOrder = 1
      OnClick = ButtonClick
    end
    object Button10: TButton
      AlignWithMargins = True
      Left = 167
      Top = 5
      Width = 75
      Height = 31
      Align = alLeft
      Caption = '6'
      TabOrder = 2
      OnClick = ButtonClick
    end
  end
  object Panel6: TPanel
    AlignWithMargins = True
    Left = 3
    Top = 97
    Width = 330
    Height = 41
    Align = alTop
    BevelInner = bvLowered
    TabOrder = 5
    object Button5: TButton
      AlignWithMargins = True
      Left = 5
      Top = 5
      Width = 75
      Height = 31
      Align = alLeft
      Caption = '7'
      TabOrder = 0
      OnClick = ButtonClick
    end
    object Button6: TButton
      AlignWithMargins = True
      Left = 86
      Top = 5
      Width = 75
      Height = 31
      Align = alLeft
      Caption = '8'
      TabOrder = 1
      OnClick = ButtonClick
    end
    object Button7: TButton
      AlignWithMargins = True
      Left = 167
      Top = 5
      Width = 75
      Height = 31
      Align = alLeft
      Caption = '9'
      TabOrder = 2
      OnClick = ButtonClick
    end
  end
  object TrayIcon1: TTrayIcon
    Visible = True
    Left = 24
    Top = 8
  end
end
