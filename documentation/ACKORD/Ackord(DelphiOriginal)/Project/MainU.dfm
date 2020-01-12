object MainF: TMainF
  Left = 160
  Top = 147
  Width = 484
  Height = 198
  Caption = 'Chord analyzer 0.1. (C) David Henningsson 2004, freeware.'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnKeyDown = FormKeyDown
  PixelsPerInch = 96
  TextHeight = 13
  object AckordPanel: TPanel
    Left = 0
    Top = 0
    Width = 476
    Height = 164
    Align = alClient
    Color = clBlue
    Font.Charset = ANSI_CHARSET
    Font.Color = clYellow
    Font.Height = -96
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    OnResize = AckordPanelResize
    DesignSize = (
      476
      164)
    object SettingsBtn: TButton
      Left = 8
      Top = 142
      Width = 75
      Height = 17
      Anchors = [akLeft, akBottom]
      Caption = 'Settings...'
      Font.Charset = ANSI_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'Microsoft Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      OnClick = SettingsBtnClick
    end
  end
end
