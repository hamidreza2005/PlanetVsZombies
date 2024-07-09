[Setup]
AppName=PlanetVsZombies
AppVersion=1.0
DefaultDirName={pf}\PlanetVsZombies
DefaultGroupName=PlanetVsZombies
OutputDir=.
OutputBaseFilename=PlanetVsZombies_Setup
Compression=lzma
SolidCompression=yes

[Files]
; Include all files from the cmake-build-release directory
Source: ".\cmake-build-release\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs

[Icons]
Name: "{group}\PlanetVsZombies"; Filename: "{app}\PlanetVsZombies.exe"
Name: "{group}\Uninstall PlanetVsZombies"; Filename: "{uninstallexe}"

[Run]
Filename: "{app}\PlanetVsZombies.exe"; Description: "Launch PlanetVsZombies"; Flags: nowait postinstall skipifsilent
