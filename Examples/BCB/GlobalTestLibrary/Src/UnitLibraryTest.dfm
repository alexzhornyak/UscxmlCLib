object FormGlobalLibraryTest: TFormGlobalLibraryTest
  Left = 0
  Top = 0
  Caption = 'Global Library Test'
  ClientHeight = 745
  ClientWidth = 841
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Splitter1: TSplitter
    Left = 0
    Top = 546
    Width = 841
    Height = 3
    Cursor = crVSplit
    Align = alBottom
    ExplicitTop = 0
    ExplicitWidth = 416
  end
  object RichEdit1: TRichEdit
    AlignWithMargins = True
    Left = 3
    Top = 552
    Width = 835
    Height = 190
    Align = alBottom
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    HideScrollBars = False
    ParentFont = False
    ScrollBars = ssBoth
    TabOrder = 0
    WordWrap = False
  end
  object PanelInstruments: TPanel
    Left = 0
    Top = 0
    Width = 841
    Height = 41
    Align = alTop
    Caption = 'PanelInstruments'
    ShowCaption = False
    TabOrder = 1
    object CheckInitTesting: TCheckBox
      AlignWithMargins = True
      Left = 4
      Top = 4
      Width = 205
      Height = 33
      Align = alLeft
      Caption = 'INIT TESTING'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      OnClick = CheckInitTestingClick
    end
    object ComboModes: TComboBox
      AlignWithMargins = True
      Left = 215
      Top = 4
      Width = 170
      Height = 31
      Align = alLeft
      Style = csDropDownList
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ItemIndex = 0
      ParentFont = False
      TabOrder = 1
      Text = 'State 0'
      Visible = False
      OnChange = ComboModesChange
      Items.Strings = (
        'State 0'
        'State 1'
        'State 2')
    end
    object BtnCheckCyrillic: TButton
      AlignWithMargins = True
      Left = 391
      Top = 4
      Width = 90
      Height = 33
      Align = alLeft
      Caption = 'Check Cyrillic'
      TabOrder = 2
      Visible = False
      OnClick = BtnCheckCyrillicClick
    end
    object CheckPause: TCheckBox
      AlignWithMargins = True
      Left = 487
      Top = 4
      Width = 98
      Height = 33
      Align = alLeft
      Caption = 'PAUSE'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 3
      Visible = False
      OnClick = CheckPauseClick
    end
  end
  object PanelState2: TPanel
    Left = 424
    Top = 41
    Width = 417
    Height = 505
    Align = alRight
    Caption = 'PanelState2'
    ShowCaption = False
    TabOrder = 2
    object Label1: TLabel
      AlignWithMargins = True
      Left = 4
      Top = 29
      Width = 409
      Height = 13
      Hint = 'Out.Main.Int'
      Align = alTop
      Caption = 'Out.Main.Int'
      ExplicitWidth = 62
    end
    object Label2: TLabel
      AlignWithMargins = True
      Left = 4
      Top = 48
      Width = 409
      Height = 13
      Hint = 'Out.Main.Double'
      Align = alTop
      Caption = 'Out.Main.Double'
      ExplicitWidth = 81
    end
    object Label3: TLabel
      AlignWithMargins = True
      Left = 4
      Top = 67
      Width = 409
      Height = 13
      Hint = 'Out.Main.String'
      Align = alTop
      Caption = 'Out.Main.String'
      ExplicitWidth = 76
    end
    object Label4: TLabel
      AlignWithMargins = True
      Left = 4
      Top = 4
      Width = 409
      Height = 19
      Align = alTop
      Alignment = taCenter
      Caption = 'MAIN SESSION GLOBAL DATA TEST'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      ExplicitWidth = 290
    end
    object Label5: TLabel
      AlignWithMargins = True
      Left = 4
      Top = 86
      Width = 409
      Height = 13
      Hint = 'Out.Main.TypeCheck'
      Align = alTop
      Caption = 'Out.Main.TypeCheck: This Must Not Change!'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsItalic]
      ParentFont = False
      ExplicitWidth = 217
    end
    object Memo1: TMemo
      AlignWithMargins = True
      Left = 4
      Top = 105
      Width = 409
      Height = 396
      Hint = 'Out.Main.Table'
      Align = alClient
      Lines.Strings = (
        'Out.Main.Table')
      TabOrder = 0
    end
  end
end
