object FormTrafficLight: TFormTrafficLight
  Left = 0
  Top = 0
  Caption = 'TrafficLight'
  ClientHeight = 292
  ClientWidth = 222
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
    Left = 64
    Top = 8
    Width = 97
    Height = 233
  end
  object ShapeRed: TShape
    Left = 80
    Top = 16
    Width = 65
    Height = 65
    Brush.Color = clGray
    Shape = stCircle
  end
  object ShapeYellow: TShape
    Left = 80
    Top = 87
    Width = 65
    Height = 65
    Brush.Color = clGray
    Shape = stCircle
  end
  object ShapeGreen: TShape
    Left = 80
    Top = 158
    Width = 65
    Height = 65
    Brush.Color = clGray
    Shape = stCircle
  end
  object CheckNightMode: TCheckBox
    Left = 64
    Top = 256
    Width = 97
    Height = 17
    Caption = 'Night Mode'
    TabOrder = 0
    OnClick = CheckNightModeClick
  end
  object TrayIcon1: TTrayIcon
    Visible = True
    Left = 176
    Top = 8
  end
end
