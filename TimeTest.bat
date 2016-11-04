@Echo Off

Echo Programm being runned...

@Echo Off

g++ Trie.cpp -o Trie

Call :SaveTime

Trie.exe count_1w.txt 0

Call :DiffTime
Echo Exec Time: %DiffTime% sec
Echo.
Pause
Exit

:SaveTime
 For /F "Tokens=1 Delims=." %%i In ('WMIC OS Get LocalDateTime^|Find "."') Do Set $Time=%%i
 Set $D0=%$Time:~6,2%
 Set $H0=%$Time:~8,2%
 Set $N0=%$Time:~10,2%
 Set $S0=%$Time:~12,2%
GoTo :EOF

:DiffTime
 For /F "Tokens=1 Delims=." %%i In ('WMIC OS Get LocalDateTime^|Find "."') Do Set $Time=%%i
 Set $D1=%$Time:~6,2%
 Set $H1=%$Time:~8,2%
 Set $N1=%$Time:~10,2%
 Set $S1=%$Time:~12,2%

 Set DiffTime=-1
 If %$D1% EQU %$D0% Set /A DiffTime=%$S1%+(%$N1%*60)+(%$H1%*3600)-(%$H0%*3600)-(%$N0%*60)-%$S0%
GoTo :EOF