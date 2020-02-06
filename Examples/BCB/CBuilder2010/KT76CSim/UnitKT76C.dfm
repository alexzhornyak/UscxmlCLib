object FormKT76C: TFormKT76C
  Left = 0
  Top = 0
  Caption = 'KT76C Simulator'
  ClientHeight = 246
  ClientWidth = 735
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object SpeedButton1: TSpeedButton
    Left = 8
    Top = 8
    Width = 57
    Height = 33
    Hint = 'Inp.KT76.Button.IDT'
    Caption = 'IDT'
    OnMouseDown = SpeedButton1MouseDown
    OnMouseUp = SpeedButton1MouseUp
  end
  object BtnSupply: TSpeedButton
    Left = 8
    Top = 192
    Width = 89
    Height = 33
    Hint = 'Inp.KT76.PowerOn'
    AllowAllUp = True
    GroupIndex = 1
    Caption = 'SUPPLY'
    OnClick = BtnSupplyClick
  end
  object SpeedButton3: TSpeedButton
    Left = 141
    Top = 192
    Width = 44
    Height = 41
    Hint = 'Inp.KT76.Button.0'
    Caption = '0'
    OnMouseDown = SpeedButton1MouseDown
    OnMouseUp = SpeedButton1MouseUp
  end
  object SpeedButton4: TSpeedButton
    Left = 191
    Top = 192
    Width = 42
    Height = 41
    Hint = 'Inp.KT76.Button.1'
    Caption = '1'
    OnMouseDown = SpeedButton1MouseDown
    OnMouseUp = SpeedButton1MouseUp
  end
  object SpeedButton5: TSpeedButton
    Left = 239
    Top = 192
    Width = 42
    Height = 41
    Hint = 'Inp.KT76.Button.2'
    Caption = '2'
    OnMouseDown = SpeedButton1MouseDown
    OnMouseUp = SpeedButton1MouseUp
  end
  object SpeedButton6: TSpeedButton
    Left = 287
    Top = 192
    Width = 42
    Height = 41
    Hint = 'Inp.KT76.Button.3'
    Caption = '3'
    OnMouseDown = SpeedButton1MouseDown
    OnMouseUp = SpeedButton1MouseUp
  end
  object SpeedButton7: TSpeedButton
    Left = 335
    Top = 192
    Width = 42
    Height = 41
    Hint = 'Inp.KT76.Button.4'
    Caption = '4'
    OnMouseDown = SpeedButton1MouseDown
    OnMouseUp = SpeedButton1MouseUp
  end
  object SpeedButton8: TSpeedButton
    Left = 383
    Top = 192
    Width = 42
    Height = 41
    Hint = 'Inp.KT76.Button.5'
    Caption = '5'
    OnMouseDown = SpeedButton1MouseDown
    OnMouseUp = SpeedButton1MouseUp
  end
  object SpeedButton9: TSpeedButton
    Left = 431
    Top = 192
    Width = 42
    Height = 41
    Hint = 'Inp.KT76.Button.6'
    Caption = '6'
    OnMouseDown = SpeedButton1MouseDown
    OnMouseUp = SpeedButton1MouseUp
  end
  object SpeedButton10: TSpeedButton
    Left = 479
    Top = 192
    Width = 42
    Height = 41
    Hint = 'Inp.KT76.Button.7'
    Caption = '7'
    OnMouseDown = SpeedButton1MouseDown
    OnMouseUp = SpeedButton1MouseUp
  end
  object SpeedButton11: TSpeedButton
    Left = 600
    Top = 192
    Width = 57
    Height = 33
    Hint = 'Inp.KT76.Button.CLR'
    Caption = 'CLR'
    OnMouseDown = SpeedButton1MouseDown
    OnMouseUp = SpeedButton1MouseUp
  end
  object SpeedButton12: TSpeedButton
    Left = 670
    Top = 192
    Width = 57
    Height = 33
    Hint = 'Inp.KT76.Button.VFR'
    Caption = 'VFR'
    OnMouseDown = SpeedButton1MouseDown
    OnMouseUp = SpeedButton1MouseUp
  end
  object Label1: TLabel
    Left = 691
    Top = 13
    Width = 20
    Height = 13
    Caption = 'OFF'
  end
  object Label2: TLabel
    Left = 691
    Top = 45
    Width = 18
    Height = 13
    Caption = 'SBY'
  end
  object Label3: TLabel
    Left = 691
    Top = 76
    Width = 18
    Height = 13
    Caption = 'TST'
  end
  object Label4: TLabel
    Left = 691
    Top = 109
    Width = 15
    Height = 13
    Caption = 'ON'
  end
  object Label5: TLabel
    Left = 691
    Top = 146
    Width = 18
    Height = 13
    Caption = 'ALT'
  end
  object TrackBar1: TTrackBar
    Left = 640
    Top = 8
    Width = 45
    Height = 151
    Hint = 'Inp.KT76.SwmMode'
    Max = 4
    Orientation = trVertical
    TabOrder = 0
    OnChange = TrackBar1Change
  end
  object Panel1: TPanel
    Left = 141
    Top = 18
    Width = 493
    Height = 138
    Caption = 'Panel1'
    Color = clBlack
    ParentBackground = False
    ShowCaption = False
    TabOrder = 1
    object Label6: TLabel
      Tag = 1
      Left = 10
      Top = 20
      Width = 33
      Height = 49
      Hint = 'Out.KT76.Label.Minus'
      AutoSize = False
      Caption = '-'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clYellow
      Font.Height = -53
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label7: TLabel
      Tag = 1
      Left = 10
      Top = 94
      Width = 33
      Height = 36
      Hint = 'Out.KT76.Label.FL'
      AutoSize = False
      Caption = 'FL'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clYellow
      Font.Height = -27
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label8: TLabel
      Left = 39
      Top = 30
      Width = 122
      Height = 65
      Hint = 'Out.KT76.Str.Left'
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'HHH'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clYellow
      Font.Height = -53
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label9: TLabel
      Tag = 1
      Left = 167
      Top = 20
      Width = 50
      Height = 37
      Hint = 'Out.KT76.Label.ALT'
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'ALT'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clYellow
      Font.Height = -27
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label10: TLabel
      Tag = 1
      Left = 223
      Top = 52
      Width = 50
      Height = 37
      Hint = 'Out.KT76.Label.SBY'
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'SBY'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clYellow
      Font.Height = -27
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label11: TLabel
      Tag = 1
      Left = 223
      Top = 20
      Width = 50
      Height = 37
      Hint = 'Out.KT76.Label.ON'
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'ON'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clYellow
      Font.Height = -27
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label12: TLabel
      Tag = 1
      Left = 288
      Top = 20
      Width = 17
      Height = 37
      Hint = 'Out.KT76.Label.R'
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'R'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clYellow
      Font.Height = -27
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label13: TLabel
      Left = 311
      Top = 26
      Width = 154
      Height = 65
      Hint = 'Out.KT76.Str.Right'
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'HHHH'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clYellow
      Font.Height = -53
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label14: TLabel
      Tag = 1
      Left = 317
      Top = 81
      Width = 154
      Height = 47
      Hint = 'Out.KT76.Markers'
      Alignment = taRightJustify
      AutoSize = False
      Caption = '^^^^'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clYellow
      Font.Height = -53
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
  end
  object TrayIcon1: TTrayIcon
    Visible = True
    Left = 16
    Top = 56
  end
end