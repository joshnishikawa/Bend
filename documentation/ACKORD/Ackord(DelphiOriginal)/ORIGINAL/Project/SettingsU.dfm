object SettingsF: TSettingsF
  Left = 267
  Top = 217
  Width = 389
  Height = 418
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
    381
    384)
  PixelsPerInch = 96
  TextHeight = 13
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
  object OkBtn: TButton
    Left = 72
    Top = 346
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
    Top = 346
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Cancel = True
    Caption = '&Cancel'
    ModalResult = 2
    TabOrder = 1
    OnClick = CancelBtnClick
  end
  object MidiInputsLB: TCheckListBox
    Left = 32
    Top = 72
    Width = 313
    Height = 113
    ItemHeight = 13
    TabOrder = 2
  end
  object IgnoreSustainCB: TCheckBox
    Left = 32
    Top = 16
    Width = 193
    Height = 17
    Caption = 'Ignore sustain pedal when analyzing'
    TabOrder = 3
  end
  object MidiThruLB: TCheckListBox
    Left = 32
    Top = 216
    Width = 313
    Height = 113
    OnClickCheck = MidiThruLBClickCheck
    ItemHeight = 13
    TabOrder = 4
  end
end
