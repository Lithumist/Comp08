@echo on

rmdir /s /q "Release/resources"
rmdir /s /q "Debug/resources"
rmdir /s /q "Comp08/resources"

xcopy "RESOURCES_MASTER" "Release" /s /e /v
xcopy "RESOURCES_MASTER" "Debug" /s /e /v
xcopy "RESOURCES_MASTER" "Comp08" /s /e /v