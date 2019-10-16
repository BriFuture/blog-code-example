@echo off
set PATHONPATH=%cd%
python -m ballclient.main %1 %2 %3
@echo on

EXIT