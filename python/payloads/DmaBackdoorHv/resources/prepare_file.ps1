
param ([Parameter(Mandatory)]$Path, [Parameter(Mandatory)]$VarName)

# create compressed stream
$Out = $(New-Object System.IO.MemoryStream);
$In = $(New-Object System.IO.Compression.DeflateStream($Out, [System.IO.Compression.CompressionMode]::Compress));

# read input file
$(New-Object System.IO.MemoryStream(,[System.IO.File]::ReadAllBytes($Path))).CopyTo($In);
$In.Close();
$Out.Close();

# print encoded data
Write-Host("`n$" + $VarName + " = `"" + [Convert]::ToBase64String($Out.ToArray()) + "`";");
