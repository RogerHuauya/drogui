$PYTHON_PATH = 'C:\\Users\\roger\\AppData\\Local\\Programs\\Python\\Python38'
#g++ main.cpp ss.cpp control.cpp -std=c++11 -I C:\\Users\\roger\\AppData\\Local\\Programs\\Python\\Python38\\include -IC:\\Users\\roger\\AppData\\Local\\Programs\\Python\\Python38\\Lib\\site-packages\\numpy\\core\\include -LC:\\Users\\roger\\AppData\\Local\\Programs\\Python\\Python38\\libs -lpython38
g++ main.cpp ss.cpp sensors.cpp control.cpp -std=c++11 -I $PYTHON_PATH\\include -I $PYTHON_PATH\\Lib\\site-packages\\numpy\\core\\include -L $PYTHON_PATH\\libs -lpython38

