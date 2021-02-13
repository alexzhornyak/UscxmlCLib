object FormCalculator: TFormCalculator
  Left = 0
  Top = 0
  AutoSize = True
  Caption = 'Calculator'
  ClientHeight = 315
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
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -21
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      Layout = tlCenter
      ExplicitLeft = 306
      ExplicitWidth = 12
      ExplicitHeight = 24
    end
  end
  object Panel2: TPanel
    AlignWithMargins = True
    Left = 3
    Top = 50
    Width = 330
    Height = 50
    Align = alTop
    BevelInner = bvLowered
    TabOrder = 1
    object Button1: TButton
      AlignWithMargins = True
      Left = 248
      Top = 3
      Width = 75
      Height = 44
      Hint = 'OPER.MINUS'
      Margins.Top = 1
      Margins.Bottom = 1
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
      ExplicitTop = 5
      ExplicitHeight = 31
    end
    object Button2: TButton
      AlignWithMargins = True
      Left = 167
      Top = 3
      Width = 75
      Height = 44
      Hint = 'OPER.PLUS'
      Margins.Top = 1
      Margins.Bottom = 1
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
      ExplicitTop = 5
      ExplicitHeight = 31
    end
    object Button3: TButton
      AlignWithMargins = True
      Left = 86
      Top = 3
      Width = 75
      Height = 44
      Hint = 'OPER.STAR'
      Margins.Top = 1
      Margins.Bottom = 1
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
      ExplicitTop = 5
      ExplicitHeight = 31
    end
    object Button4: TButton
      AlignWithMargins = True
      Left = 5
      Top = 3
      Width = 75
      Height = 44
      Hint = 'OPER.DIV'
      Margins.Top = 1
      Margins.Bottom = 1
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
      ExplicitTop = 5
      ExplicitHeight = 31
    end
  end
  object Panel3: TPanel
    AlignWithMargins = True
    Left = 3
    Top = 262
    Width = 330
    Height = 50
    Align = alTop
    BevelInner = bvLowered
    TabOrder = 2
    object Button14: TButton
      AlignWithMargins = True
      Left = 5
      Top = 3
      Width = 75
      Height = 44
      Margins.Top = 1
      Margins.Bottom = 1
      Align = alLeft
      Caption = '0'
      TabOrder = 0
      OnClick = ButtonClick
      ExplicitTop = 5
      ExplicitHeight = 31
    end
    object Button15: TButton
      AlignWithMargins = True
      Left = 86
      Top = 3
      Width = 75
      Height = 44
      Hint = 'POINT'
      Margins.Top = 1
      Margins.Bottom = 1
      Align = alLeft
      Caption = '.'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 1
      OnClick = ButtonClick
      ExplicitTop = 5
      ExplicitHeight = 31
    end
    object Button16: TButton
      AlignWithMargins = True
      Left = 248
      Top = 3
      Width = 75
      Height = 44
      Hint = 'EQUALS'
      Margins.Top = 1
      Margins.Bottom = 1
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
      ExplicitTop = 5
      ExplicitHeight = 31
    end
    object Button17: TButton
      AlignWithMargins = True
      Left = 167
      Top = 3
      Width = 75
      Height = 44
      Hint = 'C'
      Margins.Top = 1
      Margins.Bottom = 1
      Align = alLeft
      Caption = 'C'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 3
      OnClick = ButtonClick
      ExplicitTop = 5
      ExplicitHeight = 31
    end
  end
  object Panel4: TPanel
    AlignWithMargins = True
    Left = 3
    Top = 208
    Width = 330
    Height = 50
    Margins.Top = 1
    Margins.Bottom = 1
    Align = alTop
    BevelInner = bvLowered
    TabOrder = 3
    object Button11: TButton
      AlignWithMargins = True
      Left = 5
      Top = 3
      Width = 75
      Height = 44
      Margins.Top = 1
      Margins.Bottom = 1
      Align = alLeft
      Caption = '1'
      TabOrder = 0
      OnClick = ButtonClick
      ExplicitTop = 5
      ExplicitHeight = 31
    end
    object Button12: TButton
      AlignWithMargins = True
      Left = 86
      Top = 3
      Width = 75
      Height = 44
      Margins.Top = 1
      Margins.Bottom = 1
      Align = alLeft
      Caption = '2'
      TabOrder = 1
      OnClick = ButtonClick
      ExplicitTop = 5
      ExplicitHeight = 31
    end
    object Button13: TButton
      AlignWithMargins = True
      Left = 167
      Top = 3
      Width = 75
      Height = 44
      Margins.Top = 1
      Margins.Bottom = 1
      Align = alLeft
      Caption = '3'
      TabOrder = 2
      OnClick = ButtonClick
      ExplicitTop = 5
      ExplicitHeight = 31
    end
  end
  object Panel5: TPanel
    AlignWithMargins = True
    Left = 3
    Top = 156
    Width = 330
    Height = 50
    Margins.Top = 1
    Margins.Bottom = 1
    Align = alTop
    BevelInner = bvLowered
    TabOrder = 4
    object Button8: TButton
      AlignWithMargins = True
      Left = 5
      Top = 3
      Width = 75
      Height = 44
      Margins.Top = 1
      Margins.Bottom = 1
      Align = alLeft
      Caption = '4'
      TabOrder = 0
      OnClick = ButtonClick
      ExplicitTop = 5
      ExplicitHeight = 31
    end
    object Button9: TButton
      AlignWithMargins = True
      Left = 86
      Top = 3
      Width = 75
      Height = 44
      Margins.Top = 1
      Margins.Bottom = 1
      Align = alLeft
      Caption = '5'
      TabOrder = 1
      OnClick = ButtonClick
      ExplicitTop = 5
      ExplicitHeight = 31
    end
    object Button10: TButton
      AlignWithMargins = True
      Left = 167
      Top = 3
      Width = 75
      Height = 44
      Margins.Top = 1
      Margins.Bottom = 1
      Align = alLeft
      Caption = '6'
      TabOrder = 2
      OnClick = ButtonClick
      ExplicitTop = 5
      ExplicitHeight = 31
    end
  end
  object Panel6: TPanel
    AlignWithMargins = True
    Left = 3
    Top = 104
    Width = 330
    Height = 50
    Margins.Top = 1
    Margins.Bottom = 1
    Align = alTop
    BevelInner = bvLowered
    TabOrder = 5
    object Button5: TButton
      AlignWithMargins = True
      Left = 5
      Top = 3
      Width = 75
      Height = 44
      Margins.Top = 1
      Margins.Bottom = 1
      Align = alLeft
      Caption = '7'
      TabOrder = 0
      OnClick = ButtonClick
      ExplicitTop = 5
      ExplicitHeight = 31
    end
    object Button6: TButton
      AlignWithMargins = True
      Left = 86
      Top = 3
      Width = 75
      Height = 44
      Margins.Top = 1
      Margins.Bottom = 1
      Align = alLeft
      Caption = '8'
      TabOrder = 1
      OnClick = ButtonClick
      ExplicitTop = 5
      ExplicitHeight = 31
    end
    object Button7: TButton
      AlignWithMargins = True
      Left = 167
      Top = 3
      Width = 75
      Height = 44
      Margins.Top = 1
      Margins.Bottom = 1
      Align = alLeft
      Caption = '9'
      TabOrder = 2
      OnClick = ButtonClick
      ExplicitTop = 5
      ExplicitHeight = 31
    end
  end
  object TrayIcon1: TTrayIcon
    Visible = True
    Left = 24
    Top = 8
  end
end
