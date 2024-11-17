Import-Module .\PsUtils.psm1

$buildDir = ".\build"
New-Item -Path $buildDir -ItemType Directory -Force > $null


$testMainInputFile = ".\src\task2_test_main.cpp"
$testMainOutputFile = Get-ObjectFileOutputPath $buildDir $testMainInputFile
if (!(Test-Path $testMainOutputFile)) {
    Write-Output "Compiling:"
    .\VcWrapper64.bat cl /c /EHsc /MDd /std:c++20 /Od /W4 /WX /nologo /Fo$testMainOutputFile $testMainInputFile
}

$task2InputFile = "task2.asm"
$task2OutputFile = Get-ObjectFileOutputPath $buildDir $task2InputFile

.\VcWrapper64.bat ml64 /c /nologo /Fo$task2OutputFile $task2InputFile

if (!(Test-Path $task2OutputFile)) {
   Write-Output "Assembly failed. Aborting."
   exit
}

wsl objcopy `
    --redefine-sym main=task2_main `
    --redefine-sym recursiveLowerCase=task2_recursiveLowerCase `
    "$(Convert-ToWslPath $task2OutputFile)"

wsl objcopy `
    --redefine-sym _start=task2_main `
    "$(Convert-ToWslPath $task2OutputFile)"

$task2OutputExecutable = Join-Path $buildDir "task2_test_main.exe"
Write-Output "Linking..."
.\VcWrapper64.bat link /SUBSYSTEM:CONSOLE /OPT:NOREF,NOICF,NOLBR /NOLOGO `
    /OUT:$task2OutputExecutable legacy_stdio_definitions.lib $testMainOutputFile $task2OutputFile 

if (!(Test-Path $task2OutputExecutable)) {
    Write-Output "Linking failed. Aborting."
    exit
}

Write-Output "Running..."
& $task2OutputExecutable
