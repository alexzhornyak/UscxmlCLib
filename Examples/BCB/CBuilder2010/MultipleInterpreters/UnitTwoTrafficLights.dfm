object FormTrafficLight: TFormTrafficLight
  Left = 0
  Top = 0
  Caption = 'TrafficLight'
  ClientHeight = 375
  ClientWidth = 350
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Shape4: TShape
    Left = 24
    Top = 40
    Width = 97
    Height = 233
  end
  object ShapeRed: TShape
    Left = 40
    Top = 48
    Width = 65
    Height = 65
    Brush.Color = clGray
    Shape = stCircle
  end
  object ShapeYellow: TShape
    Left = 40
    Top = 119
    Width = 65
    Height = 65
    Brush.Color = clGray
    Shape = stCircle
  end
  object ShapeGreen: TShape
    Left = 40
    Top = 190
    Width = 65
    Height = 65
    Brush.Color = clGray
    Shape = stCircle
  end
  object Label1: TLabel
    Left = 24
    Top = 8
    Width = 99
    Height = 19
    Caption = 'Traffic Light 1'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label2: TLabel
    Left = 232
    Top = 8
    Width = 99
    Height = 19
    Caption = 'Traffic Light 2'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Shape1: TShape
    Left = 232
    Top = 40
    Width = 97
    Height = 233
  end
  object ShapeRed2: TShape
    Left = 248
    Top = 48
    Width = 65
    Height = 65
    Brush.Color = clGray
    Shape = stCircle
  end
  object ShapeYellow2: TShape
    Left = 248
    Top = 119
    Width = 65
    Height = 65
    Brush.Color = clGray
    Shape = stCircle
  end
  object ShapeGreen2: TShape
    Left = 248
    Top = 190
    Width = 65
    Height = 65
    Brush.Color = clGray
    Shape = stCircle
  end
  object CheckNightMode: TCheckBox
    Left = 24
    Top = 288
    Width = 97
    Height = 17
    Caption = 'Night Mode 1'
    TabOrder = 0
    OnClick = CheckNightModeClick
  end
  object CheckNightMode2: TCheckBox
    Left = 232
    Top = 288
    Width = 97
    Height = 17
    Caption = 'Night Mode 2'
    TabOrder = 1
    OnClick = CheckNightMode2Click
  end
  object CheckPause1: TCheckBox
    Left = 24
    Top = 350
    Width = 129
    Height = 17
    Caption = 'Pause 1'
    TabOrder = 2
    OnClick = CheckPause1Click
  end
  object CheckPause2: TCheckBox
    Left = 232
    Top = 350
    Width = 97
    Height = 17
    Caption = 'Pause 2'
    TabOrder = 3
    OnClick = CheckPause2Click
  end
  object TrayIcon1: TTrayIcon
    Visible = True
    Left = 160
    Top = 136
  end
end
