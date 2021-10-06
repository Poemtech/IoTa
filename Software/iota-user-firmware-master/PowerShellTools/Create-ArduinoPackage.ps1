<#
.SYNOPSIS
Builds the package and JSON required by the Arduino environment for the POEM Technology IoTa.

.DESCRIPTION
Builds the package and JSON required by the Arduino environment for the POEM Technology IoTa.
The defaults are based on the standard POEM Technology build environment. ;)
Debug note: Open the PowerShell Interacgive Window (Under View->PowerShell menu) and set the current directory
to PowerShellTools then run the debugger. This make sthings eastier by aligning the diretory defaultswith the
execution directory.

.PARAMETER Version
The version of the package to create (new version) or update (existing version)

.PARAMETER ProjectDir
The install location of the project source code files. Typically, one directory up (i.e. "..\") from the
location from which the script is executer.

Default: ..

.PARAMETER DestinationPath
The location of the final package files.

Default: $ProjectDir\arduino_integration

.PARAMETER ProductName
The name of the Arduino board product to build (usually "iota").

Default: iota

.PARAMETER TargetToPackage
Indicates which package (User, System, or both) to build. Note: Not need to build a System MCU package.
However, the option exists for documentation pruposes.

Default: User

.PARAMETER PackageIndexFileName
The name of the Arduino board index file to modify.

Default: package_iota_index.json

.PARAMETER PackageURL
The destintation URL where the Arduino IDE points to retrieve the package.

Default: https://poemtechnology.com/downloads/iota/arduino_integration/iota-1.0.0.zip

.PARAMETER WhatIf
Switch that, if sepcified, shows what the script will do without creating or modifying any files.

Default: $false
#>

Param(
	[Parameter(Mandatory=$True)]
	[string]$Version,

	[Parameter(Mandatory=$False)]
	[string]$ProjectDir = "..",

	[Parameter(Mandatory=$False)]
	[string]$DestinationPath = "$ProjectDir\arduino_integration",

	[Parameter(Mandatory=$False)]
	[string]$ProductName = "iota",

	[Parameter(Mandatory=$False)]
	[ValidateSet("User", "System", "UserAndSystem")]
	[string]$TargetToPackage = "User",

	[Parameter(Mandatory=$False)]
	[string]$PackageIndexFileName = "package_iota_index.json",

	[Parameter(Mandatory=$False)]
	[string]$PackageURL = "https://poemtechnology.com/downloads/iota/arduino_integration",

	[Parameter(Mandatory=$False)]
	[switch]$WhatIf = $false
)

function CreatePackageInfo()
{
	$productType = "user"

	$srcPath = "{0}\{1}" -f $ProjectDir, $ProductName
	$pkgFullPath = "{0}\pkg\{1}" -f $ProjectDir, $PackageIndexFileName
	$destName = "{0}-{1}.zip" -f $ProductName, $Version
	$destFullPath = "{0}\{1}" -f $DestinationPath, $destName

	return @{"pkgFullPath" = $pkgFullPath;  "productType" = $productType; "srcPath" = $srcPath; "destPath" = $DestinationPath; "destName" = $destName; "destFullPath" = $destFullPath}
}

function CreateArchive([HashTable] $pkgInfo, [HashTable] $CommonParams)
{
	$boardsSrcPath = "{0}\boards.txt" -f $pkgInfo.srcPath

	# Create the archive
	Write-Host "Building" $pkgInfo.productType "archive in" $pkgInfo.destPath -ForegroundColor Yellow
	Compress-Archive -Path $pkgInfo.srcPath -DestinationPath $pkgInfo.destFullPath -Force @CommonParams
}

function UpdatePackageInfo([HashTable] $pkgInfo, [HashTable] $CommonParams)
{
	$pkgDest = "{0}\{1}" -f $pkgInfo.destPath, $PackageIndexFileName

	Write-Host "Writing package info JSON " $pkgDest -ForegroundColor Yellow

	$pkgJSON = Get-Content -Path $pkgInfo.pkgFullPath -ErrorAction Stop
	$pkg = $pkgJSON | ConvertFrom-Json

	$platforms = $pkg.packages[0].platforms
	$lastPlatform = $platforms[$platforms.count - 1]
	$lastVersion = $lastPlatform.version

	# If this is a new version (as opposed to an update of an existing version),
	# create a new spot in the platforms array to store it.
	if ($lastVersion -ne $Version) {
		# Copy the last element of the array by converting it to JSON and then
		# back to an object (i.e. make a deep copy).
		$tmpJSON = ConvertTo-Json $platforms[$platforms.count - 1]
		$lastPlatform = ConvertFrom-Json $tmpJSON
		$pkg.packages[0].platforms += $lastPlatform
	}

	# Update relevant info for package (version, url, archiveFileName, checksum, size
	$sha256Hash = Get-FileHash -Path $pkgInfo.destFullPath -Algorithm SHA256
	$size = (Get-Item -Path $pkgInfo.destFullPath).Length

	$lastPlatform.version = $Version
	$lastPlatform.url = "$PackageURL/" + $pkgInfo.destName
	$lastPlatform.archiveFileName = $pkgInfo.destName
	$lastPlatform.checksum = "SHA-256:" + $sha256Hash.Hash
	$lastPlatform.size = $size
	
	$pkg | ConvertTo-Json -Depth 10 | .\Format-Json | Out-File $pkgDest -Encoding ascii -Force @CommonParams
}

# Used for "parameter splatting" on standard cmdlets
$CommonParams = @{}
if ($WhatIf) {
	Write-Host "WhatIf: No files will be updated." -ForegroundColor Yellow -BackgroundColor Black
	$CommonParams.add("WhatIf", $true)
}

Write-Host "POEM Technology IoTa Packaging System" -ForegroundColor Green

if (-not (Test-Path $DestinationPath)) {
	New-Item -ItemType Directory -Force -Path $DestinationPath @CommonParams | Out-Null
}

try {
	if ($TargetToPackage.Contains("System")) {
		Write-Host "Packaging SYSTEM MCU files..." -ForegroundColor Cyan
		Write-Host "Simply commit the latest code to the GitHub repository." -ForegroundColor Yellow
		Write-Host "No action taken." -ForegroundColor Yellow -BackgroundColor Black
	}

	if ($TargetToPackage.Contains("User")) {
		Write-Host "Packaging USER MCU files..." -ForegroundColor Cyan
		$pkgInfo = CreatePackageInfo

		CreateArchive $pkgInfo $CommonParams
		UpdatePackageInfo $pkgInfo $CommonParams
	}
}
catch {
	Write-Host "ERROR: $_" -ForegroundColor Red -BackgroundColor Black
}
