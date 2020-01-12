object SettingsF: TSettingsF
  Left = 267
  Top = 217
  Width = 437
  Height = 485
  Caption = 'Settings...'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnCreate = FormCreate
  DesignSize = (
    429
    451)
  PixelsPerInch = 96
  TextHeight = 13
  object OkBtn: TButton
    Left = 72
    Top = 413
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Caption = '&OK'
    Default = True
    ModalResult = 1
    TabOrder = 0
    OnClick = OkBtnClick
  end
  object CancelBtn: TButton
    Left = 216
    Top = 413
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Cancel = True
    Caption = '&Cancel'
    ModalResult = 2
    TabOrder = 1
    OnClick = CancelBtnClick
  end
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 429
    Height = 385
    ActivePage = MidiIOTS
    Align = alTop
    TabIndex = 0
    TabOrder = 2
    object MidiIOTS: TTabSheet
      Caption = 'Midi I/O'
      object Label1: TLabel
        Left = 32
        Top = 48
        Width = 121
        Height = 13
        Caption = 'Analyze these midi inputs:'
      end
      object Label2: TLabel
        Left = 32
        Top = 200
        Width = 200
        Height = 13
        Caption = 'Midi thru to one of these outputs (or none):'
      end
      object MidiInputsLB: TCheckListBox
        Left = 32
        Top = 72
        Width = 313
        Height = 113
        ItemHeight = 13
        TabOrder = 0
      end
      object IgnoreSustainCB: TCheckBox
        Left = 32
        Top = 16
        Width = 193
        Height = 17
        Caption = 'Ignore sustain pedal when analyzing'
        TabOrder = 1
      end
      object MidiThruLB: TCheckListBox
        Left = 32
        Top = 220
        Width = 313
        Height = 113
        OnClickCheck = MidiThruLBClickCheck
        ItemHeight = 13
        TabOrder = 2
      end
    end
    object ColorsTS: TTabSheet
      Caption = 'Colors'
      ImageIndex = 1
      object Panel1: TPanel
        Left = 8
        Top = 32
        Width = 161
        Height = 41
        Caption = 'C7#5b9'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -19
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
      end
      object Panel2: TPanel
        Left = 8
        Top = 88
        Width = 161
        Height = 41
        Caption = 'C, C#, Ab, Bb'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -19
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
      end
      object BgColorBtn: TButton
        Left = 200
        Top = 16
        Width = 161
        Height = 33
        Caption = 'Background color'
        TabOrder = 2
        OnClick = UnrecBtnClick
      end
      object RecogBtn: TButton
        Left = 200
        Top = 64
        Width = 161
        Height = 33
        Caption = 'Recognized chord color'
        TabOrder = 3
        OnClick = UnrecBtnClick
      end
      object UnrecBtn: TButton
        Left = 200
        Top = 112
        Width = 161
        Height = 33
        Caption = 'Unrecognized chord color'
        TabOrder = 4
        OnClick = UnrecBtnClick
      end
    end
  end
  object ColorDlg: TColorDialog
    Ctl3D = True
    Left = 168
    Top = 184
  end
end
