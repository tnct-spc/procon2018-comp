g++ -shared -fPIC -I../../Field -L../../Field -lpython3.6m -I/usr/include/python3.6m -lboost_numpy3 -lboost_python3 -o communication.so ../../Visualizer/visualizer.cpp ../../Solver/gamemanager.cpp ../../Field/field.cpp ../communication.cpp
# LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/home/shibh308/workspace/procon2018/procon2018-comp/Field" 
python main.py
