set dir=%1
echo "%dir%"
set binDir="%dir%/bin"
set objDir="%dir%/obj"

if not exist %binDir% mkdir %binDir%
if not exist %objDir% mkdir %objDir%