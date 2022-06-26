[Setup]
AppName=AstroStack
AppVersion=1.0

DefaultDirName={pf}\AstroStack
DefaultGroupName=AstroStack
Compression=lzma2
SolidCompression=yes
OutputDir=.\
ArchitecturesInstallIn64BitMode=x64
OutputBaseFilename=AstroStack Installer
LicenseFile=..\LICENSE
SetupLogging=yes

[Types]
Name: "full"; Description: "Full installation"
Name: "custom"; Description: "Custom installation"; Flags: iscustom

[Components]
Name: "AstroStack"; Description: "AstroStack application"; Types: full custom;
Name: "workflows"; Description: "AstroStack workflows"; Types: full custom;
Name: "manual"; Description: "User guide"; Types: full custom; Flags: fixed

[Files]
Source: ".\AstroStack\bin\AstroStack.exe"; DestDir: "{app}\bin\"; Components:AstroStack; Flags: ignoreversion;
Source: ".\AstroStack\bin\*.dll"; DestDir: "{app}\bin\"; Components:AstroStack; Flags: ignoreversion;
Source: ".\AstroStack\bin\plugins\*"; DestDir: "{app}\bin\plugins\"; Components:AstroStack; Flags: ignoreversion recursesubdirs;
Source: ".\AstroStack\workflows\*.json"; DestDir: "{app}\workflows\";  Components:workflows; Flags: ignoreversion;
;Source: "..\manual\AstroStack.pdf"; DestDir: "{app}"

[Icons]
Name: "{group}\AstroStack"; Filename: "{app}\AstroStack.exe"
Name: "{group}\User guide"; Filename: "{app}\AstroStack.pdf"
Name: "{group}\Uninstall AstroStack"; Filename: "{app}\unins000.exe"

[Code]
var
  OkToCopyLog : Boolean;

procedure InitializeWizard;
begin
end;

procedure CurStepChanged(CurStep: TSetupStep);
begin
  if CurStep = ssDone then
    OkToCopyLog := True;
end;

procedure DeinitializeSetup();
begin
  if OkToCopyLog then
    FileCopy (ExpandConstant ('{log}'), ExpandConstant ('{app}\InstallationLogFile.log'), FALSE);
  RestartReplace (ExpandConstant ('{log}'), '');
end;

[UninstallDelete]
Type: files; Name: "{app}\InstallationLogFile.log"
