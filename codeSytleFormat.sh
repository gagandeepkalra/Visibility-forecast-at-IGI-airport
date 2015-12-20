#Artistic Style 2.05 has been used to format the code sytle of this project

#http://astyle.sourceforge.net/astyle.html

astyle --style=stroustrup --suffix=none --indent=spaces=4 --indent-col1-comments --break-blocks --align-pointer=name --align-reference=name --recursive ./*.cpp ./*.h
 
