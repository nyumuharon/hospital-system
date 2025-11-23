$certName = "HospitalSystemCert"
$exePath = ".\HospitalSystem.exe"

# 1. Check for existing certificate
$cert = Get-ChildItem Cert:\CurrentUser\My | Where-Object { $_.Subject -eq "CN=$certName" }

if (-not $cert) {
    Write-Host "Certificate '$certName' not found. Creating new self-signed code signing certificate..."
    # Create a new self-signed certificate for code signing
    $cert = New-SelfSignedCertificate -Type CodeSigningCert -Subject "CN=$certName" -CertStoreLocation Cert:\CurrentUser\My
    Write-Host "Certificate created successfully. Thumbprint: $($cert.Thumbprint)"
} else {
    Write-Host "Found existing certificate '$certName'. Thumbprint: $($cert.Thumbprint)"
}

# 2. Check if executable exists
if (-not (Test-Path $exePath)) {
    Write-Error "Executable not found at $exePath. Please build the project first."
    exit 1
}

# 3. Sign the executable
Write-Host "Signing $exePath..."
try {
    $signature = Set-AuthenticodeSignature -Certificate $cert -FilePath $exePath
    
    if ($signature.Status -eq "Valid") {
        Write-Host "SUCCESS: $exePath has been successfully signed." -ForegroundColor Green
    } else {
        Write-Warning "SIGNING FAILED: Status is '$($signature.Status)'. Message: $($signature.StatusMessage)"
    }
} catch {
    Write-Error "An error occurred during signing: $_"
}
