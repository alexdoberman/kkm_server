@set ss=.;%classpath%
@set sss=.;%path%

@set ss=%ss%;..\classes
@set ss=%ss%;..\posiflex\jpos17.jar
@set ss=%ss%;..\posiflex\jpos17-controls.jar
@set ss=%ss%;..\posiflex\jcl.jar
@set ss=%ss%;..\posiflex\jcl_editor.jar
@set ss=%ss%;..\posiflex\xerces.jar
@set ss=%ss%;..\posiflex\comm.jar
@set ss=%ss%;..\posiflex\Serialio.jar
@set ss=%ss%;..\apache\commons-codec-1.9.jar

path = %sss%;...\posiflex;"C:\Program Files (x86)\Java\jdk1.8.0_20\bin";"C:\Program Files (x86)\Java\jdk1.8.0_20\jre\bin";

javac -classpath %ss% PosPrinter.java PosPrinterEngine.java
jar.exe cfm ..\pos.jar manifest.txt *.class 
copy *.class ..\classes
PAUSE
@del *.class
