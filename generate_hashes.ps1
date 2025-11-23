# Get all .cpp, .h, and .exe files in the current directory and subdirectories
$files = Get-ChildItem -Path . -Include *.cpp, *.h, *.exe -Recurse

# Calculate SHA-256 hashes
$hashes = $files | Get-FileHash -Algorithm SHA256

# Select Hash and Path (relative path for cleaner output)
$output = $hashes | Select-Object Hash, @{Name="Path"; Expression={$_.Path.Replace($PWD.Path + "\", "")}}

# Format as table and save to signatures.txt
$output | Format-Table -AutoSize | Out-File signatures.txt -Encoding UTF8

Write-Host "Successfully generated hashes for $($files.Count) files in signatures.txt"
