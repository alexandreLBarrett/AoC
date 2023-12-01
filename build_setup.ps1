$dir = $args[1]
$binPath = "$dir/bin"
$objPath = "$dir/obj"

Write-Output $dir
Write-Output $binPath
Write-Output $objPath


If (-Not (Test-Path -Path $binPath -PathType Leaf)) 
{
    mkdir $binPath
}

If (-Not (Test-Path -Path $objPath -PathType Leaf)) 
{
    mkdir $objPath 
}