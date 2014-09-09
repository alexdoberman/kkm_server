@set var1 = %1
@set var2 = %var1:http=%
@if NOT "%var1%" == "%var2%" (echo ok)


