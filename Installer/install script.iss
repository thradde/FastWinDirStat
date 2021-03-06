; Script generated by the Inno Script Studio Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "Fast WinDirStat"
#define MyExeName "windirstat"

[Setup]
AppMutex=IS$$Mutex$$FastWindirStat
AppName={#MyAppName}
AppVersion=2.0
AppCopyright=Copyright (c) 2021 IDEAL Software GmbH
PrivilegesRequired=none
AppId={{2BF4D9E6-8C8A-4E91-8F36-64B338B57A40}
LicenseFile=..\make\windirstat\res\license.txt
SolidCompression=True
MinVersion=6.0
SetupIconFile=..\make\windirstat\res\icon1.ico
DefaultGroupName={#MyAppName}
DefaultDirName={pf}\{#MyAppName}
UninstallDisplayIcon={app}\{#MyExeName}.exe
OutputBaseFilename={#MyAppName} Setup
AppPublisher=IDEAL Software GmbH
AppPublisherURL=www.idealsoftware.com
AppSupportURL=www.idealsoftware.com
AppUpdatesURL=www.idealsoftware.com
VersionInfoVersion=2.0
VersionInfoCompany=IDEAL Software GmbH
VersionInfoCopyright=Copyright (c) 2021 IDEAL Software GmbH. All rights reserved.
VersionInfoProductName={#MyAppName}
VersionInfoProductVersion=2.0
VersionInfoProductTextVersion=2.0
ArchitecturesInstallIn64BitMode=x64

[Files]
Source: "..\..\CognosNT\CognosNT\Release\CognosNT.exe"; DestDir: "{sys}"; Flags: sharedfile uninsnosharedfileprompt 32bit
Source: "..\..\CognosNT\CognosDemo\Release\COGNOSAPI.dll"; DestDir: "{sys}"; Flags: sharedfile uninsnosharedfileprompt 32bit
Source: "..\..\CognosNT\CognosNT\x64\Release\CognosNT.exe"; DestDir: "{sys}"; Flags: sharedfile uninsnosharedfileprompt 64bit
Source: "..\..\CognosNT\CognosDemo\x64\Release\COGNOSAPI.dll"; DestDir: "{sys}"; Flags: sharedfile uninsnosharedfileprompt 64bit

Source: "..\make\windirstat\UnicodeRelease\{#MyExeName}.exe"; DestDir: "{app}"; Flags: 32bit; Check: not IsWin64
Source: "..\urelease\wdsr040a.dll"; DestDir: "{app}"; Flags: 32bit; Check: not IsWin64
Source: "..\urelease\wdsr040c.dll"; DestDir: "{app}"; Flags: 32bit; Check: not IsWin64
Source: "..\urelease\wdsr040e.dll"; DestDir: "{app}"; Flags: 32bit; Check: not IsWin64
Source: "..\urelease\wdsr0405.dll"; DestDir: "{app}"; Flags: 32bit; Check: not IsWin64
Source: "..\urelease\wdsr0407.dll"; DestDir: "{app}"; Flags: 32bit; Check: not IsWin64
Source: "..\urelease\wdsr0410.dll"; DestDir: "{app}"; Flags: 32bit; Check: not IsWin64
Source: "..\urelease\wdsr0415.dll"; DestDir: "{app}"; Flags: 32bit; Check: not IsWin64
Source: "..\urelease\wdsr0419.dll"; DestDir: "{app}"; Flags: 32bit; Check: not IsWin64

Source: "..\make\windirstat\x64\UnicodeRelease\{#MyExeName}.exe"; DestDir: "{app}"; Flags: 64bit; Check: IsWin64
Source: "..\urelease_x64\wdsr040a.dll"; DestDir: "{app}"; Flags: 64bit; Check: IsWin64
Source: "..\urelease_x64\wdsr040c.dll"; DestDir: "{app}"; Flags: 64bit; Check: IsWin64
Source: "..\urelease_x64\wdsr040e.dll"; DestDir: "{app}"; Flags: 64bit; Check: IsWin64
Source: "..\urelease_x64\wdsr0405.dll"; DestDir: "{app}"; Flags: 64bit; Check: IsWin64
Source: "..\urelease_x64\wdsr0407.dll"; DestDir: "{app}"; Flags: 64bit; Check: IsWin64
Source: "..\urelease_x64\wdsr0410.dll"; DestDir: "{app}"; Flags: 64bit; Check: IsWin64
Source: "..\urelease_x64\wdsr0415.dll"; DestDir: "{app}"; Flags: 64bit; Check: IsWin64
Source: "..\urelease_x64\wdsr0419.dll"; DestDir: "{app}"; Flags: 64bit; Check: IsWin64

Source: "..\urelease\windirstat.chm"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\urelease\wdsh040e.chm"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\urelease\wdsh0407.chm"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\urelease\wdsh0415.chm"; DestDir: "{app}"; Flags: ignoreversion

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "brazilianportuguese"; MessagesFile: "compiler:Languages\BrazilianPortuguese.isl"
Name: "catalan"; MessagesFile: "compiler:Languages\Catalan.isl"
Name: "corsican"; MessagesFile: "compiler:Languages\Corsican.isl"
Name: "czech"; MessagesFile: "compiler:Languages\Czech.isl"
Name: "danish"; MessagesFile: "compiler:Languages\Danish.isl"
Name: "dutch"; MessagesFile: "compiler:Languages\Dutch.isl"
Name: "finnish"; MessagesFile: "compiler:Languages\Finnish.isl"
Name: "french"; MessagesFile: "compiler:Languages\French.isl"
Name: "german"; MessagesFile: "compiler:Languages\German.isl"
Name: "hebrew"; MessagesFile: "compiler:Languages\Hebrew.isl"
Name: "italian"; MessagesFile: "compiler:Languages\Italian.isl"
Name: "japanese"; MessagesFile: "compiler:Languages\Japanese.isl"
Name: "norwegian"; MessagesFile: "compiler:Languages\Norwegian.isl"
Name: "polish"; MessagesFile: "compiler:Languages\Polish.isl"
Name: "portuguese"; MessagesFile: "compiler:Languages\Portuguese.isl"
Name: "russian"; MessagesFile: "compiler:Languages\Russian.isl"
Name: "slovenian"; MessagesFile: "compiler:Languages\Slovenian.isl"
Name: "spanish"; MessagesFile: "compiler:Languages\Spanish.isl"
Name: "turkish"; MessagesFile: "compiler:Languages\Turkish.isl"
Name: "ukrainian"; MessagesFile: "compiler:Languages\Ukrainian.isl"

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyExeName}.exe"; WorkingDir: "{userappdata}\{#MyExeName}"; IconFilename: "{app}\{#MyExeName}.exe"; IconIndex: 0

[Run]
Filename: "{sys}\CognosNT.exe"; Parameters: "-i"; Flags: runhidden
Filename: "net.exe"; Parameters: "start cognosnt"; Flags: runhidden

[Code]
procedure CurUninstallStepChanged(CurUninstallStep: TUninstallStep);
var
  ResultCode: Integer;
begin
  if CurUninstallStep = usUninstall then begin
    if DecrementSharedCount(Is64BitInstallMode, 'CognosNT.exe')
    then begin
	  Exec('net.exe', 'stop cognosnt', '', SW_HIDE, ewWaitUntilTerminated, ResultCode);
	  Exec(ExpandConstant('{sys}\CognosNT.exe'), '-u', '', SW_HIDE, ewWaitUntilTerminated, ResultCode);
    end;
	IncrementSharedCount(Is64BitInstallMode, 'CognosNT.exe', False)
  end;
end;
