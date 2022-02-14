

xcopy /y ..\Public\"*.h" ..\..\Reference\Inc\
xcopy /y ..\Bin\"*.lib" ..\..\Reference\bin\
xcopy /y ..\Bin\"*.dll" ..\..\Client\Bin\

// Tool DLL 추가
xcopy /y ..\Bin\"*.dll" ..\..\Tool\bin\