#define MyAppName "Flanger 2"
#define MyAppVersion "2.0.13"
#define MyAppPublisher "recluse-audio"
#define MyAppURL "https://recluse-audio.com"
#define VST3Source SourcePath + "\..\..\BUILD\Flanger2_artefacts\Release\VST3\Flanger 2.vst3"
#define OutputDir SourcePath + "\BUILD"

[Setup]
AppId={{4B9EC899-3CBA-5712-B05A-6198D109A04B}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
DefaultDirName={autopf}\{#MyAppPublisher}\{#MyAppName}
UninstallFilesDir={app}
DefaultGroupName={#MyAppName}
DisableDirPage=yes
DisableProgramGroupPage=yes
OutputDir={#OutputDir}
OutputBaseFilename={#MyAppName}_v{#MyAppVersion}_Windows_Installer
Compression=lzma
SolidCompression=yes
WizardStyle=modern
PrivilegesRequired=admin
ArchitecturesInstallIn64BitMode=x64compatible

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Files]
Source: "{#VST3Source}\*"; DestDir: "{commoncf}\VST3\{#MyAppName}.vst3"; Flags: recursesubdirs createallsubdirs ignoreversion

[Icons]
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
