Import-Module .\PsUtils.psm1

$masm32Path = "C:\masm32\bin"

$buildDir = ".\build"
New-Item -Path $buildDir -ItemType Directory -Force > $null


$testMainInputFile = ".\src\task1_test_main.cpp"
$testMainOutputFile = Get-ObjectFileOutputPath $buildDir $testMainInputFile
if (!(Test-Path $testMainOutputFile)) {
    Write-Output "Compiling:"
    .\VcWrapper.bat cl /c /EHsc /MDd /std:c++20 /Od /W4 /WX /nologo /Fo$testMainOutputFile $testMainInputFile
}

$task1InputFile = "task1.asm"
$task1OutputFile = Get-ObjectFileOutputPath $buildDir $task1InputFile

#& "$masm32Path\ml.exe" /c /coff /nologo /Fo$task1OutputFile $task1InputFile
.\VcWrapper.bat ml /c /coff /nologo /Fo$task1OutputFile $task1InputFile

if (!(Test-Path $task1OutputFile)) {
   Write-Output "Assembly failed. Aborting."
   exit
}

wsl objcopy `
    --redefine-sym _main=_task1_main `
    --redefine-sym _my_product3=_task1_my_product3 `
    --redefine-sym _my_product3_wrapper=_task1_my_product3_wrapper `
    "$(Convert-ToWslPath $task1OutputFile)"

wsl objcopy `
    --redefine-sym _start=_task1_main `
    "$(Convert-ToWslPath $task1OutputFile)"

wsl objcopy `
    --redefine-sym _main@0=_task1_main `
    --redefine-sym _my_product3@0=_task1_my_product3 `
    --redefine-sym _my_product3_wrapper@0=_task1_my_product3_wrapper `
    "$(Convert-ToWslPath $task1OutputFile)"

$task1OutputExecutable = Join-Path $buildDir "task1_test_main.exe"
Write-Output "Linking..."
.\VcWrapper.bat link /SUBSYSTEM:CONSOLE /OPT:NOREF,NOICF,NOLBR /NOLOGO /LIBPATH:"C:\masm32\lib" `
    /OUT:$task1OutputExecutable masm32.lib legacy_stdio_definitions.lib $testMainOutputFile $task1OutputFile 

if (!(Test-Path $task1OutputExecutable)) {
    Write-Output "Linking failed. Aborting."
    exit
}

Write-Output "Running..."
& $task1OutputExecutable
