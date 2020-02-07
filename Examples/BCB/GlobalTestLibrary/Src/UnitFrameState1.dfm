object FrameState1: TFrameState1
  Left = 0
  Top = 0
  Width = 320
  Height = 583
  TabOrder = 0
  object Label1: TLabel
    AlignWithMargins = True
    Left = 3
    Top = 28
    Width = 314
    Height = 13
    Hint = 'Out.Invoke.Int'
    Align = alTop
    Caption = 'Out.Invoke.Int'
    ExplicitWidth = 73
  end
  object Label2: TLabel
    AlignWithMargins = True
    Left = 3
    Top = 47
    Width = 314
    Height = 13
    Hint = 'Out.Invoke.Double'
    Align = alTop
    Caption = 'Out.Invoke.Double'
    ExplicitWidth = 92
  end
  object Label3: TLabel
    AlignWithMargins = True
    Left = 3
    Top = 66
    Width = 314
    Height = 13
    Hint = 'Out.Invoke.String'
    Align = alTop
    Caption = 'Out.Invoke.String'
    ExplicitWidth = 87
  end
  object Label4: TLabel
    AlignWithMargins = True
    Left = 3
    Top = 3
    Width = 314
    Height = 19
    Align = alTop
    Alignment = taCenter
    Caption = 'INVOKED SESSION TEST'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    ExplicitWidth = 198
  end
  object Label5: TLabel
    AlignWithMargins = True
    Left = 3
    Top = 85
    Width = 314
    Height = 13
    Hint = 'Out.Invoke.TypeCheck'
    Align = alTop
    Caption = 'Out.Invoke.TypeCheck: This Must Not Change!'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsItalic]
    ParentFont = False
    ExplicitWidth = 228
  end
  object Label6: TLabel
    AlignWithMargins = True
    Left = 3
    Top = 322
    Width = 314
    Height = 19
    Align = alTop
    Alignment = taCenter
    Caption = 'INVOKED SESSION EVENT'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    ExplicitWidth = 210
  end
  object Memo1: TMemo
    AlignWithMargins = True
    Left = 3
    Top = 104
    Width = 314
    Height = 212
    Hint = 'Out.Invoke.Table'
    Align = alTop
    Lines.Strings = (
      'Out.Invoke.Table')
    TabOrder = 0
    ExplicitLeft = 72
    ExplicitTop = 80
    ExplicitWidth = 185
    ExplicitHeight = 89
  end
  object MemoIOTimerSignal: TMemo
    AlignWithMargins = True
    Left = 3
    Top = 347
    Width = 314
    Height = 233
    Align = alClient
    Lines.Strings = (
      'IO.Timer.Signal')
    TabOrder = 1
    ExplicitLeft = 6
    ExplicitTop = 112
    ExplicitHeight = 212
  end
end
