@echo off
plutoc-5.4 -o plutoc-5.4.out -s bgscript.pluto
plutoc-5.5 -o plutoc-5.5.out -s bgscript.pluto
php helper.php
del plutoc-5.4.out
del plutoc-5.5.out
