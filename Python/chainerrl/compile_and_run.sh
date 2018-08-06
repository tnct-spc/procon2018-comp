g++ -shared -fPIC -lqt -I../../Field -L../../Field -I../../Visualizer -L../../Visualizer -I../../GameManager -I/usr/include/qt -I/usr/include/qt/QtGui -I/usr/include/qt/QtCore -I/usr/include/qt/QtWidgets -L../../GameManager -lQtCore -lQtGui -lQtWidgets -lpython3.6m -I/usr/include/python3.6m -lboost_numpy3 -lboost_python3 -o communication.so ../../Field/field.cpp ../../Visualizer/visualizer.cpp ../../Solver/gamemanager.cpp ../communication.cpp
# LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/home/shibh308/workspace/procon2018/procon2018-comp/Field" 
python main.py
