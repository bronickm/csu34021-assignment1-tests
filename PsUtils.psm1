function Get-ObjectFileOutputPath {
    param (
        [string]$buildDir,
        [string]$inputFile
    )

    $fileNameWithoutExtension = [System.IO.Path]::GetFileNameWithoutExtension($inputFile)
    $outputFile = Join-Path -Path $buildDir -ChildPath ($fileNameWithoutExtension + ".obj")
    
    return $outputFile
}

function Convert-ToWslPath {
    param (
        [string]$path
    )

    $absolutePath = Resolve-Path $path
    $DriveLetter = $absolutePath.Path.Substring(0,1).ToLower()
    $PathWithoutDrive = $absolutePath.Path.Substring(2) -replace '\\', '/'
    $WslPath = "/mnt/$DriveLetter$PathWithoutDrive"
    
    return $WslPath
}

Export-ModuleMember -Function Get-ObjectFileOutputPath, Convert-ToWslPath
