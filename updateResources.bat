@echo on

rmdir /s /q "Release/resources"
rmdir /s /q "Debug/resources"
rmdir /s /q "Nomnom/resources"

xcopy "Repo/Nomnom/RES" "Release" /s /e /v
xcopy "Repo/Nomnom/RES" "Debug" /s /e /v
xcopy "Repo/Nomnom/RES" "Nomnom" /s /e /v