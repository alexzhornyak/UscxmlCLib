object FrameState0: TFrameState0
  Left = 0
  Top = 0
  Width = 509
  Height = 467
  TabOrder = 0
  object Label1: TLabel
    AlignWithMargins = True
    Left = 3
    Top = 3
    Width = 503
    Height = 13
    Align = alTop
    Alignment = taCenter
    Caption = 'USER INPUTS'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    ExplicitWidth = 73
  end
  object Label2: TLabel
    AlignWithMargins = True
    Left = 3
    Top = 263
    Width = 503
    Height = 13
    Align = alTop
    Alignment = taCenter
    Caption = 'STATE MACHINE RESPOND'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    ExplicitWidth = 143
  end
  object TreeViewRespond: TTreeView
    AlignWithMargins = True
    Left = 3
    Top = 282
    Width = 503
    Height = 182
    Align = alClient
    Indent = 19
    TabOrder = 0
  end
  object Panel1: TPanel
    AlignWithMargins = True
    Left = 3
    Top = 22
    Width = 503
    Height = 235
    Align = alTop
    Caption = 'Panel1'
    ShowCaption = False
    TabOrder = 1
    object CheckBox1: TCheckBox
      AlignWithMargins = True
      Left = 4
      Top = 4
      Width = 495
      Height = 17
      Align = alTop
      Caption = 'Click me'
      TabOrder = 0
      OnClick = OnDataChange
    end
    object TrackBar1: TTrackBar
      AlignWithMargins = True
      Left = 4
      Top = 27
      Width = 495
      Height = 45
      Align = alTop
      TabOrder = 1
      OnChange = OnDataChange
    end
    object Edit1: TEdit
      AlignWithMargins = True
      Left = 4
      Top = 78
      Width = 495
      Height = 21
      Align = alTop
      TabOrder = 2
      Text = 'Type here'
      OnChange = OnDataChange
    end
    object TreeView1: TTreeView
      AlignWithMargins = True
      Left = 4
      Top = 105
      Width = 495
      Height = 126
      Align = alClient
      Indent = 19
      TabOrder = 3
      OnEdited = TreeView1Edited
      Items.NodeData = {
        0304000000200000000000000000000000FFFFFFFFFFFFFFFF00000000000000
        000000000001013100260000000000000000000000FFFFFFFFFFFFFFFF000000
        000000000000000000010432002E0032003200300000000000000000000000FF
        FFFFFFFFFFFFFF00000000000000000000000001094300680061006E00670065
        0020006D006500240000000000000000000000FFFFFFFFFFFFFFFF0000000000
        000000030000000103530075006200280000000000000000000000FFFFFFFFFF
        FFFFFF00000000000000000000000001055300750062002E0031002800000000
        00000000000000FFFFFFFFFFFFFFFF0000000000000000000000000105530075
        0062002E003200280000000000000000000000FFFFFFFFFFFFFFFF0000000000
        0000000000000001055300750062002E003300}
    end
  end
end
