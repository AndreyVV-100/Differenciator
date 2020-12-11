cd %2
"D:\Program Files\MikTex\miktex\bin\x64\pdflatex.exe" %1.tex
del %1.log
del %1.aux
del %1.out
start %1.pdf